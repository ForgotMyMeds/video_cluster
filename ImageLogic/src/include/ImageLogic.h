/*
 * ImageLogic.h
 *
 *  Created on: 2017年10月16日
 *      Author: root
 */

#ifndef INCLUDE_IMAGELOGIC_H_
#define INCLUDE_IMAGELOGIC_H_
#include <string>
#include <vector>
#include <stdlib.h>
#include <fstream>
#include <unistd.h>
#include <unordered_map>

#include <jsoncpp/json/json.h>
#include "Storm.h"
#include "decode.h"
#include "ZBase64.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "mtcnn.h"
#include "helpers.hpp"

using namespace std;

namespace storm
{

size_t split(const std::string &src, const std::string &delim, std::vector<std::string> &dst)
{
	dst.clear();
	size_t idx = 0;
	size_t pos = src.find(delim, idx);
	while (pos != std::string::npos)
	{
		dst.push_back(src.substr(idx, pos - idx));
		idx = pos + delim.length();
		pos = src.find(delim, idx);
	}

	dst.push_back(src.substr(idx));
	return dst.size();
}

class ImageLogic : public Bolt
{
public:
	ImageLogic()
	{
		os_.open("/mnt/storm_out/test.txt", std::ios_base::out | std::ios_base::app);
		findimg=new mtcnn(480,640);
	//	input = (unsigned char*)calloc(sizeof(unsigned char),32258);
	//	output = (unsigned char*)calloc(sizeof(unsigned char),BASE64_ENCODE_OUT_SIZE(32258));
	}

	~ImageLogic()
	{
		os_.close();
	//	free(input);
	//	free(output);
	}

	void Initialize(Json::Value conf, Json::Value context) {
		//cout<<"david Initial modle here."<<endl;
	}
	void Process(Tuple &tuple)
	{
		int i = 0;
		std::string s = tuple.GetValues()[i].asString();
	//	std::string cameraId = tuple.GetValues()[1].asString();
		kCount_[s]++;
	//	os_ << s << ":" << kCount_[s] << std::endl;
		Json::Reader reader;
		Json::Value obj;
		if (!reader.parse(s, obj))
		{
			os_ << "Json object creation failed!!!" << std::endl;
			os_ << s << ":" << kCount_[s] << std::endl;
			return;
		}
		std::string cameraId=obj["cameraId"].asString();
		std::string timestamp=obj["timestamp"].asString();
		//std::string data=b64decodestring(obj["data"].asString());
		char *test_img = b64decodestring(obj["data"].asString()).c_str();
		//char *data1;
		//data1=new char[data.length()+1];
		//strcpy(data1,data.c_str());
		int cols=obj["cols"].asInt();
		int rows=obj["rows"].asInt();
		int type=obj["type"].asInt();
		cv::Mat mat(rows, cols, type, test_img);
		/*imwrite("/mnt/storm_out/"+cameraId+timestamp+".png",mat);
		os_ << "Created image: /mnt/storm_out/"<<cameraId<<timestamp<<".png" << std::endl;*/

		vector<struct Bbox> outbox;
		if (findimg->findFace(mat,outbox))
		{
			string iii="facei";
		    for(vector<struct Bbox>::iterator it=outbox.begin(); it!=outbox.end();it++){

		        if((*it).exist){
		//	    printf("face score is:%f\n",it->score);
		          //  rectangle(mat, Point((*it).y1, (*it).x1), Point((*it).y2, (*it).x2), Scalar(0,0,255), 2,8,0);
		          //  for(int num=0;num<5;num++)circle(mat,Point((int)*(it->ppoint+num), (int)*(it->ppoint+num+5)),3,Scalar(0,255,255), -1);
		            Point2f eyesCenter = Point2f( ((int)*(it->ppoint) +(int)*(it->ppoint+1)) * 0.5f, ((int)*(it->ppoint+5) + (int)*(it->ppoint+6)) * 0.5f );
		        //    circle(image,eyesCenter,3,Scalar(0,255,255), -1);
		            double dy = (int)*(it->ppoint+6) - (int)*(it->ppoint+5);
		            double dx = (int)*(it->ppoint+1) - (int)*(it->ppoint);

		            double angle = atan2(dy, dx) * 180.0/CV_PI;
		       //     printf("%f  %f   %f\n",dx,dy,angle);
		            Mat rot_mat = getRotationMatrix2D(eyesCenter, angle, 1.0);
		            Mat warp_frame;
		            warpAffine(mat, warp_frame, rot_mat, warp_frame.size());

		            cv::Rect r(Point((*it).y1, (*it).x1), Point((*it).y2, (*it).x2));
		            Mat cropped_img = cropImage(warp_frame,r);
		            Mat resized_img;
		            cv::resize(cropped_img, resized_img, cv::Size(96, 112));
		   //         uchar *pdata = resized_img.data;
					  // if (strlen(pdata)<32257)
		           //   os_ << "pdata:"<< strlen(pdata)<< std::endl;
				   //   imwrite("/mnt/storm_out/"+cameraId+timestamp+"_result"+iii+".png",resized_img);
					//   os_ << "Created image: /mnt/storm_out/"<<cameraId<<timestamp<<"result"+iii+".png" << std::endl;
					   iii=iii+"i";

					//   uchar *pdata = resized_img.data;
					   Json::Value newobj;
					   //string newdata((char*)pdata);
					//   if (strlen(pdata)>0)

				//	   bzero(input,32258);
				//	   memcpy(input,pdata,32258);
				//	   bzero(output,BASE64_ENCODE_OUT_SIZE(32258));

					   	// os_ << "resized_img.total:"<<total<< std::endl;

					  // 	unsigned char *decode = calloc(sizeof(unsigned char),rows*cols+1);

			//		   base64_encode(input, 32258, output);
			//		   string newdata((char*)output);


//################PLAN B
					   vector<uchar> vecImg;                               //Mat 图片数据转换为vector<uchar>
					   vector<int> vecCompression_params;
					   vecCompression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
					   vecCompression_params.push_back(90);
					   imencode(".jpg", resized_img, vecImg, vecCompression_params);

					   ZBase64 base64;
					   string newdata = base64.Encode(vecImg.data(), vecImg.size());
					   newobj["data"]=Json::Value(newdata);

					 //  	base64_decode(output, BASE64_ENCODE_OUT_SIZE(rows*cols+1) -1, decode);
					  // Json::Value davi_test;



					   newobj["cameraId"]=Json::Value(cameraId+iii);
					   newobj["timestamp"]=Json::Value(timestamp);
					   newobj["rows"]=Json::Value(rows);
					   newobj["cols"]=Json::Value(cols);
					   newobj["type"]=Json::Value(type);

					   //davi_test.append(newobj);
					   //   std::string out = newobj.toStyledString();
					   Json::FastWriter writer;
					   std::string out = writer.write(newobj);
					   Json::Value outobj;
					   outobj.append(out);
					   Tuple t(outobj);
					   Emit(t);

		         }
		     }
		}
	/*	else
		{
		    imwrite("/mnt/storm_out/"+cameraId+timestamp+".png",mat);
			os_ << "Created image: /mnt/storm_out/"<<cameraId<<timestamp<<".png" << std::endl;
		}*/

	}

private:
	std::ofstream os_;
	std::unordered_map<std::string, int> kCount_;
	mtcnn *findimg;
//	unsigned char *input;
//	unsigned char *output;
};

class SplitSentence : public Bolt
{
	public:
		void Initialize(Json::Value conf, Json::Value context) {
			cout<<"#david#Initial modle here."<<endl;
		}
		void Process(Tuple &tuple)
		{
			int i = 0;
			std::string s = tuple.GetValues()[i].asString();
			cout<<s<<endl;

			//std::ofstream os;
		//	os.open("/usr/local/storm/bin/split", std::ios_base::out | std::ios_base::app);
		//	os << s << std::endl;
			//os.close();

			std::vector<std::string> tokens;
			split(s, " ", tokens);
			for (int i = 0; i < tokens.size(); ++i)
			{
				Json::Value j_token;
				j_token.append(tokens[i]);
				Tuple t(j_token);
				Emit(t);
			}
		}
};

}

#endif /* INCLUDE_IMAGELOGIC_H_ */
