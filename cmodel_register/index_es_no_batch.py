import _init_paths

import cv2
import glob
import numpy as np
import argparse
import threadpool
import threading
from base64 import b64encode

from elasticsearch import Elasticsearch
from elasticsearch import helpers

from extractor import Extractor
from lib.es.ImFea_pb2 import ImFea, ImFeaArr, \
    ImFeaBinArr, ImFeaBin

import logging
logging.basicConfig(
    format='%(asctime)s : %(levelname)s : %(message)s', level=logging.INFO)
logger = logging.getLogger(__name__)


def parse_args():
    """
    Parse input arguments
    """
    parser = argparse.ArgumentParser(
        description='Index image to elasticsearch')
    #parser.add_argument('--weight', dest='weight',
    #                    help='weight to test',
     #                   default=WEIGHT_PATH, type=str)
    #parser.add_argument('--model_path', dest='model_path',
     #                   help='path to the model',
      #                  default=MODEL_PATH, type=str)

    parser.add_argument('--input', dest='input',
                        help='Input image folder',
                        default=None, type=str)

    parser.add_argument('--es_host', dest='es_host',
                        help='es sever host',
                        default='localhost', type=str)
    parser.add_argument('--es_index', dest='es_index',
                        help='index name',
                        default='img_data', type=str)
    parser.add_argument('--es_type', dest='es_type',
                        help='index type',
                        default='obj', type=str)
    parser.add_argument('--es_port', dest='es_port',
                        help='es server port',
                        default=9200, type=int)

    args = parser.parse_args()

    if not args.input:
        parser.error('Input folder not given')
    return args


def create_doc(im_src, tag, coords, fea_arr, fea_bin_arr):
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


if __name__ == '__main__':
    args = parse_args()
    #mutex = threading.Lock()
    files = open("/root/PycharmProjects/cmodel_register/pics.txt")
    other = open("./no_faces.txt", "w")
    no_faces = []
    pics = []
    bulk = []
    actions = []
    num_docs = 0
    count = 0
    extractor = Extractor()
    total_num = 58
    for line in files:
        count += 1
        im_path = line.strip()
        # create elasticsearch client
        es = Elasticsearch(hosts='{}:{}'.format(args.es_host, args.es_port))
        # load images
        # images = glob.glob(args.input + "/*")

        es_index = args.es_index
        es_type = args.es_type
        # num_imgs = len(images)
        # read image
        im = cv2.imread(im_path)
        if im is None:
            no_faces.append(im_path)
        # im = im.astype(np.float32, copy=True)
        im_name = im_path.split('/')[-1]

        boxes = extractor.extract_regions_and_feats(im, im_name)
        if len(boxes) == 0:
            no_faces.append(im_path)
        if count % 100 == 0:
            logger.info('Processing image {}/{}'.format(count, 300))

        for cl, b in boxes.iteritems():
            coords = []
            ar = []
            ar_bin = []
            coord_box = {}
            coord_box['c'] = b['lt'] + b['rb']
            coord_box['score'] = float(b['score'])
            coords.append(coord_box)
            f = b['f']
            f_bin = extractor.binarize_fea(f)

            im_fea = ImFea()
            im_fea_bin = ImFeaBin()
            im_fea.f.extend(f)
            im_fea_bin.f.extend(f_bin)
            ar.append(im_fea)
            ar_bin.append(im_fea_bin)

            doc = create_doc(im_name, cl, coords, ar, ar_bin)
            num_docs += 1

            # create index action
            action = {
                "_index": es_index,
                "_type": es_type,
                "_source": doc
            }
            actions.append(action)
            print len(actions)
            if len(actions) == 1000 or count == total_num:
                logger.info('Bulking {} docs to sever, indexed: {}'
                            .format(len(actions), num_docs))
                helpers.bulk(es, actions)
                del actions[:]
            helpers.bulk(es, actions)
            logger.info('Bulking {} docs to sever, indexed: {}'.format(len(actions), num_docs))
    files.close()
    for item in no_faces:
        other.write(item + "\n")
    other.close()
    # create feature extractor

