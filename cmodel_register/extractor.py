import _init_paths
#from model.test import extract_regions_and_feats \
 #   as _extract_regions_and_feats
#from model.test import extract_imfea \
  #  as _extract_imfea
from math import ceil
import numpy as np
import cv2
from lib.utils.im_util import read_img_base64
from mtcnn.MtcnnDetector import FaceDetector_mt
import dlib
from lib.es.ImFea_pb2 import ImFea, ImFeaArr, \
    ImFeaBinArr, ImFeaBin
from base64 import b64encode
from elasticsearch import Elasticsearch
from elasticsearch import helpers
import threading
import logging
logging.basicConfig(
    format='%(asctime)s : %(levelname)s : %(message)s', level=logging.INFO)
logger = logging.getLogger(__name__)
# import logging
# logging.basicConfig(format='%(asctime)s : %(levelname)s : %(message)s', level=logging.INFO)
# logger = logging.getLogger(__name__)

def _binarize_fea(x, thresh):
    '''binary and pack feature vector'''
    if x is None:
        x_ = np.random.rand(512)
    else:
        x_= np.array(x)
    binary_vec = np.where(x_ >= thresh, 1, 0)
    f_len = binary_vec.shape[0]
    if f_len % 32 != 0:
        new_size = int(ceil(f_len / 32.) * 32)
        num_pad = new_size - f_len
        binary_vec = np.pad(binary_vec, (num_pad, 0), 'constant')
    return np.packbits(binary_vec).view('uint32')


class Extractor:
    """ Feature extractor

    Parameter:
        - model_path: path to the model
        - weight_path: weight path
        - sess: tensorflow session
        - num_classes: number of classes
    """

    def __init__(self,anchors=[4, 8, 16, 32], num_classes=81):

        self.num_classes = num_classes
        self.detector = FaceDetector_mt(minsize=20, gpuid=1, fastresize=False)
        self.TEMPLATE = np.float32([
            (0.0792396913815, 0.339223741112), (0.0829219487236, 0.456955367943),
            (0.0967927109165, 0.575648016728), (0.122141515615, 0.691921601066),
            (0.168687863544, 0.800341263616), (0.239789390707, 0.895732504778),
            (0.325662452515, 0.977068762493), (0.422318282013, 1.04329000149),
            (0.531777802068, 1.06080371126), (0.641296298053, 1.03981924107),
            (0.738105872266, 0.972268833998), (0.824444363295, 0.889624082279),
            (0.894792677532, 0.792494155836), (0.939395486253, 0.681546643421),
            (0.96111933829, 0.562238253072), (0.970579841181, 0.441758925744),
            (0.971193274221, 0.322118743967), (0.163846223133, 0.249151738053),
            (0.21780354657, 0.204255863861), (0.291299351124, 0.192367318323),
            (0.367460241458, 0.203582210627), (0.4392945113, 0.233135599851),
            (0.586445962425, 0.228141644834), (0.660152671635, 0.195923841854),
            (0.737466449096, 0.182360984545), (0.813236546239, 0.192828009114),
            (0.8707571886, 0.235293377042), (0.51534533827, 0.31863546193),
            (0.516221448289, 0.396200446263), (0.517118861835, 0.473797687758),
            (0.51816430343, 0.553157797772), (0.433701156035, 0.604054457668),
            (0.475501237769, 0.62076344024), (0.520712933176, 0.634268222208),
            (0.565874114041, 0.618796581487), (0.607054002672, 0.60157671656),
            (0.252418718401, 0.331052263829), (0.298663015648, 0.302646354002),
            (0.355749724218, 0.303020650651), (0.403718978315, 0.33867711083),
            (0.352507175597, 0.349987615384), (0.296791759886, 0.350478978225),
            (0.631326076346, 0.334136672344), (0.679073381078, 0.29645404267),
            (0.73597236153, 0.294721285802), (0.782865376271, 0.321305281656),
            (0.740312274764, 0.341849376713), (0.68499850091, 0.343734332172),
            (0.353167761422, 0.746189164237), (0.414587777921, 0.719053835073),
            (0.477677654595, 0.706835892494), (0.522732900812, 0.717092275768),
            (0.569832064287, 0.705414478982), (0.635195811927, 0.71565572516),
            (0.69951672331, 0.739419187253), (0.639447159575, 0.805236879972),
            (0.576410514055, 0.835436670169), (0.525398405766, 0.841706377792),
            (0.47641545769, 0.837505914975), (0.41379548902, 0.810045601727),
            (0.380084785646, 0.749979603086), (0.477955996282, 0.74513234612),
            (0.523389793327, 0.748924302636), (0.571057789237, 0.74332894691),
            (0.672409137852, 0.744177032192), (0.572539621444, 0.776609286626),
            (0.5240106503, 0.783370783245), (0.477561227414, 0.778476346951)])
        self.TPL_MIN = np.min(self.TEMPLATE, axis=0)
        self.TPL_MAX = np.max(self.TEMPLATE, axis=0)
        self.MINMAX_TEMPLATE = (self.TEMPLATE - self.TPL_MIN) / (self.TPL_MAX - self.TPL_MIN)
        self.OUTER_EYES_AND_NOSE = [37, 44, 30, 48, 54]

    def extract_regions_and_feats(self, img, im_name):
        """Extract regions and feature corresponding to
        the each box"""
        boxes = {}
        if img is None:
            return boxes
        total_boxes, points, numbox = self.detector.detectface(img)
        if numbox == 0:
            total_boxes, points, numbox = self.getface_dlib(img,[37, 44, 30, 48, 54])
            if numbox == 0:
                return boxes
            for i in range(numbox):
                align_img = self.align(128, img, total_boxes[0], points[i], self.OUTER_EYES_AND_NOSE)
                cv2.imwrite("./temp.jpg",align_img)
                fea = self.detector.recognise_cm("./temp.jpg")
                print fea
                box = {}
                box['lt'] = [int(total_boxes[i][0]), int(total_boxes[i][1])]
                box['rb'] = [int(total_boxes[i][2]), int(total_boxes[i][3])]
                box['f'] = fea
                box['cl'] = im_name
                box['score'] = 0.99
                boxs_key = im_name + str(i)
                boxes[boxs_key] = box
        else:
            for i in range(numbox):
                align_img = self.align(128, img, total_boxes[0], points[:, 0], self.OUTER_EYES_AND_NOSE)
                cv2.imwrite("./temp.jpg", align_img)
                fea = self.detector.recognise_cm("./temp.jpg")
                print fea
                box = {}
                box['lt'] = [int(total_boxes[i][0]), int(total_boxes[i][1])]
                box['rb'] = [int(total_boxes[i][2]), int(total_boxes[i][3])]
                box['f'] = fea
                box['cl'] = im_name
                box['score'] = 0.99
                boxs_key = im_name + str(i)
                boxes[boxs_key] = box
        return boxes

    def align(self, imgDim, rgbImg, bb, landmarks, landmarkIndices=[40, 46, 30, 48, 54],
              skipMulti=False,net_type="cl"):
        r"""align(imgDim, rgbImg, bb=None, landmarks=None, landmarkIndices=INNER_EYES_AND_BOTTOM_LIP)

        Transform and align a face in an image.

        :param imgDim: The edge length in pixels of the square the image is resized to.
        :type imgDim: int
        :param rgbImg: RGB image to process. Shape: (height, width, 3)
        :type rgbImg: numpy.ndarray
        :param bb: Bounding box around the face to align. \
                   Defaults to the largest face.
        :type bb: dlib.rectangle
        :param landmarks: Detected landmark locations. \
                          Landmarks found on `bb` if not provided.
        :type landmarks: list of (x,y) tuples
        :param landmarkIndices: The indices to transform to.
        :type landmarkIndices: list of ints
        :param skipMulti: Skip image if more than one face detected.
        :type skipMulti: bool
        :return: The aligned RGB image. Shape: (imgDim, imgDim, 3)
        :rtype: numpy.ndarray
        """
        assert imgDim is not None
        assert rgbImg is not None
        assert landmarkIndices is not None

        if bb is None:
            return

        if landmarks is None:
            return None

        temp_r = []
        # print len(landmarks)
        for t in range(5):
            a = landmarks[t]
            b = landmarks[t + 5]
            temp_r.append([a, b])
        out = np.array(temp_r)
        npLandmarks = np.float32(out)
        # print npLandmarks
        # npLandmarks = np.string_(out)
        # nt npLandmarks
        npLandmarkIndices = np.array(landmarkIndices)
        if net_type == "lt":
            distri = np.float32(imgDim* self.MINMAX_TEMPLATE[npLandmarkIndices])
            distri[0][1] = distri[0][1] + 30
            distri[1][1] = distri[1][1] + 30
            distri[0][0] = distri[0][0] + 5
            distri[1][0] = distri[1][0] + 15
            distri[3][1] = distri[3][1] + 15
            distri[4][1] = distri[4][1] + 15
        else:
            distri = np.float32([(30.2946,51.6963),(65.5316,51.5014),(48.0252,71.7366),(33.5493,92.3655),(62.7299,92.2041)])
        print "################"
        print distri

        assert (npLandmarks.shape == distri.shape)

        A = npLandmarks.reshape(1, 5, 2)
        B = distri.reshape(1, 5, 2)
        # H, matches = cv2.findHomography(npLandmarks, distri, cv2.RANSAC)
        H = cv2.estimateRigidTransform(A, B, True)
        # H = cv2.getAffineTransform(npLandmarks, distri)  only support 3points
        # thumbnail = cv2.warpAffine(rgbImg, H[0:2,:], (128, 128))
        if net_type == "lt":
            thumbnail = cv2.warpAffine(rgbImg, H, (imgDim, imgDim))
        else:
            thumbnail = cv2.warpAffine(rgbImg, H, (96, 112))
        return thumbnail

    def multip_extract(self, img, total_boxes, points, numbox, type="mtcnn"):
        feas = []
        for i in range(numbox):
            if type == "dlib":
                align_pic = self.align(128, img, total_boxes[i], points[i],self.OUTER_EYES_AND_NOSE)
            else:
                align_pic = self.align(128, img, total_boxes[i], points[:, i], self.OUTER_EYES_AND_NOSE)
            feas.append(self.detector.recognise_cl(align_pic))
        return feas

    def extract_imfea(self, img):
        "Extract feature for image"
        if type(img) == str:
            img = self.read_img(img)
        if img is None:
            return None
        total_boxes, points, numbox = self.detector.detectface(img)
        if numbox == 0:
            total_boxes, points, numbox = self.getface_dlib(img,[37, 44, 30, 48, 54])
            if numbox == 0:
                return None
            fea= self.multip_extract(img, total_boxes, points,numbox,type="dlib")
        else:
            fea= self.multip_extract(img, total_boxes, points,numbox,type="mtcnn")
        return fea

    def binarize_fea(self, fea, thres=0.1):
        "Binarize and pack feature vector"
        return _binarize_fea(fea, thres)

    def get_tags(self, img, img_str):
        boxes = self.extract_regions_and_feats(img,img_str)
        out = {}
        for cl, b in boxes.iteritems():
            best_score = b['score']
            out[cl] = float(best_score)
        return out

    def create_doc(self,im_src, tag, coords, fea_arr, fea_bin_arr):
        """
        Create elasticsearch doc

        Params:
            im_src: image file name
            tag: tag or class for image
            coords: list of boxes corresponding to a tag
            fea_arr: list of ImFea objects
            fea_bin_arr: list of ImFeaBin objects
        """
        doc = {}
        doc['coords'] = coords
        f_bin = ImFeaBinArr()
        f = ImFeaArr()
        f.arr.extend(fea_arr)
        f_bin.arr.extend(fea_bin_arr)
        obj_bin_str = b64encode(f_bin.SerializeToString())
        obj_str = b64encode(f.SerializeToString())
        doc['sigs'] = obj_str
        doc['bin_sigs'] = obj_bin_str
        doc['im_src'] = im_src
        doc['cl'] = tag
        return doc

    def do_index(self, im,im_str,es_host,es_port):
        # create elasticsearch client
        es = Elasticsearch(hosts='{}:{}'.format(es_host, es_port))
        # load images
        # images = glob.glob(args.input + "/*")

        bulk = []
        actions = []
        num_docs = 0
        count = 0

        es_index = 'img_data'
        es_type = 'obj'
        # num_imgs = len(images)
        # read image
        if im is None:
            return {im_str: 0}
        # im = im.astype(np.float32, copy=True)
        im_name = im_str

        boxes = self.extract_regions_and_feats(im, im_name)
        if len(boxes) == 0:
            print "No face"
            return {im_str: 0}
        count += 1

        for cl, b in boxes.iteritems():
            coords = []
            ar = []
            ar_bin = []
            coord_box = {}
            coord_box['c'] = b['lt'] + b['rb']
            coord_box['score'] = float(b['score'])
            coords.append(coord_box)
            f = b['f']
            f_bin = self.binarize_fea(f)

            im_fea = ImFea()
            im_fea_bin = ImFeaBin()
            im_fea.f.extend(f)
            im_fea_bin.f.extend(f_bin)
            ar.append(im_fea)
            ar_bin.append(im_fea_bin)

            doc = self.create_doc(im_name, cl, coords, ar, ar_bin)
            num_docs += 1

            # create index action
            action = {
                "_index": es_index,
                "_type": es_type,
                "_source": doc
            }
            actions.append(action)
            if len(actions) == 1000:
                logger.info('Bulking {} docs to sever, indexed: {}'
                            .format(len(actions), num_docs))
                helpers.bulk(es, actions)
                del actions[:]

                # index document ifself
        if len(actions) > 0:
            helpers.bulk(es, actions)
            logger.info('Bulking {} docs to sever,  total {}'
                        .format(len(actions), num_docs))

    def getface_dlib(self, bgrImg,landmark_mask):
        landmark_x = []
        landmark_y = []
        landmark = []
        total_boxes = np.zeros(shape=(0, 4))
        rgbImg = cv2.cvtColor(bgrImg, cv2.COLOR_BGR2RGB)
        detector = dlib.get_frontal_face_detector()
        landmark_predictor = dlib.shape_predictor("shape_predictor_68_face_landmarks.dat")
        faces = detector(rgbImg, 1)
        if len(faces) > 0:
            for k, d in enumerate(faces):
                shape = landmark_predictor(rgbImg, d)
                for i in landmark_mask:
                    pt = shape.part(i)
                    landmark_x.append(pt.x)
                    landmark_y.append(pt.y)
                    #face = max(faces, key=lambda rect: rect.width() * rect.height())
                box = np.array([[faces[k].left(), faces[k].top(),faces[k].right(), faces[k].bottom()]])
                total_boxes=np.vstack((total_boxes, box))
                landmark.append(landmark_x + landmark_y)
        return total_boxes,landmark,len(faces)

    def read_img(self, path):
        " Read image from file "
        im = cv2.imread(path) #
        # \.astype(np.float32, copy=True)
        return im

    def GaussianBlurSigma(self,sigma,KSIZE,img):
        global SIGMA
        SIGMA = sigma / 10.0
        dst = cv2.GaussianBlur(img, (KSIZE, KSIZE), SIGMA, KSIZE)
        return dst


if __name__ == '__main__':

    extractor = Extractor()
    landmark_mask = [37, 44, 30, 48, 54]
    test_img_path = '/home/images/temp/n2.jpg'
   # with open('test.txt') as f:
    #    text = f.read().strip()
    #img = read_img_base64(test_img_path)
    img = cv2.imread(test_img_path)
    #img_r=cv2.resize(img, (128, 128), interpolation=cv2.INTER_NEAREST)
    #total_boxes, points, numbox = extractor.detector.detectface(img)
    total_boxes,landmark,numboxes= extractor.getface_dlib(img,landmark_mask)
   # print numboxes

    cv2.imshow("test", img)
    cv2.waitKey(0)
    #extractor.get_tags(img)
    import pdb;
   # pdb.set_trace()
   # img_b = cv2.Laplacian(img,cv2.CV_64F)
    #img_b = np.uint8(np.absolute(img_b))
    fea = extractor.extract_imfea(img)
  #  box = extractor.extract_regions_and_feats(img,"n3.txt")
    print len(fea[0])
    bin_fea = extractor.binarize_fea(fea)
    fea_str = ','.join([str(int(t)) for t in bin_fea])
    #print(bin_fea.shape)
    print fea
