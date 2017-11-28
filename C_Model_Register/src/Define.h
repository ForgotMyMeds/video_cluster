#ifndef __DEFINE_H__

#define __DEFINE_H__
#include <vector>
int LoadFloatFile1D(const char *sFilename,float *pFloatDat,int nCount);
int wFpgaPreluPara_hex(const char *sFloatFilename);
int wFpgaBias_hex(const char *sFloatFilename);
int wFpgaFCBias_hex(const char *sFloatFilename);
int wFpgaPic_hex(const char *sFloatFilename);
int wFpgaWeight_hex();
int wFpgaConvOut_hex(const char *sFloatFilename);
int LoadFloatFile1DByFP(FILE *fp,float *pFloatDat,int nCount);
int wFpgaConvOut_float(const char *sFloatFilename);
int wFpgaWeight_float();
int wFpgaPic_float(const char *sFloatFilename);
int FindBiggest(float *f_Biggest,float *f_Smallest,float *pfdata,int nCount,char bFlag);
int Qn(float f_Biggest,float f_Smallest);
long long Cp3x3(short Dat_short[9],short Weight_short[9] );
int ConvertF2D(float *pfloat,short *pOutD,int nCnt,int nQn);
int wFpgaFCBias_CModel(const char *sFloatFilename);
int wFpgaFCWeight_CModel(const char *sFloatFilename);
int GetAllRange();
short Cp3x3_fm(short *pDat_fm,short *pWeight_fm, short Bias_short,int nFm,int nDFixedShift,int nWFixedShift,int nBFixedShift,int nOutFixedShift,FILE  *fp_debug);
std::vector<float> CLNet_Monitor(short* pBuffIn,const char *sWeightFilename[],const char *sPreluFilename,const char *sBiasFilename,const char *sFcWeightFilename,const char *sFcBiasFilename);
int produceDat();
int wFpgaBias_CModel(const char *sFloatFilename);
int wFpgaPrelu_CModel(const char *sFloatFilename);
int wFpgaFCBias_sh(const char *sFloatFilename);
int wFpgaBias_sh(const char *sFloatFilename);
int wFpgaPreluPara_sh(const char *sFloatFilename);
int wFpgaWeight_CModel();
//FILE_PROC
int LoadHexFile_X64(const char *sHexFilename,short *pOutBuf,const int nRCnt);
int LoadHexFile_X16(const char *sHexFilename,short *pOutBuf,const int nRCnt);
//int LoadHexFile_X128(const char *sHexFilename,short *pOutBuf,const int nRCnt);
int LoadHexFile_X32(const char *sHexFilename,short *pOutBuf,const int nRCnt);
int LoadHexFile_X144(const char *sHexFilename,short *pOutBuf,const int nRCnt);
int PadProc(const short *pSource,short *pDest,int nRow,int nCln,int nFm);
int GenerateCMFile();
int Eltwise(short *pConvOut,int nQo,short *pEltBuf,int nQe,int nQz,int nCnt);
//-------�˴�������λ��
//const int PARA_SHIFT_BIT = 15;
//const int g_Q_D = 15; //d shift qn
//const int g_Q_W = 15; //w shift qn
//const int g_Q_B = 14; //bias shift qn
//const int g_Q_P = 15; //prelu shift qn
//const int g_Q_O_array[27] ={12,11,10,9,9,9,9,9 ,9 ,9 ,9 ,9,10,11,10 ,12,11,12 ,11 ,11,11,13,13,13,13,12,12};//conv out layer shift qn 
//const int g_Q_F =  13;//fc qn

//edit 20171028 Qֵ����������ͳ�ƾ���
//�����������
const int g_Q_D = 14; //pic data Q
const int g_Q_Bias_array[28] = {14,12,15,15,13,15,15,15,15,13,15,15,15,15,15,15,15,15,15,15,13,15,15,15,15,15,15,14};//0~26Ϊ����������27Ϊȫ���Ӳ����
const int g_Q_Prelu_array[28]= {14,14,14,14,14,14,14,14,14,14,14,10,14,14,14,14,14,14,14,14,14,14,14,14,14,15,14,-1};//0~26Ϊ����������27Ϊȫ���Ӳ��������Ч��1��
const int g_Q_W_array[28]    = {14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14};//0~26Ϊ����������27Ϊȫ���Ӳ����
//����������
const int g_Q_Elt_array[28]    = {-1,-1,-1,10,-1,-1,10,-1,10,-1,-1,10,-1,10,-1,10,-1,10,-1,10,-1,-1,12,-1,11,-1,11,-1};//0~26Ϊ����������27Ϊȫ���Ӳ����
//const int g_Q_Pool_array[28]   = {-1,11,-1,-1,10,-1,-1,-1,-1,10,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,12,-1,-1,-1,-1,-1,-1,-1};//0~26Ϊ����������27Ϊȫ���Ӳ����
const int g_Q_O_array[28]      = {12,11,11,10,10,10,10,10,11,10,11,11,11,11,11,12,12,12,12,12,12,12,12,12,12,11,11,13 };// conv out layer shift qn conv out layer shift qn 




//#define __DEBUGE_FILE__

#else

#endif
