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

extern "C"{
#include "init0.h"
#include "GP_IO.h"
#include "read_res.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
}

#define ADDR_BEO	0x20000000UL
#define SIZE_BEO	0x5000000UL
#define PIC_SIZE  0xFC00UL

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
		pPic   = (int*)malloc(sizeof(char)*(PIC_SIZE+1));
		feature = calloc(sizeof(char),10240);
		inputData = (float*)calloc(sizeof(float),112*96*3);
	}

	~ImageLogic()
	{
		os_.close();
		free(pPic);
		free(feature);
		free(inputData);
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
		int cols=obj["cols"].asInt();
		int rows=obj["rows"].asInt();
		int type=obj["type"].asInt();
		std::vector <uchar> ta;
		std::string decodata = davi_Decode(obj["data"].asString().c_str(),obj["data"].asString().length());
		ta.resize(decodata.size());
		ta.assign(decodata.begin(),decodata.end());
		cv::Mat mat = imdecode(cv::Mat(ta),CV_LOAD_IMAGE_COLOR);



//		###put new codes here###

		get_feature(mat, pPic, feature, inputData);

//		###put new codes above###


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
		 newobj["feature"]=Json::Value(feature);

		Json::FastWriter writer;
		std::string out = writer.write(newobj);
		Json::Value outobj;
		outobj.append(out);
		Tuple t(outobj);
		Emit(t);

	}

private:
	std::ofstream os_;
	std::unordered_map<std::string, int> kCount_;
	mtcnn *findimg;
	int *pPic;
	char* feature;
	float *inputData;

	int load_pic_hex(float* inputData,int nShift_bit,int pic_len, int* pPic){
			int i,j,nItem,nReadCount= 0;
			float fDat0=0.0;
			float fDat1 = 0.0;
			float fDat2 = 0.0;
			float fDat3 = 0.0;
			int nAllCount = pic_len/4;
			unsigned int uDat0,uDat1= 0,uDat2,uDat3;
			unsigned int uDat00,uDat11= 0,uDat22,uDat33;
			//char sbuf[256],sItem[16];
			char sItem[16],sbuf[256];
			float * _inputData = inputData;
			int lines = 0;
			for(i=0;i<3;i++){
				//printf("this is channel%d\n",i);
				_inputData = inputData+112*96*i;
					for(j=0;j<112*96;j=j+8)
					{
						lines += 1;

							fDat0 = *(_inputData+j);
							fDat1 = *(_inputData+j+1);
							fDat2 = *(_inputData+j+2);
							fDat3 = *(_inputData+j+3);

							uDat0 = (int)(fDat0*(1<<nShift_bit));
							uDat1 = (int)(fDat1*(1<<nShift_bit));
							uDat2 = (int)(fDat2*(1<<nShift_bit));
							uDat3 = (int)(fDat3*(1<<nShift_bit));


							fDat0 = *(_inputData+j+4);
							fDat1 = *(_inputData+j+4+1);
							fDat2 = *(_inputData+j+4+2);
							fDat3 = *(_inputData+j+4+3);
							uDat00 = (int)(fDat0*(1<<nShift_bit));
							uDat11 = (int)(fDat1*(1<<nShift_bit));
							uDat22 = (int)(fDat2*(1<<nShift_bit));
							uDat33 = (int)(fDat3*(1<<nShift_bit));
							sprintf(sbuf,"%04X%04X%04X%04X%04X%04X%04X%04X",uDat0&0xffff,uDat1&0xffff,uDat2&0xffff,uDat3&0xffff,uDat00&0xffff,uDat11&0xffff,uDat22&0xffff,uDat33&0xffff);
					//		fprintf(fp,"%04X%04X",uDat00&0xffff,uDat11&0xffff);
						//	fprintf(fp,"%04X%04X\n",uDat22&0xffff,uDat33&0xffff);
							//printf("%04X,%04X,%04X,%04X\n",sbuf[0],sbuf[1],sbuf[2],sbuf[3]);
						//	printf("%04X,%04X,%04X,%04X\n",sbuf[4],sbuf[5],sbuf[6],sbuf[7]);
						//	printf("this is j%d\n",lines);

							for(int ii=0;ii<32/8;ii++){
								memcpy(sItem,sbuf+ii*8,8);
								sItem[8]=0x00;
								int nRet=sscanf(sItem,"%x",&nItem);
								if(nRet<1){
									printf("sscanf error!\r\n");
									return -1;
								}
								*(pPic+nReadCount+3-ii)= (int)nItem;
							}

							nReadCount +=4;
						//	printf("##%d\n",nReadCount);
							if(nReadCount>=nAllCount)
								break;
							memset(sbuf,0,sizeof(char)*256);
					}

			}
			return 0;
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

	static void sleep_ms(unsigned int secs)

	{

	    struct timeval tval;

	    tval.tv_sec=secs/1000;

	    tval.tv_usec=(secs*1000)%1000000;

	    select(0,NULL,NULL,NULL,&tval);

	}

	void get_feature(cv::Mat mat, int *pPic, char* feature, float *inputData){
		init0();
		cv::Mat mat1;
		cv::Mat sample_float;
		int memfd_2;
		char dev[]="/dev/mem";

		//feature = calloc(sizeof(char),10240);
		memset(feature,0,sizeof(char)*10240);

		memfd_2 = open(dev, O_RDWR | O_SYNC);
		if (memfd_2 == -1)
		{
			fprintf(stderr,"Can't open %s.\n", dev);
			exit(0);
		}


		//float *inputData = (float*)calloc(sizeof(float),112*96*3);
		memset(inputData,0,sizeof(float)*112*96*3);

		//float *_inputData = inputData;
		//char *temp_str = (char*)calloc(sizeof(char),256);
	//	int *pPic   = (int*)malloc(sizeof(char)*(PIC_SIZE+1));

		void *mapped_base_dma_addr;

		std::vector<cv::Mat> channels;
		cv::resize(mat, mat1, cv::Size(96, 112), (0, 0), (0, 0), cv::INTER_LINEAR);
		mat1.convertTo(sample_float, CV_32FC3);

		cv::Mat sample_normalized;
		sample_normalized = (sample_float-127.5)/128.0;//	cv::subtract(sample_float,m, sample_normalized);
		cv::split(sample_normalized, channels);

		for (size_t i = 0; i < channels.size(); ++i) {
			/*channels[i] -= IMG_MEAN;
			  channels[i] *= IMG_INV_STDDEV;*/
			//	printf("channels is %d,%d,%d\n",channels.size(),sample_normalized.cols,sample_normalized.rows);
			memcpy(inputData+ i*sample_normalized.cols * sample_normalized.rows, channels[i].data, sizeof(float) * sample_normalized.cols * sample_normalized.rows);
			//	std::cout <<"inputData: "<< inputData[200] << std::endl;
			//_inputData += sample_normalized.cols * sample_normalized.rows;
		}

		load_pic_hex(inputData,14,PIC_SIZE,pPic);

		mapped_base_dma_addr = mmap(0, SIZE_BEO, PROT_READ | PROT_WRITE, MAP_SHARED, memfd_2, ADDR_BEO);
		close(memfd_2);
		if (mapped_base_dma_addr == (void *) -1)
		{
			fprintf(stderr,"Can't map the memory to user space.\n");
			free(pPic);
			exit(0);
		}
		fprintf(stderr,"DDR Memory mapped at address %p.\n", mapped_base_dma_addr);

		int num=0;
		for(int ii=0;ii<PIC_SIZE/4;ii++)
		{
			/*if ((i & 0x3)==0)
			  num=i>>2;
			  else
			  num=0;*/
			*(((unsigned int*)mapped_base_dma_addr)+ii) = *(pPic+ii);
			//if ((ii & 0xffff)==0)
		   //  printf ("%d\n", ii);
		}

		if (munmap(mapped_base_dma_addr, SIZE_BEO) == -1)
		{
			fprintf(stderr,"Can't unmap the memory to user space.\n");
			free(pPic);
			exit(0);
		}
		fprintf(stderr,"unmap successed.\n");

		GP_IO("write","10000", "0");
		GP_IO("read","08000", "0");
		sleep_ms(2);
		GP_IO("write","10050", "10000");
		GP_IO("write","10060", "34816E0");
		GP_IO("write","100b0", "0");
		sleep_ms(2);

		GP_IO("write","10030", "20000000");
		GP_IO("write","10040", "0000FC00");
		GP_IO("write","10090", "0");

		GP_IO("write","10070", "20010000");

		GP_IO("write","10110", "400");
		GP_IO("write","100a0", "0");
		GP_IO("write","100d0", "0");
	//	sleep_ms(45);
        uint64_t ret;
        uint32_t ret_;
      ret =  _GP_IO("read","08000");
      ret_ = ret&0x8000;
        while(ret_ != 0){
                ret = _GP_IO("read","08000");
                ret_ = ret&0x8000;
                sleep_ms(1);
        }

		read_res(feature);
	}
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
