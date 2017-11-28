//#include "stdafx.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "Define.h"
int FindBiggest(float *f_Biggest,float *f_Smallest,float *pfdata,int nCount,char bFlag)
{
	int i;

	if((pfdata==NULL)||(nCount<0)){
		printf("FindBiggest ,input error!\r\n");
		return -1;
	}
	

	for(i=0;i<nCount;i++){
		if((i==0)&&(bFlag==0)){
			*f_Biggest = *(pfdata+i); 
			*f_Smallest = *(pfdata+i); 
		}
		if((*(pfdata+i))>(*f_Biggest))
			*f_Biggest = *(pfdata+i); 
		if((*(pfdata+i))<(*f_Smallest))
			*f_Smallest = *(pfdata+i); 
	}

	return 0;
}


int Qn(float f_Biggest,float f_Smallest)
{
	int nRet = -1;
 if((-1<=    f_Biggest)&&(f_Biggest<=0.9999695    )&&(-1<=    f_Smallest)&&(f_Smallest<=0.9999695    ))
		nRet = 15;
 else if((-2<=    f_Biggest)&&((f_Biggest)<=1.9999390    )&&(-2<=    f_Smallest)&&(f_Smallest<=1.9999390    ))
		nRet = 14;
 else if((-4<=    f_Biggest)&&(f_Biggest<=3.9998779    )&&(-4<=    f_Smallest)&&(f_Smallest<=3.9998779    ))
		nRet = 13;
 else if((-8<=    f_Biggest)&&(f_Biggest<=7.9997559    )&&(-8<=    f_Smallest)&&(f_Smallest<=7.9997559    ))
		nRet = 12;
 else if((-16<=   f_Biggest)&&(f_Biggest<=15.9995117   )&&(-16<=   f_Smallest)&&(f_Smallest<=15.9995117   ))
		nRet = 11;
 else if((-32<=   f_Biggest)&&(f_Biggest<=31.9990234   )&&(-32<=   f_Smallest)&&(f_Smallest<=31.9990234   ))
		nRet = 10;
 else if((-64<=   f_Biggest)&&(f_Biggest<=63.9980469   )&&(-64<=   f_Smallest)&&(f_Smallest<=63.9980469   ))
		nRet = 9;
 else if((-128<=  f_Biggest)&&(f_Biggest<=127.9960938  )&&(-128<=  f_Smallest)&&(f_Smallest<=127.9960938  ))
		nRet = 8;
 else if((-256<=  f_Biggest)&&(f_Biggest<=255.9921875  )&&(-256<=  f_Smallest)&&(f_Smallest<=255.9921875  ))
		nRet = 7;
 else if((-512<=  f_Biggest)&&(f_Biggest<=511.9804375  )&&(-512<=  f_Smallest)&&(f_Smallest<=511.9804375  ))
		nRet = 6;
 else if((-1024<= f_Biggest)&&(f_Biggest<=1023.96875   )&&(-1024<= f_Smallest)&&(f_Smallest<=1023.96875   ))
		nRet = 5;
 else if((-2048<= f_Biggest)&&(f_Biggest<=2047.9375    )&&(-2048<= f_Smallest)&&(f_Smallest<=2047.9375    ))
		nRet = 4;
 else if((-4096<= f_Biggest)&&(f_Biggest<=4095.875     )&&(-4096<= f_Smallest)&&(f_Smallest<=4095.875     ))
		nRet = 3;
 else if((-8192<= f_Biggest)&&(f_Biggest<=8191.75      )&&(-8192<= f_Smallest)&&(f_Smallest<=8191.75      ))
		nRet = 2;
 else if((-16384<=f_Biggest)&&(f_Biggest<=16383.5      )&&(-16384<=f_Smallest)&&(f_Smallest<=16383.5      ))
		nRet = 1;
 else if((-32768<=f_Biggest)&&(f_Biggest<=32767        )&&(-32768<=f_Smallest)&&(f_Smallest<=32767        ))
		nRet = 0;

	if(nRet<0)
	{
		printf("float overflow,error\r\n");
		return -1;
	}
	return nRet;
}

int ConvertF2D(float *pfloat,short *pOutD,int nCnt,int nQn)
{
	int i=0;
	if((pfloat==NULL)||(pOutD==NULL))
	{
		printf("input error!\n");
		return -1;
	}
	for(i=0;i<nCnt;i++){
		//*(pOutD+i) = (short)((*(pfloat+i)) *(1<<nQn));
		*(pOutD+i) = (short)((*(pfloat+i)) *(1<<nQn)+0.5);
		//printf("%f ==>%02x==>%d\r\n",*(pfloat+i),*(pOutD+i),*(pOutD+i));
	}
	return 0;
}
long long Cp3x3(short Dat_short[9],short Weight_short[9] )
{
	int i;
	long long nRet = 0;

	for(i=0;i<9;i++){
        nRet += Dat_short[i]*Weight_short[i];
	}
//	printf("cp3x3,ret: %d ; 0x%x\r\n",nRet,nRet);
	return nRet;
}


int int32_Cp3x3(short Dat_short[9],short Weight_short[9] )
{
	int i;
	int nRet = 0;

	for(i=0;i<9;i++){
        nRet += Dat_short[i]*Weight_short[i];
	}
//	printf("cp3x3,ret: %d ; 0x%x\r\n",nRet,nRet);
	return nRet;
}
short Cp3x3_fm(short *pDat_fm,short *pWeight_fm, short Bias_short,int nFm,int nDFixedShift,int nWFixedShift,int nBFixedShift,int nOutFixedShift,FILE *fp_debug)
{
	int j;
	long long nRet;
	long long nBias=Bias_short;
	
	short shortRet = 0;

	if((pDat_fm==NULL)||(pWeight_fm==NULL)){
		printf("CP3x3_fm ,input NULL ,error!\r\n");
		return -1;
	}
	
	long long nRetLayer=0;
	int nAllShift		= nWFixedShift+nDFixedShift;//CHN:�˷���С�����λ��
	int nOutShift		= nAllShift-nOutFixedShift;
	int nBiasShift		= nAllShift-nBFixedShift;

//	i64RetLayer = nBias;//�ȼ�bias
	nRetLayer = (nBias)<<nBiasShift;//�ȼ�bias
	for(j=0;j<nFm;j++){
		nRet = Cp3x3(pDat_fm+9*j,pWeight_fm+9*j);
        nRetLayer += nRet;
		//printf("before shift -->nFM %d,ret=%d, 0x%x\r\n",j,nRetLayer,nRetLayer);
		nRetLayer = (nRetLayer>>(nOutShift-2));//����18λ
		//printf("after shift -->nFM %d,ret=%d, 0x%x\r\n",j,nRetLayer,nRetLayer);
		if(fp_debug!=NULL){
			fprintf(fp_debug,"nFm_%d:%05X ",j,nRetLayer&0x3ffff);
		}
		//�ж����-18λ
		if((nRetLayer>131071)||(nRetLayer<-131072)){
			printf("1->cp3x3_fm overflow error:%d!\n",nRetLayer);
			//return -1;
			if(nRetLayer>=0)
				nRetLayer = nRetLayer&0x3ffff;//����18λ���
			else{
				//<0
				long long n64Tmp= nRetLayer&0x3ffff;
				if(n64Tmp&0x20000){//�жϷ���λ-18bit
					//<0
					nRetLayer = (0xfffffffffffc0000) | (nRetLayer&0x3ffff);//����18λ���
				}
				else{
					nRetLayer = n64Tmp;
				}
				
			}
		}
		
		nRetLayer = nRetLayer <<((nOutShift-2));//������һ���ۼ�
	}
	if(fp_debug!=NULL){
		fprintf(fp_debug,"\n");
	}

	nRetLayer = nRetLayer >>nOutShift;//��Ϊ16λ���
	
	float ff = ((float)nRetLayer)/((float)(1<<nOutFixedShift));
	//printf("Cp3x3_fm, after add bias and shift, ret: %d ; 0x%x, float:%f\r\n",nRetLayer,nRetLayer,ff);
	if((nRetLayer >32767) ||(nRetLayer <-32768)){
		printf("2->cp3x3_fm overflow error:%d",nRetLayer);
		//return -1;
	}
	
	shortRet = nRetLayer;//����16λ��� ��ע�����ʱ�Ĵ���
	return shortRet;
}

//�޸ĳ�bias shift֮ǰ��ȷ���ݣ�20170825
int bak_Cp3x3_fm(short *pDat_fm,short *pWeight_fm, short Bias_short,int nFm,int nDFixedShift,int nWFixedShift,int nOutFixedShift)
{
	int j;
	int nRet = 0;
	int nBias=Bias_short;

	if((pDat_fm==NULL)||(pWeight_fm==NULL)){
		printf("CP3x3_fm ,input NULL ,error!\r\n");
		return -1;
	}
	int nRetLayer=0;
	for(j=0;j<nFm;j++){
		nRetLayer = Cp3x3(pDat_fm+9*j,pWeight_fm+9*j);
        nRet += nRetLayer;
		//printf("nFM%d,ret=%d, 0x%x\r\n",j,nRetLayer,nRetLayer);
	}
	

	//printf("Cp3x3_fm, before add bias ret: %d ; 0x%x\r\n",nRet,nRet);

	int nAllFixedShift = nWFixedShift+nDFixedShift;//CHN:ĿǰС�����λ��
	int nOutShiftBit   = nAllFixedShift-nOutFixedShift;
	if(nOutShiftBit<0){
		printf("cp3x3_fm fixedshift error:%d!\r\n",nOutShiftBit);
		return -1;
	}
	
	nRet +=(nBias<<(nDFixedShift));//��bias

	nRet = nRet >>nOutShiftBit;//ͳһ�Ƶ�β��
	
	float ff = ((float)nRet)/((float)(1<<nOutFixedShift));
//	printf("Cp3x3_fm, after add bias and shift, ret: %d ; 0x%x, float:%f\r\n",nRet,nRet,ff);
	if((nRet >32767) ||(nRet <-32768)){
		printf("cp3x3_fm overflow error:%d",nRet);
		return -1;
	}
	
	return nRet;
}

//����ֵ<0������>0��ʾ������pconvout���ݸ���
int LayerCal(
			 short *pLayerDat,		//dat input,padded datadata structure (nFm*cln*row)
			 int   nRow,
			 int   nCln,
			 short *pLayerWeight,	//weight input,structure (nKerCnt * nFm * 9)
			 short	*pBias,			//bias input ,struct nKerCnt*1
			 short	*pPrelu,			//prelu input,struct nKerCnt*1
			 int nKerCnt,			//cnn k count,
			 int nFm,				//fm count per kernal
			 int nQd,				//dat fixed shift
			 int nQw,				//weight fixed shift
			 int nQp,               //prelu fixed shift
			 int nQb,				//bias fixed shift
			 int nQo,				//out    fixed shift
			 short*pConvOut,
			 FILE  *fp_debug)		//output buf ,be pre malloced by outside,struct nKerCnt *out_cln*out_row 
{

	short *pProcDat=NULL;
	int   nDatCnt = 0;
	short *pWeight=NULL;
	short *pBiasProc = NULL;
	short *pPreluProc = NULL;
	short   shortRet = 0;
	int    nOutCnt = 0;

	if((pLayerDat==NULL)||(pLayerWeight==NULL)||(pBias==NULL)||(pPrelu==NULL)||(nFm<0))
	{
		printf("layercal input error!\r\n");
		return -1;
	}

	nDatCnt = sizeof(short)*nFm*9;
	pProcDat = (short *)malloc(nDatCnt);
	if(pProcDat==NULL){
		printf("malloc error!\r\n");
		return -1;
	}
	memset(pProcDat,0x00,nDatCnt);

	int nKloop,nRloop,nCloop;
	int nFmLoop;
	int nRowCnt,nClnCnt=0;
	pWeight = pLayerWeight;
	for(nKloop=0;nKloop<nKerCnt;nKloop++){
		pWeight = pLayerWeight;
		if(fp_debug!=NULL){
			fprintf(fp_debug,"***********************kernal:%d************************************* \n",nKloop);
			for(nFmLoop=0;nFmLoop<nFm;nFmLoop++){
					fprintf(fp_debug,"weight_%d :%04x %04x %04x %04x %04x %04x %04x %04x %04x  \n",nFmLoop,(*(pWeight+nKloop*nFm*9+nFmLoop*9))&0xffff  ,(*(pWeight+nKloop*nFm*9+nFmLoop*9+1) )&0xffff,(*(pWeight+nKloop*nFm*9+nFmLoop*9+2 ))&0xffff,(*(pWeight+nKloop*nFm*9+nFmLoop*9+3) )&0xffff,(*(pWeight+nKloop*nFm*9+nFmLoop*9+4) )&0xffff,(*(pWeight+nKloop*nFm*9+nFmLoop*9+5) )&0xffff,(*(pWeight+nKloop*nFm*9+nFmLoop*9+6 ))&0xffff,(*(pWeight+nKloop*nFm*9+nFmLoop*9+7 ))&0xffff,(*(pWeight+nKloop*nFm*9+nFmLoop*9+8) )&0xffff);
			}
		}

		for(nCloop=0;nCloop<(nCln-2);nCloop++){
			if(nCloop==0)
				nClnCnt=1;
			else
				nClnCnt++;
			for(nRloop=0;nRloop<(nRow-2);nRloop++){
				//procdat
				if(nRloop==0)
					nRowCnt = 1;
				else
					nRowCnt ++;
				
				if((nKloop==45)&&(nRowCnt==11)&&(nClnCnt==9)){
					//debug
					printf("debug,%d,%d,%d\n",nKloop,nRowCnt,nClnCnt);
				}
				for(nFmLoop=0;nFmLoop<nFm;nFmLoop++){
					//ע�⣺��������Ǹ���Kernal����ģ����ÿ��kernal֮������ʱָ�벻��˳�����µ���
					*(pProcDat+nFmLoop*9)  =*(pLayerDat+nFmLoop*nCln*nRow+nCloop*nRow+nRloop);
					*(pProcDat+nFmLoop*9+1)=*(pLayerDat+nFmLoop*nCln*nRow+nCloop*nRow+nRloop+1);
					*(pProcDat+nFmLoop*9+2)=*(pLayerDat+nFmLoop*nCln*nRow+nCloop*nRow+nRloop+2);
					*(pProcDat+nFmLoop*9+3)=*(pLayerDat+nFmLoop*nCln*nRow+(nCloop+1)*nRow+nRloop);
					*(pProcDat+nFmLoop*9+4)=*(pLayerDat+nFmLoop*nCln*nRow+(nCloop+1)*nRow+nRloop+1);
					*(pProcDat+nFmLoop*9+5)=*(pLayerDat+nFmLoop*nCln*nRow+(nCloop+1)*nRow+nRloop+2);
					*(pProcDat+nFmLoop*9+6)=*(pLayerDat+nFmLoop*nCln*nRow+(nCloop+2)*nRow+nRloop);
					*(pProcDat+nFmLoop*9+7)=*(pLayerDat+nFmLoop*nCln*nRow+(nCloop+2)*nRow+nRloop+1);
					*(pProcDat+nFmLoop*9+8)=*(pLayerDat+nFmLoop*nCln*nRow+(nCloop+2)*nRow+nRloop+2);
					if(fp_debug!=NULL){
						fprintf(fp_debug,"data_%d:%04x %04x %04x %04x %04x %04x %04x %04x %04x  \n",nFmLoop,(*(pProcDat+nFmLoop*9))&0xffff,(*(pProcDat+nFmLoop*9+1))&0xffff,(*(pProcDat+nFmLoop*9+2))&0xffff,(*(pProcDat+nFmLoop*9+3))&0xffff,(*(pProcDat+nFmLoop*9+4))&0xffff,(*(pProcDat+nFmLoop*9+5))&0xffff,(*(pProcDat+nFmLoop*9+6))&0xffff,(*(pProcDat+nFmLoop*9+7))&0xffff,(*(pProcDat+nFmLoop*9+8))&0xffff);
					}
				}
				
				//pweight
				pWeight = pLayerWeight+nKloop*nFm*9;
				//pbias
				pBiasProc = pBias+nKloop;
				//pprelu
				pPreluProc = pPrelu+nKloop;
				//int Cp3x3_fm(short *pDat_fm,short *pWeight_fm, short Bias_short,int nFm,int nDFixedShift,int nWFixedShift,int nBFixedShift,int nOutFixedShift);
				if(fp_debug!=NULL){
					fprintf(fp_debug,"cln:%d row:%d ",nCloop,nRloop);
				}
				shortRet=Cp3x3_fm(pProcDat,pWeight,*pBiasProc,nFm,nQd,nQw,nQb,nQo,fp_debug);
				
				if(shortRet<0){//prelu process
					int nRet=0;
					nRet = shortRet *(*pPreluProc);
					
					shortRet = nRet >>nQp;
					if((shortRet>32767)||(shortRet<-32768)){
						printf("prelu cal overflow error:%d! kernal:%d,nFm:%d,cln:%d,row:%d\r\n",nRet,nKloop,nFmLoop,nClnCnt,nRowCnt);
						//free(pProcDat);
						//return -1;
					}
				}
				shortRet = shortRet &0xffff;//���ʱ����16λ���
				*(pConvOut+nOutCnt)=shortRet;
				nOutCnt++;

				if(fp_debug!=NULL){
					//fprintf(fp_debug,"data  :%04x %04x %04x %04x %04x %04x %04x %04x %04x  \n",(*(pProcDat))&0xffff,(*(pProcDat+1))&0xffff,(*(pProcDat+2))&0xffff,(*(pProcDat+3))&0xffff,(*(pProcDat+4))&0xffff,(*(pProcDat+5))&0xffff,(*(pProcDat+6))&0xffff,(*(pProcDat+7))&0xffff,(*(pProcDat+8))&0xffff);
					//fprintf(fp_debug,"weight:%04x %04x %04x %04x %04x %04x %04x %04x %04x  \n",(*(pWeight))&0xffff  ,(*(pWeight+1) )&0xffff,(*(pWeight+2 ))&0xffff,(*(pWeight+3) )&0xffff,(*(pWeight+4) )&0xffff,(*(pWeight+5) )&0xffff,(*(pWeight+6 ))&0xffff,(*(pWeight+7 ))&0xffff,(*(pWeight+8) )&0xffff);
					float ff = ((float)shortRet)/((float)(1<<nQo));
					fprintf(fp_debug,"bias:%04x ;prelu:%04x ;counvout: %04x ;f_cnvout: %08f \n",(*(pBiasProc))&0xffff,(*(pPreluProc))&0xffff,shortRet&0xffff			,ff);
				}
			}//end nRloop
			if(fp_debug!=NULL){
					fprintf(fp_debug,"------------------------------------------ \n");
				}
		}//end nCloop
	}//end nKloop
	

	if(fp_debug!=NULL){
					fprintf(fp_debug,"-------------total_outcnt:%d------------out:[%d*%d*%d]-------------- \n",nOutCnt,nKerCnt,nRowCnt,nClnCnt);
				}
	

	free(pProcDat);
	return nOutCnt;
}

int MaxPool(short *pMaxPoolin,int nKSize,short *pMaxPoolout)
{
	int nLoop;
	short shortRet=0;

	if((pMaxPoolin==NULL)||(pMaxPoolout==NULL)){
		return -1;
	}

	for(nLoop=0;nLoop<nKSize*nKSize;nLoop++){
		if(nLoop==0)
			shortRet = pMaxPoolin[nLoop];
		if(pMaxPoolin[nLoop]>shortRet){
			shortRet = pMaxPoolin[nLoop];
		} 
	}
	*pMaxPoolout = shortRet;
	return 0;
}
//����ֵ<0����>0����poolin���ɵ����ݸ�����
int Pool(short *pPoolin,int nRow,int nCln,int nKSize,int nKerCnt,int nStride,short *pPoolout,FILE *fp)
{
	short *pDatProc=NULL;
	short *pSrc=NULL;
	int nOutCnt;

	if((pPoolin==NULL)||(pPoolout==NULL)||(nStride<0)||(nKSize<0))
	{
		printf("pool input error!\r\n");
		return -1;
	}

	pDatProc = (short *)malloc(nKSize*nKSize*sizeof(short));
	if(pDatProc==NULL){
		printf("pool malloc error!\r\n");
		return -1;
	}

	int nKloop,nRloop,nCloop;
	int i,nRet;
	short shortRet;
	nOutCnt = 0;
	short a0,a1,b0,b1;
	int nRowCnt,nClnCnt;
	for(nKloop=0;nKloop<nKerCnt;nKloop++){
		for(nCloop=0;nCloop<=(nCln-nKSize);nCloop+=nStride){
			if(nCloop==0)
				nClnCnt = 1;
			else
				nClnCnt++;
			for(nRloop=0;nRloop<=(nRow-nKSize);nRloop+=nStride){
				if(nRloop==0)
					nRowCnt=1;
				else
					nRowCnt++;
				for(i=0;i<nKSize;i++){
					pSrc = pPoolin+nKloop*nCln*nRow+(nCloop+i)*nRow+nRloop;
					memcpy(pDatProc+i*nKSize,pSrc,nKSize*sizeof(short));
				}
				a0 = *(pPoolin+nKloop*nCln*nRow+(nCloop+0)*nRow+nRloop);
				a1 = *(pPoolin+nKloop*nCln*nRow+(nCloop+0)*nRow+nRloop+1);
				b0 = *(pPoolin+nKloop*nCln*nRow+(nCloop+1)*nRow+nRloop);
				b1 = *(pPoolin+nKloop*nCln*nRow+(nCloop+1)*nRow+nRloop+1);
				nRet = MaxPool(pDatProc,nKSize,&shortRet);
				if(nRet<0){
					printf("maxpool ret error!\r\n");
					free(pDatProc);
					return -1;
				}
				*(pPoolout+nOutCnt)=shortRet;
				nOutCnt++;
			}//end nRloop
		}//end nCloop
	}//end nKerCnt

	free(pDatProc);
	if(fp!=NULL){
		fprintf(fp,"-------------poolout--outcnt[%d]---,cnt[%d*%d*%d]--------\n",nOutCnt,nKerCnt,nClnCnt,nRowCnt);
	}
	return nOutCnt;
}


//�������������CNN��PRELU��POOL
int LayerCNN(
			 short *pLayerDat,		//dat input,padded datadata structure (nFm*cln*row)
			 int   nRow,
			 int   nCln,
			 short *pLayerWeight,	//weight input,structure (nKerCnt * nFm * 9)
			 short	*pBias,			//bias input ,struct nKerCnt*1
			 short	*pPrelu,			//prelu input,struct nKerCnt*1
			 int nKerCnt,			//cnn k count,
			 int nFm,				//fm count per kernal
			 int nQd,				//dat fixed shift
			 int nQw,				//weight fixed shift
			 int nQp,               //prelu fixed shift
			 int nQb,				//bias fixed shift
			 int nQo,				//out    fixed shift
			 int nQe,				//in Qe fixed shift
			 short*pConvOut,        //output buf ,be pre malloced by outside,struct nKerCnt *out_cln*out_row 

			//pooling 
			int nPoolKSize,
			int nStride,		//if pooling unenable ,nStride should be <0
			short *pPoolout,
			FILE  *fp_debug,
			int nEltREn,
			int nEltWEn,
			int *ptrnQe,//IOֵ
			short *pEltBuf
			 )		
{
	int nLayerRet = 0,nRet;
	int nPoolRet=0;


	nLayerRet= LayerCal(
			 pLayerDat,		//dat input,padded datadata structure (nFm*cln*row)
			 nRow,
			 nCln,
			 pLayerWeight,	//weight input,structure (nKerCnt * nFm * 9)
			 pBias,			//bias input ,struct nKerCnt*1
			 pPrelu,			//prelu input,struct nKerCnt*1
			 nKerCnt,			//cnn k count,
			 nFm,				//fm count per kernal
			 nQd,				//dat fixed shift
			 nQw,				//weight fixed shift
			 nQp,               //prelu fixed shift
			 nQb,				//bias fixed shift
			 nQo,				//out    fixed shift
			 pConvOut,
			 fp_debug
			 );
	if(nLayerRet <0){
		printf("LayerCal ret error!\r\n");
		return -1;
	}
	
	if(nStride>0){
		nPoolRet=Pool(pConvOut,nRow-2,nCln-2,nPoolKSize,nKerCnt,nStride,pPoolout,fp_debug);
		if(nPoolRet <0){
			printf("Pool ret error!\r\n");
			return -1;
		}
	}

	//eltwise����,�ȶ���д,����ʾ����Eltwise����
	if(nEltREn){
		if((nQe<0)||(nQe>15)){
			printf("eltwise Qe error:%d\r\n",nQe);
			return -1;
		}
		nRet = Eltwise(pConvOut,nQo,pEltBuf,*ptrnQe,nQe,nLayerRet);
		if(nRet <0){
			printf("eltwise ret error!\r\n");
			return -1;
		}
	}

	if(nEltWEn){
		if(nStride>0){//�˴���nStride����pooling���ƣ�����������
			//��Pool�����ֵ
			memcpy(pEltBuf,pPoolout,nPoolRet*sizeof(short));
			*ptrnQe = nQo;
		}else{
			//��pConvout���ֵ
			memcpy(pEltBuf,pConvOut,nLayerRet*sizeof(short));
			if(nEltREn)//����Eltwise���㣬QֵΪQe
				*ptrnQe = nQe;
			else//û�н���Eltwise���㣬QֵΪQo
				*ptrnQe = nQo;
		}
	}
	return 0;
}

//��䴦��,ֻ���1��1�У�������ݹ̶�Ϊ0;
int PadProc(const short *pSource,short *pDest,int nRow,int nCln,int nFm)
{
	int nCloop,nFloop;
	int nDestCln;

	if((pSource==NULL)||(pDest==NULL)){
		printf("PadProc input error!\r\n");
		return -1;
	}
	
	nDestCln=0;
	memset(pDest,0x00,sizeof(short)*(nRow+2)*(nCln+2)*nFm);//��0
	for(nFloop=0;nFloop<nFm;nFloop++){
		for(nCloop=0;nCloop<nCln;nCloop++){
			{
				memcpy(pDest+nFloop*(nRow+2)*(nCln+2)+(nCloop+1)*(nRow+2)+1,pSource+nFloop*nRow*nCln+nCloop*nRow,nRow*sizeof(short));
			}
		}
	}
	
	return 0;
}
int max(int n1,int n2)
{
	if(n1>=n2)
		return n1;
	else
		return n2;
}
//������������pConvOut������
//�ȶ��굽max()
int Eltwise(short *pConvOut,int nQo,short *pEltBuf,int nQe,int nQz,int nCnt)//nQzΪeltwise�������
{
	int nLoop=0;
	int nQshift = 0;
	int nLeftShift = 0;
	short shortEltDat = 0;
	long long  nResult=0;

	if((pConvOut==NULL)||(pEltBuf==NULL)||(nQe<0)||(nQo<0)){
		printf("eltwise input error!\n");
		return -1;
	}
	
	if(nQe>nQo){
		nQshift = nQe - nQo;
		nLeftShift=0;
	}else{
		nQshift = nQo-nQe;
		nLeftShift=1;
	}
	for(nLoop=0;nLoop<nCnt;nLoop++){
		shortEltDat = *(pEltBuf+nLoop);
		if(nLeftShift)
			shortEltDat <<= nQshift;
		else 
			shortEltDat >>= nQshift;
		nResult = *(pConvOut+nLoop) + shortEltDat;

		if(nQo>=nQz){
			nResult = nResult>>(nQo-nQz) ;
		}else{
			nResult = nResult<<(nQz-nQo) ;
		}
		if((nResult>32767)||(nResult<-32768)){
			printf("eltwise overflow error!\n");
			//return -1;
		}

		*(pConvOut+nLoop) = (short)nResult;
		if((nResult>32767)||(nResult<-32768)){
			printf("eltwise overflow error!\n");
			//return -1;
		}
	}
	return 0;
}
//������������pConvOut������,�ȶ��굽Qo���ٶ��굽Qz
int tmp_Eltwise(short *pConvOut,int nQo,short *pEltBuf,int nQe,int nQz,int nCnt)//nQzΪeltwise�������
{
	int nLoop=0;
	int nQshift = 0;
	int nLeftShift = 0;
	short shortEltDat = 0;
	int  nResult=0;

	if((pConvOut==NULL)||(pEltBuf==NULL)||(nQe<0)||(nQo<0)){
		printf("eltwise input error!\n");
		return -1;
	}
	
	if(nQe>nQo){
		nQshift = nQe - nQo;
		nLeftShift=0;
	}else{
		nQshift = nQo-nQe;
		nLeftShift=1;
	}
	for(nLoop=0;nLoop<nCnt;nLoop++){
		shortEltDat = *(pEltBuf+nLoop);
		if(nLeftShift)
			shortEltDat <<= nQshift;
		else 
			shortEltDat >>= nQshift;
		nResult = *(pConvOut+nLoop) + shortEltDat;

		if(nQo>=nQz){
			nResult = nResult>>(nQo-nQz) ;
		}else{
			nResult = nResult<<(nQz-nQo) ;
		}
		if((nResult>32767)||(nResult<-32768)){
			printf("eltwise overflow error!\n");
			//return -1;
		}

		*(pConvOut+nLoop) = (short)nResult;
		//*(pConvOut+nLoop) +=shortEltDat;
		//if((*(pConvOut+nLoop)>32767)||(*(pConvOut+nLoop)<-32768)){
		//	printf("eltwise overflow error!\n");
		//	return -1;
		//}
	}
	return 0;
}
//������������pConvOut������
int old_Eltwise(short *pConvOut,int nQo,short *pEltBuf,int nQe,int nCnt)
{
	int nLoop=0;
	int nQshift = 0;
	int nLeftShift = 0;
	short shortEltDat = 0;

	if((pConvOut==NULL)||(pEltBuf==NULL)||(nQe<0)||(nQo<0)){
		printf("eltwise input error!\n");
		return -1;
	}
	
	if(nQe>nQo){
		nQshift = nQe - nQo;
		nLeftShift=0;
	}else{
		nQshift = nQo-nQe;
		nLeftShift=1;
	}
	for(nLoop=0;nLoop<nCnt;nLoop++){
		shortEltDat = *(pEltBuf+nLoop);
		if(nLeftShift)
			shortEltDat <<= nQshift;
		else 
			shortEltDat >>= nQshift;

		*(pConvOut+nLoop) +=shortEltDat;
		if((*(pConvOut+nLoop)>32767)||(*(pConvOut+nLoop)<-32768)){
			printf("eltwise overflow error!\n");
			//return -1;
		}
	}
	return 0;
}
short FCCal(const short *pConvBufIn,const short *pWeight,const short Bias_short,int nQd,int nQw,int nQb,int nQo,FILE *fp_test)
{
	short shortRet=0;
	int nBias,j,i;
	long long nRetLayer=0;
	long long n64Temp;
	int    nnTemp;

	int nAllShift		= nQd+nQw;//CHN:�˷���С�����λ��
	int nOutShift		= nAllShift-nQo;
	int nBiasShift		= nAllShift-nQb;

	if((pConvBufIn==NULL)||(pWeight==NULL)){
		printf("FCCal input error!\n");
		shortRet = -1;
		return shortRet;
	}
	nBias = Bias_short;
	nRetLayer = nBias<<nBiasShift;//�ȼ�bias
	if(fp_test!=NULL){
		fprintf(fp_test,"bias:%04X(%d),bias_shift:%08X(%d)\n",Bias_short&0xffff,Bias_short,(int)nRetLayer,(int)nRetLayer);
	}
	for(j=0;j<512;j=j+4){
		for(i=0;i<20;i++){
			//��4�����ݽ��г˼Ӽ���
			//ret0=d0*w0+nRetLayer
			//ret1=d1*w1+ret0
			//ret2=d2*w2
			//ret3=d3*w3+ret2
			//nRetLayer=ret1+ret3
			int ret0,ret1,ret2,ret3;
			long long nTemp=nRetLayer;

			if(fp_test!=NULL){
			//	fprintf(fp_test,"nRetLayer:%08X(%d)\n",nRetLayer,nRetLayer);
				fprintf(fp_test,"%04x * %04X(%d);  ",(*(pConvBufIn+j*20+i))&0xffff,(*(pWeight+j*20+i))&0xffff,*(pConvBufIn+j*20+i)*(*(pWeight+j*20+i)));
				fprintf(fp_test,"%04X * %04X(%d);  ",(*(pConvBufIn+(j+1)*20+i))&0xffff,(*(pWeight+(j+1)*20+i))&0xffff,*(pConvBufIn+(j+1)*20+i)*(*(pWeight+(j+1)*20+i)));
				fprintf(fp_test,"%04X * %04X(%d);  ",(*(pConvBufIn+(j+2)*20+i))&0xffff,(*(pWeight+(j+2)*20+i))&0xffff,*(pConvBufIn+(j+2)*20+i)*(*(pWeight+(j+2)*20+i)));
				fprintf(fp_test,"%04X * %04X(%d);  \r\n",(*(pConvBufIn+(j+3)*20+i))&0xffff,(*(pWeight+(j+3)*20+i))&0xffff,*(pConvBufIn+(j+3)*20+i)*(*(pWeight+(j+3)*20+i)));
			}
			ret0=*(pConvBufIn+j*20+i) *(*(pWeight+j*20+i));
			ret1=*(pConvBufIn+(j+1)*20+i) *(*(pWeight+(j+1)*20+i))+ret0;//��1�����ݸ���һ�������
			ret2=*(pConvBufIn+(j+2)*20+i) *(*(pWeight+(j+2)*20+i));//��2�����ݸ���2�������
			ret3=*(pConvBufIn+(j+3)*20+i) *(*(pWeight+(j+3)*20+i))+ret2;//��3�����ݸ���3�������
			
			nRetLayer=ret1 + ret3+nRetLayer;//+bias
			 n64Temp=(nRetLayer&0xfffffff);
			   nnTemp=n64Temp;
			fprintf(fp_test,"ssssnRet:%08x  ;nRet[27:12]:%04x,%08x\r\n",(int)nRetLayer,(nnTemp>>12)&0xffff,nnTemp);
			nTemp+=*(pConvBufIn+j*20+i) *(*(pWeight+j*20+i));
			nTemp+=*(pConvBufIn+(j+1)*20+i) *(*(pWeight+(j+1)*20+i));
			nTemp+=*(pConvBufIn+(j+2)*20+i) *(*(pWeight+(j+2)*20+i));
			nTemp+=*(pConvBufIn+(j+3)*20+i) *(*(pWeight+(j+3)*20+i));
			if(nTemp!=nRetLayer){
				printf("fc cal confict!!!\r\n");
				return -1;
			}
		}
		 n64Temp=(nRetLayer&0xfffffff);
		 nnTemp=(n64Temp>>12)&0xffff;
		fprintf(fp_test,"===>%d  :  nRet:%08x ; nRet[29:14]:%04x \r\n",j/4,(int)nRetLayer,nnTemp);
		fflush(fp_test);
	}
	nRetLayer = nRetLayer >>nOutShift;//��Ϊ16λ���
	fprintf(fp_test,"******nRet:%08x  \r\n",(int)nRetLayer);
	float ff = ((float)nRetLayer)/((float)(1<<nQo));
	if((nRetLayer >32767) ||(nRetLayer <-32768)){
		printf("FCCal overflow error:%d",nRetLayer);
		return -1;
	}
	
	shortRet = nRetLayer;
	return shortRet;
}
//˵����(1)������16���Ƶ��ļ���(2)��fpgaһ��
std::vector<float> CLNet_Monitor(short *pBuffIn,const char *sWeightFilename[],const char *sPreluFilename,const char *sBiasFilename,const char *sFcWeightFilename,const char *sFcBiasFilename)
{
	std::vector<float> out_put;
	int nKerCnt_array[27]      ={32, 64, 64, 64,128,128,128,128,128,256,256,256,256,256,256,256,256,256,256,256,512,512,512,512,512,512,512};//ÿ�����˸���
	int nRow_array[27]         ={96, 94, 46, 46, 46, 22, 22, 22, 22, 22, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,  4,  4,  4,  4,  4,  4};//ÿ�������ݾ���Ŀ�
	int nCln_array[27]         ={112,110,54, 54, 54, 26, 26, 26, 26, 26, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,  5,  5,  5,  5,  5,  5};//ÿ�������ݾ���ĳ�
	int nFm_array[27]          ={  3, 32,64, 64, 64,128,128,128,128,128,256,256,256,256,256,256,256,256,256,256,256,512,512,512,512,512,512};//ÿ�������������ݵ�ͨ����channel
	int nPoolKerSize_array[27] ={0  , 2,  0,  0,  2,  0,  0,  0,  0,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  2,  0,  0,  0,  0,  0,  0};//ÿ�����POOL��Kernal�ߴ磬������POOL��̶�Ϊ2������Ϊ0
	int nStride_array [27]     ={-1 , 2, -1, -1,  2, -1, -1, -1, -1,  2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  2, -1, -1, -1, -1, -1, -1};//ÿ�����POOLʱ������ָʾ��������POOL��ֵ�̶�Ϊ2������Ϊ-1
	int nPaden_array  [27]     ={0  , 0,  1,  1,  0,  1,  1,  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  1,  1,  1,  1,  1,  1};//�Ƿ���Ҫpad��ָʾ���������Ƿ���Ҫ����PAD0��PAD0Ϊ��ΧһȦ��0�����ȱ�Ϊcln+2,��ȱ�Ϊrow+2
	int nEltREn_array  [27]    ={0  , 0,  0,  1,  0,  0,  1,  0,  1,  0,  0,  1,  0,  1,  0,  1,  0,  1,  0,  1,  0,  0,  1,  0,  1,  0,  1};//�����Ƿ����Eltwise���㣬(��Ҫ����ʱӦ���ж�pEltFifo����)
	int nEltWEn_array  [27]    ={0  , 1,  0,  0,  1,  0,  1,  0,  1,  1,  0,  1,  0,  1,  0,  1,  0,  1,  0,  0,  1,  0,  1,  0,  1,  0,  0};//�����Ƿ����Eltwise���ݱ��棬���ں������Eltwise���㣬����ʱ��Ҫ����Qe(����Eltwise�ӷ�����ʱ����Qֵ)
	int nLoop          [27]    ={1  , 1,  1,  1,  2,  2,  2,  2,  2,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4, 16,  16, 16, 16, 16, 16, 16};
	int nQe=0;//��ʾEfifo��ֵ��Ӧ��Qֵ
	const char *sLayerName  [27]     ={"conv1a\0","conv1b\0","conv2_1\0","conv2_2\0","conv2\0",
	"conv3_1\0","conv3_2\0","conv3_3\0","conv3_4\0","conv3\0",
	"conv4_1\0","conv4_2\0","conv4_3\0","conv4_4\0","conv4_5\0",
	"conv4_6\0","conv4_7\0","conv4_8\0","conv4_9\0","conv4_10\0",
	"conv4\0","conv5_1\0","conv5_2\0","conv5_3\0","conv5_4\0",
	"conv5_5\0","conv5_6\0"};
	int i,nRet;
	short shortRet=0;
	
	//CHN:ע��!����Ļ����еĵ������ݾ��ǰ�short�ͽ��д洢
	//short *pBuffIn=NULL;//����ľ����������������ڸû����С�(��ʽΪ��ά����nFm*cln*row(ͨ��*��*��))�����pic.cmodel�ж�ȡ��������pConvBufout��pPoolBufout�п���
	short *pConvBufout=NULL;//����ľ��������;��ʽΪnKernal*cln*row(�����*��*��)
	short *pPoolBufout=NULL;//�����POOL������;��ʽΪnKerlnal*cln*row
	short *pBias=NULL;      //�����Bias����;��ʽΪLayer*nKernal������˳��洢��ÿ�㱣��nKernal��BiasԪ�أ���ӦnKernal�������
	short *pPrelu = NULL;   //�����Prelu����;��ʽΪLayer*nKernal;����˳��洢��ÿ�㱣��nKernal��PreluԪ�أ���ӦnKernal�������
	short *pWeight = NULL;  //�����Ȩֵ����;����ʱֻ���ص�ǰʹ�õ�һ�����;��ʽnKernal*nFm*3*3��ÿ������˵Ĳ���ΪnFm*3*3������Ӧͨ����*3*3
	short *pEltFifo=NULL;   //������������е�EltFifo���ݡ���ʽΪnKernal*cln*row
	FILE *fp_debug=NULL;    //�����ã�������������е����ݣ��Ծ����Ϊ��λ���д�ӡ�����ݽ϶�
	FILE *fp_io = NULL;     //�����ã�����һ����������е�����ͽ�����ݣ����ݽ���
	FILE *fp_std=NULL;      //�����ã�����һ����������ݣ����ݽ�������V�����Զ��Ƚ���
	FILE *fp_output=NULL;   //�����յ�Clnet���
	short ClNetRet[512]={0x00};//����������


	if((sWeightFilename==NULL)||(sPreluFilename==NULL)||(sBiasFilename==NULL)||(sFcWeightFilename==NULL)||(sFcBiasFilename==NULL))
	{
		printf("CLNET_Monitor input filename error!\r\n");
		return out_put;
	}

	//������������ռ䣬�˴�ʡ��
	int nBuffInSize = 64*94*110*sizeof(short);
	int nConvBufSize = nBuffInSize;
	int nPoolBufSize = 64*46*54*sizeof(short);
	int nEltBufSize  = nPoolBufSize;
	int nBiasCnt = 0;
	int nLayerLoop=0;
	int nBiasPtrCnt=0;
	int nRow,nCln;
	short *pCurBias=NULL;
	short *pCurPrelu=NULL;

	//ͳ�ƾ���˵���������Bias��prelu�Ĳ���������ͬ
	for(i=0;i<27;i++){
		nBiasCnt += nKerCnt_array[i];
	}
	//ͳ�Ʋ�������
	int nWeightCnt = 0;
	for(i=0;i<27;i++){
		nWeightCnt +=nKerCnt_array[i]*nFm_array[i]*9;
	}
	nWeightCnt+=512*512*5*4;
	printf("weightcnt:%d",nWeightCnt);
	//����ռ�
	/*pBuffIn = (short*)malloc(sizeof(short)*nBuffInSize);
	if(pBuffIn==NULL){
		printf("malloc pbuffin error!\r\n");
		goto err;
	}*/
	pConvBufout = (short*)malloc(sizeof(short)*nConvBufSize);
	if(pConvBufout==NULL){
		printf("malloc pConvBufout error!\r\n");
		goto err;
	}
	pPoolBufout = (short*)malloc(sizeof(short)*nPoolBufSize);
	if(pPoolBufout==NULL){
		printf("malloc pPoolBufout error!\r\n");
		goto err;
	}


	pEltFifo = (short*)malloc(sizeof(short)*nPoolBufSize);
	if(pEltFifo==NULL){
		printf("malloc pEltFifo error!\r\n");
		goto err;
	}

	pBias    = (short *)malloc(sizeof(short)*nBiasCnt);
	if(pBias==NULL){
		printf("malloc pBias error!\r\n");
		goto err;
	}

	pPrelu    = (short *)malloc(sizeof(short)*nBiasCnt);
	if(pPrelu==NULL){
		printf("malloc pPrelu error!\r\n");
		goto err;
	}

	pWeight   = (short*)malloc(sizeof(short)*512*512*5*4+1);
	if(pWeight==NULL){
		printf("malloc pWeight error!\r\n");
		goto err;
	}

	//����pic
	nRet = 0;
	/*for(i=0;i<3;i++){
		nRet = LoadHexFile_X16(sPicFilename[i],pBuffIn+i*96*112,96*112);
		if(nRet!=(96*112)){
			printf("Load pic%d file error!\r\n",i);
			goto err;
		}
	}*/
	
	//Load prelu
	nRet = LoadHexFile_X32(sPreluFilename,pPrelu,nBiasCnt);
	if(nRet!=nBiasCnt){
		printf("Load prelu file error!\r\n");
		goto err;
	}
	//load bias
	nRet = LoadHexFile_X32(sBiasFilename,pBias,nBiasCnt);
	if(nRet!=nBiasCnt){
		printf("Load bias file error!\r\n");
		goto err;
	}

	nBiasPtrCnt=0;
	//�ֲ�ģ������
	for(nLayerLoop=0;nLayerLoop<27;nLayerLoop++){
		printf("layer : %s process!\r\n",sLayerName[nLayerLoop]);
#ifdef __DEBUGE_FILE__
		char sname[128]={0x00};
		//��Ҫ�����м������ſ��˴���ע��
		fp_debug = NULL;
		sprintf(sname,"../debug_out/debug_out_%s.cmodel\0",sLayerName[nLayerLoop]);
		fp_debug = fopen(sname,"w");
		if(fp_debug==NULL){
			printf("fopen debug error!\r\n");
			goto err;
		}
		
		if(fp_debug!=NULL) fprintf(fp_debug,"-------------------------layer :%s--------Qo:%d---------------------\n",sLayerName[nLayerLoop],g_Q_O_array[nLayerLoop]);
		
		sprintf(sname,"../debug_out/io_%s.dat\0",sLayerName[nLayerLoop]);
		fp_io = fopen(sname,"w");
		if(fp_io==NULL){
			printf("fopen fpio error!\r\n");
			goto err;
		}

		sprintf(sname,"../debug_out/cstd_%x.hex\0",nLayerLoop);
		fp_std = fopen(sname,"w");
		if(fp_std==NULL){
			printf("fopen fp_std error!\r\n");
			goto err;
		}
		
#endif
		//PAD����
		if(nLayerLoop>0){
			//�ж��Ƿ���Ҫ����PAD����
			if(nPaden_array[nLayerLoop]){
				//��Ҫ����pad
				if(nPoolKerSize_array[nLayerLoop-1]>0){
					//��POOLBUF����PAD����
					//int PadProc(const short *pSource,short *pDest,int nRow,int nCln)
					if(0> PadProc(pPoolBufout,pBuffIn,nRow_array[nLayerLoop],nCln_array[nLayerLoop],nFm_array[nLayerLoop])){
						printf("Pad error!\n");
						goto err;
					}
				}else {
					//��pConvBufoutt����PAD����
					if(0> PadProc(pConvBufout,pBuffIn,nRow_array[nLayerLoop],nCln_array[nLayerLoop],nFm_array[nLayerLoop])){
						printf("Pad error!\n");
						goto err;
					}
				}
				//�����ɺ���Ҫ����nRow��nCln��ֵ(+2)
				nRow =nRow_array[nLayerLoop]+2;
				nCln = nCln_array[nLayerLoop]+2;
			}else {
				//����ҪPAD
				if(nPoolKerSize_array[nLayerLoop-1]>0){
					//��POOLBUF���п�������
					memcpy(pBuffIn,pPoolBufout,nFm_array[nLayerLoop]*nRow_array[nLayerLoop]*nCln_array[nLayerLoop]*sizeof(short));
					
				}else {
					//��pConvBufoutt���п�������
					memcpy(pBuffIn,pConvBufout,nFm_array[nLayerLoop]*nRow_array[nLayerLoop]*nCln_array[nLayerLoop]*sizeof(short));
				}
				
				nRow =nRow_array[nLayerLoop];
				nCln = nCln_array[nLayerLoop];
			}
		}else {
			nRow = nRow_array[nLayerLoop];
			nCln = nCln_array[nLayerLoop];
		}
#ifdef __DEBUGE_FILE__
		fprintf(fp_io,"-------------------------datain[%d*%d*%d]---------------------\n",nFm_array[nLayerLoop],nCln,nRow);
		{
			int xx,yy,zz;
			for(zz=0;zz<nFm_array[nLayerLoop];zz++){
				for(xx=0;xx<nCln;xx++){
					for(yy=0;yy<nRow;yy++){
						fprintf(fp_io,"%04x ",(*(pBuffIn+zz*nCln*nRow+xx*(nRow)+yy))&0xffff);
					}
					fprintf(fp_io,"\n");
				}
				fprintf(fp_io,"----------------------------------------------\n");
			}

			int nLoopCnt;
			for(nLoopCnt=0;nLoopCnt<nLoop[nLayerLoop];nLoopCnt++){
				for(zz=0;zz<nFm_array[nLayerLoop];zz++){
					for(xx=0;xx<nCln;xx++){
						for(yy=0;yy<nRow;yy++){
							fprintf(fp_std,"%04x ",(*(pBuffIn+zz*nCln*nRow+xx*(nRow)+yy))&0xffff);
						}
						fprintf(fp_std,"\n");
					}
					fprintf(fp_std,"//----------------------------------------------\n");
				}
			}
			
		}
#endif
		//������ص�ǰ��������
		nRet = LoadHexFile_X144(sWeightFilename[nLayerLoop] , pWeight , nKerCnt_array[nLayerLoop]*nFm_array[nLayerLoop]*9);
		if(nRet!=(nKerCnt_array[nLayerLoop]*nFm_array[nLayerLoop]*9)){
			printf("Load weight%d error!\n",nLayerLoop);
			goto err;
		}

		pCurBias = pBias+nBiasPtrCnt;
		pCurPrelu= pPrelu+nBiasPtrCnt;
		
		memset(pConvBufout,0x00,sizeof(short)*nConvBufSize);
		
		//��ǰ��QdӦ����һ���Qo���˴�����Qd������
		int nQd = 0;
		if(nLayerLoop==0)
			nQd = g_Q_D;
		else
			nQd = g_Q_O_array[nLayerLoop-1];
		//���ò�����
		nRet= LayerCNN(
			 pBuffIn,									//dat input,padded datadata structure (nFm*cln*row)
			 nRow,
			 nCln,
			 pWeight,									//weight input,structure (nKerCnt * nFm * 9)
			 pCurBias,									//bias input ,struct nKerCnt*1
			 pCurPrelu,									//prelu input,struct nKerCnt*1
			 nKerCnt_array[nLayerLoop],					//cnn k count,
			 nFm_array[nLayerLoop],						//fm count per kernal
			 nQd,										//dat fixed shift
			 g_Q_W_array[nLayerLoop]    ,				//weight fixed shift
			 g_Q_Prelu_array[nLayerLoop],               //prelu fixed shift
			 g_Q_Bias_array [nLayerLoop],				//bias fixed shift
			 g_Q_O_array[nLayerLoop]    ,				//out    fixed shift
			 g_Q_Elt_array[nLayerLoop] ,				//in Qe fixed shift
			 pConvBufout,								//conv output buf ,be pre malloced by outside,struct nKerCnt *out_cln*out_row 

			//pooling 
			nPoolKerSize_array[nLayerLoop],				//Pool�ĺ˳ߴ�
			nStride_array[nLayerLoop],					//if pooling unenable ,nStride should be <0
			pPoolBufout,								//�����ǰ�����pool,���������ڸû�����
			fp_debug,
			nEltREn_array[nLayerLoop],					//���Ƶ�ǰ���Ƿ����EltFifo��
			nEltWEn_array[nLayerLoop],					//���Ƶ�ǰ���Ƿ����EltFifo�洢
			&nQe,										//���ڲ��ᱻ�޸ģ�EltFifo���ݶ�Ӧ��Qֵ
			pEltFifo									//����Eltwise�����ݻ���
			 )	;
		
		if(nRet<0){
			printf("LayerCNN ret error!\r\n");
			goto err;
		}
#ifdef __DEBUGE_FILE__
		int xx,yy,zz;
		{
			fprintf(fp_io,"-------------------------convout--[%d*%d*%d]-----Qo[%d]--Qd[%d]--------------------\n",nKerCnt_array[nLayerLoop],nCln-2,nRow-2,g_Q_O_array[nLayerLoop],nQd)				;
			for(zz=0;zz<nKerCnt_array[nLayerLoop];zz++){
				fprintf(fp_io,"---------------------kernal:%d-------------------------\n",zz);
				for(xx=0;xx<nCln-2;xx++){
					for(yy=0;yy<nRow-2;yy++){
						fprintf(fp_io,"%04x ",(*(pConvBufout+zz*(nCln-2)*(nRow-2)+xx*(nRow-2)+yy))&0xffff);
					}
					fprintf(fp_io,"\n");
				}
				
			}
			
		}

		{
			fprintf(fp_io,"-------------------------convout_float-----------------------------\n")				;
			short shortRet = 0;
			for(zz=0;zz<nKerCnt_array[nLayerLoop];zz++){
				for(xx=0;xx<nCln-2;xx++){
					for(yy=0;yy<nRow-2;yy++){
						float ff ;//= ((float)shortRet)/((float)(1<<g_Q_O_array[nLayerLoop]));
						shortRet = (*(pConvBufout+zz*(nCln-2)*(nRow-2)+xx*(nRow-2)+yy))&0xffff;
						ff = ((float)shortRet)/((float)(1<<g_Q_O_array[nLayerLoop]));
						fprintf(fp_io,"%f ",ff);
					}
					fprintf(fp_io,"\n");
				}
			}
		}

		if(nStride_array[nLayerLoop]>0){
			fprintf(fp_io,"-------------------------poolout[%d*%d*%d]-----------------------------\n",nKerCnt_array[nLayerLoop],nCln_array[nLayerLoop+1],nRow_array[nLayerLoop+1])				;
			for(zz=0;zz<nKerCnt_array[nLayerLoop];zz++){
				fprintf(fp_io,"---------------------kernal:%d-------------------------\n",zz);
				for(xx=0;xx<nCln_array[nLayerLoop+1];xx++){
					for(yy=0;yy<nRow_array[nLayerLoop+1];yy++){
						fprintf(fp_io,"%04x ",(*(pPoolBufout+zz*nCln_array[nLayerLoop+1]*nRow_array[nLayerLoop+1]+xx*nRow_array[nLayerLoop+1]+yy))&0xffff);
					}
					fprintf(fp_io,"\n");
				}
			
			}
		}
		
#endif

		//�����Ƶ���һ��
		nBiasPtrCnt+=nKerCnt_array[nLayerLoop];
#ifdef __DEBUGE_FILE__
		if(fp_debug!=NULL) fclose(fp_debug);
		fp_debug=NULL;
		fclose(fp_io);
		fp_io = NULL;
		fclose(fp_std);
		fp_std = NULL;
#endif
		
	}

	//FC������㵥������
	{
		FILE *fp_test=NULL;
		fp_test = fopen("fc_debug.txt\0","w");
		if(fp_test==NULL){
			printf("fopen clnet fcdebug file error!\n");
			goto err;
		}
		fp_output = fopen("ClNet_out.txt\0","w");
		if(fp_output==NULL){
			printf("fopen clnet out file error!\n");
			goto err;
		}
		//Load bias
		nBiasCnt = 512;
		nRet = LoadHexFile_X32(sFcBiasFilename,pBias,nBiasCnt);
		if(nRet!=nBiasCnt){
			printf("Load fcbias file error!\r\n");
			goto err;
		}
		//Load weight
		nBiasCnt= 512*512*5*4;
		nRet = LoadHexFile_X32(sFcWeightFilename,pWeight,nBiasCnt);
		if(nRet!=nBiasCnt){
			printf("Load fcweight file error!\r\n");
			goto err;
		}
		for(i=0;i<512;i++){
			ClNetRet[i]=FCCal(pConvBufout,pWeight+i*512*5*4,*(pBias+i),g_Q_O_array[26],g_Q_W_array[27],g_Q_Bias_array[27],g_Q_O_array[27],fp_test);
			fprintf(fp_test,"-----------loop:%d  [%d]----------\r\n",i/4,i);
		}
		fclose(fp_test);
		for(i=0;i<512;i++){
			shortRet = ClNetRet[i];
			fprintf(fp_output,"%04X ",(shortRet &0xffff));
			if((i+1)%16==0)
				fprintf(fp_output,"\n");
		}

		fprintf(fp_output,"-----------------float_out---------------------------------\n");
		for(i=0;i<512;i++){
			shortRet = ClNetRet[i];
			float ff ;
			ff = ((float)shortRet)/((float)(1<<g_Q_O_array[27]));
			out_put.push_back(ff);
		}
	}





	//free(pBuffIn);
	free(pConvBufout);
	free(pPoolBufout);
	free(pBias);
	free(pPrelu);
	free(pWeight);
	free(pEltFifo);
	if(fp_debug!=NULL)     fclose(fp_debug);
	if(fp_io!=NULL) fclose(fp_io);
	if(fp_output!=NULL)   fclose(fp_output);
	if(fp_std!=NULL) 	fclose(fp_std);
	return out_put;

err:
	printf("CLNET_Monitor err process!\r\n");
	if(pBuffIn!=NULL)		free(pBuffIn);
	if(pConvBufout!=NULL)	free(pConvBufout);
	if(pPoolBufout!=NULL)	free(pPoolBufout);
	if(pPrelu!=NULL)		free(pPrelu);
	if(pBias!=NULL)			free(pBias);
	if(pWeight!=NULL)       free(pWeight);
	if(fp_debug!=NULL)     fclose(fp_debug);
	if(fp_io!=NULL)   fclose(fp_io);
	if(pEltFifo!=NULL)      free(pEltFifo);
	if(fp_output!=NULL)   fclose(fp_output);
	if(fp_std!=NULL) 	fclose(fp_std);
	return out_put;
}


