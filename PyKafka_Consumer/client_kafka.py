from pykafka import KafkaClient
import json
import base64
import cv2
import cv
import numpy as np
from elasticsearch import Elasticsearch

QUERY = """
{
"_source": ["im_src", "cl", "coords"],
"query": {
  "function_score" : {
    "query" : {
      "match_all" : {
        "boost" : 1.0
      }
    },
    "functions" : [
      {
        "filter" : {
          "match_all" : {
            "boost" : 1.0
          }
        },
        "script_score" : {
          "script" : {
            "inline" : "cosine_score",
            "lang" : "native",
            "params" : {
              "f" : "sigs",
              "fea" : [##fea##],
              "verbose" : true
            }
          }
        }
      }
    ],
    "score_mode" : "sum",
    "boost_mode" : "replace",
    "max_boost" : 3.4028235E38,
    "boost" : 1.0
  }
}
}
"""

queries = []
results = []

es = Elasticsearch(hosts='localhost:9200')

client = KafkaClient(hosts="129.168.6.53:9092")
topic = client.topics[b'output1']
consumer = topic.get_simple_consumer(
   # consumer_group=b'test1',
    auto_commit_enable=True,
    auto_commit_interval_ms=1,
        consumer_id=b'test'
)
for x in consumer:
    if x is not None:
        meta = json.loads(x.value.decode('utf-8'))
        pdata =  base64.b64decode(meta['data'])
        #feature = base64.b64decode(meta['feature'])
        feature = meta['feature']
        feature = feature[:-1]
        if pdata is not None:
            nparr = np.fromstring(pdata, np.uint8)
            img_np = cv2.imdecode(nparr, cv2.CV_LOAD_IMAGE_COLOR)
            img_ipl = cv.CreateImageHeader((img_np.shape[1], img_np.shape[0]), cv.IPL_DEPTH_8U, 3)
            cv.SetData(img_ipl, img_np.tostring(), img_np.dtype.itemsize * 3 * img_np.shape[1])
            filename = "test"
            #  cv.imwrite(filename,mat)
            #string_featrue = feature.split(',')
	   # print string_featrue
           # float_feature = []
            #for temp in string_featrue:
             #   float_feature.append(float(temp))

            cv.ShowImage("output", img_ipl)
            cv.WaitKey(1)

            #fea_str = ','.join([str(t) for t in float_feature])
            query = QUERY.replace('##fea##', feature)
            queries.append(query)
           # print "###############"
            #print(queries)
            #print "###############"
            for query_i in queries:
                result = es.search(index='img_data', doc_type='obj', body=query_i)
                results.append(result)
            rs = []
            for result_i in results:
                if 'hits' in result_i and \
                                'hits' in result['hits']:
                    # distinct
                    all_imgs = set([])
                    hits = result_i['hits']['hits']
                    for hit in hits:
                        o = hit['_source']
                        o['score'] = hit['_score']
			if o['score'] > 0.985:
                            print o['score']
                            print o['im_src']
			    print "______________________________"
