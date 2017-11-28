# coding=utf-8
import cmodel
import numpy as np
feature = np.random.random((1,512)).astype('float32')
#feature = cmodel.feature_main('/home/imgs/Aaron_Eckhart_0001.jpg',feature)
feature = cmodel.feature_main('./test_face.jpg',feature)
print feature

