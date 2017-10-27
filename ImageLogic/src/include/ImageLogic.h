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
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "mtcnn.h"

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
	}

	~ImageLogic()
	{
		os_.close();
	}

	void Initialize(Json::Value conf, Json::Value context) {
		//cout<<"david Initial modle here."<<endl;
	}
	void Process(Tuple &tuple)
	{
		int i = 0;
		std::string s = tuple.GetValues()[i].asString();
		kCount_[s]++;
	//	os_ << s << ":" << kCount_[s] << std::endl;
		Json::Reader reader;
		Json::Value obj;
		if (!reader.parse(s, obj))
		{
			os_ << "Json object creation failed!!!" << std::endl;
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

		findimg->findFace(mat);
	    imwrite("/mnt/storm_out/"+cameraId+timestamp+"_result.png",mat);
		os_ << "Created image: /mnt/storm_out/"<<cameraId<<timestamp<<"result.png" << std::endl;

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
