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
    	os_.open("/home/zym/storm_out/test.txt", std::ios_base::out | std::ios_base::app);
		findimg=new mtcnn(480,640);
	}

	~ImageLogic()
	{
		os_.close();
	}

	void Initialize(Json::Value conf, Json::Value context) {
		//cout<<"david Initial modle here."<<endl;
	}
	std::string davi_Decode(const char* Data,int DataByte)
	{
	    //解码表
	    const char DecodeTable[] =
	    {
	        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	        62, // '+'
	        0, 0, 0,
	        63, // '/'
	        52, 53, 54, 55, 56, 57, 58, 59, 60, 61, // '0'-'9'
	        0, 0, 0, 0, 0, 0, 0,
	        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
	        13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, // 'A'-'Z'
	        0, 0, 0, 0, 0, 0,
	        26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
	        39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, // 'a'-'z'
	    };
	    //返回值
	    std::string strDecode;
	    int nValue;
	    int i= 0;
	    while (i < DataByte)
	    {
	        if (*Data != '\r' && *Data!='\n')
	        {
	            nValue = DecodeTable[*Data++] << 18;
	            nValue += DecodeTable[*Data++] << 12;
	            strDecode+=(nValue & 0x00FF0000) >> 16;
	         //   OutByte++;
	            if (*Data != '=')
	            {
	                nValue += DecodeTable[*Data++] << 6;
	                strDecode+=(nValue & 0x0000FF00) >> 8;
	           //     OutByte++;
	                if (*Data != '=')
	                {
	                    nValue += DecodeTable[*Data++];
	                    strDecode+=nValue & 0x000000FF;
	               //     OutByte++;
	                }
	            }
	            i += 4;
	        }
	        else// 回车换行,跳过
	        {
	            Data++;
	            i++;
	        }
	     }
	    return strDecode;
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
		int cols=obj["cols"].asInt();
		int rows=obj["rows"].asInt();
		int type=obj["type"].asInt();
		std::vector <uchar> ta;
		std::string decodata = davi_Decode(obj["data"].asString().c_str(),obj["data"].asString().length());
		ta.resize(decodata.size());
		ta.assign(decodata.begin(),decodata.end());
		cv::Mat mat = imdecode(cv::Mat(ta),CV_LOAD_IMAGE_COLOR);

//		###put new codes here###




		Json::Value newobj;
		vector<uchar> vecImg;                               //Mat 图片数据转换为vector<uchar>
		vector<int> vecCompression_params;
		vecCompression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
		vecCompression_params.push_back(90);
		imencode(".jpg", mat, vecImg, vecCompression_params);

		 ZBase64 base64;
		 string newdata = base64.Encode(vecImg.data(), vecImg.size());

		 newobj["data"]=Json::Value(newdata);
		 newobj["cameraId"]=Json::Value(cameraId);
		 newobj["timestamp"]=Json::Value(timestamp);
		 newobj["rows"]=Json::Value(mat.rows);
		 newobj["cols"]=Json::Value(mat.cols);
		 newobj["type"]=Json::Value(mat.type());

		Json::FastWriter writer;
		std::string out = writer.write(obj);
		Json::Value outobj;
		outobj.append(out);
		Tuple t(outobj);
		Emit(t);

	}

private:
	std::ofstream os_;
	std::unordered_map<std::string, int> kCount_;
	mtcnn *findimg;
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
