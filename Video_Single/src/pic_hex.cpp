/*
 * pic_hex.cpp
 *
 *  Created on: 2017年11月20日
 *      Author: root
 */
#include "pic_hex.hpp"

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
	sprintf(sname,"../out/pic.hex\0");
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



