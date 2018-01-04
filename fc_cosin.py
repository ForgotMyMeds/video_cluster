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
IMG_DIR = '/home/zym/lfw-deepfunneled'

NUMSPLIT = 1
COS_CRITICAL= 0.65
#TARGET_DIR='/media/out'

np.set_printoptions(threshold='nan')
#params_txt = 'params.txt'
#pf = open(params_txt, 'w')

#net = caffe.Net(MODEL_FILE, PRETRAIN_FILE, caffe.TEST)
namelist=[]
pairlist=[]
fs = open("pairs.txt",'r+')
ns = open("pic_names_new.txt",'r+')
for line in ns.readlines():
	namelist.append(str(line.split("\n")[0]))
for line in fs.readlines():
	pairlist.append(line.split())

if (len(pairlist) / NUMSPLIT) > 0:
    pairs=[pairlist[i:i+len(pairlist)/NUMSPLIT] for i in range(0, len(pairlist), len(pairlist)/NUMSPLIT)]
else:
    pairs = [pairlist[i:i + 1] for i in range(0, len(pairlist))]


def getaccuracy(x):
  sys.path.append("/home/zym/caffe-master/build/install/python/")
  import caffe

  net = caffe.Net(MODEL_FILE, PRETRAIN_FILE, caffe.TEST)
  aclist=[]
  for pair in x:
      real_df = 0
      if (len(pair) == 3):
          jpg1 = "%s_%4.4d.jpg" % (pair[0], int(pair[1]))
          jpg2 = "%s_%4.4d.jpg" % (pair[0], int(pair[2]))
          index1 = namelist.index(jpg1)
          index2 = namelist.index(jpg2)
      else:
          jpg1 = "%s_%4.4d.jpg" % (pair[0], int(pair[1]))
          jpg2 = "%s_%4.4d.jpg" % (pair[2], int(pair[3]))
          index1 = namelist.index(jpg1)
          index2 = namelist.index(jpg2)
          real_df = 1
      img_name=jpg1.strip()
      img = cv2.imread(IMG_DIR+'/'+pair[0]+'/'+img_name)
      img = (np.float32(img) - 127.5) * 0.0078125
      im_in = cv2.resize(img, (96, 112), interpolation=cv2.INTER_NEAREST)
      im_in = im_in[None, :]
      im_in = im_in.transpose((0, 3, 1, 2))
      net.forward_all(data=im_in)
      all_out = []
      out = net.blobs['fc5'].data[0]
      all_out = out.flatten().tolist()
      fc1 = np.array(all_out)
      img_name=jpg2.strip()
      if real_df == 0:
          img = cv2.imread(IMG_DIR + '/' + pair[0] + '/' + img_name)
      else:
          img = cv2.imread(IMG_DIR + '/' + pair[2] + '/' + img_name)
      img = (np.float32(img) - 127.5) * 0.0078125
      im_in = cv2.resize(img, (96, 112), interpolation=cv2.INTER_NEAREST)
      im_in = im_in[None, :]
      im_in = im_in.transpose((0, 3, 1, 2))
      net.forward_all(data=im_in)
      all_out = []
      out = net.blobs['fc5'].data[0]
      all_out = out.flatten().tolist()
      fc2 = np.array(all_out)
      L1 = np.sqrt(fc1.dot(fc1))
      L2 = np.sqrt(fc1.dot(fc2))
      cos_angle = fc1.dot(fc2)/(L1*L2)
      if cos_angle > COS_CRITICAL:
          df = 0
      else:
          df = 1

      if df == real_df:
          correct = 1
      else:
          correct = 0
      aclist.append([index1,index2,real_df,correct])

  return aclist

#sc = SparkContext('spark://127.0.0.1:7077', 'pyspark')
sc = SparkContext( 'local', 'pyspark')
pairs = sc.parallelize(pairs)
acc = pairs.map(lambda x:getaccuracy(x)).collect()
acc_list = []
for ll in acc:
    acc_list.extend(ll)

print acc_list

rate=0.0
for result in acc_list:
    if result[3]==1:
        rate=rate+1
rate=rate/len(acc_list)

print rate
