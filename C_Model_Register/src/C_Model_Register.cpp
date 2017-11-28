#include <iostream>
#include "glog/logging.h"
#include <iomanip>
#include <string>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <boost/python.hpp>
#include <numpy/ndarrayobject.h>
namespace bp = boost::python;
#define PIC_SIZE 0xFC00UL
extern std::vector<float> extract_feature(short* img);

extern "C" {
int load_pic_hex(float* inputData,int nShift_bit,int pic_len, short* pPic){
		int i,j,nItem,nReadCount= 0;
		float fDat0=0.0;
		float fDat1 = 0.0;
		float fDat2 = 0.0;
		float fDat3 = 0.0;
		int nAllCount = pic_len/4;
		unsigned int uDat0,uDat1= 0,uDat2,uDat3;
		unsigned int uDat00,uDat11= 0,uDat22,uDat33;
		//char sbuf[256],sItem[16];
		//char sItem[16],sbuf[256];
		float * _inputData = inputData;
		int lines = 0;
		for(i=0;i<3;i++){
			printf("this is channel%d\n",i);
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
						//sprintf(sbuf,"%04X%04X%04X%04X%04X%04X%04X%04X",uDat0&0xffff,uDat1&0xffff,uDat2&0xffff,uDat3&0xffff,uDat00&0xffff,uDat11&0xffff,uDat22&0xffff,uDat33&0xffff);
				//		fprintf(fp,"%04X%04X",uDat00&0xffff,uDat11&0xffff);
					//	fprintf(fp,"%04X%04X\n",uDat22&0xffff,uDat33&0xffff);
						//printf("%04X,%04X,%04X,%04X\n",sbuf[0],sbuf[1],sbuf[2],sbuf[3]);
					//	printf("%04X,%04X,%04X,%04X\n",sbuf[4],sbuf[5],sbuf[6],sbuf[7]);
					//	printf("this is j%d\n",lines);
						*(pPic +i*112*96 + j + 0) = short(uDat0&0xffff);
						*(pPic +i*112*96+ j + 1) = short(uDat1&0xffff);
						*(pPic +i*112*96+ j + 2) = short(uDat2&0xffff);
						*(pPic+i*112*96 + j + 3) = short(uDat3&0xffff);

						*(pPic+i*112*96 + j + 4) = short(uDat00&0xffff);
						*(pPic+i*112*96 + j + 5) = short(uDat11&0xffff);
						*(pPic+i*112*96 + j + 6) = short(uDat22&0xffff);
						*(pPic+i*112*96 + j + 7) = short(uDat33&0xffff);


						/*for(int ii=0;ii<32/8;ii++){
							memcpy(sItem,sbuf+ii*8,8);
							sItem[8]=0x00;
							int nRet=sscanf(sItem,"%x",&nItem);
							if(nRet<1){
								printf("sscanf error!\r\n");
								return -1;
							}
							*(pPic+nReadCount+3-ii)= (short)nItem;
						}

						nReadCount +=4;
					//	printf("##%d\n",nReadCount);
						if(nReadCount>=nAllCount)
							break;
						memset(sbuf,0,sizeof(char)*256);*/
				}

		}
		return 0;
}

std::vector<float> extract_feature_(cv::Mat mat){
	//cv::Mat mat = cv::imread(filename);
	cv::Mat mat1;
	cv::Mat sample_float;

	float *inputData = (float*)calloc(sizeof(float),112*96*3);
	//float *_inputData = inputData;
	//char *temp_str = (char*)calloc(sizeof(char),256);
	int nBuffInSize = 64*94*110*sizeof(short);
	short* pPic = (short*)malloc(sizeof(short)*nBuffInSize);
		if(pPic==NULL){
			printf("malloc pbuffin error!\r\n");
		}
   //int *pPic   = (int*)malloc(sizeof(char)*(PIC_SIZE+1));

 //  void *mapped_base_dma_addr;

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
		//_inputData += sample_normalized.cols * sample_normalized.rows;
	}

	load_pic_hex(inputData,14,PIC_SIZE,pPic);
	std::vector<float> result = extract_feature(pPic);
	/*for(int j = 0; j < 512; j++){
		printf("%f,",result[j]);
	}*/

	free(pPic);
	free(inputData);
	return result;
	}


void reference_contiguous_array(PyObject* in,
        PyArrayObject* &in_con,
        double* &ptr, int &count)
{
    in_con = PyArray_GETCONTIGUOUS((PyArrayObject*)in);
    ptr = (double*)PyArray_DATA(in_con);
    int num_dim = PyArray_NDIM(in_con);
    npy_intp* pdim = PyArray_DIMS(in_con);
  //  printf("+++++++[0]is %d [1]%d\n",pdim[0],pdim[1]);
    //printf("+++++++data is %f\n",ptr);
    count = 1;
    for (int i = 0; i < num_dim; i++)
    {
        count *= pdim[i];
    }
}

void dereference(PyObject* o)
{
    Py_DECREF(o);
}

PyObject* feature_main(char* filename,PyObject* input_matrix)
{
    // get the input array

    double* ptr;
    int count;
    PyArrayObject* input_contigous_array;
    reference_contiguous_array(input_matrix, input_contigous_array, ptr, count);

    // create the output array
    npy_intp dst_dim[1];
    dst_dim[0] = count;
    PyObject* out_matrix = PyArray_SimpleNew(1, dst_dim, NPY_FLOAT64);
    double* ptr_out;
    PyArrayObject* output_contigous_array;
    reference_contiguous_array(out_matrix, output_contigous_array, ptr_out, count);

   std::vector<double> output_all;
 	//google::InitGoogleLogging("do_extract");
	//FLAGS_stderrthreshold = google::ERROR;
	char strTemp[sizeof(float)];
	std::string strTotal;
	std::string pic_name;

	std::ofstream noface;
	noface.open("./noface.txt");

	/*std::ifstream fin(filename);
	if(!fin){
		std::cerr <<"OPEN ERROR" << std::endl;
		return NULL;
	}*/

	std::vector<float> feature;
//	while(std::getline(fin, pic_name)){
		std::vector<cv::Mat> feature_davi;
		//	std::string path_and_name = PATH + "/" + pic_name;
		std::string path_and_name = filename;
		count += 1;
		std::cout<< "Current picturename is :"  << path_and_name <<std::endl;
		cv::Mat image = cv::imread(path_and_name);
		if(!image.data){
			std::cout << "No image data" << std::endl;
			noface <<path_and_name<< std::endl;
	 		feature.clear();
	 		for(int i = 0; i < 512;i++){
	          feature.push_back(0);
	        }
		}else{
		//cv::Mat img = histo(image);

	 	feature.clear();
		feature = extract_feature_(image);
		if(feature.size() == 0){
	 		feature.clear();
			noface <<path_and_name<< std::endl;
			for(int i = 0; i < 1024;i++){
                feature.push_back(0);
                }
		}
		//std::cout << "Elapsed time (ms): " << timer.stop() << std::endl;
	  // output_all.insert(output_all.end(),feature.begin(),feature.end());
		}
		output_all.insert(output_all.end(),feature.begin(),feature.end());

	//}

 for(int m = 0; m < output_all.size();m++){
    	ptr_out[m] = output_all.at(m);
 }
    dereference((PyObject*)input_contigous_array);
    dereference((PyObject*)output_contigous_array);

	//fin.close();
	noface.close();
   //google::ShutdownGoogleLogging();
   return out_matrix;
}

BOOST_PYTHON_MODULE(cmodel)
{
   import_array();
   bp::def("feature_main", feature_main);
}

}
