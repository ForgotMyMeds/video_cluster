#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "Define.h"	

int LoadHexFile_X64(const char *sHexFilename,short *pOutBuf,const int nRCnt)
{
	char sbuf[256],sItem[16];
	FILE *fp =NULL;
	int  nAllCount = 0,nReadCount,nRet,i;


	if((sHexFilename==NULL)||(pOutBuf==NULL)||(nRCnt<0)){
		printf("LoadHexFile_X32 input error!\r\n");
		return -1;
	}	
	fp = fopen(sHexFilename,"r");
	if(fp==NULL){
		printf("fopen file %s error!\n",sHexFilename);
		return -1;
	}
	

	nAllCount = nRCnt;
	nReadCount = 0;
	memset(sbuf,0,sizeof(sbuf));

	int nDataLen = 64/4;
	int nItem=0;
	//while(fgets(sbuf,512,fp)!=NULL){
	while(fscanf(fp,"%s\0",sbuf)!=EOF){
		nRet = strlen(sbuf);
		if(nRet!=nDataLen){
			printf("file hex dat format error!\r\n");
			fclose(fp);
			return -1;
		}
		
		for(i=0;i<nDataLen/4;i++){
			memcpy(sItem,sbuf+i*4,4);
			sItem[5]=0x00;
			nRet=sscanf(sItem,"%x",&nItem);
			if(nRet<1){
				printf("sscanf error!\r\n");
				fclose(fp);
				return -1;
			}
			*(pOutBuf+nReadCount)= (short)nItem;
			nReadCount ++;
		}		
		
		if(nReadCount>=nAllCount)
			break;		
		memset(sbuf,0,sizeof(sbuf));
		
	}

	return nReadCount;
}

int LoadHexFile_X144(const char *sHexFilename,short *pOutBuf,const int nRCnt)
{
	char sbuf[256],sItem[16];
	FILE *fp =NULL;
	int  nAllCount = 0,nReadCount,nRet,i;


	if((sHexFilename==NULL)||(pOutBuf==NULL)||(nRCnt<0)){
		printf("LoadHexFile_X32 input error!\r\n");
		return -1;
	}	
	fp = fopen(sHexFilename,"r");
	if(fp==NULL){
		printf("fopen file %s error!\n",sHexFilename);
		return -1;
	}
	

	nAllCount = nRCnt;
	nReadCount = 0;
	memset(sbuf,0,sizeof(sbuf));

	int nDataLen = 144/4;
	int nItem=0;
	//while(fgets(sbuf,512,fp)!=NULL){
	while(fscanf(fp,"%s\0",sbuf)!=EOF){
		nRet = strlen(sbuf);
		if(nRet!=nDataLen){
			printf("file hex dat format error!\r\n");
			fclose(fp);
			return -1;
		}
		
		for(i=0;i<nDataLen/4;i++){
			memcpy(sItem,sbuf+i*4,4);
			sItem[5]=0x00;
			nRet=sscanf(sItem,"%x",&nItem);
			if(nRet<1){
				printf("sscanf error!\r\n");
				fclose(fp);
				return -1;
			}
			*(pOutBuf+nReadCount)= (short)nItem;
			nReadCount ++;
		}		
		
		if(nReadCount>=nAllCount)
			break;		
		memset(sbuf,0,sizeof(sbuf));
		
	}

	return nReadCount;
}

int LoadHexFile_X32(const char *sHexFilename,short *pOutBuf,const int nRCnt)
{
	char sbuf[256],sItem[16];
	FILE *fp =NULL;
	int  nAllCount = 0,nReadCount,nRet,i;


	if((sHexFilename==NULL)||(pOutBuf==NULL)||(nRCnt<0)){
		printf("LoadHexFile_X32 input error!\r\n");
		return -1;
	}	
	fp = fopen(sHexFilename,"r");
	if(fp==NULL){
		printf("fopen file %s error!\n",sHexFilename);
		return -1;
	}
	

	nAllCount = nRCnt;
	nReadCount = 0;
	memset(sbuf,0,sizeof(sbuf));

	int nDataLen = 32/4;
	int nItem=0;
	//while(fgets(sbuf,512,fp)!=NULL){
	while(fscanf(fp,"%s\0",sbuf)!=EOF){
		nRet = strlen(sbuf);
		if(nRet!=nDataLen){
			printf("file hex dat format error!\r\n");
			fclose(fp);
			return -1;
		}
		
		for(i=0;i<nDataLen/4;i++){
			memcpy(sItem,sbuf+i*4,4);
			sItem[5]=0x00;
			nRet=sscanf(sItem,"%x",&nItem);
			if(nRet<1){
				printf("sscanf error!\r\n");
				fclose(fp);
				return -1;
			}
			*(pOutBuf+nReadCount)= (short)nItem;
			nReadCount ++;
		}		
		
		if(nReadCount>=nAllCount)
			break;		
		memset(sbuf,0,sizeof(sbuf));
		
	}

	return nReadCount;
}



int LoadHexFile_X16(const char *sHexFilename,short *pOutBuf,const int nRCnt)
{
	char sbuf[256],sItem[16];
	FILE *fp =NULL;
	int  nAllCount = 0,nReadCount,nRet,i;

	if((sHexFilename==NULL)||(pOutBuf==NULL)||(nRCnt<0)){
		printf("LoadHexFile_X32 input error!\r\n");
		return -1;
	}	
	fp = fopen(sHexFilename,"r");
	if(fp==NULL){
		printf("fopen file %s error!\n",sHexFilename);
		return -1;
	}
	

	nAllCount = nRCnt;
	nReadCount = 0;
	memset(sbuf,0,sizeof(sbuf));

	int nDataLen = 16/4;
	int nItem=0;
	//while(fgets(sbuf,512,fp)!=NULL){
	while(fscanf(fp,"%s\0",sbuf)!=EOF){
		nRet = strlen(sbuf);
		if(nRet!=nDataLen){
			printf("file hex dat format error!\r\n");
			fclose(fp);
			return -1;
		}
		
		for(i=0;i<nDataLen/4;i++){
			memcpy(sItem,sbuf+i*4,4);
			sItem[5]=0x00;
			nRet=sscanf(sItem,"%x",&nItem);
			if(nRet<1){
				printf("sscanf error!\r\n");
				fclose(fp);
				return -1;
			}
			*(pOutBuf+nReadCount)= (short)nItem;
			nReadCount ++;
		}		
		
		if(nReadCount>=nAllCount)
			break;		
		memset(sbuf,0,sizeof(sbuf));
		
	}

	return nReadCount;
}
