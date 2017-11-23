#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <jsoncpp/json/json.h>
#include "include/decode.h"
#include "include/ZBase64.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "include/mtcnn.h"
#include "include/helpers.hpp"
#include <librdkafka/rdkafka.h>
#include<time.h>

static int run = 1;

static void stop(int sig){
	run = 0;
	fclose(stdin);
}

/*
    每条消息调用一次该回调函数，说明消息是传递成功(rkmessage->err == RD_KAFKA_RESP_ERR_NO_ERROR)
    还是传递失败(rkmessage->err != RD_KAFKA_RESP_ERR_NO_ERROR)
    该回调函数由rd_kafka_poll()触发，在应用程序的线程上执行
 */
static void dr_msg_cb(rd_kafka_t *rk,const rd_kafka_message_t *rkmessage, void *opaque){
	if(rkmessage->err)
		fprintf(stderr, "%% Message delivery failed: %s\n",rd_kafka_err2str(rkmessage->err));
	else
		fprintf(stderr,"%% Message delivered (%d bytes, partition %d PRId32)\n",
                        rkmessage->len, rkmessage->partition);
        /* rkmessage被librdkafka自动销毁*/
}

void string_replace(string&s1,const string&s2,const string&s3)
{
	string::size_type pos=0;
	string::size_type a=s2.size();
	string::size_type b=s3.size();
	while((pos=s1.find(s2,pos))!=string::npos)
	{
		s1.replace(pos,a,s3);
		pos+=b;
	}
}

std::vector<string> process_Img(mtcnn* findimg,Mat mat,const char* cam){
	vector<struct Bbox> outbox;
	vector<string> out;
			if (findimg->findFace(mat,outbox))
			{
				string iii="_face";
				time_t now;
				struct tm *timenow;
				time(&now);
				timenow = localtime(&now);
				string timestamp = asctime(timenow);
				string_replace(timestamp," ","-");     //去掉空格
				timestamp.erase(timestamp.end()-1);    //去掉最后的换行符

				string cameraId=cam;
			    for(vector<struct Bbox>::iterator it=outbox.begin(); it!=outbox.end();it++){

			        if((*it).exist){

			            Point2f eyesCenter = Point2f( ((int)*(it->ppoint) +(int)*(it->ppoint+1)) * 0.5f, ((int)*(it->ppoint+5) + (int)*(it->ppoint+6)) * 0.5f );
			            double dy = (int)*(it->ppoint+6) - (int)*(it->ppoint+5);
			            double dx = (int)*(it->ppoint+1) - (int)*(it->ppoint);

			            double angle = atan2(dy, dx) * 180.0/CV_PI;
			            Mat rot_mat = getRotationMatrix2D(eyesCenter, angle, 1.0);
			            Mat warp_frame;
			            warpAffine(mat, warp_frame, rot_mat, warp_frame.size());

			            cv::Rect r(Point((*it).y1, (*it).x1), Point((*it).y2, (*it).x2));
			            Mat cropped_img = cropImage(warp_frame,r);
			            Mat resized_img;
			            cv::resize(cropped_img, resized_img, cv::Size(96, 112));
						   iii=iii+"i";

						   Json::Value newobj;

						   vector<uchar> vecImg;                               //Mat 图片数据转换为vector<uchar>
						  	vector<int> vecCompression_params;
						  	vecCompression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
						  	vecCompression_params.push_back(90);
						  	imencode(".jpg", resized_img, vecImg, vecCompression_params);

						  	ZBase64 base64;
						  	string newdata = base64.Encode(vecImg.data(), vecImg.size());

						   newobj["data"]=Json::Value(newdata);
						   newobj["cameraId"]=Json::Value(cameraId+iii);
						   newobj["timestamp"]=Json::Value(timestamp);
						   newobj["rows"]=Json::Value(resized_img.rows);
						   newobj["cols"]=Json::Value(resized_img.cols);
						   newobj["type"]=Json::Value(resized_img.type());
						 //  os_ << "Created image: "<<cameraId<<timestamp<< std::endl;
						   //davi_test.append(newobj);
						   //   std::string out = newobj.toStyledString();
						   Json::FastWriter writer;
						   out.push_back(writer.write(newobj));

			         }
		//			Ack(tuple.GetID());
			    }
			}
		return out;
}
int main(int argc, char **argv){

	mtcnn *findimg;
	Mat video_frame;
	findimg=new mtcnn(480,640);
	VideoCapture cap(0);
	std::vector<string> pic_out;
    if(!cap.isOpened())
    {
        return -1;
    }

	rd_kafka_t *rk;            /*Producer instance handle*/
	rd_kafka_topic_t *rkt;     /*topic对象*/
	rd_kafka_conf_t *conf;     /*临时配置对象*/
	char errstr[512];
	//char buf[512];
	const char *brokers;
	const char *topic;
	const char *cam="cam";

	if(argc != 3 && argc != 4){
		fprintf(stderr, "%% Usage: %s <broker> <topic> [<camID>]\n", argv[0]);
        return 1;
	}

	brokers = argv[1];
	topic = argv[2];
	if (argc == 4)
		cam = argv[3];

    /* 创建一个kafka配置占位 */
	conf = rd_kafka_conf_new();

    /*创建broker集群*/
	if (rd_kafka_conf_set(conf, "bootstrap.servers", brokers, errstr,
				sizeof(errstr)) != RD_KAFKA_CONF_OK){
		fprintf(stderr, "%s\n", errstr);
		return 1;
	}

    /*设置发送报告回调函数，rd_kafka_produce()接收的每条消息都会调用一次该回调函数
     *应用程序需要定期调用rd_kafka_poll()来服务排队的发送报告回调函数*/
	rd_kafka_conf_set_dr_msg_cb(conf, dr_msg_cb);

    /*创建producer实例
      rd_kafka_new()获取conf对象的所有权,应用程序在此调用之后不得再次引用它*/
	rk = rd_kafka_new(RD_KAFKA_PRODUCER, conf, errstr, sizeof(errstr));
	if(!rk){
		fprintf(stderr, "%% Failed to create new producer:%s\n", errstr);
		return 1;
	}

    /*实例化一个或多个topics(`rd_kafka_topic_t`)来提供生产或消费，topic
    对象保存topic特定的配置，并在内部填充所有可用分区和leader brokers，*/
	rkt = rd_kafka_topic_new(rk, topic, NULL);
	if (!rkt){
		fprintf(stderr, "%% Failed to create topic object: %s\n",
				rd_kafka_err2str(rd_kafka_last_error()));
		rd_kafka_destroy(rk);
		return 1;
	}

    /*用于中断的信号*/
	signal(SIGINT, stop);

	fprintf(stderr,
                "%% Type some text and hit enter to produce message\n"
                "%% Or just hit enter to only serve delivery reports\n"
                "%% Press Ctrl-C or Ctrl-D to exit\n");

   //  while(run && fgets(buf, sizeof(buf), stdin)){

	 while(run){
		cap>>video_frame;
		imshow("raw_video", video_frame);
		waitKey(10);
		pic_out =  process_Img(findimg,video_frame,cam);
	    for(vector<string>::iterator it=pic_out.begin(); it!=pic_out.end();it++){


     	if((*it).length() == 0){
            /*轮询用于事件的kafka handle，
            事件将导致应用程序提供的回调函数被调用
            第二个参数是最大阻塞时间，如果设为0，将会是非阻塞的调用*/
     		rd_kafka_poll(rk, 0);
     		continue;
     	}

     retry:
         /*Send/Produce message.
           这是一个异步调用，在成功的情况下，只会将消息排入内部producer队列，
           对broker的实际传递尝试由后台线程处理，之前注册的传递回调函数(dr_msg_cb)
           用于在消息传递成功或失败时向应用程序发回信号*/
     	if (rd_kafka_produce(
                    /* Topic object */
     				rkt,
                    /*使用内置的分区来选择分区*/
     				RD_KAFKA_PARTITION_UA,
                    /*生成payload的副本*/
     				RD_KAFKA_MSG_F_COPY,
                    /*消息体和长度*/
     				(*it).c_str(), (*it).length(),
                    /*可选键及其长度*/
     				NULL, 0,
     				NULL) == -1){
     		fprintf(stderr,
     			"%% Failed to produce to topic %s: %s\n",
     			rd_kafka_topic_name(rkt),
     			rd_kafka_err2str(rd_kafka_last_error()));

     		if (rd_kafka_last_error() == RD_KAFKA_RESP_ERR__QUEUE_FULL){
                /*如果内部队列满，等待消息传输完成并retry,
                内部队列表示要发送的消息和已发送或失败的消息，
                内部队列受限于queue.buffering.max.messages配置项*/
     			rd_kafka_poll(rk, 1000);
     			goto retry;
     		}
     	}else{
     		fprintf(stderr, "%% Enqueued message (%zd bytes) for topic %s\n",
     			(*it).length(), rd_kafka_topic_name(rkt));
     	}

        /*producer应用程序应不断地通过以频繁的间隔调用rd_kafka_poll()来为
        传送报告队列提供服务。在没有生成消息以确定先前生成的消息已发送了其
        发送报告回调函数(和其他注册过的回调函数)期间，要确保rd_kafka_poll()
        仍然被调用*/
     	rd_kafka_poll(rk, 0);
	 	 }
     }

     fprintf(stderr, "%% Flushing final message.. \n");
     /*rd_kafka_flush是rd_kafka_poll()的抽象化，
     等待所有未完成的produce请求完成，通常在销毁producer实例前完成
     以确保所有排列中和正在传输的produce请求在销毁前完成*/
     rd_kafka_flush(rk, 10*1000);

     /* Destroy topic object */
     rd_kafka_topic_destroy(rkt);

     /* Destroy the producer instance */
     rd_kafka_destroy(rk);

     return 0;
}
