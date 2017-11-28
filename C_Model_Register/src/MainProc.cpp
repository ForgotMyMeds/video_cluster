
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "Define.h"
#include <opencv2/opencv.hpp>
#include <iostream>



std::vector<float> extract_feature(short* img)
{
	 std::vector<float> output;
	FILE *fp =NULL;
	int nRet=0;
	float fRet = 0.0;
	char sbuf[512]={0x0};
	float fBuf[1024]={0x0};
	

//	float f_data[3][9]={{6.1,6.2,6.3,6.4,6.5,6.6,6.7,6.8,6.9},{7.1,7.2,7.3,7.4,7.5,7.6,7.7,7.8,7.9},{8.1,8.2,8.3,8.4,8.5,8.6,8.7,8.8,8.9}};
//	float f_weight[3][9]={{6.1,6.2,6.3,6.4,6.5,6.6,6.7,6.8,6.9},{7.1,7.2,7.3,7.4,7.5,7.6,7.7,7.8,7.9},{8.1,8.2,8.3,8.4,8.5,8.6,8.7,8.8,8.9}};

	unsigned short data[3][9]={{0xC080,0xBB80,0xBC80,0xB980,0xB680,0xBA80,0xB580,0xB480,0xB880},{0xC880,0xC280,0xBF80,0xC080,0xBC80,0xBD80,0xBB80,0xB980,0xBA80},\
	{0x2680,0x2380,0x2880,0x2180,0x1F80,0x2680,0x1E80,0x2080,0x2680}};

	unsigned short weight[3][9]={{0xF2C7,0xEF6B,0xF1BB,0xF243,0xEE85,0xF16A,0xF448,0xF334,0xF43F},\
{0xF46B,0xF0F1,0xF36C,0xF4A5,0xF0D5,0xF3E4,0xF7BC,0xF6BA,0xF7D6},\
{0x0201,0xFF2A,0x009F,0x0364,0x003A,0x0245,0x05AF,0x054D,0x057D}};
	//end test fixed float
	float f_biggest_dat=0.0,f_smallest_dat=0.0;
	float f_biggest_w=0.0,f_smallest_w=0.0;
	int Q_D=0,Q_W = 0;
	//short bias = 0xE430;
	

	//int LoadHexFile_X32(const char *sHexFilename,short *pOutBuf,const int nRCnt);
	//nRet = LoadHexFile_X32();

	//Cp3x3_fm(short *pDat_fm,short *pWeight_fm, short Bias_short,int nFm,int nDFixedShift,int nWFixedShift,int nBFixedShift,int nOutFixedShift)
	//Cp3x3_fm((short *)data,(short *)weight, bias,3,PARA_SHIFT_BIT,PARA_SHIFT_BIT,15,12);

	//GetAllRange();
	
	//int LoadHexFile_X64(const char *sHexFilename,short *pOutBuf,const int nRCnt);
	
	//����Cmodel�������ļ�
	printf("new produce 20171106.....\r\n");
	nRet=0;
	//nRet=produceDat();
	if(0>nRet){
		printf("produceDat para data error!\r\n");
		return output;
	}

	const char *sPicfile[]={"/home/C_Model_Register/out/pic_0.cmodel\0","/home/C_Model_Register/out/pic_1.cmodel\0","/home/C_Model_Register/out/pic_2.cmodel\0"};
	const char *sWeightFile[27]={
		                         "/home/C_Model_Register/out/weight_00.cmodel\0","/home/C_Model_Register/out/weight_01.cmodel\0","/home/C_Model_Register/out/weight_02.cmodel\0","/home/C_Model_Register/out/weight_03.cmodel\0",
		                         "/home/C_Model_Register/out/weight_04.cmodel\0","/home/C_Model_Register/out/weight_05.cmodel\0","/home/C_Model_Register/out/weight_06.cmodel\0","/home/C_Model_Register/out/weight_07.cmodel\0",
														 "/home/C_Model_Register/out/weight_08.cmodel\0","/home/C_Model_Register/out/weight_09.cmodel\0","/home/C_Model_Register/out/weight_10.cmodel\0","/home/C_Model_Register/out/weight_11.cmodel\0",
														 "/home/C_Model_Register/out/weight_12.cmodel\0","/home/C_Model_Register/out/weight_13.cmodel\0","/home/C_Model_Register/out/weight_14.cmodel\0","/home/C_Model_Register/out/weight_15.cmodel\0",
														 "/home/C_Model_Register/out/weight_16.cmodel\0","/home/C_Model_Register/out/weight_17.cmodel\0","/home/C_Model_Register/out/weight_18.cmodel\0","/home/C_Model_Register/out/weight_19.cmodel\0",
														 "/home/C_Model_Register/out/weight_20.cmodel\0","/home/C_Model_Register/out/weight_21.cmodel\0","/home/C_Model_Register/out/weight_22.cmodel\0","/home/C_Model_Register/out/weight_23.cmodel\0",
														 "/home/C_Model_Register/out/weight_24.cmodel\0","/home/C_Model_Register/out/weight_25.cmodel\0","/home/C_Model_Register/out/weight_26.cmodel\0",
	};
	const char *sPreluFile="/home/C_Model_Register/out/prelu.cmodel\0";
	const char *sBiasFile="/home/C_Model_Register/out/bias.cmodel\0";
	const char *sFcWeightFile="/home/C_Model_Register/out/fc_weight.cmodel\0";
	const char *sFcBiasFile = "/home/C_Model_Register/out/fc_bias.cmodel\0";

	//����Cģ������
	//nRet = GenerateCMFile();
	if(0>nRet){
		printf("Generate c model data error!\r\n");
		return output;
	}
	
	 memset(sbuf,0,sizeof(sbuf));
	 sprintf(sbuf,"/home/C_Model_Register/float/fc_bias.txt\0");
	 wFpgaFCBias_sh(sbuf);

	 memset(sbuf,0,sizeof(sbuf));
	 sprintf(sbuf,"/home/C_Model_Register/float/bias.txt\0");
	 wFpgaBias_sh(sbuf);

	 memset(sbuf,0,sizeof(sbuf));
	 sprintf(sbuf,"/home/C_Model_Register/float/prelu.txt\0");
	 wFpgaPreluPara_sh(sbuf);
	//��Cģ�����ݼ���
	 output = CLNet_Monitor(img, sWeightFile,sPreluFile,sBiasFile,sFcWeightFile,sFcBiasFile);

	return output;
}
