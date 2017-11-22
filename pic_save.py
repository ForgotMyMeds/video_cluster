import time
import base64
import json
from kafka import KafkaClient, SimpleProducer, SimpleConsumer
import numpy as np
import cv2
from cv2 import cv

kafka = KafkaClient("129.168.6.53:9092")
consumer = SimpleConsumer(kafka,"python","output1",max_buffer_size=9932768)
for msg in consumer:
  print msg.offset, msg.message.value
  jsonobj = json.loads(msg.message.value)
  #print jsonobj['data']
  pdata = base64.b64decode(jsonobj['data'])
#  print pdata
  #mat= cv.CreateMat(jsonobj['rows'], jsonobj['cols'], jsonobj['type'])
  #cv.imshow("Image", mat) 
  nparr = np.fromstring(pdata, np.uint8)
  img_np = cv2.imdecode(nparr, cv2.CV_LOAD_IMAGE_COLOR) 
  img_ipl = cv.CreateImageHeader((img_np.shape[1], img_np.shape[0]), cv.IPL_DEPTH_8U, 3)
  cv.SetData(img_ipl, img_np.tostring(), img_np.dtype.itemsize * 3 * img_np.shape[1])

#  mat = cv.pImgMat(img_ipl,0);
  filename="/home/zym/storm_out/"+jsonobj['cameraId']+jsonobj['timestamp']+".png"
#  cv.imwrite(filename,mat)
  cv.ShowImage("output", img_ipl)
  cv.SaveImage(filename, img_ipl)
  cv.WaitKey(42)
  if not msg.message.value.strip():
    break
#  time.sleep(2)
kafka.close()
