// ProduceDat.cpp : Defines the entry point for the console application.
//

#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "Define.h"

//����FPGA�����bias.hex��prelu.hex��fc_bias.hex���Լ�weight.hex�ļ���
int produceDat()
{
	FILE *fp =NULL;
	int nRet=0;
	float fRet = 0.0;
	char sbuf[512]={0x0};
	float fBuf[1024]={0x0};
	

	//end test fixed float
	float f_biggest_dat=0.0,f_smallest_dat=0.0;
	float f_biggest_w=0.0,f_smallest_w=0.0;
	int Q_D=0,Q_W = 0;
	
	//ͳһ��ֵ��Χ��ȷ��Qֵ
//	GetAllRange();

	
	//prelu_para
	memset(sbuf,0,sizeof(sbuf));
	sprintf(sbuf,"..\\float\\prelu.txt\0");
	nRet=wFpgaPreluPara_hex(sbuf);
	if(nRet){
		printf("write prelu para error!\n");
		return -1;
	}
	printf("fwrite prelu over!");

	nRet=wFpgaPrelu_CModel(sbuf);
	if(nRet){
		printf("write prelu para error!\n");
		return -1;
	}
	printf("fwrite prelu_cmodel over!");
	
	
	//bias
	memset(sbuf,0,sizeof(sbuf));
	sprintf(sbuf,"..\\float\\bias.txt\0");
	nRet=wFpgaBias_hex(sbuf);
	if(nRet){
		printf("write prelu para error!\n");
		return -1;
	}
	printf("fwrite bias over!");

	memset(sbuf,0,sizeof(sbuf));
	sprintf(sbuf,"..\\float\\bias.txt\0");
	nRet=wFpgaBias_CModel(sbuf);
	if(nRet){
		printf("write prelu para error!\n");
		return -1;
	}
	printf("fwrite bias_cmodel over!");

	//fc_bias
	memset(sbuf,0,sizeof(sbuf));
	sprintf(sbuf,"..\\float\\fc_bias.txt\0");
	nRet=wFpgaFCBias_hex(sbuf);
	if(nRet){
		printf("write fcbias error!\n");
		return -1;
	}
	printf("fwrite fc bias over!");

	

	//pic
	memset(sbuf,0,sizeof(sbuf));
	sprintf(sbuf,"..\\float\\pic.txt\0");
	nRet=wFpgaPic_hex(sbuf);
	if(nRet){
		printf("write pic error!\n");
		return -1;
	}

	nRet=wFpgaPic_float(sbuf);
	if(nRet){
		printf("write pic float error!\n");
		return -1;
	}
	printf("fwrite pic over!");

#if 1

	//weight data
	nRet=wFpgaWeight_hex();
	if(nRet){
		printf("write weight para error!\n");
		return -1;
	}
	printf("fwrite weight over!");

	//weight data
	nRet=wFpgaWeight_float();
	if(nRet){
		printf("write weight para error!\n");
		return -1;
	}
	printf("fwrite weight over!");

	
#endif

	//convout
	/*memset(sbuf,0,sizeof(sbuf));
	sprintf(sbuf,"..\\float\\conv_out.txt\0");
	nRet=wFpgaConvOut_float(sbuf);
	if(nRet){
		printf("write convout file error!\n");
		return -1;
	}
	printf("fwrite convout float over!");


	nRet=wFpgaConvOut_hex(sbuf);
	if(nRet){
		printf("write convout file error!\n");
		return -1;
	}
	printf("fwrite convout over!");
*/
	return 0;
}


int wFpgaPic_cmodel(const char *sFloatFilename);
int GenerateCMFile()
{
	FILE *fp =NULL;
	int nRet=0;
	float fRet = 0.0;
	char sbuf[512]={0x0};
	float fBuf[1024]={0x0};
	

	//end test fixed float
	float f_biggest_dat=0.0,f_smallest_dat=0.0;
	float f_biggest_w=0.0,f_smallest_w=0.0;
	int Q_D=0,Q_W = 0;
	

//	GetAllRange();

	
	//prelu_para
	memset(sbuf,0,sizeof(sbuf));
	sprintf(sbuf,"..\\float\\prelu.txt\0");
	nRet=wFpgaPrelu_CModel(sbuf);
	if(nRet){
		printf("write prelu para error!\n");
		return -1;
	}
	printf("fwrite prelu_cmodel over!");
	
	
	//bias	
	memset(sbuf,0,sizeof(sbuf));
	sprintf(sbuf,"..\\float\\bias.txt\0");
	nRet=wFpgaBias_CModel(sbuf);
	if(nRet){
		printf("write prelu para error!\n");
		return -1;
	}
	printf("fwrite bias_cmodel over!");


	//fc_bias cmodel
	memset(sbuf,0,sizeof(sbuf));
	sprintf(sbuf,"..\\float\\fc_bias.txt\0");
	nRet=wFpgaFCBias_CModel(sbuf);
	if(nRet){
		printf("write fc bias cmodel para error!\n");
		return -1;
	}
	printf("fwrite fc bias cmodel over!");

	//pic
	memset(sbuf,0,sizeof(sbuf));
	sprintf(sbuf,"..\\float\\pic.txt\0");
	nRet=wFpgaPic_cmodel(sbuf);
	if(nRet){
		printf("write pic error!\n");
		return -1;
	}

	nRet=wFpgaPic_float(sbuf);
	if(nRet){
		printf("write pic float error!\n");
		return -1;
	}
	printf("fwrite pic over!");

	//fc weight cmodel
	memset(sbuf,0,sizeof(sbuf));
	sprintf(sbuf,"..\\float\\weight_fc.txt\0");
	nRet=wFpgaFCWeight_CModel(sbuf);
	if(nRet){
		printf("write fc weight cmodel error!\n");
		return -1;
	}
	printf("fwrite fc weight cmodel over!");


#if 1
	
	//weight data
	nRet=wFpgaWeight_CModel();
	if(nRet){
		printf("write weight cmodel error!\n");
		return -1;
	}
	printf("fwrite weight cmodel over!");


#endif
	printf("fwrite convout over!");

	return 0;
}

//bias,������˷��ļ�������BIAS����
int wFpgaBias_hex(const char *sFloatFilename)
{
	//prelu_para data
	 int nShift_bit = -1;
	//////////////////
	int nLayerPreluParaCfg[27]={32,64,64,64,128,128,128,128,128,256,256,256,256,256,256,256,256,256,256,256,512,512,512,512,512,512,512}; 
	int nAllCount=0;
	int i=0,j=0;
	int nKerCount=16;
	FILE *fp = NULL;
	float *pFloatDat=NULL;
	float *ptr=NULL;
	float fDat0=0.0;
	float fDat1 = 0.0;
	float fDat2 = 0.0;
	float fDat3 = 0.0;
	int n = 0,m=0;
	unsigned int uDat0,uDat1,uDat2,uDat3= 0;
	char sname[512]={0x0};

	for(i=0;i<27;i++){
		nAllCount += nLayerPreluParaCfg[i];
	}
	printf("to load %d prelupara data\n",nAllCount);

	pFloatDat = (float *)malloc(sizeof(float)*nAllCount);
	if(pFloatDat==NULL){
		printf("malloc error!\n");
		return -1;
	}
	
	
	
	if(nAllCount!=LoadFloatFile1D(sFloatFilename,pFloatDat,nAllCount)){
		printf("fread error!\n");
		free(pFloatDat);
		return -1;
	}
	

	//output
	//ǰ8�������
	for(i=0;i<8;i++){
		memset(sname,0,512);
		sprintf(sname,"..\\out\\bias_%d.hex\0",i);
		fp = fopen(sname,"w+");
		if(fp==NULL){
			printf("fopen out file error!\n");
			free(pFloatDat);
			return -1;
		}
		ptr = pFloatDat;
		for(j=0;j<27;j++){
			if(nLayerPreluParaCfg[j]==32)
				n = 32;
			else if(nLayerPreluParaCfg[j]==64)
				n = 64;
			else if(nLayerPreluParaCfg[j]==128)
				n = 64;
			else if(nLayerPreluParaCfg[j]==256)
				n = 64;
			else {//512
			/*	if(j==20)//CHN:conv4==20�Ƚ����⣬n=64//ȥ���ָ���32��kernal
					n = 64;
				else*/
					n = 32;
			}
			nShift_bit = g_Q_Bias_array[j];
			if((nShift_bit<0)||(nShift_bit>15)){
				printf("wFpgaBias_hex nShift_bit error:%d\r\n",nShift_bit);
				return -1;
			}
			for(m=0;m<nLayerPreluParaCfg[j]/n;m++){
				fDat0 = *(ptr+i*4+m*n);
				fDat1 = *(ptr+i*4+1+m*n);
				fDat2 = *(ptr+i*4+2+m*n);
				fDat3 = *(ptr+i*4+3+m*n);
				
				
				uDat0 = (int)(fDat0*(1<<nShift_bit));
				uDat1 = (int)(fDat1*(1<<nShift_bit));
				uDat2 = (int)(fDat2*(1<<nShift_bit));
				uDat3 = (int)(fDat3*(1<<nShift_bit));
				fprintf(fp,"%04X%04X\n",uDat0&0xffff,uDat1&0xffff);
				fprintf(fp,"%04X%04X\n",uDat2&0xffff,uDat3&0xffff);
			}
			
			ptr +=nLayerPreluParaCfg[j];
		}
		fclose(fp);
	}

	//��8�������
	for(i=8;i<nKerCount;i++){
		memset(sname,0,512);
		sprintf(sname,"..\\out\\bias_%d.hex\0",i);
		fp = fopen(sname,"w+");
		if(fp==NULL){
			printf("fopen out file error!\n");
			free(pFloatDat);
			return -1;
		}
		ptr = pFloatDat;
		ptr +=nLayerPreluParaCfg[0];//ָ��������1������
		for(j=1;j<27;j++){	//��1�����󼸲�����Ϊ512�Ĳ����ݲ���
			if(nLayerPreluParaCfg[j]==32){
				n = 32;
				continue;
			}
			else if(nLayerPreluParaCfg[j]==64)
				n = 64;
			else if(nLayerPreluParaCfg[j]==128)
				n = 64;
			else if(nLayerPreluParaCfg[j]==256)
				n = 64;
			else //512
			{
				//ȥ�����ָ���32��kernal2017/10/09
			/*	if(j==20){//CHN:conv4==20�Ƚ����⣬n=64
					n = 64;
				}
				else*/{
					n = 32;
					continue;
				}
			}
			nShift_bit = g_Q_Bias_array[j];
			if((nShift_bit<0)||(nShift_bit>15)){
				printf("wFpgaBias_hex nShift_bit error:%d\r\n",nShift_bit);
				return -1;
			}
			for(m=0;m<nLayerPreluParaCfg[j]/n;m++){
				fDat0 = *(ptr+i*4+m*n);
				fDat1 = *(ptr+i*4+1+m*n);
				fDat2 = *(ptr+i*4+2+m*n);
				fDat3 = *(ptr+i*4+3+m*n);

				uDat0 = (int)(fDat0*(1<<nShift_bit));
				uDat1 = (int)(fDat1*(1<<nShift_bit));
				uDat2 = (int)(fDat2*(1<<nShift_bit));
				uDat3 = (int)(fDat3*(1<<nShift_bit));
				fprintf(fp,"%04X%04X\n",uDat0&0xffff,uDat1&0xffff);
				fprintf(fp,"%04X%04X\n",uDat2&0xffff,uDat3&0xffff);
			}
			
			ptr +=nLayerPreluParaCfg[j];
		}
		fclose(fp);
	}
	free(pFloatDat);
	return 0;
}
//bias,������˷��ļ�������BIAS����
int wFpgaBias_sh(const char *sFloatFilename)
{
	//prelu_para data
	 int nShift_bit = -1;
	//////////////////
	int nLayerPreluParaCfg[27]={32,64,64,64,128,128,128,128,128,256,256,256,256,256,256,256,256,256,256,256,512,512,512,512,512,512,512}; 
	int nCmd[27] = {0x10410,0x10810,0x10c10,0x11010,0x11410,0x11810,0x11c10,0x12010,0x12410,0x12810,0x12c10,0x13010,0x13410,0x13810,0x13c10,0x14010};
	int nAllCount=0;
	int i=0,j=0;
	int nKerCount=16;
	FILE *fp = NULL;
	float *pFloatDat=NULL;
	float *ptr=NULL;
	float fDat0=0.0;
	float fDat1 = 0.0;
	float fDat2 = 0.0;
	float fDat3 = 0.0;
	int n = 0,m=0;
	unsigned int uDat0,uDat1,uDat2,uDat3= 0;
	char sname[512]={0x0};

	for(i=0;i<27;i++){
		nAllCount += nLayerPreluParaCfg[i];
	}
	printf("to load %d prelupara data\n",nAllCount);

	pFloatDat = (float *)malloc(sizeof(float)*nAllCount);
	if(pFloatDat==NULL){
		printf("malloc error!\n");
		return -1;
	}
	
	
	
	if(nAllCount!=LoadFloatFile1D(sFloatFilename,pFloatDat,nAllCount)){
		printf("fread error!\n");
		free(pFloatDat);
		return -1;
	}
	

	//output
	//ǰ8�������
	for(i=0;i<8;i++){
		memset(sname,0,512);
		sprintf(sname,"..\\out\\bias_%d.sh\0",i);
		fp = fopen(sname,"w+");
		if(fp==NULL){
			printf("fopen out file error!\n");
			free(pFloatDat);
			return -1;
		}
		ptr = pFloatDat;
		for(j=0;j<27;j++){
			if(nLayerPreluParaCfg[j]==32)
				n = 32;
			else if(nLayerPreluParaCfg[j]==64)
				n = 64;
			else if(nLayerPreluParaCfg[j]==128)
				n = 64;
			else if(nLayerPreluParaCfg[j]==256)
				n = 64;
			else {//512
			/*	if(j==20)//CHN:conv4==20�Ƚ����⣬n=64//ȥ���ָ���32��kernal
					n = 64;
				else*/
					n = 32;
			}
			nShift_bit = g_Q_Bias_array[j];
			if((nShift_bit<0)||(nShift_bit>15)){
				printf("wFpgaBias_hex nShift_bit error:%d\r\n",nShift_bit);
				return -1;
			}
			for(m=0;m<nLayerPreluParaCfg[j]/n;m++){
				fDat0 = *(ptr+i*4+m*n);
				fDat1 = *(ptr+i*4+1+m*n);
				fDat2 = *(ptr+i*4+2+m*n);
				fDat3 = *(ptr+i*4+3+m*n);
				
				
				uDat0 = (int)(fDat0*(1<<nShift_bit));
				uDat1 = (int)(fDat1*(1<<nShift_bit));
				uDat2 = (int)(fDat2*(1<<nShift_bit));
				uDat3 = (int)(fDat3*(1<<nShift_bit));
				fprintf(fp,"./GP_OP write %X %04X%04X \n",nCmd[i],uDat0&0xffff,uDat1&0xffff);
				fprintf(fp,"./GP_OP write %X %04X%04X \n",nCmd[i],uDat2&0xffff,uDat3&0xffff);
			}
			
			ptr +=nLayerPreluParaCfg[j];
		}
		fclose(fp);
	}

	//��8�������
	for(i=8;i<nKerCount;i++){
		memset(sname,0,512);
		sprintf(sname,"..\\out\\bias_%d.sh\0",i);
		fp = fopen(sname,"w+");
		if(fp==NULL){
			printf("fopen out file error!\n");
			free(pFloatDat);
			return -1;
		}
		ptr = pFloatDat;
		ptr +=nLayerPreluParaCfg[0];//ָ��������1������
		for(j=1;j<27;j++){	//��1�����󼸲�����Ϊ512�Ĳ����ݲ���
			if(nLayerPreluParaCfg[j]==32){
				n = 32;
				continue;
			}
			else if(nLayerPreluParaCfg[j]==64)
				n = 64;
			else if(nLayerPreluParaCfg[j]==128)
				n = 64;
			else if(nLayerPreluParaCfg[j]==256)
				n = 64;
			else //512
			{
				//ȥ�����ָ���32��kernal2017/10/09
			/*	if(j==20){//CHN:conv4==20�Ƚ����⣬n=64
					n = 64;
				}
				else*/{
					n = 32;
					continue;
				}
			}
			nShift_bit = g_Q_Bias_array[j];
			if((nShift_bit<0)||(nShift_bit>15)){
				printf("wFpgaBias_hex nShift_bit error:%d\r\n",nShift_bit);
				return -1;
			}
			for(m=0;m<nLayerPreluParaCfg[j]/n;m++){
				fDat0 = *(ptr+i*4+m*n);
				fDat1 = *(ptr+i*4+1+m*n);
				fDat2 = *(ptr+i*4+2+m*n);
				fDat3 = *(ptr+i*4+3+m*n);

				uDat0 = (int)(fDat0*(1<<nShift_bit));
				uDat1 = (int)(fDat1*(1<<nShift_bit));
				uDat2 = (int)(fDat2*(1<<nShift_bit));
				uDat3 = (int)(fDat3*(1<<nShift_bit));
				fprintf(fp,"./GP_OP write %X %04X%04X \n",nCmd[i],uDat0&0xffff,uDat1&0xffff);
				fprintf(fp,"./GP_OP write %X %04X%04X \n",nCmd[i],uDat2&0xffff,uDat3&0xffff);
			}
			
			ptr +=nLayerPreluParaCfg[j];
		}
		fclose(fp);
	}
	free(pFloatDat);
	return 0;
}
//fcbias��Cģ���ã����ļ�˳����BIAS
int wFpgaFCBias_CModel(const char *sFloatFilename)
{
	//prelu_para data
	const int nShift_bit = g_Q_Bias_array[27];
	//////////////////
	int nLayerPreluParaCfg[27]={32,64,64,64,128,128,128,128,128,256,256,256,256,256,256,256,256,256,256,256,512,512,512,512,512,512,512}; 
	int nAllCount=0;
	int i=0,j=0;
	int nKerCount=16;
	FILE *fp = NULL;
	float *pFloatDat=NULL;
	float *ptr=NULL;
	float fDat0=0.0;
	float fDat1 = 0.0;
	float fDat2 = 0.0;
	float fDat3 = 0.0;
	int n = 0,m=0;
	unsigned int uDat0,uDat1;
	char sname[512]={0x0};

	
	nAllCount = 512;//�̶�512��
	printf("to load %d fcbias data\n",nAllCount);

	pFloatDat = (float *)malloc(sizeof(float)*nAllCount);
	if(pFloatDat==NULL){
		printf("malloc error!\n");
		return -1;
	}	
	
	if(nAllCount!=LoadFloatFile1D(sFloatFilename,pFloatDat,nAllCount)){
		printf("fread error!\n");
		free(pFloatDat);
		return -1;
	}
	memset(sname,0,512);
	sprintf(sname,"..\\out\\fc_bias.cmodel\0",i);
	fp = fopen(sname,"w");
	if(fp==NULL){
		printf("fopen out file error!\n");
		free(pFloatDat);
		return -1;
	}
	
	ptr = pFloatDat;
	for(j=0;j<nAllCount;j=j+2){
		fDat0 = *(ptr+j);
		fDat1 = *(ptr+j+1);		
		uDat0 = (int)(fDat0*(1<<nShift_bit));
		uDat1 = (int)(fDat1*(1<<nShift_bit));
		fprintf(fp,"%04X%04X\n",uDat0&0xffff,uDat1&0xffff);
		if((j+1)%100==0){
			printf("get 100\n");
		}
	}
	printf("load file over!\n");
	free(pFloatDat);
	fclose(fp);
	return 0;
}
//fcweight��Cģ���ã����ļ�˳����,ÿ��32λ��
int wFpgaFCWeight_CModel(const char *sFloatFilename)
{
	//prelu_para data
	const int nShift_bit = g_Q_W_array[27];
	//////////////////
	int nLayerPreluParaCfg[27]={32,64,64,64,128,128,128,128,128,256,256,256,256,256,256,256,256,256,256,256,512,512,512,512,512,512,512}; 
	int nAllCount=0;
	int i=0,j=0;
	int nKerCount=16;
	FILE *fp = NULL;
	float *pFloatDat=NULL;
	float *ptr=NULL;
	float fDat0=0.0;
	float fDat1 = 0.0;
	float fDat2 = 0.0;
	float fDat3 = 0.0;
	int n = 0,m=0;
	unsigned int uDat0,uDat1;
	char sname[512]={0x0};


	nAllCount = 512*512*5*4;//�̶�512��
	printf("to load %d fcweight data\n",nAllCount);

	pFloatDat = (float *)malloc(sizeof(float)*nAllCount);
	if(pFloatDat==NULL){
		printf("malloc error!\n");
		return -1;
	}	
	
	if(nAllCount!=LoadFloatFile1D(sFloatFilename,pFloatDat,nAllCount)){
		printf("fread error!\n");
		free(pFloatDat);
		return -1;
	}
	memset(sname,0,512);
	sprintf(sname,"..\\out\\fc_weight.cmodel\0",i);
	fp = fopen(sname,"w");
	if(fp==NULL){
		printf("fopen out file error!\n");
		free(pFloatDat);
		return -1;
	}
	
	ptr = pFloatDat;
	for(j=0;j<nAllCount;j=j+2){
		fDat0 = *(ptr+j);
		fDat1 = *(ptr+j+1);	
		uDat0 = (int)(fDat0*(1<<nShift_bit));
		uDat1 = (int)(fDat1*(1<<nShift_bit));
		fprintf(fp,"%04X%04X%\n",uDat0&0xffff,uDat1&0xffff);
	}
	free(pFloatDat);
	fclose(fp);
	return 0;
}



//bias��Cģ���ã����ļ�˳����BIAS
int wFpgaBias_CModel(const char *sFloatFilename)
{
	//prelu_para data
	 int nShift_bit = -1;
	//////////////////
	int nLayerPreluParaCfg[27]={32,64,64,64,128,128,128,128,128,256,256,256,256,256,256,256,256,256,256,256,512,512,512,512,512,512,512}; 
	int nAllCount=0;
	int i=0,j=0;
	int nKerCount=16;
	FILE *fp = NULL;
	float *pFloatDat=NULL;
	float *ptr=NULL;
	float fDat0=0.0;
	float fDat1 = 0.0;
	float fDat2 = 0.0;
	float fDat3 = 0.0;
	int n = 0,m=0;
	unsigned int uDat0,uDat1;
	char sname[512]={0x0};

	for(i=0;i<27;i++){
		nAllCount += nLayerPreluParaCfg[i];
	}
	printf("to load %d prelupara data\n",nAllCount);

	pFloatDat = (float *)malloc(sizeof(float)*nAllCount);
	if(pFloatDat==NULL){
		printf("malloc error!\n");
		return -1;
	}	
	
	if(nAllCount!=LoadFloatFile1D(sFloatFilename,pFloatDat,nAllCount)){
		printf("fread error!\n");
		free(pFloatDat);
		return -1;
	}
	memset(sname,0,512);
	sprintf(sname,"..\\out\\bias.cmodel\0",i);
	fp = fopen(sname,"w+");
	if(fp==NULL){
		printf("fopen out file error!\n");
		free(pFloatDat);
		return -1;
	}
	
	ptr = pFloatDat;
	for(j=0;j<27;j++){
		nShift_bit = g_Q_Bias_array[j];
		if((nShift_bit<0)||(nShift_bit>15)){
			printf("wFpgaBias_hex nShift_bit error:%d\r\n",nShift_bit);
			return -1;
		}
		for(i=0;i<nLayerPreluParaCfg[j];i=i+2){
			fDat0 = *(ptr);
			fDat1 = *(ptr+1);		
			uDat0 = (int)(fDat0*(1<<nShift_bit));
			uDat1 = (int)(fDat1*(1<<nShift_bit));
			fprintf(fp,"%04X%04X\n",uDat0&0xffff,uDat1&0xffff);
			ptr +=2;
		}
	}
	/*for(j=0;j<nAllCount;j=j+2){
		fDat0 = *(ptr+j);
		fDat1 = *(ptr+j+1);		
		uDat0 = (int)(fDat0*(1<<nShift_bit));
		uDat1 = (int)(fDat1*(1<<nShift_bit));
		fprintf(fp,"%04X%04X\n",uDat0&0xffff,uDat1&0xffff);
	}*/
	free(pFloatDat);
	fclose(fp);
	return 0;
}


//prelu��Cģ���ã����ļ�˳����prelu
int wFpgaPrelu_CModel(const char *sFloatFilename)
{
	//prelu_para data
	 int nShift_bit = -1;
	//////////////////
	int nLayerPreluParaCfg[27]={32,64,64,64,128,128,128,128,128,256,256,256,256,256,256,256,256,256,256,256,512,512,512,512,512,512,512}; 
	int nAllCount=0;
	int i=0,j=0;
	int nKerCount=16;
	FILE *fp = NULL;
	float *pFloatDat=NULL;
	float *ptr=NULL;
	float fDat0=0.0;
	float fDat1 = 0.0;
	float fDat2 = 0.0;
	float fDat3 = 0.0;
	int n = 0,m=0;
	unsigned int uDat0,uDat1,uDat3= 0;
	char sname[512]={0x0};

	for(i=0;i<27;i++){
		nAllCount += nLayerPreluParaCfg[i];
	}
	printf("to load %d prelupara data\n",nAllCount);

	pFloatDat = (float *)malloc(sizeof(float)*nAllCount);
	if(pFloatDat==NULL){
		printf("malloc error!\n");
		return -1;
	}	
	
	if(nAllCount!=LoadFloatFile1D(sFloatFilename,pFloatDat,nAllCount)){
		printf("fread error!\n");
		free(pFloatDat);
		return -1;
	}
	memset(sname,0,512);
	sprintf(sname,"..\\out\\prelu.cmodel\0",i);
	fp = fopen(sname,"w+");
	if(fp==NULL){
		printf("fopen out file error!\n");
		free(pFloatDat);
		return -1;
	}
	
	ptr = pFloatDat;
	for(j=0;j<27;j++){//CHN:27�������
		nShift_bit = g_Q_Prelu_array[j];
		if((nShift_bit<0)||(nShift_bit>15)){
			printf("nShift_bit error:%d!\r\n",nShift_bit);
			return -1;
		}
		for(i=0;i<nLayerPreluParaCfg[j];i=i+2){
			fDat0 = *(ptr);
			fDat1 = *(ptr+1);		
			uDat0 = (int)(fDat0*(1<<nShift_bit));
			uDat1 = (int)(fDat1*(1<<nShift_bit));
			fprintf(fp,"%04X%04X\n",uDat0&0xffff,uDat1&0xffff);
			ptr+=2;
		}
	}
	/*for(j=0;j<nAllCount;j=j+2){
		fDat0 = *(ptr+j);
		fDat1 = *(ptr+j+1);		
		uDat0 = (int)(fDat0*(1<<nShift_bit));
		uDat1 = (int)(fDat1*(1<<nShift_bit));
		fprintf(fp,"%04X%04X\n",uDat0&0xffff,uDat1&0xffff);
	}*/
	free(pFloatDat);
	fclose(fp);
	return 0;
}
//ע�⣺FCȫ���Ӳ�Ϊ�����FPGA_fcģ��Ĵ���
//�洢�ĸ�ʽ��������bias.hex��Щ����
//�洢�ĸ�ʽΪ��{bias3,bias2,bias1,bias0}
int wFpgaFCBias_hex(const char *sFloatFilename)
{
	//prelu_para data
	const int nShift_bit = g_Q_Bias_array[27];
	//////////////////
	int nLayerPreluParaCfg[1]={512}; 
	int nAllCount=0;
	int i=0,j=0;
	int nKerCount=16;
	FILE *fp = NULL;
	float *pFloatDat=NULL;
	float *ptr=NULL;
	float fDat0=0.0;
	float fDat1 = 0.0;
	float fDat2 = 0.0;
	float fDat3 = 0.0;
	int n = 0,m=0;
	unsigned int uDat0,uDat1,uDat2,uDat3= 0;
	char sname[512]={0x0};

	for(i=0;i<1;i++){
		nAllCount += nLayerPreluParaCfg[i];
	}
	printf("to load %d prelupara data\n",nAllCount);

	pFloatDat = (float *)malloc(sizeof(float)*nAllCount);
	if(pFloatDat==NULL){
		printf("malloc error!\n");
		return -1;
	}
	
	
	
	if(nAllCount!=LoadFloatFile1D(sFloatFilename,pFloatDat,nAllCount)){
		printf("fread error!\n");
		free(pFloatDat);
		return -1;
	}
	

	//output
	
		memset(sname,0,512);
		sprintf(sname,"..\\out\\fc_bias.hex\0");
		fp = fopen(sname,"w+");
		if(fp==NULL){
			printf("fopen out file error!\n");
			free(pFloatDat);
			return -1;
		}
		ptr = pFloatDat;
		for(j=0;j<512/4;j++){
			
				fDat0 = *(ptr+j*4);
				fDat1 = *(ptr+j*4+1);
				fDat2 = *(ptr+j*4+2);
				fDat3 = *(ptr+j*4+3);
				
				
				uDat0 = (int)(fDat0*(1<<nShift_bit));
				uDat1 = (int)(fDat1*(1<<nShift_bit));
				uDat2 = (int)(fDat2*(1<<nShift_bit));
				uDat3 = (int)(fDat3*(1<<nShift_bit));
				//����FPGA�ڲ�fcģ�飬�洢��С�˷�ʽ�洢
				//fc_bias.hex�ļ��洢��ʽ��������ͬ{bias3,bias2,bias1,bias0}
				fprintf(fp,"%04X%04X\n",uDat3&0xffff,uDat2&0xffff);
				fprintf(fp,"%04X%04X\n",uDat1&0xffff,uDat0&0xffff);
				//fprintf(fp,"%04X%04X\n",uDat0&0xffff,uDat1&0xffff);
				//fprintf(fp,"%04X%04X\n",uDat2&0xffff,uDat3&0xffff);
			
		}
		fclose(fp);

	free(pFloatDat);
	return 0;
}

int wFpgaFCBias_sh(const char *sFloatFilename)
{
	//prelu_para data
	const int nShift_bit = g_Q_Bias_array[27];
	//////////////////
	int nLayerPreluParaCfg[1]={512}; 
	int nAllCount=0;
	int i=0,j=0;
	int nKerCount=16;
	FILE *fp = NULL;
	float *pFloatDat=NULL;
	float *ptr=NULL;
	float fDat0=0.0;
	float fDat1 = 0.0;
	float fDat2 = 0.0;
	float fDat3 = 0.0;
	int n = 0,m=0;
	unsigned int uDat0,uDat1,uDat2,uDat3= 0;
	char sname[512]={0x0};

	for(i=0;i<1;i++){
		nAllCount += nLayerPreluParaCfg[i];
	}
	printf("to load %d prelupara data\n",nAllCount);

	pFloatDat = (float *)malloc(sizeof(float)*nAllCount);
	if(pFloatDat==NULL){
		printf("malloc error!\n");
		return -1;
	}
	
	
	
	if(nAllCount!=LoadFloatFile1D(sFloatFilename,pFloatDat,nAllCount)){
		printf("fread error!\n");
		free(pFloatDat);
		return -1;
	}
	

	//output
	
		memset(sname,0,512);
		sprintf(sname,"..\\out\\fcbias.sh\0");
		fp = fopen(sname,"w+");
		if(fp==NULL){
			printf("fopen out file error!\n");
			free(pFloatDat);
			return -1;
		}
		ptr = pFloatDat;
		for(j=0;j<512/4;j++){
			
				fDat0 = *(ptr+j*4);
				fDat1 = *(ptr+j*4+1);
				fDat2 = *(ptr+j*4+2);
				fDat3 = *(ptr+j*4+3);
				
				
				uDat0 = (int)(fDat0*(1<<nShift_bit));
				uDat1 = (int)(fDat1*(1<<nShift_bit));
				uDat2 = (int)(fDat2*(1<<nShift_bit));
				uDat3 = (int)(fDat3*(1<<nShift_bit));
				//����FPGA�ڲ�fcģ�飬�洢��С�˷�ʽ�洢
				//fc_bias.hex�ļ��洢��ʽ��������ͬ{bias3,bias2,bias1,bias0}
				fprintf(fp,"./GP_OP write 0x10130 %04X%04X \n",uDat3&0xffff,uDat2&0xffff);
				fprintf(fp,"./GP_OP write 0x10130 %04X%04X \n",uDat1&0xffff,uDat0&0xffff);
				//fprintf(fp,"%04X%04X\n",uDat0&0xffff,uDat1&0xffff);
				//fprintf(fp,"%04X%04X\n",uDat2&0xffff,uDat3&0xffff);
			
		}
		fclose(fp);

	free(pFloatDat);
	return 0;
}
//pic data���ϲ�д��һ���ļ�,FPGA�����ã�ÿ��128λ����
int wFpgaPic_hex(const char *sFloatFilename)
{
	//prelu_para data
	const int nShift_bit = g_Q_D;
	//////////////////
	int nLayerPreluParaCfg[1]={3*96*112}; 
	int nAllCount=0;
	int i=0,j=0;
	int nKerCount=16;
	FILE *fp = NULL;
	float *pFloatDat=NULL;
	float *ptr=NULL;
	float fDat0=0.0;
	float fDat1 = 0.0;
	float fDat2 = 0.0;
	float fDat3 = 0.0;
	int n = 0,m=0;
	unsigned int uDat0,uDat1= 0,uDat2,uDat3;
	char sname[512]={0x0};

	for(i=0;i<1;i++){
		nAllCount += nLayerPreluParaCfg[i];
	}
	printf("to load %d pic data\n",nAllCount);

	pFloatDat = (float *)malloc(sizeof(float)*nAllCount);
	if(pFloatDat==NULL){
		printf("malloc error!\n");
		return -1;
	}
	
	if(nAllCount!=LoadFloatFile1D(sFloatFilename,pFloatDat,nAllCount)){
		printf("fread error!\n");
		free(pFloatDat);
		return -1;
	}
	

	//output
	sprintf(sname,"..\\out\\pic.hex\0");
	fp = fopen(sname,"w");
	if(fp==NULL){
		printf("fopen out file error!\n");
		free(pFloatDat);
		return -1;
	}
	for(i=0;i<3;i++){
		memset(sname,0,512);
	
		ptr = pFloatDat+112*96*i;
		for(j=0;j<112*96;j=j+8){
			
				fDat0 = *(ptr+j);
				fDat1 = *(ptr+j+1);
				fDat2 = *(ptr+j+2);
				fDat3 = *(ptr+j+3);
				
				
				uDat0 = (int)(fDat0*(1<<nShift_bit));
				uDat1 = (int)(fDat1*(1<<nShift_bit));
				uDat2 = (int)(fDat2*(1<<nShift_bit));
				uDat3 = (int)(fDat3*(1<<nShift_bit));
				//uDat1 = (int)(fDat1*(1<<nShift_bit));
				//uDat2 = (int)(fDat2*(1<<nShift_bit));
				//uDat3 = (int)(fDat3*(1<<nShift_bit));
				fprintf(fp,"%04X%04X",uDat0&0xffff,uDat1&0xffff);
				fprintf(fp,"%04X%04X",uDat2&0xffff,uDat3&0xffff);

				fDat0 = *(ptr+j+4);
				fDat1 = *(ptr+j+4+1);
				fDat2 = *(ptr+j+4+2);
				fDat3 = *(ptr+j+4+3);
				uDat0 = (int)(fDat0*(1<<nShift_bit));
				uDat1 = (int)(fDat1*(1<<nShift_bit));
				uDat2 = (int)(fDat2*(1<<nShift_bit));
				uDat3 = (int)(fDat3*(1<<nShift_bit));
				fprintf(fp,"%04X%04X",uDat0&0xffff,uDat1&0xffff);
				fprintf(fp,"%04X%04X\n",uDat2&0xffff,uDat3&0xffff);
			
		}
		
	}
	fclose(fp);
	free(pFloatDat);
	return 0;
}

//pic data
int wFpgaPic_cmodel(const char *sFloatFilename)
{
	//prelu_para data
	const int nShift_bit = g_Q_D;
	//////////////////
	int nLayerPreluParaCfg[1]={3*96*112}; 
	int nAllCount=0;
	int i=0,j=0;
	int nKerCount=16;
	FILE *fp = NULL;
	float *pFloatDat=NULL;
	float *ptr=NULL;
	float fDat0=0.0;
	float fDat1 = 0.0;
	float fDat2 = 0.0;
	float fDat3 = 0.0;
	int n = 0,m=0;
	unsigned int uDat0,uDat3= 0;
	char sname[512]={0x0};

	for(i=0;i<1;i++){
		nAllCount += nLayerPreluParaCfg[i];
	}
	printf("to load %d prelupara data\n",nAllCount);

	pFloatDat = (float *)malloc(sizeof(float)*nAllCount);
	if(pFloatDat==NULL){
		printf("malloc error!\n");
		return -1;
	}
	
	
	
	if(nAllCount!=LoadFloatFile1D(sFloatFilename,pFloatDat,nAllCount)){
		printf("fread error!\n");
		free(pFloatDat);
		return -1;
	}
	

	//output
	for(i=0;i<3;i++){
		memset(sname,0,512);
		sprintf(sname,"..\\out\\pic_%d.cmodel\0",i);
		fp = fopen(sname,"w+");
		if(fp==NULL){
			printf("fopen out file error!\n");
			free(pFloatDat);
			return -1;
		}
		ptr = pFloatDat+112*96*i;
		for(j=0;j<112*96;j++){
			
				fDat0 = *(ptr+j);
				//fDat1 = *(ptr+i*4+1);
				//fDat2 = *(ptr+i*4+2);
				//fDat3 = *(ptr+i*4+3);
				
				
				uDat0 = (int)(fDat0*(1<<nShift_bit));
				fprintf(fp,"%04X\n",uDat0&0xffff);
				//uDat1 = (int)(fDat1*(1<<nShift_bit));
				//uDat2 = (int)(fDat2*(1<<nShift_bit));
				//uDat3 = (int)(fDat3*(1<<nShift_bit));
			//	fprintf(fp,"%04X%04X\n",uDat0&0xffff,uDat1&0xffff);
			//	fprintf(fp,"%04X%04X\n",uDat2&0xffff,uDat3&0xffff);
			
		}
		fclose(fp);
	}

	free(pFloatDat);
	return 0;
}

int wFpgaPic_float(const char *sFloatFilename)
{
	//prelu_para data
	//const int nShift_bit = -1;
	//////////////////
	int nLayerPreluParaCfg[1]={3*96*112}; 
	int nAllCount=0;
	int i=0,j=0;
	int nKerCount=16;
	FILE *fp = NULL;
	float *pFloatDat=NULL;
	float *ptr=NULL;
	float fDat0=0.0;
	float fDat1 = 0.0;
	float fDat2 = 0.0;
	float fDat3 = 0.0;
	int n = 0,m=0;
	unsigned int uDat0,uDat3= 0;
	char sname[512]={0x0};

	for(i=0;i<1;i++){
		nAllCount += nLayerPreluParaCfg[i];
	}
	printf("to load %d wFpgaPic_float data\n",nAllCount);

	pFloatDat = (float *)malloc(sizeof(float)*nAllCount);
	if(pFloatDat==NULL){
		printf("malloc error!\n");
		return -1;
	}
	
	if(nAllCount!=LoadFloatFile1D(sFloatFilename,pFloatDat,nAllCount)){
		printf("fread error!\n");
		free(pFloatDat);
		return -1;
	}
	

	//output
	for(i=0;i<3;i++){
		memset(sname,0,512);
		sprintf(sname,"..\\out\\pic_%d.dat\0",i);
		fp = fopen(sname,"w+");
		if(fp==NULL){
			printf("fopen out file error!\n");
			free(pFloatDat);
			return -1;
		}
		ptr = pFloatDat+112*96*i;
		for(j=0;j<112*96;j++){
				fDat0 = *(ptr+j);
				//fDat1 = *(ptr+i*4+1);
				//fDat2 = *(ptr+i*4+2);
				//fDat3 = *(ptr+i*4+3);				
				uDat0 = (int)(fDat0);
				fprintf(fp,"%0f\n",fDat0);
				//uDat1 = (int)(fDat1*(1<<nShift_bit));
				//uDat2 = (int)(fDat2*(1<<nShift_bit));
				//uDat3 = (int)(fDat3*(1<<nShift_bit));
			//	fprintf(fp,"%04X%04X\n",uDat0&0xffff,uDat1&0xffff);
			//	fprintf(fp,"%04X%04X\n",uDat2&0xffff,uDat3&0xffff);
			
		}
		fclose(fp);
	}

	free(pFloatDat);
	return 0;
}



/*************************************************************************************
	��ת�������������������float�ļ���ʽ��kernal��ţ���kernal*fm*ksize*ksize
	//weight data�����ļ��ͺϲ��ļ���������һ�ݡ�ÿ��128λ��
	//�γ�8x�����ݸ�ʽ��ÿ�д�8������˵�����
**************************************************************************************/
unsigned short FcDatArray[655362][8];//[0]~[3]��ǰ4��PE�Ĳ�����[4~7]���4��PE����
int wFpgaWeight_hex()
{
	//prelu_para data
	 int nShift_bit = -1;
	//////////////////,Դ�ļ��洢�ṹ��KerCount*fm(channel)**k*k
	int nLayerPreluParaCfg[27][2]={{3,32},{32,64},{64,64},{64,64},{64,128},{128,128},\
	{128,128},{128,128},{128,128},{128,256},{256,256},{256,256},{256,256},\
	{256,256},{256,256},{256,256},{256,256},{256,256},{256,256},{256,256},\
	{256,512},{512,512},{512,512},{512,512},{512,512},{512,512},{512,512}};
		/*nLayerPreluParaCfg[27]={3*32*3*3,32*64*3*3,64*64*3*3,64*64*3*3,64*128*3*3,128*128*3*3,\
		128*128*3*3,128*128*3*3,128*128*3*3,128*256*3*3,256*256*3*3,256*256*3*3,256*256*3*3,\
		256*256*3*3,256*256*3*3,256*256*3*3,256*256*3*3,256*256*3*3,256*256*3*3,256*256*3*3,\
		256*512*3*3,512*512*3*3,512*512*3*3,512*512*3*3,512*512*3*3,512*512*3*3,512*512*3*3}; */
	int nAllCount=0;
	int i=0,j=0;
	int nKerCount=16;
	FILE *fp = NULL;
	float *pFloatDat=NULL;
	float *ptr=NULL;
	float fDat0=0.0;
	float fDat1 = 0.0;
	float fDat2 = 0.0;
	float fDat3 = 0.0,fDat4,fDat5,fDat6,fDat7;
	int n = 0,m=0;
	unsigned int uDat0,uDat1,uDat2,uDat3= 0,uDat4,uDat5,uDat6,uDat7;
	char sname[512]={0x0};
	int jj,ii=0;
	int nDepth;
	char sFloatFilename[512];
	int nTotalWeightCnt = 0;
	int ReturnFlag = 0;
	FILE *fp_all=NULL;

	memset(sname,0,512);
	sprintf(sname,"..\\out\\weight.hex\0");
	fp_all = fopen(sname,"w+");
	if(fp_all==NULL){
		printf("fopen out file error!\n");
		free(pFloatDat);
		return -1;
	}
	
	//������������
	for(jj=0;jj<27;jj++){
		printf("produce weight_%d data------------------\n",jj);
		nAllCount = nLayerPreluParaCfg[jj][0]*nLayerPreluParaCfg[jj][1]*9;
		printf("to load %d wFpgaWeight_hex data\n",nAllCount);
		memset(sFloatFilename,0,512);
		sprintf(sFloatFilename,"..\\float\\weight_%d.txt\0",jj+1);
		pFloatDat = (float *)malloc(sizeof(float)*nAllCount);
		if(pFloatDat==NULL){
			printf("malloc error!\n");
			return -1;
		}
	
		if(nAllCount!=LoadFloatFile1D(sFloatFilename,pFloatDat,nAllCount)){
			printf("fread error!\n");
			free(pFloatDat);
			return -1;
		}

	//output,ÿһ�����ݱ��浥������,nFm*Kernal*9
		fp = NULL;
		ptr = pFloatDat;
		printf("layer:%d,shiftbits:%d\n",jj,nShift_bit);
		ReturnFlag = 0;
		if(fp==NULL){
			memset(sname,0,512);
			sprintf(sname,"..\\out\\weight_%02d.hex\0",jj);
			fp = fopen(sname,"w+");
			if(fp==NULL){
				printf("fopen out file error!\n");
				free(pFloatDat);
				return -1;
			}
		}
		if(nLayerPreluParaCfg[jj][1]==32)
			n = 32;
		else if(nLayerPreluParaCfg[jj][1]==64)
			n = 64;
		else if(nLayerPreluParaCfg[jj][1]==128)
			n = 64;
		else if(nLayerPreluParaCfg[jj][1]==256){
			n = 64;
		}
		else {//512
			
				n= 32;
		}
		nShift_bit=g_Q_W_array[jj];
		if((nShift_bit<0)||(nShift_bit>15)){
			printf("wfpgaWeight_hex nShift_bit error:%d\r\n",nShift_bit);
			return -1;
		}
		//ע��:����ģ��������64�������ģ��ֿ�����
		//Ӧ��֤ÿ������˵Ķ��fm�����Ѿ����������д�����kernal
		//����FPGA���ļ���ʽ���£�
		// 00 01 02 03 08 09 10 11(9��)
		// 04 05 06 07 12 13 14 15(9��)
		// 16 17 18 19 24 25 26 27(9��)
		// 20 21 22 23 28 29 30 31(9��)
		// 32 33 34 35 40 41 42 43(9��)
		// 36 37 38 39 44 45 46 47(9��)
		// 48 49 50 51 56 57 58 59(9��)
		// 52 53 54 55 60 61 62 63(9��)

		// 64 65 66 67 72 73 74 75(9��)
		// 68 69 70 71 76 77 78 79(9��)
		// 80 81 82 83 08 09 10 11(9��)
		// 84 85 86 87 88 89 90 91(9��)
		for(m=0;m<nLayerPreluParaCfg[jj][1]/n;m++){//ÿһ�����ݣ���ģ�������д���
			//ǰ32�������,4*8(FPGA��ǰ4��ͨ��)
			//���ɵ��ļ���Ÿ�ʽ:����ͨ������һ��,128bit��
			//ͨ��1��| ͨ��2 00_01_02_03��08_09_10_11(9��)
			//ͨ��1  | ͨ��2 04_05_06_07��12_13_14_15(9��)������18��Ϊһ����λ����Ӧ0��1��FIFO
			//ͨ��3  | ͨ��4 16_17_18_19��24_25_26_27(9��)
			//ͨ��3  | ͨ��4 20_21_22_23��28_29_30_31(9��)������18��Ϊһ����λ����Ӧ2��3��FIFO
			
			//������FPGA�ڲ����洢��ʽ����
			//��1��ͨ�������00_01_02_03��04_05_06_07����8�������
			//��2��ͨ�������08_09_10_11��12_13_14_15����8�������(channel*8*9)
			//��3��ͨ�������16_17_18_19��20_21_22_23
			//��4��ͨ�������24_25_26_27��28_29_30_31
			for(nDepth = 0;nDepth <nLayerPreluParaCfg[jj][0];nDepth ++){
				//��дͨ��1��2��18������
				for(i=0;i<2;i++){
					//ÿ������˹�9������ÿ8��������ƴ����һ��ʹ��
					for(ii=0;ii<9;ii++){
						/*fDat0 = *(ptr+nDepth*nLayerPreluParaCfg[jj][1]*9 + m*n*9+i*4*9+ii);//nDepth*kernal*k*k
						fDat1 = *((ptr+nDepth*nLayerPreluParaCfg[jj][1]*9 + m*n*9+(i*4+1)*9+ii));
						fDat2 = *((ptr+nDepth*nLayerPreluParaCfg[jj][1]*9 + m*n*9+(i*4+2)*9+ii));
						fDat3 = *(ptr+nDepth*nLayerPreluParaCfg[jj][1]*9 + m*n*9+(i*4+3)*9+ii);
						
						fDat4 = *(ptr+nDepth*nLayerPreluParaCfg[jj][1]*9 + m*n*9+(i*4+8)*9+ii);//nDepth*kernal*k*k
						fDat5 = *((ptr+nDepth*nLayerPreluParaCfg[jj][1]*9 + m*n*9+(i*4+9)*9+ii));
						fDat6 = *((ptr+nDepth*nLayerPreluParaCfg[jj][1]*9 + m*n*9+(i*4+10)*9+ii));
						fDat7 = *(ptr+nDepth*nLayerPreluParaCfg[jj][1]*9 + m*n*9+(i*4+11)*9+ii);*/

						fDat0 = *(ptr+nDepth*9 + (m*n+i*4)*nLayerPreluParaCfg[jj][0]*9+ii);//nDepth*kernal*k*k
						fDat1 = *((ptr+nDepth*9 + (m*n+(i*4+1))*nLayerPreluParaCfg[jj][0]*9+ii));
						fDat2 = *((ptr+nDepth*9 + (m*n+(i*4+2))*nLayerPreluParaCfg[jj][0]*9+ii));
						fDat3 = *(ptr+nDepth*9 + (m*n+(i*4+3))*nLayerPreluParaCfg[jj][0]*9+ii);
						
						fDat4 = *(ptr+nDepth*9 + (m*n+(i*4+8))*nLayerPreluParaCfg[jj][0]*9+ii);//nDepth*kernal*k*k
						fDat5 = *((ptr+nDepth*9 + (m*n+(i*4+9))*nLayerPreluParaCfg[jj][0]*9+ii));
						fDat6 = *((ptr+nDepth*9 + (m*n+(i*4+10))*nLayerPreluParaCfg[jj][0]*9+ii));
						fDat7 = *(ptr+nDepth*9 + (m*n+(i*4+11))*nLayerPreluParaCfg[jj][0]*9+ii);

						uDat0 = (int)(fDat0*(1<<nShift_bit));
						uDat1 = (int)(fDat1*(1<<nShift_bit));
						uDat2 = (int)(fDat2*(1<<nShift_bit));
						uDat3 = (int)(fDat3*(1<<nShift_bit));
						
						uDat4 = (int)(fDat4*(1<<nShift_bit));
						uDat5 = (int)(fDat5*(1<<nShift_bit));
						uDat6 = (int)(fDat6*(1<<nShift_bit));
						uDat7 = (int)(fDat7*(1<<nShift_bit));
						
						nTotalWeightCnt+=8;
						fprintf(fp,"%04X%04X",uDat0&0xffff,uDat1&0xffff);
						fprintf(fp,"%04X%04X",uDat2&0xffff,uDat3&0xffff);
						fprintf(fp,"%04X%04X",uDat4&0xffff,uDat5&0xffff);
						fprintf(fp,"%04X%04X\n",uDat6&0xffff,uDat7&0xffff);
						
						fprintf(fp_all,"%04X%04X",uDat0&0xffff,uDat1&0xffff);
						fprintf(fp_all,"%04X%04X",uDat2&0xffff,uDat3&0xffff);
						fprintf(fp_all,"%04X%04X",uDat4&0xffff,uDat5&0xffff);
						fprintf(fp_all,"%04X%04X\n",uDat6&0xffff,uDat7&0xffff);
						
					}
					
				}
				
				//��дͨ��3��4,��֮ǰ�Ļ�����ƫ��16�������16*9
				for(i=0;i<2;i++){
					//ÿ������˹�9������ÿ8��������ƴ����һ��ʹ��
					for(ii=0;ii<9;ii++){
						fDat0 = *(ptr+nDepth*9 + (m*n+(16+i*4))*nLayerPreluParaCfg[jj][0]*9+ii);//kernal*fm*k*k
						fDat1 = *((ptr+nDepth*9 + (m*n+(i*4+16+1))*nLayerPreluParaCfg[jj][0]*9+ii));
						fDat2 = *((ptr+nDepth*9 + (m*n+(i*4+16+2))*nLayerPreluParaCfg[jj][0]*9+ii));
						fDat3 = *(ptr+nDepth*9 + (m*n+(i*4+16+3))*nLayerPreluParaCfg[jj][0]*9+ii);
						
						fDat4 = *(ptr+nDepth*9 + (m*n+(i*4+16+8))*nLayerPreluParaCfg[jj][0]*9+ii);//kernal*fm*k*k
						fDat5 = *((ptr+nDepth*9 + (m*n+(i*4+16+9))*nLayerPreluParaCfg[jj][0]*9+ii));
						fDat6 = *((ptr+nDepth*9 + (m*n+(i*4+16+10))*nLayerPreluParaCfg[jj][0]*9+ii));
						fDat7 = *(ptr+nDepth*9 + (m*n+(i*4+16+11))*nLayerPreluParaCfg[jj][0]*9+ii);
						
						uDat0 = (int)(fDat0*(1<<nShift_bit));
						uDat1 = (int)(fDat1*(1<<nShift_bit));
						uDat2 = (int)(fDat2*(1<<nShift_bit));
						uDat3 = (int)(fDat3*(1<<nShift_bit));
						
						uDat4 = (int)(fDat4*(1<<nShift_bit));
						uDat5 = (int)(fDat5*(1<<nShift_bit));
						uDat6 = (int)(fDat6*(1<<nShift_bit));
						uDat7 = (int)(fDat7*(1<<nShift_bit));
						
						nTotalWeightCnt+=8;
						fprintf(fp,"%04X%04X",uDat0&0xffff,uDat1&0xffff);
						fprintf(fp,"%04X%04X",uDat2&0xffff,uDat3&0xffff);
						fprintf(fp,"%04X%04X",uDat4&0xffff,uDat5&0xffff);
						fprintf(fp,"%04X%04X\n",uDat6&0xffff,uDat7&0xffff);
						
						fprintf(fp_all,"%04X%04X",uDat0&0xffff,uDat1&0xffff);
						fprintf(fp_all,"%04X%04X",uDat2&0xffff,uDat3&0xffff);
						fprintf(fp_all,"%04X%04X",uDat4&0xffff,uDat5&0xffff);
						fprintf(fp_all,"%04X%04X\n",uDat6&0xffff,uDat7&0xffff);
						
					}
					
				}
				
				
				
				if(n!=32){
					//��Ҫд��32�������(FPGA�ĺ�4��ͨ��)
					
					//��дͨ��1��2��18������
					//��֮ǰ�ĵ�ַ�����ϣ�ƫ��32�������32*9
					for(i=0;i<2;i++){
						//ÿ������˹�9������ÿ8��������ƴ����һ��ʹ��
						for(ii=0;ii<9;ii++){
							fDat0 = *(ptr+nDepth*9 + (m*n+(32+i*4))*nLayerPreluParaCfg[jj][0]*9+ii);//nDepth*kernal*k*k
							fDat1 = *((ptr+nDepth*9 + (m*n+(32+i*4+1))*nLayerPreluParaCfg[jj][0]*9+ii));
							fDat2 = *((ptr+nDepth*9 + (m*n+(32+i*4+2))*nLayerPreluParaCfg[jj][0]*9+ii));
							fDat3 = *(ptr+nDepth*9 + (m*n+(32+i*4+3))*nLayerPreluParaCfg[jj][0]*9+ii);
							
							fDat4 = *(ptr+nDepth*9 + (m*n+(32+i*4+8))*nLayerPreluParaCfg[jj][0]*9+ii);//nDepth*kernal*k*k
							fDat5 = *((ptr+nDepth*9 + (m*n+(32+i*4+9))*nLayerPreluParaCfg[jj][0]*9+ii));
							fDat6 = *((ptr+nDepth*9 + (m*n+(32+i*4+10))*nLayerPreluParaCfg[jj][0]*9+ii));
							fDat7 = *(ptr+nDepth*9 + (m*n+(32+i*4+11))*nLayerPreluParaCfg[jj][0]*9+ii);
							
							uDat0 = (int)(fDat0*(1<<nShift_bit));
							uDat1 = (int)(fDat1*(1<<nShift_bit));
							uDat2 = (int)(fDat2*(1<<nShift_bit));
							uDat3 = (int)(fDat3*(1<<nShift_bit));
							
							uDat4 = (int)(fDat4*(1<<nShift_bit));
							uDat5 = (int)(fDat5*(1<<nShift_bit));
							uDat6 = (int)(fDat6*(1<<nShift_bit));
							uDat7 = (int)(fDat7*(1<<nShift_bit));
							
							nTotalWeightCnt+=8;
							fprintf(fp,"%04X%04X",uDat0&0xffff,uDat1&0xffff);
							fprintf(fp,"%04X%04X",uDat2&0xffff,uDat3&0xffff);
							fprintf(fp,"%04X%04X",uDat4&0xffff,uDat5&0xffff);
							fprintf(fp,"%04X%04X\n",uDat6&0xffff,uDat7&0xffff);
							
							fprintf(fp_all,"%04X%04X",uDat0&0xffff,uDat1&0xffff);
							fprintf(fp_all,"%04X%04X",uDat2&0xffff,uDat3&0xffff);
							fprintf(fp_all,"%04X%04X",uDat4&0xffff,uDat5&0xffff);
							fprintf(fp_all,"%04X%04X\n",uDat6&0xffff,uDat7&0xffff);
							
						}
						
					}
					
					//��дͨ��3��4
					//��֮ǰ�ĵ�ַ�����ϣ�ƫ��48�������48*9
					for(i=0;i<2;i++){
						//ÿ������˹�9������ÿ8��������ƴ����һ��ʹ��
						for(ii=0;ii<9;ii++){
							fDat0 = *(ptr+nDepth*9 + (m*n+(48+i*4))*nLayerPreluParaCfg[jj][0]*9+ii);//kernal*fm*k*k
							fDat1 = *((ptr+nDepth*9 + (m*n+(48+i*4+1))*nLayerPreluParaCfg[jj][0]*9+ii));
							fDat2 = *((ptr+nDepth*9 + (m*n+(48+i*4+2))*nLayerPreluParaCfg[jj][0]*9+ii));
							fDat3 = *(ptr+nDepth*9 + (m*n+(48+i*4+3))*nLayerPreluParaCfg[jj][0]*9+ii);
							
							fDat4 = *(ptr+nDepth*9 + (m*n+(48+i*4+8))*nLayerPreluParaCfg[jj][0]*9+ii);//kernal*fm*k*k
							fDat5 = *((ptr+nDepth*9 + (m*n+(48+i*4+9))*nLayerPreluParaCfg[jj][0]*9+ii));
							fDat6 = *((ptr+nDepth*9 + (m*n+(48+i*4+10))*nLayerPreluParaCfg[jj][0]*9+ii));
							fDat7 = *(ptr+nDepth*9 + (m*n+(48+i*4+11))*nLayerPreluParaCfg[jj][0]*9+ii);
							
							uDat0 = (int)(fDat0*(1<<nShift_bit));
							uDat1 = (int)(fDat1*(1<<nShift_bit));
							uDat2 = (int)(fDat2*(1<<nShift_bit));
							uDat3 = (int)(fDat3*(1<<nShift_bit));
							
							uDat4 = (int)(fDat4*(1<<nShift_bit));
							uDat5 = (int)(fDat5*(1<<nShift_bit));
							uDat6 = (int)(fDat6*(1<<nShift_bit));
							uDat7 = (int)(fDat7*(1<<nShift_bit));
							
							nTotalWeightCnt+=8;
							fprintf(fp,"%04X%04X",uDat0&0xffff,uDat1&0xffff);
							fprintf(fp,"%04X%04X",uDat2&0xffff,uDat3&0xffff);
							fprintf(fp,"%04X%04X",uDat4&0xffff,uDat5&0xffff);
							fprintf(fp,"%04X%04X\n",uDat6&0xffff,uDat7&0xffff);
							
							fprintf(fp_all,"%04X%04X",uDat0&0xffff,uDat1&0xffff);
							fprintf(fp_all,"%04X%04X",uDat2&0xffff,uDat3&0xffff);
							fprintf(fp_all,"%04X%04X",uDat4&0xffff,uDat5&0xffff);
							fprintf(fp_all,"%04X%04X\n",uDat6&0xffff,uDat7&0xffff);
							
						}
						
					}
					
				}
					}

		}//end 
		fclose(fp);
		fp = NULL;//һ��Ҫ�ÿ�
		
	free(pFloatDat);
	
	}

	//FCȫ���Ӳ�
	//Ϊ��֤д���˳��������һ�£���Ҫ��һ�´洢˳��
	//����㣺ÿ��128λ����������ͨ��8��kernal,ÿ��ͨ����������18�ģ���֤2��PE�����ݣ�
	//�����ڴ�����֯���� 512*512*5*4��short���ݣ���֯��655360*8
	//���ڲ���18������������Ҫ�����Ч����
	char bFlag=0;//д��4�����ݱ�־
	int  nPos=0;
	char cLoop=0;//0~17 loop;
	{
		printf("produce weight_fc data------------------\n",jj);
		nShift_bit=g_Q_W_array[27];
		if((nShift_bit<0)||(nShift_bit>15)){
			printf("wfpgaWeight_hex nShift_bit error:%d\r\n",nShift_bit);
			return -1;
		}

		nAllCount = 512*512*5*4;
		printf("to load %d weight_fc data\n",nAllCount);
		memset(sFloatFilename,0,512);
		sprintf(sFloatFilename,"..\\float\\weight_fc.txt\0");
		pFloatDat = (float *)malloc(sizeof(float)*nAllCount);
		if(pFloatDat==NULL){
			printf("malloc error!\n");
			return -1;
		}
		
		if(nAllCount!=LoadFloatFile1D(sFloatFilename,pFloatDat,nAllCount)){
			printf("fread error!\n");
			free(pFloatDat);
			return -1;
		}
		
		fp = NULL;
		ptr = pFloatDat;

		//512*512*20
		memset(sname,0,512);
		sprintf(sname,"..\\out\\weight_fc.hex\0");
		fp = fopen(sname,"w");
		if(fp==NULL){
			printf("fopen out file error!\n");
			free(pFloatDat);
			return -1;
		}
		nPos=0;//����λ��
		cLoop=0;//0~17
		bFlag = 0;//�ߵ�4��kernal�ı�־
		int kernal,fm;
		for(kernal=0;kernal<512;kernal++){//kernal=512
			for(fm=0;fm<512;fm=fm+4){
				for(ii=0;ii<20;ii++){
					fDat0 = *(ptr+kernal*512*20 +fm*20+ii);//nDepth*kernal*k*k
					fDat1 = *((ptr+kernal*512*20 + (fm+1)*20+ii));
					fDat2 = *((ptr+kernal*512*20 + (fm+2)*20+ii));
					fDat3 = *(ptr+kernal*512*20 + (fm+3)*20+ii);
					
					
					uDat0 = (int)(fDat0*(1<<nShift_bit));
					uDat1 = (int)(fDat1*(1<<nShift_bit));
					uDat2 = (int)(fDat2*(1<<nShift_bit));
					
					uDat3 = (int)(fDat3*(1<<nShift_bit));
					uDat0 = (int)(fDat0*(1<<nShift_bit));
					uDat1 = (int)(fDat1*(1<<nShift_bit));
					uDat2 = (int)(fDat2*(1<<nShift_bit));					
					uDat3 = (int)(fDat3*(1<<nShift_bit));
					FcDatArray[nPos+cLoop][bFlag*4+0]=uDat0&0xffff;
					FcDatArray[nPos+cLoop][bFlag*4+1]=uDat1&0xffff;
					FcDatArray[nPos+cLoop][bFlag*4+2]=uDat2&0xffff;
					FcDatArray[nPos+cLoop][bFlag*4+3]=uDat3&0xffff;
					
					if(cLoop>=17){
						cLoop = 0;
						if(bFlag){//˵��18�����ݸߵ;���д������Ҫλ�ü�18
							nPos+=18;
							bFlag=0;
						}else{	//˵��д��18����λ�õ�������ʼд��λ������
							bFlag = 1;
						}
					}else {
						cLoop++;
					}
					nTotalWeightCnt+=4;
				
				}

			}

		}
	}

		//pad��Ч����
	while(1){
		if((bFlag==0)&&(cLoop==0))
			break;
		FcDatArray[nPos+cLoop][bFlag*4+0]=0x0000;
		FcDatArray[nPos+cLoop][bFlag*4+1]=0x0000;
		FcDatArray[nPos+cLoop][bFlag*4+2]=0x0000;
		FcDatArray[nPos+cLoop][bFlag*4+3]=0x0000;
		if(cLoop>=17){
			if(bFlag){
				bFlag=0;
				nPos+=18;
			}else{
				bFlag=1;
			}
			cLoop=0;
		}else {
			cLoop++;
		}			
	}

	//ȫ���Ӳ�����д�ļ�
	for(j=0;j<655362;j++){
		fprintf(fp,"%04X%04X%04X%04X%04X%04X%04X%04X\n",FcDatArray[j][0]&0xffff,FcDatArray[j][1]&0xffff,FcDatArray[j][2]&0xffff,FcDatArray[j][3]&0xffff,FcDatArray[j][4]&0xffff,FcDatArray[j][5]&0xffff,FcDatArray[j][6]&0xffff,FcDatArray[j][7]&0xffff);
		fprintf(fp_all,"%04X%04X%04X%04X%04X%04X%04X%04X\n",FcDatArray[j][0]&0xffff,FcDatArray[j][1]&0xffff,FcDatArray[j][2]&0xffff,FcDatArray[j][3]&0xffff,FcDatArray[j][4]&0xffff,FcDatArray[j][5]&0xffff,FcDatArray[j][6]&0xffff,FcDatArray[j][7]&0xffff);
	}
	fclose(fp);
	fclose(fp_all);
	printf("**************weight total cnt:%d***********\n",nTotalWeightCnt);
	return 0;
}

//�ú��������float weight��ʽ����channel��ţ�ÿ��channel���kernal*ksize*ksize������
//�ϵ�ת������
int bk_wFpgaWeight_hex()
{
	//prelu_para data
	 int nShift_bit = -1;
	//////////////////,Դ�ļ��洢�ṹ��nDepth*KerCount*k*k
	int nLayerPreluParaCfg[27][2]={{3,32},{32,64},{64,64},{64,64},{64,128},{128,128},\
	{128,128},{128,128},{128,128},{128,256},{256,256},{256,256},{256,256},\
	{256,256},{256,256},{256,256},{256,256},{256,256},{256,256},{256,256},\
	{256,512},{512,512},{512,512},{512,512},{512,512},{512,512},{512,512}};
		/*nLayerPreluParaCfg[27]={3*32*3*3,32*64*3*3,64*64*3*3,64*64*3*3,64*128*3*3,128*128*3*3,\
		128*128*3*3,128*128*3*3,128*128*3*3,128*256*3*3,256*256*3*3,256*256*3*3,256*256*3*3,\
		256*256*3*3,256*256*3*3,256*256*3*3,256*256*3*3,256*256*3*3,256*256*3*3,256*256*3*3,\
		256*512*3*3,512*512*3*3,512*512*3*3,512*512*3*3,512*512*3*3,512*512*3*3,512*512*3*3}; */
	int nAllCount=0;
	int i=0,j=0;
	int nKerCount=16;
	FILE *fp = NULL;
	float *pFloatDat=NULL;
	float *ptr=NULL;
	float fDat0=0.0;
	float fDat1 = 0.0;
	float fDat2 = 0.0;
	float fDat3 = 0.0,fDat4,fDat5,fDat6,fDat7;
	int n = 0,m=0;
	unsigned int uDat0,uDat1,uDat2,uDat3= 0,uDat4,uDat5,uDat6,uDat7;
	char sname[512]={0x0};
	int jj,ii=0;
	int nDepth;
	char sFloatFilename[512];
	int nTotalWeightCnt = 0;
	int ReturnFlag = 0;
	FILE *fp_all=NULL;

	memset(sname,0,512);
	sprintf(sname,"..\\out\\weight.hex\0");
	fp_all = fopen(sname,"w+");
	if(fp_all==NULL){
		printf("fopen out file error!\n");
		free(pFloatDat);
		return -1;
	}
	
	//������������
	for(jj=0;jj<27;jj++){
		printf("produce weight_%d data------------------\n",jj);
		nAllCount = nLayerPreluParaCfg[jj][0]*nLayerPreluParaCfg[jj][1]*9;
		printf("to load %d wFpgaWeight_hex data\n",nAllCount);
		memset(sFloatFilename,0,512);
		sprintf(sFloatFilename,"..\\float\\weight_%d.txt\0",jj+1);
		pFloatDat = (float *)malloc(sizeof(float)*nAllCount);
		if(pFloatDat==NULL){
			printf("malloc error!\n");
			return -1;
		}
	
		if(nAllCount!=LoadFloatFile1D(sFloatFilename,pFloatDat,nAllCount)){
			printf("fread error!\n");
			free(pFloatDat);
			return -1;
		}

	//output,ÿһ�����ݱ��浥������,nFm*Kernal*9
		fp = NULL;
		ptr = pFloatDat;
		printf("layer:%d,shiftbits:%d\n",jj,nShift_bit);
		ReturnFlag = 0;
		if(fp==NULL){
			memset(sname,0,512);
			sprintf(sname,"..\\out\\weight_%02d.hex\0",jj);
			fp = fopen(sname,"w+");
			if(fp==NULL){
				printf("fopen out file error!\n");
				free(pFloatDat);
				return -1;
			}
		}
		if(nLayerPreluParaCfg[jj][1]==32)
			n = 32;
		else if(nLayerPreluParaCfg[jj][1]==64)
			n = 64;
		else if(nLayerPreluParaCfg[jj][1]==128)
			n = 64;
		else if(nLayerPreluParaCfg[jj][1]==256){
			n = 64;
		}
		else {//512
			//ȥ���ָ���32��keral
			/*//conv4Ҫ�ر���
			if(nLayerPreluParaCfg[jj][0]==256){
				n = 64;
			}
			else*/
				n= 32;
		}
		nShift_bit=g_Q_W_array[jj];
		if((nShift_bit<0)||(nShift_bit>15)){
			printf("wfpgaWeight_hex nShift_bit error:%d\r\n",nShift_bit);
			return -1;
		}
		//ע��:����ģ��������64�������ģ��ֿ�����
		//Ӧ��֤ÿ������˵Ķ��fm�����Ѿ����������д�����kernal
		//����FPGA���ļ���ʽ���£�
		// 00 01 02 03 08 09 10 11(9��)
		// 04 05 06 07 12 13 14 15(9��)
		// 16 17 18 19 24 25 26 27(9��)
		// 20 21 22 23 28 29 30 31(9��)
		// 32 33 34 35 40 41 42 43(9��)
		// 36 37 38 39 44 45 46 47(9��)
		// 48 49 50 51 56 57 58 59(9��)
		// 52 53 54 55 60 61 62 63(9��)

		// 64 65 66 67 72 73 74 75(9��)
		// 68 69 70 71 76 77 78 79(9��)
		// 80 81 82 83 08 09 10 11(9��)
		// 84 85 86 87 88 89 90 91(9��)
		for(m=0;m<nLayerPreluParaCfg[jj][1]/n;m++){//ÿһ�����ݣ���ģ�������д���
			//ǰ32�������,4*8(FPGA��ǰ4��ͨ��)
			//���ɵ��ļ���Ÿ�ʽ:����ͨ������һ��,128bit��
			//ͨ��1��| ͨ��2 00_01_02_03��08_09_10_11(9��)
			//ͨ��1  | ͨ��2 04_05_06_07��12_13_14_15(9��)������18��Ϊһ����λ����Ӧ0��1��FIFO
			//ͨ��3  | ͨ��4 16_17_18_19��24_25_26_27(9��)
			//ͨ��3  | ͨ��4 20_21_22_23��28_29_30_31(9��)������18��Ϊһ����λ����Ӧ2��3��FIFO
			
			//������FPGA�ڲ����洢��ʽ����
			//��1��ͨ�������00_01_02_03��04_05_06_07����8�������
			//��2��ͨ�������08_09_10_11��12_13_14_15����8�������(channel*8*9)
			//��3��ͨ�������16_17_18_19��20_21_22_23
			//��4��ͨ�������24_25_26_27��28_29_30_31
			for(nDepth = 0;nDepth <nLayerPreluParaCfg[jj][0];nDepth ++){
				//��дͨ��1��2��18������
				for(i=0;i<2;i++){
					//ÿ������˹�9������ÿ8��������ƴ����һ��ʹ��
					for(ii=0;ii<9;ii++){
						fDat0 = *(ptr+nDepth*nLayerPreluParaCfg[jj][1]*9 + m*n*9+i*4*9+ii);//nDepth*kernal*k*k
						fDat1 = *((ptr+nDepth*nLayerPreluParaCfg[jj][1]*9 + m*n*9+(i*4+1)*9+ii));
						fDat2 = *((ptr+nDepth*nLayerPreluParaCfg[jj][1]*9 + m*n*9+(i*4+2)*9+ii));
						fDat3 = *(ptr+nDepth*nLayerPreluParaCfg[jj][1]*9 + m*n*9+(i*4+3)*9+ii);
						
						fDat4 = *(ptr+nDepth*nLayerPreluParaCfg[jj][1]*9 + m*n*9+(i*4+8)*9+ii);//nDepth*kernal*k*k
						fDat5 = *((ptr+nDepth*nLayerPreluParaCfg[jj][1]*9 + m*n*9+(i*4+9)*9+ii));
						fDat6 = *((ptr+nDepth*nLayerPreluParaCfg[jj][1]*9 + m*n*9+(i*4+10)*9+ii));
						fDat7 = *(ptr+nDepth*nLayerPreluParaCfg[jj][1]*9 + m*n*9+(i*4+11)*9+ii);
						
						uDat0 = (int)(fDat0*(1<<nShift_bit));
						uDat1 = (int)(fDat1*(1<<nShift_bit));
						uDat2 = (int)(fDat2*(1<<nShift_bit));
						uDat3 = (int)(fDat3*(1<<nShift_bit));
						
						uDat4 = (int)(fDat4*(1<<nShift_bit));
						uDat5 = (int)(fDat5*(1<<nShift_bit));
						uDat6 = (int)(fDat6*(1<<nShift_bit));
						uDat7 = (int)(fDat7*(1<<nShift_bit));
						
						nTotalWeightCnt+=8;
						fprintf(fp,"%04X%04X",uDat0&0xffff,uDat1&0xffff);
						fprintf(fp,"%04X%04X",uDat2&0xffff,uDat3&0xffff);
						fprintf(fp,"%04X%04X",uDat4&0xffff,uDat5&0xffff);
						fprintf(fp,"%04X%04X\n",uDat6&0xffff,uDat7&0xffff);
						
						fprintf(fp_all,"%04X%04X",uDat0&0xffff,uDat1&0xffff);
						fprintf(fp_all,"%04X%04X",uDat2&0xffff,uDat3&0xffff);
						fprintf(fp_all,"%04X%04X",uDat4&0xffff,uDat5&0xffff);
						fprintf(fp_all,"%04X%04X\n",uDat6&0xffff,uDat7&0xffff);
						
					}
					
				}
				
				//��дͨ��3��4,��֮ǰ�Ļ�����ƫ��16�������16*9
				for(i=0;i<2;i++){
					//ÿ������˹�9������ÿ8��������ƴ����һ��ʹ��
					for(ii=0;ii<9;ii++){
						fDat0 = *(ptr+nDepth*nLayerPreluParaCfg[jj][1]*9 + m*n*9+(16+i*4)*9+ii);//nDepth*kernal*k*k
						fDat1 = *((ptr+nDepth*nLayerPreluParaCfg[jj][1]*9 + m*n*9+(i*4+16+1)*9+ii));
						fDat2 = *((ptr+nDepth*nLayerPreluParaCfg[jj][1]*9 + m*n*9+(i*4+16+2)*9+ii));
						fDat3 = *(ptr+nDepth*nLayerPreluParaCfg[jj][1]*9 + m*n*9+(i*4+16+3)*9+ii);
						
						fDat4 = *(ptr+nDepth*nLayerPreluParaCfg[jj][1]*9 + m*n*9+(i*4+16+8)*9+ii);//nDepth*kernal*k*k
						fDat5 = *((ptr+nDepth*nLayerPreluParaCfg[jj][1]*9 + m*n*9+(i*4+16+9)*9+ii));
						fDat6 = *((ptr+nDepth*nLayerPreluParaCfg[jj][1]*9 + m*n*9+(i*4+16+10)*9+ii));
						fDat7 = *(ptr+nDepth*nLayerPreluParaCfg[jj][1]*9 + m*n*9+(i*4+16+11)*9+ii);
						
						uDat0 = (int)(fDat0*(1<<nShift_bit));
						uDat1 = (int)(fDat1*(1<<nShift_bit));
						uDat2 = (int)(fDat2*(1<<nShift_bit));
						uDat3 = (int)(fDat3*(1<<nShift_bit));
						
						uDat4 = (int)(fDat4*(1<<nShift_bit));
						uDat5 = (int)(fDat5*(1<<nShift_bit));
						uDat6 = (int)(fDat6*(1<<nShift_bit));
						uDat7 = (int)(fDat7*(1<<nShift_bit));
						
						nTotalWeightCnt+=8;
						fprintf(fp,"%04X%04X",uDat0&0xffff,uDat1&0xffff);
						fprintf(fp,"%04X%04X",uDat2&0xffff,uDat3&0xffff);
						fprintf(fp,"%04X%04X",uDat4&0xffff,uDat5&0xffff);
						fprintf(fp,"%04X%04X\n",uDat6&0xffff,uDat7&0xffff);
						
						fprintf(fp_all,"%04X%04X",uDat0&0xffff,uDat1&0xffff);
						fprintf(fp_all,"%04X%04X",uDat2&0xffff,uDat3&0xffff);
						fprintf(fp_all,"%04X%04X",uDat4&0xffff,uDat5&0xffff);
						fprintf(fp_all,"%04X%04X\n",uDat6&0xffff,uDat7&0xffff);
						
					}
					
				}
				
				
				
				if(n!=32){
					//��Ҫд��32�������(FPGA�ĺ�4��ͨ��)
					
					//��дͨ��1��2��18������
					//��֮ǰ�ĵ�ַ�����ϣ�ƫ��32�������32*9
					for(i=0;i<2;i++){
						//ÿ������˹�9������ÿ8��������ƴ����һ��ʹ��
						for(ii=0;ii<9;ii++){
							fDat0 = *(ptr+nDepth*nLayerPreluParaCfg[jj][1]*9 + m*n*9+(32+i*4)*9+ii);//nDepth*kernal*k*k
							fDat1 = *((ptr+nDepth*nLayerPreluParaCfg[jj][1]*9 + m*n*9+(32+i*4+1)*9+ii));
							fDat2 = *((ptr+nDepth*nLayerPreluParaCfg[jj][1]*9 + m*n*9+(32+i*4+2)*9+ii));
							fDat3 = *(ptr+nDepth*nLayerPreluParaCfg[jj][1]*9 + m*n*9+(32+i*4+3)*9+ii);
							
							fDat4 = *(ptr+nDepth*nLayerPreluParaCfg[jj][1]*9 + m*n*9+(32+i*4+8)*9+ii);//nDepth*kernal*k*k
							fDat5 = *((ptr+nDepth*nLayerPreluParaCfg[jj][1]*9 + m*n*9+(32+i*4+9)*9+ii));
							fDat6 = *((ptr+nDepth*nLayerPreluParaCfg[jj][1]*9 + m*n*9+(32+i*4+10)*9+ii));
							fDat7 = *(ptr+nDepth*nLayerPreluParaCfg[jj][1]*9 + m*n*9+(32+i*4+11)*9+ii);
							
							uDat0 = (int)(fDat0*(1<<nShift_bit));
							uDat1 = (int)(fDat1*(1<<nShift_bit));
							uDat2 = (int)(fDat2*(1<<nShift_bit));
							uDat3 = (int)(fDat3*(1<<nShift_bit));
							
							uDat4 = (int)(fDat4*(1<<nShift_bit));
							uDat5 = (int)(fDat5*(1<<nShift_bit));
							uDat6 = (int)(fDat6*(1<<nShift_bit));
							uDat7 = (int)(fDat7*(1<<nShift_bit));
							
							nTotalWeightCnt+=8;
							fprintf(fp,"%04X%04X",uDat0&0xffff,uDat1&0xffff);
							fprintf(fp,"%04X%04X",uDat2&0xffff,uDat3&0xffff);
							fprintf(fp,"%04X%04X",uDat4&0xffff,uDat5&0xffff);
							fprintf(fp,"%04X%04X\n",uDat6&0xffff,uDat7&0xffff);
							
							fprintf(fp_all,"%04X%04X",uDat0&0xffff,uDat1&0xffff);
							fprintf(fp_all,"%04X%04X",uDat2&0xffff,uDat3&0xffff);
							fprintf(fp_all,"%04X%04X",uDat4&0xffff,uDat5&0xffff);
							fprintf(fp_all,"%04X%04X\n",uDat6&0xffff,uDat7&0xffff);
							
						}
						
					}
					
					//��дͨ��3��4
					//��֮ǰ�ĵ�ַ�����ϣ�ƫ��48�������48*9
					for(i=0;i<2;i++){
						//ÿ������˹�9������ÿ8��������ƴ����һ��ʹ��
						for(ii=0;ii<9;ii++){
							fDat0 = *(ptr+nDepth*nLayerPreluParaCfg[jj][1]*9 + m*n*9+(48+i*4)*9+ii);//nDepth*kernal*k*k
							fDat1 = *((ptr+nDepth*nLayerPreluParaCfg[jj][1]*9 + m*n*9+(48+i*4+1)*9+ii));
							fDat2 = *((ptr+nDepth*nLayerPreluParaCfg[jj][1]*9 + m*n*9+(48+i*4+2)*9+ii));
							fDat3 = *(ptr+nDepth*nLayerPreluParaCfg[jj][1]*9 + m*n*9+(48+i*4+3)*9+ii);
							
							fDat4 = *(ptr+nDepth*nLayerPreluParaCfg[jj][1]*9 + m*n*9+(48+i*4+8)*9+ii);//nDepth*kernal*k*k
							fDat5 = *((ptr+nDepth*nLayerPreluParaCfg[jj][1]*9 + m*n*9+(48+i*4+9)*9+ii));
							fDat6 = *((ptr+nDepth*nLayerPreluParaCfg[jj][1]*9 + m*n*9+(48+i*4+10)*9+ii));
							fDat7 = *(ptr+nDepth*nLayerPreluParaCfg[jj][1]*9 + m*n*9+(48+i*4+11)*9+ii);
							
							uDat0 = (int)(fDat0*(1<<nShift_bit));
							uDat1 = (int)(fDat1*(1<<nShift_bit));
							uDat2 = (int)(fDat2*(1<<nShift_bit));
							uDat3 = (int)(fDat3*(1<<nShift_bit));
							
							uDat4 = (int)(fDat4*(1<<nShift_bit));
							uDat5 = (int)(fDat5*(1<<nShift_bit));
							uDat6 = (int)(fDat6*(1<<nShift_bit));
							uDat7 = (int)(fDat7*(1<<nShift_bit));
							
							nTotalWeightCnt+=8;
							fprintf(fp,"%04X%04X",uDat0&0xffff,uDat1&0xffff);
							fprintf(fp,"%04X%04X",uDat2&0xffff,uDat3&0xffff);
							fprintf(fp,"%04X%04X",uDat4&0xffff,uDat5&0xffff);
							fprintf(fp,"%04X%04X\n",uDat6&0xffff,uDat7&0xffff);
							
							fprintf(fp_all,"%04X%04X",uDat0&0xffff,uDat1&0xffff);
							fprintf(fp_all,"%04X%04X",uDat2&0xffff,uDat3&0xffff);
							fprintf(fp_all,"%04X%04X",uDat4&0xffff,uDat5&0xffff);
							fprintf(fp_all,"%04X%04X\n",uDat6&0xffff,uDat7&0xffff);
							
						}
						
					}
					
				}
					}

		}//end 
		fclose(fp);
		fp = NULL;//һ��Ҫ�ÿ�
		
	free(pFloatDat);
	
	}

	//FCȫ���Ӳ�
	//Ϊ��֤д���˳��������һ�£���Ҫ��һ�´洢˳��
	//����㣺ÿ��128λ����������ͨ��8��kernal,ÿ��ͨ����������18�ģ���֤2��PE�����ݣ�
	//�����ڴ�����֯���� 512*512*5*4��short���ݣ���֯��655360*8
	//���ڲ���18������������Ҫ�����Ч����
	char bFlag=0;//д��4�����ݱ�־
	int  nPos=0;
	char cLoop=0;//0~17 loop;
	{
		printf("produce weight_fc data------------------\n",jj);
		nShift_bit=g_Q_W_array[27];
		if((nShift_bit<0)||(nShift_bit>15)){
			printf("wfpgaWeight_hex nShift_bit error:%d\r\n",nShift_bit);
			return -1;
		}

		nAllCount = 512*512*5*4;
		printf("to load %d weight_fc data\n",nAllCount);
		memset(sFloatFilename,0,512);
		sprintf(sFloatFilename,"..\\float\\weight_fc.txt\0");
		pFloatDat = (float *)malloc(sizeof(float)*nAllCount);
		if(pFloatDat==NULL){
			printf("malloc error!\n");
			return -1;
		}
		
		if(nAllCount!=LoadFloatFile1D(sFloatFilename,pFloatDat,nAllCount)){
			printf("fread error!\n");
			free(pFloatDat);
			return -1;
		}
		
		fp = NULL;
		ptr = pFloatDat;

		//512*512*20
		memset(sname,0,512);
		sprintf(sname,"..\\out\\weight_fc.hex\0");
		fp = fopen(sname,"w");
		if(fp==NULL){
			printf("fopen out file error!\n");
			free(pFloatDat);
			return -1;
		}
		nPos=0;//����λ��
		cLoop=0;//0~17
		bFlag = 0;//�ߵ�4��kernal�ı�־
		for(nDepth=0;nDepth<512;nDepth++){//nFm=512
			for(j=0;j<512;j=j+4){
				for(ii=0;ii<20;ii++){
					fDat0 = *(ptr+nDepth*512*20 +j*20+ii);//nDepth*kernal*k*k
					fDat1 = *((ptr+nDepth*512*20 + (j+1)*20+ii));
					fDat2 = *((ptr+nDepth*512*20 + (j+2)*20+ii));
					fDat3 = *(ptr+nDepth*512*20 + (j+3)*20+ii);
					
					
					uDat0 = (int)(fDat0*(1<<nShift_bit));
					uDat1 = (int)(fDat1*(1<<nShift_bit));
					uDat2 = (int)(fDat2*(1<<nShift_bit));
					
					uDat3 = (int)(fDat3*(1<<nShift_bit));
					uDat0 = (int)(fDat0*(1<<nShift_bit));
					uDat1 = (int)(fDat1*(1<<nShift_bit));
					uDat2 = (int)(fDat2*(1<<nShift_bit));					
					uDat3 = (int)(fDat3*(1<<nShift_bit));
					FcDatArray[nPos+cLoop][bFlag*4+0]=uDat0&0xffff;
					FcDatArray[nPos+cLoop][bFlag*4+1]=uDat1&0xffff;
					FcDatArray[nPos+cLoop][bFlag*4+2]=uDat2&0xffff;
					FcDatArray[nPos+cLoop][bFlag*4+3]=uDat3&0xffff;
					
					if(cLoop>=17){
						cLoop = 0;
						if(bFlag){//˵��18�����ݸߵ;���д������Ҫλ�ü�18
							nPos+=18;
							bFlag=0;
						}else{	//˵��д��18����λ�õ�������ʼд��λ������
							bFlag = 1;
						}
					}else {
						cLoop++;
					}
					nTotalWeightCnt+=4;
				
				}

			}

		}
	}

		//pad��Ч����
	while(1){
		if((bFlag==0)&&(cLoop==0))
			break;
		FcDatArray[nPos+cLoop][bFlag*4+0]=0x0000;
		FcDatArray[nPos+cLoop][bFlag*4+1]=0x0000;
		FcDatArray[nPos+cLoop][bFlag*4+2]=0x0000;
		FcDatArray[nPos+cLoop][bFlag*4+3]=0x0000;
		if(cLoop>=17){
			if(bFlag){
				bFlag=0;
				nPos+=18;
			}else{
				bFlag=1;
			}
			cLoop=0;
		}else {
			cLoop++;
		}			
	}

	//ȫ���Ӳ�����д�ļ�
	for(j=0;j<655362;j++){
		fprintf(fp,"%04X%04X%04X%04X%04X%04X%04X%04X\n",FcDatArray[j][0]&0xffff,FcDatArray[j][1]&0xffff,FcDatArray[j][2]&0xffff,FcDatArray[j][3]&0xffff,FcDatArray[j][4]&0xffff,FcDatArray[j][5]&0xffff,FcDatArray[j][6]&0xffff,FcDatArray[j][7]&0xffff);
		fprintf(fp_all,"%04X%04X%04X%04X%04X%04X%04X%04X\n",FcDatArray[j][0]&0xffff,FcDatArray[j][1]&0xffff,FcDatArray[j][2]&0xffff,FcDatArray[j][3]&0xffff,FcDatArray[j][4]&0xffff,FcDatArray[j][5]&0xffff,FcDatArray[j][6]&0xffff,FcDatArray[j][7]&0xffff);
	}
	fclose(fp);
	fclose(fp_all);
	printf("**************weight total cnt:%d***********\n",nTotalWeightCnt);
	return 0;
}
//weight data,Cģ���ã���27���ļ�˳���š�kernal*fm(channel)*9
//**ע�⣺Դ����float�ͣ����ڸĳɴ��˳����kernal*fm*9**,������ʱ�Ľ�
int wFpgaWeight_CModel()
{
	//prelu_para data
	 int nShift_bit = -1;
	//////////////////,Դ�ļ��洢�ṹ��nDepth*KerCount*k*k
	int nLayerPreluParaCfg[27][2]={{3,32},{32,64},{64,64},{64,64},{64,128},{128,128},\
	{128,128},{128,128},{128,128},{128,256},{256,256},{256,256},{256,256},\
	{256,256},{256,256},{256,256},{256,256},{256,256},{256,256},{256,256},\
	{256,512},{512,512},{512,512},{512,512},{512,512},{512,512},{512,512}};
		/*nLayerPreluParaCfg[27]={3*32*3*3,32*64*3*3,64*64*3*3,64*64*3*3,64*128*3*3,128*128*3*3,\
		128*128*3*3,128*128*3*3,128*128*3*3,128*256*3*3,256*256*3*3,256*256*3*3,256*256*3*3,\
		256*256*3*3,256*256*3*3,256*256*3*3,256*256*3*3,256*256*3*3,256*256*3*3,256*256*3*3,\
		256*512*3*3,512*512*3*3,512*512*3*3,512*512*3*3,512*512*3*3,512*512*3*3,512*512*3*3}; */
	int nAllCount=0;
	int i=0,j=0;
	int nKerCount=16;
	FILE *fp = NULL;
	float *pFloatDat=NULL;
	float *ptr=NULL;
	float fDat0=0.0;
	float fDat1 = 0.0;
	float fDat2 = 0.0;
	float fDat3 = 0.0;
	int n = 0,m=0;
	unsigned int uDat0,uDat3= 0;
	char sname[512]={0x0};
	int jj,ii=0;
	int nDepth;
	char sFloatFilename[512];
	
	//������������
	for(jj=0;jj<27;jj++){
		printf("produce weight_%d data------------------\n",jj);
		nAllCount = nLayerPreluParaCfg[jj][0]*nLayerPreluParaCfg[jj][1]*9;
		printf("to load %d wFpgaWeight_CModel data\n",nAllCount);
		memset(sFloatFilename,0,512);
		sprintf(sFloatFilename,"..\\float\\weight_%d.txt\0",jj+1);
		pFloatDat = (float *)malloc(sizeof(float)*nAllCount);
		if(pFloatDat==NULL){
			printf("malloc error!\n");
			return -1;
		}
	
		if(nAllCount!=LoadFloatFile1D(sFloatFilename,pFloatDat,nAllCount)){
			printf("fread error!\n");
			free(pFloatDat);
			return -1;
		}

	//output,ÿһ�����ݱ��浥������
		if(fp==NULL){
			memset(sname,0,512);
			sprintf(sname,"..\\out\\weight_%02d.cmodel\0",jj);
			fp = fopen(sname,"w+");
			if(fp==NULL){
				printf("fopen out file error!\n");
				free(pFloatDat);
				return -1;
			}
		}
		ptr = pFloatDat;
		printf("layer:%d,shiftbits:%d\n",jj,nShift_bit);

		nShift_bit=g_Q_W_array[jj];
		if((nShift_bit<0)||(nShift_bit>15)){
			printf("wfpgaWeight_cmodel nShift_bit error:%d\r\n",nShift_bit);
			return -1;
		}

		int ReturnFlag = 0;
		for(m=0;m<nLayerPreluParaCfg[jj][1];m++){	//kernal cnt
			for(nDepth = 0;nDepth <nLayerPreluParaCfg[jj][0];nDepth ++){//fm or channel
				//ÿ������˹�9����
				for(ii=0;ii<9;ii++){
					fDat0 = *(ptr+m*nLayerPreluParaCfg[jj][0]*9 + nDepth*9+ii);//kernal*nDepth*k*k
					uDat0 = (int)(fDat0*(1<<nShift_bit));						
					fprintf(fp,"%04X",uDat0&0xffff);
				}				
				fprintf(fp,"\n");

			}//end fm
		}//end m ,kernal
		fclose(fp);
		fp = NULL;//һ��Ҫ�ÿ�
	}//end layerloop

	
	
	free(pFloatDat);
	return 0;
}

//weight data,Cģ���ã���27���ļ�˳���š�kernal*fm(channel)*9
//**ע�⣺Դ����float�ͣ����˳���ǰ�layer*fm*kernal*9������ʱ��Ҫע�⡣**
//**��ŵ�ʱ���ǰ�layer���ļ���ÿ���ļ���kernal*fm*9����ÿ������˵Ķ�fm��������������**
int bk_wFpgaWeight_CModel()
{
	//prelu_para data
	 int nShift_bit = -1;
	//////////////////,Դ�ļ��洢�ṹ��nDepth*KerCount*k*k
	int nLayerPreluParaCfg[27][2]={{3,32},{32,64},{64,64},{64,64},{64,128},{128,128},\
	{128,128},{128,128},{128,128},{128,256},{256,256},{256,256},{256,256},\
	{256,256},{256,256},{256,256},{256,256},{256,256},{256,256},{256,256},\
	{256,512},{512,512},{512,512},{512,512},{512,512},{512,512},{512,512}};
		/*nLayerPreluParaCfg[27]={3*32*3*3,32*64*3*3,64*64*3*3,64*64*3*3,64*128*3*3,128*128*3*3,\
		128*128*3*3,128*128*3*3,128*128*3*3,128*256*3*3,256*256*3*3,256*256*3*3,256*256*3*3,\
		256*256*3*3,256*256*3*3,256*256*3*3,256*256*3*3,256*256*3*3,256*256*3*3,256*256*3*3,\
		256*512*3*3,512*512*3*3,512*512*3*3,512*512*3*3,512*512*3*3,512*512*3*3,512*512*3*3}; */
	int nAllCount=0;
	int i=0,j=0;
	int nKerCount=16;
	FILE *fp = NULL;
	float *pFloatDat=NULL;
	float *ptr=NULL;
	float fDat0=0.0;
	float fDat1 = 0.0;
	float fDat2 = 0.0;
	float fDat3 = 0.0;
	int n = 0,m=0;
	unsigned int uDat0,uDat3= 0;
	char sname[512]={0x0};
	int jj,ii=0;
	int nDepth;
	char sFloatFilename[512];
	
	//������������
	for(jj=0;jj<27;jj++){
		printf("produce weight_%d data------------------\n",jj);
		nAllCount = nLayerPreluParaCfg[jj][0]*nLayerPreluParaCfg[jj][1]*9;
		printf("to load %d wFpgaWeight_CModel data\n",nAllCount);
		memset(sFloatFilename,0,512);
		sprintf(sFloatFilename,"..\\float\\weight_%d.txt\0",jj+1);
		pFloatDat = (float *)malloc(sizeof(float)*nAllCount);
		if(pFloatDat==NULL){
			printf("malloc error!\n");
			return -1;
		}
	
		if(nAllCount!=LoadFloatFile1D(sFloatFilename,pFloatDat,nAllCount)){
			printf("fread error!\n");
			free(pFloatDat);
			return -1;
		}

	//output,ÿһ�����ݱ��浥������
		if(fp==NULL){
			memset(sname,0,512);
			sprintf(sname,"..\\out\\weight_%02d.cmodel\0",jj);
			fp = fopen(sname,"w+");
			if(fp==NULL){
				printf("fopen out file error!\n");
				free(pFloatDat);
				return -1;
			}
		}
		ptr = pFloatDat;
		printf("layer:%d,shiftbits:%d\n",jj,nShift_bit);

		nShift_bit=g_Q_W_array[jj];
		if((nShift_bit<0)||(nShift_bit>15)){
			printf("wfpgaWeight_cmodel nShift_bit error:%d\r\n",nShift_bit);
			return -1;
		}

		int ReturnFlag = 0;
		for(m=0;m<nLayerPreluParaCfg[jj][1];m++){	//kernal cnt
			for(nDepth = 0;nDepth <nLayerPreluParaCfg[jj][0];nDepth ++){//fm or channel
				//ÿ������˹�9����
				for(ii=0;ii<9;ii++){
					fDat0 = *(ptr+nDepth*nLayerPreluParaCfg[jj][1]*9 + m*9+ii);//nDepth*kernal*k*k
					uDat0 = (int)(fDat0*(1<<nShift_bit));						
					fprintf(fp,"%04X",uDat0&0xffff);
				}
/*
				for(ii=0;ii<9;ii++){
					fDat0 = *(ptr+nDepth*nLayerPreluParaCfg[jj][1]*9 + m*9+ii);//nDepth*kernal*k*k
					fprintf(fp," %08f",fDat0);
				}*/
				
				fprintf(fp,"\n");

			}//end fm
		}//end m ,kernal
		fclose(fp);
		fp = NULL;//һ��Ҫ�ÿ�
	}//end layerloop

	
	
	free(pFloatDat);
	return 0;
}
int wFpgaWeight_float()
{
	//prelu_para data
	//const int nShift_bit = PARA_SHIFT_BIT;
	//////////////////,Դ�ļ��洢�ṹ��nDepth*KerCount*k*k
	int nLayerPreluParaCfg[27][2]={{3,32},{32,64},{64,64},{64,64},{64,128},{128,128},\
	{128,128},{128,128},{128,128},{128,256},{256,256},{256,256},{256,256},\
	{256,256},{256,256},{256,256},{256,256},{256,256},{256,256},{256,256},\
	{256,512},{512,512},{512,512},{512,512},{512,512},{512,512},{512,512}};
		/*nLayerPreluParaCfg[27]={3*32*3*3,32*64*3*3,64*64*3*3,64*64*3*3,64*128*3*3,128*128*3*3,\
		128*128*3*3,128*128*3*3,128*128*3*3,128*256*3*3,256*256*3*3,256*256*3*3,256*256*3*3,\
		256*256*3*3,256*256*3*3,256*256*3*3,256*256*3*3,256*256*3*3,256*256*3*3,256*256*3*3,\
		256*512*3*3,512*512*3*3,512*512*3*3,512*512*3*3,512*512*3*3,512*512*3*3,512*512*3*3}; */
	int nAllCount=0;
	int i=0,j=0;
	int nKerCount=16;
	FILE *fp = NULL;
	float *pFloatDat=NULL;
	float *ptr=NULL;
	float fDat0=0.0;
	float fDat1 = 0.0;
	float fDat2 = 0.0;
	float fDat3 = 0.0;
	int n = 0,m=0;
	unsigned int uDat0,uDat1,uDat2,uDat3= 0;
	char sname[512]={0x0};
	int jj,ii=0;
	int nDepth;
	char sFloatFilename[512];

	//������������
	for(jj=0;jj<27;jj++){
		printf("produce weight_%d data------------------\n",jj);
		nAllCount = nLayerPreluParaCfg[jj][0]*nLayerPreluParaCfg[jj][1]*9;
		printf("to load %d wFpgaWeight_float data\n",nAllCount);
		memset(sFloatFilename,0,512);
		sprintf(sFloatFilename,"..\\float\\weight_%d.txt\0",jj+1);
		pFloatDat = (float *)malloc(sizeof(float)*nAllCount);
		if(pFloatDat==NULL){
			printf("malloc error!\n");
			return -1;
		}
	
		if(nAllCount!=LoadFloatFile1D(sFloatFilename,pFloatDat,nAllCount)){
			printf("fread error!\n");
			free(pFloatDat);
			return -1;
		}

	//output,ÿһ�����ݱ��浥������
		fp = NULL;
		ptr = pFloatDat;
		for(nDepth = 0;nDepth <nLayerPreluParaCfg[jj][0];nDepth ++){
			
				if(fp==NULL){
					memset(sname,0,512);
					sprintf(sname,"..\\out\\weight_%02d.dat\0",jj);
					fp = fopen(sname,"w+");
					if(fp==NULL){
						printf("fopen out file error!\n");
						free(pFloatDat);
						return -1;
					}
				}
				
				if(nLayerPreluParaCfg[jj][1]==32)
						n = 32;
					else if(nLayerPreluParaCfg[jj][1]==64)
						n = 64;
					else if(nLayerPreluParaCfg[jj][1]==128)
						n = 64;
					else if(nLayerPreluParaCfg[jj][1]==256)
						n = 64;
					else //512
						n = 32;

					int ReturnFlag = 0;
					for(m=0;m<nLayerPreluParaCfg[jj][1]/n;m++){//ÿһ�����ݣ���ģ�������д���
						//ǰ8�������
						for(i=0;i<4;i++){
							//ÿ������˹�9������ÿ4��������ƴ����һ��ʹ��
							for(ii=0;ii<9;ii++){
								fDat0 = *(ptr+nDepth*nLayerPreluParaCfg[jj][1]*9 + m*n+i*9+ii);//nDepth*kernal*k*k
								fDat1 = *((ptr+nDepth*nLayerPreluParaCfg[jj][1]*9 + m*n+(i+1)*9+ii));
								fDat2 = *((ptr+nDepth*nLayerPreluParaCfg[jj][1]*9 + m*n+(i+2)*9+ii));
								fDat3 = *(ptr+nDepth*nLayerPreluParaCfg[jj][1]*9 + m*n+(i+3)*9+ii);
								
								
								/*uDat0 = (int)(fDat0*(1<<nShift_bit));
								uDat1 = (int)(fDat1*(1<<nShift_bit));
								uDat2 = (int)(fDat2*(1<<nShift_bit));
								uDat3 = (int)(fDat3*(1<<nShift_bit));*/
								
								fprintf(fp,"%0f %0f ",fDat0,fDat1);
								fprintf(fp,"%0f %0f",fDat2,fDat3);
								if(ReturnFlag){
									fprintf(fp,"\n");
									ReturnFlag = 0;
								}else{
									ReturnFlag=1;
								}
							}
							
						}
						
						for(i=4;i<8;i++){
							//ÿ������˹�9������ÿ4��������ƴ����һ��ʹ��
							for(ii=0;ii<9;ii++){
								fDat0 = *(ptr+nDepth*nLayerPreluParaCfg[jj][1]*9 + m*n+i*9+ii);//nDepth*kernal*k*k
								fDat1 = *((ptr+nDepth*nLayerPreluParaCfg[jj][1]*9 + m*n+(i+1)*9+ii));
								fDat2 = *((ptr+nDepth*nLayerPreluParaCfg[jj][1]*9 + m*n+(i+2)*9+ii));
								fDat3 = *(ptr+nDepth*nLayerPreluParaCfg[jj][1]*9 + m*n+(i+3)*9+ii);
								
								
								uDat0 = (int)(fDat0);
								uDat1 = (int)(fDat1);
								uDat2 = (int)(fDat2);
								uDat3 = (int)(fDat3);
								fprintf(fp,"%0f %0f ",fDat0,fDat1);
								fprintf(fp,"%0f %0f\n",fDat2,fDat3);
							}
						}

						if(n!=32){
							//��Ҫд��8�������
								for(i=8;i<12;i++){
								//ÿ������˹�9������ÿ4��������ƴ����һ��ʹ��
								for(ii=0;ii<9;ii++){
									fDat0 = *(ptr+nDepth*nLayerPreluParaCfg[jj][1]*9 + m*n+i*9+ii);//nDepth*kernal*k*k
									fDat1 = *((ptr+nDepth*nLayerPreluParaCfg[jj][1]*9 + m*n+(i+1)*9+ii));
									fDat2 = *((ptr+nDepth*nLayerPreluParaCfg[jj][1]*9 + m*n+(i+2)*9+ii));
									fDat3 = *(ptr+nDepth*nLayerPreluParaCfg[jj][1]*9 + m*n+(i+3)*9+ii);
									
									
									uDat0 = (int)(fDat0);
									uDat1 = (int)(fDat1);
									uDat2 = (int)(fDat2);
									uDat3 = (int)(fDat3);
									fprintf(fp,"%0f %0f ",fDat0,fDat1);
									fprintf(fp,"%0f %0f",fDat2,fDat3);
									if(ReturnFlag){
										fprintf(fp,"\n");
										ReturnFlag = 0;
									}else{
										ReturnFlag=1;
									}
								}
								
							}
							
							for(i=13;i<16;i++){
								//ÿ������˹�9������ÿ4��������ƴ����һ��ʹ��
								for(ii=0;ii<9;ii++){
									fDat0 = *(ptr+nDepth*nLayerPreluParaCfg[jj][1]*9 + m*n+i*9+ii);//nDepth*kernal*k*k
									fDat1 = *((ptr+nDepth*nLayerPreluParaCfg[jj][1]*9 + m*n+(i+1)*9+ii));
									fDat2 = *((ptr+nDepth*nLayerPreluParaCfg[jj][1]*9 + m*n+(i+2)*9+ii));
									fDat3 = *(ptr+nDepth*nLayerPreluParaCfg[jj][1]*9 + m*n+(i+3)*9+ii);
									
									
									uDat0 = (int)(fDat0);
									uDat1 = (int)(fDat1);
									uDat2 = (int)(fDat2);
									uDat3 = (int)(fDat3);
									fprintf(fp,"%0f %0f ",fDat0,fDat1);
									fprintf(fp,"%0f %0f",fDat2,fDat3);
									if(ReturnFlag){
										fprintf(fp,"\n");
										ReturnFlag = 0;
									}else{
										ReturnFlag=1;
									}
								}
							}
						}
					}
			//}

		}//end ndepth
		fclose(fp);
		fp = NULL;//һ��Ҫ�ÿ�
		
	free(pFloatDat);

	}
	return 0;
}

//conv out
int wFpgaConvOut_hex(const char *sFloatFilename)
{
	//convout data
	int nShift_bit = -1;
	//////////////////,Դ�ļ��洢�ṹ��nKerCount*row*cln
	//ע�⣺��������ݽṹ�У�����pool1b�����ã�ԭ����Դ������ȱ�ٸ�����
	int nLayerPreluParaCfg[31][3]={
		{32,94,110},{64,92,108},	//layer 1a,1b,ȱpool1b 
		{64,46,54},{64,46,54},{128,44,52},{128,22,26},	//layer 2_1,2_2,2,pool2
		{128,22,26},{128,22,26},{128,22,26},{128,22,26},{256,20,24},{256,10,12}, //layer 3
		{256,10,12},{256,10,12},{256,10,12},{256,10,12},{256,10,12},{256,10,12},{256,10,12},{256,10,12},{256,10,12},{256,10,12},
		{512,8,10},{512,4,5},	//layer4
		{512,4,5},{512,4,5},{512,4,5},{512,4,5},{512,4,5},{512,4,5}}; //layer5
		
	int ShiftBit_Array[31]={12,11,11,10,10,10,10,10,10,11,10,10,11,11,11,11,11,12,12,12,12,12,12,12,12,12,12,12,11,11,13 };
	int nAllCount=0;
	int i=0,j=0,nRet;
	FILE *fp = NULL;
	float *pFloatDat=NULL;
	float *ptr=NULL;
	float fDat0=0.0;
	float fDat1 = 0.0;
	float fDat2 = 0.0;
	float fDat3 = 0.0;
	unsigned int uDat0;
	char sname[512]={0x0};
	FILE *fp_read=NULL;
	int nnnn = 0;

	nAllCount = 64*96*110;

	pFloatDat = (float *)malloc(sizeof(float)*nAllCount);
	if(pFloatDat==NULL){
		printf("malloc error!\n");
		return -1;
	}
	
	fp_read = fopen(sFloatFilename,"rb");
	if(fp_read==NULL){
		printf("fopen error!\n");
		return -1;
	}
	
	int nLayer,nKerCount,nRow,nCln;
	memset(sname,0,512);
	sprintf(sname,"..\\out\\convout.hex\0",i);
	fp = fopen(sname,"w+");
	if(fp==NULL){
		printf("fopen out file error!\n");
		free(pFloatDat);
		return -1;
	}
	
	
	for(nLayer=0;nLayer<30;nLayer++){
		fprintf(fp,"------------layer:%d-----------=\n",nLayer);
		nAllCount= nLayerPreluParaCfg[nLayer][0]*nLayerPreluParaCfg[nLayer][1]*nLayerPreluParaCfg[nLayer][2];
		
		nRet=LoadFloatFile1DByFP(fp_read,pFloatDat,nAllCount);
		if(nRet!=nAllCount){
			fclose(fp_read);
			printf("load conv layrer%d data file error!\n",nLayer);
			free(pFloatDat);
			return -1;
		}

		
		nShift_bit=ShiftBit_Array[nLayer];
		ptr = pFloatDat;
		printf("layer:%d,shiftbits:%d\n",nLayer,nShift_bit);
		for(nKerCount=0;nKerCount <nLayerPreluParaCfg[nLayer][0];nKerCount++)
			for(nCln = 0 ;nCln <nLayerPreluParaCfg[nLayer][2];nCln++)
				for(nRow=0;nRow<nLayerPreluParaCfg[nLayer][1];nRow++){
					nnnn++;

					fDat0 = *(ptr+nKerCount*nLayerPreluParaCfg[nLayer][2]*nLayerPreluParaCfg[nLayer][1]+nCln*nLayerPreluParaCfg[nLayer][1]+nRow);/*
					fDat1 = *(ptr+nCln*nLayerPreluParaCfg[nLayer][1]+nRow+1);
					fDat2 = *(ptr+nCln*nLayerPreluParaCfg[nLayer][1]+nRow+2);
					fDat3 = *(ptr+nCln*nLayerPreluParaCfg[nLayer][1]+nRow+3);*/

					uDat0 = (int)(fDat0*(1<<nShift_bit));/*
					uDat1 = (int)(fDat1*(1<<nShift_bit));
					uDat2 = (int)(fDat2*(1<<nShift_bit));
					uDat3 = (int)(fDat3*(1<<nShift_bit));*/
					if((nnnn%4==0)&&(nnnn) || ((nRow+1)==nLayerPreluParaCfg[nLayer][1])){
						fprintf(fp,"%04X \n",uDat0&0xffff);
						if((nRow+1)==nLayerPreluParaCfg[nLayer][1])
							nnnn = 0;
					}
					else
						fprintf(fp,"%04X ",uDat0&0xffff);
				}
		//ptr = ptr+nLayerPreluParaCfg[nLayer][0]*nLayerPreluParaCfg[nLayer][1]*nLayerPreluParaCfg[nLayer][2];
	}

	free(pFloatDat);
	fclose(fp);
	fclose(fp_read);
	return 0;
}

int wFpgaConvOut_float(const char *sFloatFilename)
{
	//convout data
	//////////////////,Դ�ļ��洢�ṹ��nKerCount*row*cln
	//ע�⣺��������ݽṹ�У�����pool1b�����ã�ԭ����Դ������ȱ�ٸ�����
	int nLayerPreluParaCfg[31][3]={
		{32,94,110},{64,92,108},\
		{64,46,54},{64,46,54},{128,44,52},{128,22,26},\
		{128,22,26},{128,22,26},{128,22,26},{128,22,26},{256,20,24},{256,10,12},\
		{256,10,12},{256,10,12},{256,10,12},\
	{256,10,12},{256,10,12},{256,10,12},{256,10,12},{256,10,12},{256,10,12},{256,10,12},\
	{512,8,10},{512,4,5},\
		{512,4,5},{512,4,5},{512,4,5},{512,4,5},{512,4,5},{512,4,5}};
		
	int nAllCount=0;
	int i=0,j=0,nRet;
	FILE *fp = NULL;
	float *pFloatDat=NULL;
	float *ptr=NULL;
	float fDat0=0.0;
	float fDat1 = 0.0;
	float fDat2 = 0.0;
	float fDat3 = 0.0;
	char sname[512]={0x0};
	FILE *fp_read=NULL;
	int  nnnn;

	nAllCount = 64*96*110;

	pFloatDat = (float *)malloc(sizeof(float)*nAllCount);
	if(pFloatDat==NULL){
		printf("malloc error!\n");
		return -1;
	}
	
	fp_read = fopen(sFloatFilename,"rb");
	if(fp_read==NULL){
		printf("fopen error!\n");
		return -1;
	}
	
	int nLayer,nKerCount,nRow,nCln;
	memset(sname,0,512);
	sprintf(sname,"..\\out\\convout.dat\0",i);
	fp = fopen(sname,"w+");
	if(fp==NULL){
		printf("fopen out file error!\n");
		free(pFloatDat);
		return -1;
	}
	
	
	for(nLayer=0;nLayer<30;nLayer++){
		fprintf(fp,"------------layer:%d--row:%d,cln:%d---------=\n",nLayer,nLayerPreluParaCfg[nLayer][1],nLayerPreluParaCfg[nLayer][2]);
		nAllCount= nLayerPreluParaCfg[nLayer][0]*nLayerPreluParaCfg[nLayer][1]*nLayerPreluParaCfg[nLayer][2];
		
		nRet=LoadFloatFile1DByFP(fp_read,pFloatDat,nAllCount);
		if(nRet!=nAllCount){
			fclose(fp_read);
			printf("load conv layrer%d data file error!\n",nLayer);
			free(pFloatDat);
			return -1;
		}
		ptr = pFloatDat;
		nnnn = 0;
		for(nKerCount=0;nKerCount <nLayerPreluParaCfg[nLayer][0];nKerCount++)
			for(nCln = 0 ;nCln <nLayerPreluParaCfg[nLayer][2];nCln++)
				for(nRow=0;nRow<nLayerPreluParaCfg[nLayer][1];nRow++){
					fDat0 = *(ptr+nKerCount*nLayerPreluParaCfg[nLayer][2]*nLayerPreluParaCfg[nLayer][1]+nCln*nLayerPreluParaCfg[nLayer][1]+nRow);/*
					fDat1 = *(ptr+nCln*nLayerPreluParaCfg[nLayer][1]+nRow+1);
					fDat2 = *(ptr+nCln*nLayerPreluParaCfg[nLayer][1]+nRow+2);
					fDat3 = *(ptr+nCln*nLayerPreluParaCfg[nLayer][1]+nRow+3);*/

				//	uDat0 = (int)(fDat0*(1<<nShift_bit));
					//uDat1 = (int)(fDat1*(1<<nShift_bit));
				//	uDat2 = (int)(fDat2*(1<<nShift_bit));
				//	uDat3 = (int)(fDat3*(1<<nShift_bit));
					nnnn++;
					if((nnnn%4==0)&&(nnnn) ||( (nRow+1)==nLayerPreluParaCfg[nLayer][1])){
						fprintf(fp,"%0f \n",fDat0);
						fflush(fp);
						if((nRow+1)==nLayerPreluParaCfg[nLayer][1])
							nnnn = 0;
					}
					else {
						fprintf(fp,"%0f ",fDat0);
						fflush(fp);
					}
				}
		//ptr = ptr+nLayerPreluParaCfg[nLayer][0]*nLayerPreluParaCfg[nLayer][1]*nLayerPreluParaCfg[nLayer][2];
	}

	free(pFloatDat);
	fclose(fp);
	fclose(fp_read);
	return 0;
}


//prelu data
int wFpgaPreluPara_hex(const char *sFloatFilename)
{
	//prelu_para data
	 int nShift_bit = -1;
	//////////////////
	int nLayerPreluParaCfg[27]={32,64,64,64,128,128,128,128,128,256,256,256,256,256,256,256,256,256,256,256,512,512,512,512,512,512,512}; 
	int nAllCount=0;
	int i=0,j=0;
	int nKerCount=16;
	FILE *fp = NULL;
	float *pFloatDat=NULL;
	float *ptr=NULL;
	float fDat0=0.0;
	float fDat1 = 0.0;
	float fDat2 = 0.0;
	float fDat3 = 0.0;
	int n = 0,m=0;
	unsigned int uDat0,uDat1,uDat2,uDat3= 0;
	char sname[512]={0x0};

	for(i=0;i<27;i++){
		nAllCount += nLayerPreluParaCfg[i];
	}
	printf("to load %d wFpgaPreluPara_hex data\n",nAllCount);

	pFloatDat = (float *)malloc(sizeof(float)*nAllCount);
	if(pFloatDat==NULL){
		printf("malloc error!\n");
		return -1;
	}
	
	
	
	if(nAllCount!=LoadFloatFile1D(sFloatFilename,pFloatDat,nAllCount)){
		printf("fread error!\n");
		free(pFloatDat);
		return -1;
	}
	

	//output
	//ǰ8�������
	for(i=0;i<8;i++){
		memset(sname,0,512);
		sprintf(sname,"..\\out\\prelu_para_%d.hex\0",i);
		fp = fopen(sname,"w+");
		if(fp==NULL){
			printf("fopen out file error!\n");
			free(pFloatDat);
			return -1;
		}
		ptr = pFloatDat;
		for(j=0;j<27;j++){//CHN:27�������
			if(nLayerPreluParaCfg[j]==32)
				n = 32;
			else if(nLayerPreluParaCfg[j]==64)
				n = 64;
			else if(nLayerPreluParaCfg[j]==128)
				n = 64;
			else if(nLayerPreluParaCfg[j]==256)
				n = 64;
			else {//512
				//ȥ���ָ���32��keral
				/*if(j==20)//conv4==20;Ҫ���⴦��
					n=64;
				else*/
					n = 32;
			}
			nShift_bit = g_Q_Prelu_array[j];
			if((nShift_bit <0)||(nShift_bit >15)){
				printf("nShift_bit error :%d\r\n",nShift_bit);
				return -1;
			}
			for(m=0;m<nLayerPreluParaCfg[j]/n;m++){
				fDat0 = *(ptr+i*4+m*n);
				fDat1 = *(ptr+i*4+1+m*n);
				fDat2 = *(ptr+i*4+2+m*n);
				fDat3 = *(ptr+i*4+3+m*n);
				
				
				uDat0 = (int)(fDat0*(1<<nShift_bit));
				uDat1 = (int)(fDat1*(1<<nShift_bit));
				uDat2 = (int)(fDat2*(1<<nShift_bit));
				uDat3 = (int)(fDat3*(1<<nShift_bit));
				//fprintf(fp,"%d--->%f %f\n",j,fDat0,fDat1);
				//fprintf(fp,"%d--->%f %f\n",j,fDat2,fDat3);
				fprintf(fp,"%04X%04X\n",uDat0&0xffff,uDat1&0xffff);
				fprintf(fp,"%04X%04X\n",uDat2&0xffff,uDat3&0xffff);
			}
			
			ptr +=nLayerPreluParaCfg[j];
		}
		fclose(fp);
	}

	//��8�������
	for(i=8;i<nKerCount;i++){
		memset(sname,0,512);
		sprintf(sname,"..\\out\\prelu_para_%d.hex\0",i);
		fp = fopen(sname,"w+");
		if(fp==NULL){
			printf("fopen out file error!\n");
			free(pFloatDat);
			return -1;
		}
		ptr = pFloatDat;
		ptr +=nLayerPreluParaCfg[0];//ָ��������1������
		for(j=1;j<27;j++){	//��1�����󼸲�����Ϊ512�Ĳ����ݲ���
			if(nLayerPreluParaCfg[j]==32){
				n = 32;
				continue;
			}
			else if(nLayerPreluParaCfg[j]==64)
				n = 64;
			else if(nLayerPreluParaCfg[j]==128)
				n = 64;
			else if(nLayerPreluParaCfg[j]==256)
				n = 64;
			else //512
			{
				//ȥ���ָ���32��keral
				/*//conv4==20;Ҫ���⴦��
				if(j==20){
					n= 64;
				}
				else*/{
					n = 32;
					continue;
				}
			}
			nShift_bit = g_Q_Prelu_array[j];
			if((nShift_bit <0)||(nShift_bit >15)){
				printf("nShift_bit error :%d\r\n",nShift_bit);
				return -1;
			}
			for(m=0;m<nLayerPreluParaCfg[j]/n;m++){
				fDat0 = *(ptr+i*4+m*n);
				fDat1 = *(ptr+i*4+1+m*n);
				fDat2 = *(ptr+i*4+2+m*n);
				fDat3 = *(ptr+i*4+3+m*n);

				uDat0 = (int)(fDat0*(1<<nShift_bit));
				uDat1 = (int)(fDat1*(1<<nShift_bit));
				uDat2 = (int)(fDat2*(1<<nShift_bit));
				uDat3 = (int)(fDat3*(1<<nShift_bit));
				fprintf(fp,"%04X%04X\n",uDat0&0xffff,uDat1&0xffff);
				fprintf(fp,"%04X%04X\n",uDat2&0xffff,uDat3&0xffff);
			}
			
			ptr +=nLayerPreluParaCfg[j];
		}
		fclose(fp);
	}
	free(pFloatDat);
	return 0;
}
int wFpgaPreluPara_sh(const char *sFloatFilename)
{
	//prelu_para data
	 int nShift_bit = -1;
	//////////////////
	int nLayerPreluParaCfg[27]={32,64,64,64,128,128,128,128,128,256,256,256,256,256,256,256,256,256,256,256,512,512,512,512,512,512,512}; 
	int nCmd[16]={0x10420,0x10820,0x10c20,0x11020,0x11420,0x11820,0x11c20,0x12020,0x12420,0x12820,0x12c20,0x13020,0x13420,0x13820,0x13c20,0x14020};
	int nAllCount=0;
	int i=0,j=0;
	int nKerCount=16;
	FILE *fp = NULL;
	float *pFloatDat=NULL;
	float *ptr=NULL;
	float fDat0=0.0;
	float fDat1 = 0.0;
	float fDat2 = 0.0;
	float fDat3 = 0.0;
	int n = 0,m=0;
	unsigned int uDat0,uDat1,uDat2,uDat3= 0;
	char sname[512]={0x0};

	for(i=0;i<27;i++){
		nAllCount += nLayerPreluParaCfg[i];
	}
	printf("to load %d wFpgaPreluPara_hex data\n",nAllCount);

	pFloatDat = (float *)malloc(sizeof(float)*nAllCount);
	if(pFloatDat==NULL){
		printf("malloc error!\n");
		return -1;
	}
	
	
	
	if(nAllCount!=LoadFloatFile1D(sFloatFilename,pFloatDat,nAllCount)){
		printf("fread error!\n");
		free(pFloatDat);
		return -1;
	}
	

	//output
	//ǰ8�������
	for(i=0;i<8;i++){
		memset(sname,0,512);
		sprintf(sname,"..\\out\\prelu_para_%d.sh\0",i);
		fp = fopen(sname,"w+");
		if(fp==NULL){
			printf("fopen out file error!\n");
			free(pFloatDat);
			return -1;
		}
		ptr = pFloatDat;
		for(j=0;j<27;j++){//CHN:27�������
			if(nLayerPreluParaCfg[j]==32)
				n = 32;
			else if(nLayerPreluParaCfg[j]==64)
				n = 64;
			else if(nLayerPreluParaCfg[j]==128)
				n = 64;
			else if(nLayerPreluParaCfg[j]==256)
				n = 64;
			else {//512
				//ȥ���ָ���32��keral
				/*if(j==20)//conv4==20;Ҫ���⴦��
					n=64;
				else*/
					n = 32;
			}
			nShift_bit = g_Q_Prelu_array[j];
			if((nShift_bit <0)||(nShift_bit >15)){
				printf("nShift_bit error :%d\r\n",nShift_bit);
				return -1;
			}
			for(m=0;m<nLayerPreluParaCfg[j]/n;m++){
				fDat0 = *(ptr+i*4+m*n);
				fDat1 = *(ptr+i*4+1+m*n);
				fDat2 = *(ptr+i*4+2+m*n);
				fDat3 = *(ptr+i*4+3+m*n);
				
				
				uDat0 = (int)(fDat0*(1<<nShift_bit));
				uDat1 = (int)(fDat1*(1<<nShift_bit));
				uDat2 = (int)(fDat2*(1<<nShift_bit));
				uDat3 = (int)(fDat3*(1<<nShift_bit));
				//fprintf(fp,"%d--->%f %f\n",j,fDat0,fDat1);
				//fprintf(fp,"%d--->%f %f\n",j,fDat2,fDat3);
				fprintf(fp,"./GP_OP write %X %04X%04X \n",nCmd[i],uDat0&0xffff,uDat1&0xffff);
				fprintf(fp,"./GP_OP write %X %04X%04X \n",nCmd[i],uDat2&0xffff,uDat3&0xffff);
			}
			
			ptr +=nLayerPreluParaCfg[j];
		}
		fclose(fp);
	}

	//��8�������
	for(i=8;i<nKerCount;i++){
		memset(sname,0,512);
		sprintf(sname,"..\\out\\prelu_para_%d.sh\0",i);
		fp = fopen(sname,"w+");
		if(fp==NULL){
			printf("fopen out file error!\n");
			free(pFloatDat);
			return -1;
		}
		ptr = pFloatDat;
		ptr +=nLayerPreluParaCfg[0];//ָ��������1������
		for(j=1;j<27;j++){	//��1�����󼸲�����Ϊ512�Ĳ����ݲ���
			if(nLayerPreluParaCfg[j]==32){
				n = 32;
				continue;
			}
			else if(nLayerPreluParaCfg[j]==64)
				n = 64;
			else if(nLayerPreluParaCfg[j]==128)
				n = 64;
			else if(nLayerPreluParaCfg[j]==256)
				n = 64;
			else //512
			{
				//ȥ���ָ���32��keral
				/*//conv4==20;Ҫ���⴦��
				if(j==20){
					n= 64;
				}
				else*/{
					n = 32;
					continue;
				}
			}
			nShift_bit = g_Q_Prelu_array[j];
			if((nShift_bit <0)||(nShift_bit >15)){
				printf("nShift_bit error :%d\r\n",nShift_bit);
				return -1;
			}
			for(m=0;m<nLayerPreluParaCfg[j]/n;m++){
				fDat0 = *(ptr+i*4+m*n);
				fDat1 = *(ptr+i*4+1+m*n);
				fDat2 = *(ptr+i*4+2+m*n);
				fDat3 = *(ptr+i*4+3+m*n);

				uDat0 = (int)(fDat0*(1<<nShift_bit));
				uDat1 = (int)(fDat1*(1<<nShift_bit));
				uDat2 = (int)(fDat2*(1<<nShift_bit));
				uDat3 = (int)(fDat3*(1<<nShift_bit));
				fprintf(fp,"./GP_OP write %X %04X%04X \n",nCmd[i],uDat0&0xffff,uDat1&0xffff);
				fprintf(fp,"./GP_OP write %X %04X%04X \n",nCmd[i],uDat2&0xffff,uDat3&0xffff);
			}
			
			ptr +=nLayerPreluParaCfg[j];
		}
		fclose(fp);
	}
	free(pFloatDat);
	return 0;
}
int wFpgaPreluPara_float()
{
	//prelu_para data
	int nLayerPreluParaCfg[27]={32,64,64,64,128,128,128,128,128,256,256,256,256,256,256,256,256,256,256,256,512,512,512,512,512,512,512}; 
	int nAllCount=0;
	int i=0,j=0;
	int nKerCount=16;
	FILE *fp = NULL;
	float *pFloatDat=NULL;
	float *ptr=NULL;
	float fDat0=0.0;
	float fDat1 = 0.0;
	float fDat2 = 0.0;
	float fDat3 = 0.0;
	int n = 0,m=0;
	unsigned int uDat= 0;
	char sname[512]={0x0};
	unsigned char soutBuf[255]={0x0};

	for(i=0;i<27;i++){
		nAllCount += nLayerPreluParaCfg[i];
	}
	printf("to load %d wFpgaPreluPara_float data\n",nAllCount);

	pFloatDat = (float *)malloc(sizeof(float)*nAllCount);
	if(pFloatDat==NULL){
		printf("malloc error!\n");
		return -1;
	}
	
	memset(sname,0,512);
	sprintf(sname,"..\\float\\prelu_para.txt\0");
	
	if(nAllCount!=LoadFloatFile1D(sname,pFloatDat,nAllCount)){
		printf("fread error!\n");
		free(pFloatDat);
		return -1;
	}
	
	////////////////
		memset(sname,0,512);
		sprintf(sname,"..\\out\\debug.dat\0",i);
		fp = fopen(sname,"w+");
		if(fp==NULL){
			printf("fopen out file error!\n");
			free(pFloatDat);
			return -1;
		}
		ptr = pFloatDat;
		for(j=0;j<27;j++){
			fprintf(fp,"layer-----------%d\n",j);
			for(m=0;m<nLayerPreluParaCfg[j];m++){
				fDat0 = *(ptr+m);
				fprintf(fp,"%f ",fDat0,fDat1);
			}
			fprintf(fp,"\n");
			ptr +=nLayerPreluParaCfg[j];
		}
		fclose(fp);
	////////////////
	//output
	//ǰ8�������
	for(i=0;i<8;i++){
		memset(sname,0,512);
		sprintf(sname,"..\\out\\prelu_para_%d.dat\0",i);
		fp = fopen(sname,"w+");
		if(fp==NULL){
			printf("fopen out file error!\n");
			free(pFloatDat);
			return -1;
		}
		ptr = pFloatDat;
		for(j=0;j<27;j++){
			if(nLayerPreluParaCfg[j]==32)
				n = 32;
			else if(nLayerPreluParaCfg[j]==64)
				n = 64;
			else if(nLayerPreluParaCfg[j]==128)
				n = 64;
			else if(nLayerPreluParaCfg[j]==256)
				n = 64;
			else //512
				n = 32;
			for(m=0;m<nLayerPreluParaCfg[j]/n;m++){
				fDat0 = *(ptr+i*4+m*n);
				fDat1 = *(ptr+i*4+1+m*n);
				fDat2 = *(ptr+i*4+2+m*n);
				fDat3 = *(ptr+i*4+3+m*n);
				//fprintf(fp,"%d--->%f %f\n",j,fDat0,fDat1);
				//fprintf(fp,"%d--->%f %f\n",j,fDat2,fDat3);
				fprintf(fp,"%f %f\n",fDat0,fDat1);
				fprintf(fp,"%f %f\n",fDat2,fDat3);
			}
			
			ptr +=nLayerPreluParaCfg[j];
		}
		fclose(fp);
	}

	//��8�������
	for(i=8;i<nKerCount;i++){
		memset(sname,0,512);
		sprintf(sname,"..\\out\\prelu_para_%d.dat\0",i);
		fp = fopen(sname,"w+");
		if(fp==NULL){
			printf("fopen out file error!\n");
			free(pFloatDat);
			return -1;
		}
		ptr = pFloatDat;
		ptr +=nLayerPreluParaCfg[0];//ָ��������1������
		for(j=1;j<27;j++){	//��1�����󼸲�����Ϊ512�Ĳ����ݲ���
			if(nLayerPreluParaCfg[j]==32){
				n = 32;
				continue;
			}
			else if(nLayerPreluParaCfg[j]==64)
				n = 64;
			else if(nLayerPreluParaCfg[j]==128)
				n = 64;
			else if(nLayerPreluParaCfg[j]==256)
				n = 64;
			else //512
			{
				n = 32;
				continue;
			}
			for(m=0;m<nLayerPreluParaCfg[j]/n;m++){
				fDat0 = *(ptr+i*4+m*n);
				fDat1 = *(ptr+i*4+1+m*n);
				fDat2 = *(ptr+i*4+2+m*n);
				fDat3 = *(ptr+i*4+3+m*n);
				fprintf(fp,"%f %f\n",fDat0,fDat1);
				fprintf(fp,"%f %f\n",fDat2,fDat3);
			}
			
			ptr +=nLayerPreluParaCfg[j];
		}
		fclose(fp);
	}
	free(pFloatDat);
	return 0;
}

int LoadFloatFile1DByFP(FILE *fp,float *pFloatDat,int nCount)
{
	
	int  nAllCount = 0,nReadCount;
	char sbuf[512]={0x00};
	char sItem[64]={0x00};
	char *ptr=NULL;
	int  nn=0;
	int  nRet = 0;
	
	if(fp==NULL){
		printf("input fp error!\n");
		return -1;
	}

	nAllCount = nCount;
	//printf("all data count :%d to read!\n",nAllCount);
	nReadCount = 0;
	memset(sbuf,0,sizeof(sbuf));
	nn = 0;
	while(fgets(sbuf,512,fp)!=NULL){
		nn++;
		//if(nn==2640){
		//	printf("notice!!!\n");
		//}
		ptr = sbuf;
		//for(;;)
		{
			nRet=sscanf(sbuf,"%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f",(pFloatDat+nReadCount),(pFloatDat+nReadCount+1),(pFloatDat+nReadCount+2),(pFloatDat+nReadCount+3),\
				  (pFloatDat+nReadCount+4),(pFloatDat+nReadCount+5),(pFloatDat+nReadCount+6),(pFloatDat+nReadCount+7),\
				  (pFloatDat+nReadCount+8),(pFloatDat+nReadCount+9),(pFloatDat+nReadCount+10),(pFloatDat+nReadCount+11),\
				  (pFloatDat+nReadCount+12),(pFloatDat+nReadCount+13),(pFloatDat+nReadCount+14),(pFloatDat+nReadCount+15));
			if(nRet<1){
				continue;
			}
			else{
				nAllCount=nAllCount-nRet;
				nReadCount=nRet+nReadCount;
			}
			
			if(nReadCount>=nCount)
				break;
		}
	/*	ptr = strtok(sbuf,";");
		while(ptr){
			sscanf(ptr,"%f",(pFloatDat+nReadCount));
			ptr = strtok(NULL,";");
			nAllCount--;
			nReadCount++;
			if(nReadCount>=nCount)
				break;
		}	*/	
		memset(sbuf,0,sizeof(sbuf));
		if(nReadCount>=nCount)
			break;
		
	}
	
//	printf("read over,%d",nReadCount);
	//fclose(fp);
	return nReadCount;
}


int LoadFloatFile1D(const char *sFilename,float *pFloatDat,int nCount)
{
	FILE *fp =NULL;
	int  nAllCount = 0,nReadCount;

	if(sFilename==NULL){
		printf("input filename error!\n");
		return -1;
	}
	fp = fopen(sFilename,"rb");
	if(fp==NULL){
		printf("fopen file %s error!\n",sFilename);
		return -1;
	}
	
	nAllCount = nCount;
//	printf("all data count :%d to read!\n",nAllCount);
	nReadCount = 0;
	while(fscanf(fp,"%f",(pFloatDat+nReadCount))!=EOF){
		//printf("%f",*(pFloatDat+nReadCount));
		nAllCount--;
		nReadCount++;
		/*if((nReadCount%10==0)&&(nReadCount))
			printf("\n");*/
		
	}
//	printf("read over,%d",nReadCount);
	fclose(fp);
	return nReadCount;
}

int LoadFloatFile3D(const char *sFilename,float *pFloatDat,int nKerCount,int nRow,int nCln,int nDepth)
{
	FILE *fp =NULL;
	int  nAllCount = 0,nReadCount;

	if(sFilename==NULL){
		printf("input filename error!\n");
		return -1;
	}
	fp = fopen(sFilename,"rb");
	if(fp==NULL){
		printf("fopen file %s error!\n",sFilename);
		return -1;
	}
	
	nAllCount = nKerCount*nDepth*nRow*nCln;
//	printf("all data count :%d to read!\n",nAllCount);
	nReadCount = 0;
	while(fscanf(fp,"%f",(pFloatDat+nReadCount))!=EOF){
		printf("%f",*(pFloatDat+nReadCount));
		nAllCount--;
		nReadCount++;
		if((nReadCount%10==0)&&(nReadCount))
			printf("\n");
	}
//	printf("read over,%d",nReadCount);
	fclose(fp);
	return nReadCount;
}


int GetWeightRange(float *pfloatBiggest,float *pfloatSmallest)
{
	//prelu_para data
	//////////////////,Դ�ļ��洢�ṹ��nDepth*KerCount*k*k
	int nLayerPreluParaCfg[27][2]={{3,32},{32,64},{64,64},{64,64},{64,128},{128,128},\
	{128,128},{128,128},{128,128},{128,256},{256,256},{256,256},{256,256},\
	{256,256},{256,256},{256,256},{256,256},{256,256},{256,256},{256,256},\
	{256,512},{512,512},{512,512},{512,512},{512,512},{512,512},{512,512}};
		/*nLayerPreluParaCfg[27]={3*32*3*3,32*64*3*3,64*64*3*3,64*64*3*3,64*128*3*3,128*128*3*3,\
		128*128*3*3,128*128*3*3,128*128*3*3,128*256*3*3,256*256*3*3,256*256*3*3,256*256*3*3,\
		256*256*3*3,256*256*3*3,256*256*3*3,256*256*3*3,256*256*3*3,256*256*3*3,256*256*3*3,\
		256*512*3*3,512*512*3*3,512*512*3*3,512*512*3*3,512*512*3*3,512*512*3*3,512*512*3*3}; */
	int nAllCount=0;
	int i=0,j=0;
	int nKerCount=16;
	FILE *fp = NULL;
	float *pFloatDat=NULL;
	float *ptr=NULL;
	float fDat0=0.0;
	float fDat1 = 0.0;
	float fDat2 = 0.0;
	float fDat3 = 0.0;
	int n = 0,m=0;
	char sname[512]={0x0};
	int jj,ii=0;
	char sFloatFilename[512];
	
	//������������
	for(jj=0;jj<27;jj++){
		//printf("produce weight_%d data------------------\n",jj);
		nAllCount = nLayerPreluParaCfg[jj][0]*nLayerPreluParaCfg[jj][1]*9;
//		printf("to load %d prelupara data\n",nAllCount);
		memset(sFloatFilename,0,512);
		sprintf(sFloatFilename,"..\\float\\weight_%d.txt\0",jj+1);
		pFloatDat = (float *)malloc(sizeof(float)*nAllCount);
		if(pFloatDat==NULL){
			printf("malloc error!\n");
			return -1;
		}
	
		if(nAllCount!=LoadFloatFile1D(sFloatFilename,pFloatDat,nAllCount)){
			printf("fread error!\n");
			free(pFloatDat);
			return -1;
		}

	//output,ÿһ�����ݱ��浥������
		ptr = pFloatDat;
		int nRet = 0;
	////xxxxxxxxxxxxxxxxxxxxxxxxx
		if(jj==0)
			nRet=FindBiggest(pfloatBiggest,pfloatSmallest,ptr,nAllCount,0);
		else
			nRet=FindBiggest(pfloatBiggest,pfloatSmallest,ptr,nAllCount,1);

		free(pFloatDat);
		if(nRet<0){
			printf("get weight range error!\n");
			return -1;
		}

	}
	return 0;
}


//ȫ���Ӳ�Ĳ�����Χ
int GetFCWeightRange(float *pfloatBiggest,float *pfloatSmallest)
{
	//fc bias data
	int nAllCount=0;
	int i=0,j=0;
	int nKerCount=16;
	FILE *fp = NULL;
	float *pFloatDat=NULL;
	float *ptr=NULL;
	float fDat0=0.0;
	float fDat1 = 0.0;
	float fDat2 = 0.0;
	float fDat3 = 0.0;
	int n = 0,m=0;
	char sname[512]={0x0};
	int ii=0;
	char sFloatFilename[512];
	

	nAllCount = 512*512*5*4;
	memset(sFloatFilename,0,512);
	sprintf(sFloatFilename,"..\\float\\fc_para.txt\0");
	pFloatDat = (float *)malloc(sizeof(float)*nAllCount);
	if(pFloatDat==NULL){
		printf("malloc error!\n");
		return -1;
	}
	
	if(nAllCount!=LoadFloatFile1D(sFloatFilename,pFloatDat,nAllCount)){
		printf("fread error!\n");
		free(pFloatDat);
		return -1;
	}
	
	ptr = pFloatDat;
	int nRet = 0;
	nRet=FindBiggest(pfloatBiggest,pfloatSmallest,ptr,nAllCount,0);
	
	free(pFloatDat);
	if(nRet<0){
		printf("get fc weight range error!\n");
		return -1;
	}
	return 0;
}

int GetPicRange(float *pfloatBiggest,float *pfloatSmallest)
{
	//prelu_para data
	//////////////////
	int nLayerPreluParaCfg[1]={3*96*112}; 
	int nAllCount=0;
	int i=0,j=0;
	int nKerCount=16;
	FILE *fp = NULL;
	float *pFloatDat=NULL;
	float *ptr=NULL;
	float fDat0=0.0;
	float fDat1 = 0.0;
	float fDat2 = 0.0;
	float fDat3 = 0.0;
	int n = 0,m=0;
	char sname[512]={0x0};

	for(i=0;i<1;i++){
		nAllCount += nLayerPreluParaCfg[i];
	}
//	printf("to load %d  data\n",nAllCount);

	pFloatDat = (float *)malloc(sizeof(float)*nAllCount);
	if(pFloatDat==NULL){
		printf("malloc error!\n");
		return -1;
	}
	
	char sFloatFilename[512]={0x00};
	sprintf(sFloatFilename,"..//float//pic.txt\0");
	if(nAllCount!=LoadFloatFile1D(sFloatFilename,pFloatDat,nAllCount)){
		printf("fread error!\n");
		free(pFloatDat);
		return -1;
	}	

	//output
	ptr = pFloatDat;
	int nRet = 0;
	nRet=FindBiggest(pfloatBiggest,pfloatSmallest,ptr,nAllCount,0);

	free(pFloatDat);
	if(nRet<0){
			printf("get Pic range error!\n");
			return -1;
	}
	return 0;
}



//bias--ȱ��fc_bias
int GetBiasRange(float *pfloatBiggest,float *pfloatSmallest)
{
	//prelu_para data
	//////////////////
	int nLayerPreluParaCfg[27]={32,64,64,64,128,128,128,128,128,256,256,256,256,256,256,256,256,256,256,256,512,512,512,512,512,512,512}; 
	int nAllCount=0;
	int i=0,j=0;
	int nKerCount=16;
	FILE *fp = NULL;
	float *pFloatDat=NULL;
	float *ptr=NULL;
	float fDat0=0.0;
	float fDat1 = 0.0;
	float fDat2 = 0.0;
	float fDat3 = 0.0;
	int n = 0,m=0;
	char sname[512]={0x0};

	for(i=0;i<27;i++){
		nAllCount += nLayerPreluParaCfg[i];
	}
//	printf("to load %d prelupara data\n",nAllCount);

	pFloatDat = (float *)malloc(sizeof(float)*nAllCount);
	if(pFloatDat==NULL){
		printf("malloc error!\n");
		return -1;
	}
	
	char sFloatFilename[512]={0x00};
	sprintf(sFloatFilename,"..//float//bias.txt\0");
	if(nAllCount!=LoadFloatFile1D(sFloatFilename,pFloatDat,nAllCount)){
		printf("fread error!\n");
		free(pFloatDat);
		return -1;
	}	

	//output
	ptr = pFloatDat;
	int nRet = 0;
	nRet=FindBiggest(pfloatBiggest,pfloatSmallest,ptr,nAllCount,0);

	free(pFloatDat);
	if(nRet<0){
			printf("get bias range error!\n");
			return -1;
	}
	return 0;
}
//FC conv
int GetFCConvRange(float *pfloatBiggest,float *pfloatSmallest)
{
	//prelu_para data
	//////////////////
	int nLayerPreluParaCfg[27]={32,64,64,64,128,128,128,128,128,256,256,256,256,256,256,256,256,256,256,256,512,512,512,512,512,512,512}; 
	int nAllCount=0;
	int i=0,j=0;
	int nKerCount=16;
	FILE *fp = NULL;
	float *pFloatDat=NULL;
	float *ptr=NULL;
	float fDat0=0.0;
	float fDat1 = 0.0;
	float fDat2 = 0.0;
	float fDat3 = 0.0;
	int n = 0,m=0;
	char sname[512]={0x0};

	
		nAllCount =512;
//	printf("to load %d prelupara data\n",nAllCount);

	pFloatDat = (float *)malloc(sizeof(float)*nAllCount);
	if(pFloatDat==NULL){
		printf("malloc error!\n");
		return -1;
	}
	
	char sFloatFilename[512]={0x00};
	sprintf(sFloatFilename,"..//float//weight_fc.txt\0");
	if(nAllCount!=LoadFloatFile1D(sFloatFilename,pFloatDat,nAllCount)){
		printf("fread error!\n");
		free(pFloatDat);
		return -1;
	}	

	//output
	ptr = pFloatDat;
	int nRet = 0;
	nRet=FindBiggest(pfloatBiggest,pfloatSmallest,ptr,nAllCount,0);

	free(pFloatDat);
	if(nRet<0){
			printf("get bias range error!\n");
			return -1;
	}
	return 0;
}

//FC bias
int GetFCBiasRange(float *pfloatBiggest,float *pfloatSmallest)
{
	//prelu_para data
	//////////////////
	int nLayerPreluParaCfg[27]={32,64,64,64,128,128,128,128,128,256,256,256,256,256,256,256,256,256,256,256,512,512,512,512,512,512,512}; 
	int nAllCount=0;
	int i=0,j=0;
	int nKerCount=16;
	FILE *fp = NULL;
	float *pFloatDat=NULL;
	float *ptr=NULL;
	float fDat0=0.0;
	float fDat1 = 0.0;
	float fDat2 = 0.0;
	float fDat3 = 0.0;
	int n = 0,m=0;
	char sname[512]={0x0};

	
		nAllCount =512;
//	printf("to load %d prelupara data\n",nAllCount);

	pFloatDat = (float *)malloc(sizeof(float)*nAllCount);
	if(pFloatDat==NULL){
		printf("malloc error!\n");
		return -1;
	}
	
	char sFloatFilename[512]={0x00};
	sprintf(sFloatFilename,"..//float//fc_bias.txt\0");
	if(nAllCount!=LoadFloatFile1D(sFloatFilename,pFloatDat,nAllCount)){
		printf("fread error!\n");
		free(pFloatDat);
		return -1;
	}	

	//output
	ptr = pFloatDat;
	int nRet = 0;
	nRet=FindBiggest(pfloatBiggest,pfloatSmallest,ptr,nAllCount,0);

	free(pFloatDat);
	if(nRet<0){
			printf("get bias range error!\n");
			return -1;
	}
	return 0;
}

//prelu data
int GetPrluRange(float *pfloatBiggest,float *pfloatSmallest)
{
	//prelu_para data
	//////////////////
	int nLayerPreluParaCfg[27]={32,64,64,64,128,128,128,128,128,256,256,256,256,256,256,256,256,256,256,256,512,512,512,512,512,512,512}; 
	int nAllCount=0;
	int i=0,j=0;
	int nKerCount=16;
	FILE *fp = NULL;
	float *pFloatDat=NULL;
	float *ptr=NULL;
	float fDat0=0.0;
	float fDat1 = 0.0;
	float fDat2 = 0.0;
	float fDat3 = 0.0;
	int n = 0,m=0;
	char sname[512]={0x0};

	for(i=0;i<27;i++){
		nAllCount += nLayerPreluParaCfg[i];
	}
//	printf("to load %d prelupara data\n",nAllCount);

	pFloatDat = (float *)malloc(sizeof(float)*nAllCount);
	if(pFloatDat==NULL){
		printf("malloc error!\n");
		return -1;
	}
	
	char sFloatFilename[512]={0x00};
	sprintf(sFloatFilename,"..//float//prelu_para.txt\0");
	if(nAllCount!=LoadFloatFile1D(sFloatFilename,pFloatDat,nAllCount)){
		printf("fread error!\n");
		free(pFloatDat);
		return -1;
	}

	//output
	ptr = pFloatDat;
	int nRet = 0;
	nRet=FindBiggest(pfloatBiggest,pfloatSmallest,ptr,nAllCount,0);

	free(pFloatDat);
	if(nRet<0){
			printf("get prelu range error!\n");
			return -1;
	}
	return 0;
}


int GetConvRange(float *pfloatBiggest,float *pfloatSmallest)
{
	//convout data
	//////////////////,Դ�ļ��洢�ṹ��nKerCount*row*cln
	//ע�⣺��������ݽṹ�У�����pool1b�����ã�ԭ����Դ������ȱ�ٸ�����
	int nLayerPreluParaCfg[31][3]={
		{32,94,110},{64,92,108},\
		{64,46,54},{64,46,54},{128,44,52},{128,22,26},\
		{128,22,26},{128,22,26},{128,22,26},{128,22,26},{256,20,24},{256,10,12},\
		{256,10,12},{256,10,12},{256,10,12},\
	{256,10,12},{256,10,12},{256,10,12},{256,10,12},{256,10,12},{256,10,12},{256,10,12},\
	{512,8,10},{512,4,5},\
		{512,4,5},{512,4,5},{512,4,5},{512,4,5},{512,4,5},{512,4,5}};
		
	int nAllCount=0;
	int i=0,j=0,nRet;
	FILE *fp = NULL;
	float *pFloatDat=NULL;
	float *ptr=NULL;
	float fDat0=0.0;
	float fDat1 = 0.0;
	float fDat2 = 0.0;
	float fDat3 = 0.0;
	char sname[512]={0x0};
	FILE *fp_read=NULL;
	int nnnn = 0;

	nAllCount = 64*96*110;

	pFloatDat = (float *)malloc(sizeof(float)*nAllCount);
	if(pFloatDat==NULL){
		printf("malloc error!\n");
		return -1;
	}
	
	char sFloatFilename[512]={0x00};
	sprintf(sFloatFilename,"..//float//conv_out.txt\0");
	fp_read = fopen(sFloatFilename,"rb");
	if(fp_read==NULL){
		printf("fopen error!\n");
		return -1;
	}
	
	int nLayer;
	
	for(nLayer=0;nLayer<30;nLayer++){
		//fprintf(fp,"------------layer:%d-----------=\n",nLayer);
		nAllCount= nLayerPreluParaCfg[nLayer][0]*nLayerPreluParaCfg[nLayer][1]*nLayerPreluParaCfg[nLayer][2];
		
		nRet=LoadFloatFile1DByFP(fp_read,pFloatDat,nAllCount);
		if(nRet!=nAllCount){
			fclose(fp_read);
			printf("load conv layrer%d data file error!\n",nLayer);
			free(pFloatDat);
			return -1;
		}
		ptr = pFloatDat;
		
		if(nLayer==0)
			nRet=FindBiggest(pfloatBiggest,pfloatSmallest,ptr,nAllCount,0);
		else
			//nRet=FindBiggest(pfloatBiggest,pfloatSmallest,ptr,nAllCount,1);
			nRet=FindBiggest(pfloatBiggest,pfloatSmallest,ptr,nAllCount,0);
		if(nRet<0){
			printf("get conv range error!\n");
			free(pFloatDat);
			fclose(fp_read);
			return -1;
		}
			int nQd=0;
			nQd=Qn(*pfloatBiggest,*pfloatSmallest);
			printf("----layer_%d--[%f,%f]-----\n",nLayer,*pfloatSmallest,*pfloatBiggest);
			printf(" Qn==%d\n",nQd);
	}

	free(pFloatDat);
	fclose(fp_read);
	return 0;
}


int GetAllRange(){

	float fBiggest,fSmallest;
	int nRet = 0;
	int nQd=0;
	float fBigest_final,fSmallest_final;

	if(0>GetPicRange(&fBiggest,&fSmallest)){
		return -1;
	}
	printf("pic:%f,%f\n",fBiggest,fSmallest);
	fBigest_final = fBiggest;
	fSmallest_final = fSmallest;

	if(0>GetFCWeightRange(&fBiggest,&fSmallest)){
		return -1;
	}
	printf("fc weight:%f,%f\n",fBiggest,fSmallest);
	if(fBiggest>fBigest_final)
		fBigest_final = fBiggest;
	if(fSmallest>fSmallest_final)
		fSmallest_final = fSmallest;


	if(0>GetBiasRange(&fBiggest,&fSmallest)){
		return -1;
	}
	printf("bias:%f,%f\n",fBiggest,fSmallest);
	if(fBiggest>fBigest_final)
		fBigest_final = fBiggest;
	if(fSmallest>fSmallest_final)
		fSmallest_final = fSmallest;

	if(0>GetFCBiasRange(&fBiggest,&fSmallest)){
		return -1;
	}
	printf("FCbias:%f,%f\n",fBiggest,fSmallest);
	if(fBiggest>fBigest_final)
		fBigest_final = fBiggest;
	if(fSmallest>fSmallest_final)
		fSmallest_final = fSmallest;

	if(0>GetPrluRange(&fBiggest,&fSmallest)){
		return -1;
	}
	printf("prelu:%f,%f\n",fBiggest,fSmallest);
	if(fBiggest>fBigest_final)
		fBigest_final = fBiggest;
	if(fSmallest>fSmallest_final)
		fSmallest_final = fSmallest;

	if(0>GetWeightRange(&fBiggest,&fSmallest)){
		return -1;
	}
	printf("weight:%f,%f\n",fBiggest,fSmallest);
	if(fBiggest>fBigest_final)
		fBigest_final = fBiggest;
	if(fSmallest>fSmallest_final)
		fSmallest_final = fSmallest;


	if(0>GetFCConvRange(&fBiggest,&fSmallest)){
		return -1;
	}
	printf("fc conv:%f,%f\n",fBiggest,fSmallest);
	if(fBiggest>fBigest_final)
		fBigest_final = fBiggest;
	if(fSmallest>fSmallest_final)
		fSmallest_final = fSmallest;

	printf("before convout [%f,%f]\n",fBigest_final,fSmallest_final);
	nQd=Qn(fBigest_final,fSmallest_final);
	printf("before convout Qn==%d\n",nQd);

	if(0>GetConvRange(&fBiggest,&fSmallest)){
		return -1;
	}
	printf("conv:%f,%f\n",fBiggest,fSmallest);
	if(fBiggest>fBigest_final)
		fBigest_final = fBiggest;
	if(fSmallest>fSmallest_final)
		fSmallest_final = fSmallest;


	printf("final====>:%f,%f\n",fBiggest,fSmallest);

	nQd=Qn(fBiggest,fSmallest);
	printf("Qn==%d\n",nQd);
	return 0;
}
