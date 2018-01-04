#getting Qn from multiple pics using spark

# -*- coding: utf-8 -*-
import sys
import os
import commands
sys.path.append("/home/zym/caffe-master/build/install/python/")
import caffe
import pylab
import numpy as np
import cv2
import math
import matplotlib.pyplot as plt
import seaborn as sns
from pyspark import  SparkContext
from pyspark import  SparkConf
MODEL_FILE = '/home/face_deploy.prototxt'
PRETRAIN_FILE = '/home/fix_cl_model_conv1b_res.caffemodel'
IMG_DIR = '/home/zym/imgs'

NUMSPLIT = 2
#TARGET_DIR='/media/out'

np.set_printoptions(threshold='nan')
#params_txt = 'params.txt'
#pf = open(params_txt, 'w')

#net = caffe.Net(MODEL_FILE, PRETRAIN_FILE, caffe.TEST)
imgs1 = commands.getoutput('ls -l %s | awk \'{print $9}\''%IMG_DIR)
imgs1 = imgs1.split('\n')
del(imgs1[0])

if (len(imgs1) / NUMSPLIT) > 0:
    imgs=[imgs1[i:i+len(imgs1)/NUMSPLIT] for i in range(0, len(imgs1), len(imgs1)/NUMSPLIT)]
else:
    imgs = [imgs1[i:i + 1] for i in range(0, len(imgs1))]

pp=["data","conv1a","relu1a","conv1b","relu1b","pool1b","conv2_1","relu2_1","conv2_2","relu2_2","res2_2","conv2","relu2","pool2","conv3_1","relu3_1","conv3_2","relu3_2","res3_2","conv3_3","relu3_3","conv3_4","relu3_4","res3_4","conv3","relu3","pool3","conv4_1","relu4_1","conv4_2","relu4_2","res4_2","conv4_3","relu4_3","conv4_4","relu4_4","res4_4","conv4_5","relu4_5","conv4_6","relu4_6","res4_6","conv4_7","relu4_7","conv4_8","relu4_8","res4_8","conv4_9","relu4_9","conv4_10","relu4_10","res4_10","conv4","relu4","pool4","conv5_1","relu5_1","conv5_2","relu5_2","res5_2","conv5_3","relu5_3","conv5_4","relu5_4","res5_4","conv5_5","relu5_5","conv5_6","relu5_6","res5_6","fc5"]
#pp.extend(["data", "pool1b","pool2","pool3","pool4","res2_2","res3_2","res3_4","res4_2","res4_4","res4_6","res4_8","res4_10","res5_2","res5_4","res5_6"])

#f = file("qn_newmodel.txt",'w+')

def davi(x):
  if x < 0:
    return math.log(-x, 2)
  elif x == 0:
    return -20
  else:
    return math.log(x, 2)

def getQn(x):
  sys.path.append("/home/zym/caffe-master/build/install/python/")
  import caffe
  qn = []
  for i in range(127):
      qn.append(16)
  net = caffe.Net(MODEL_FILE, PRETRAIN_FILE, caffe.TEST)
  for pic in x:

    i=0
    img_name = pic.strip()
    img = cv2.imread(IMG_DIR + '/' + img_name)
    img = (np.float32(img) - 127.5) * 0.0078125
    im_in = cv2.resize(img, (96, 112), interpolation=cv2.INTER_NEAREST)
    im_in = im_in[None, :]
    im_in = im_in.transpose((0, 3, 1, 2))
    net.forward_all(data=im_in)
    for param_name in pp:
#  if not "relu" in param_name:

      all_param = []
      all_out = []

      if "conv" in param_name or "fc5" in param_name or "relu" in param_name:
        weight = net.params[param_name][0].data
      if not "relu" in param_name:
        out = net.blobs[param_name].data[0]
      if "conv" in param_name or "fc5" in param_name:
        bias = net.params[param_name][1].data

#    bias = net.params[param_name][1].data

   # pf.write(param_name)
    #pf.write('\n')

   # pf.write('\n' + param_name + '_weight:\n\n')
      if "conv" in param_name or "fc5" in param_name or "relu" in param_name:
        weight.shape = (-1, 1)
        all_param = weight.flatten().tolist()
      if not "relu" in param_name:
        all_out = out.flatten().tolist()
      if "conv" in param_name or "fc5" in param_name:
        all_bias = bias.flatten().tolist()

      if "conv" in param_name or "fc5" in param_name or "relu" in param_name:
        all_param = map(lambda x:davi(x+1), all_param)
      if not "relu" in param_name:
        all_out = map(lambda x:davi(x+1), all_out)
      if "conv" in param_name or "fc5" in param_name or "relu" in param_name:
        max_param = np.r_[all_param].max()
        Qw=15-math.ceil(max_param)
        if qn[i]>Qw:
            qn[i]=Qw
        i=i+1
#        print param_name,'    \tQw\t',int(Qw)
#        f.write('%s    \tQw\t%d\n' % (param_name,int(Qw)))
      if not "relu" in param_name:
        max_data = np.r_[all_out].max()
        Qo=15-math.ceil(max_data)
        if qn[i]>Qo:
            qn[i]=Qo
        i=i+1
 #       print param_name,'    \tQo\t',int(Qo)
 #       f.write('%s    \tQo\t%d\n' % (param_name,int(Qo)))
      if "conv" in param_name or "fc5" in param_name:
        all_bias = map(lambda x:davi(x+1), all_bias)
        max_bias = np.r_[all_bias].max()
        Qb=15-math.ceil(max_bias)
        if qn[i]>Qb:
            qn[i]=Qb
        i=i+1
  return qn

#conf = new SparkConf()
#conf.set("spark.app.name","GetQn")
#conf.set("spark.master","localhost")
#conf.set("spark.ui.port","8080")
#sc = SparkContext('spark://127.0.0.1:7077', 'pyspark')
sc = SparkContext( 'local', 'pyspark')
imgs = sc.parallelize(imgs)
qns = imgs.map(lambda x:getQn(x)).collect()

Qn=[]
if len(qns)>1:
    for i in range(127):
        Qn.append(16)
    for i in range(NUMSPLIT):
        for j in range(len(qns[0])):
            if Qn[j]>qns[i][j]:
                Qn[j]=qns[i][j]
else:
    Qn=qns[0]
print Qn


#        print param_name,'    \tQb\t',int(Qb)
#        f.write('%s    \tQb\t%d\n' % (param_name,int(Qb)))

#f.close()

#      bins = np.linspace(min_data, max_data, max_data+1)
#      if param_name != "data":
#        sns.kdeplot(all_param, shade=True)
#      sns.kdeplot(all_out, shade=True)
      #plt.hist(all_param, bins=bins,color="#FF0000", normed=True)
      #plt.hist(all_out, bins=bins,color="#C1F320", normed=True)
#    pylab.savefig(param_name)
#    pylab.clf()
