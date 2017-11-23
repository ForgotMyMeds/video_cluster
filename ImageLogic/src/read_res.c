/*
 * read_res.c
 *
 *  Created on: 2017年11月22日
 *      Author: root
 */
#include "include/read_res.h"

int read_res(char * feature){
			int i;
	    int memfd_2;
	    void *mapped_base_dma_addr;
	    char dev[]="/dev/mem";
	    memfd_2 = open(dev, O_RDWR | O_SYNC);
	    if (memfd_2 == -1)
	    {
	        fprintf(stderr,"Can't open %s.\n", dev);
	        exit(0);
	    }
	    fprintf(stderr,"%s opened.\n", dev);
	    mapped_base_dma_addr = mmap(0, SIZE_BEO, PROT_READ | PROT_WRITE, MAP_SHARED, memfd_2, ADDR_BEO);
	    close(memfd_2);
	    if (mapped_base_dma_addr == (void *) -1)
	    {
	        fprintf(stderr,"Can't map the memory to user space.\n");
	        exit(0);
	    }
	    fprintf(stderr,"DDR Memory mapped at address %p.\n", mapped_base_dma_addr);

			char flag = 0;
	    for(i=0;i<SIZE_BEO/16;i++)
	    {
	       sprintf(feature+strlen(feature),"%f,%f,%f,%f,%f,%f,%f,%f,", ((int)(short)(*(((unsigned int*)mapped_base_dma_addr)+4*i) &0xffff))/((float)(1<<13)),((int)(short)((*(((unsigned int*)mapped_base_dma_addr)+4*i) >>16) &0xffff))/((float)(1<<13)),
	        ((int)(short)(*(((unsigned int*)mapped_base_dma_addr)+4*i+1)&0xffff))/((float)(1<<13)),((int)(short)((*(((unsigned int*)mapped_base_dma_addr)+4*i+1)>>16)&0xffff))/((float)(1<<13)),
	         ((int)(short)(*(((unsigned int*)mapped_base_dma_addr)+4*i+2)&0xffff))/((float)(1<<13)),((int)(short)((*(((unsigned int*)mapped_base_dma_addr)+4*i+2)>>16)&0xffff))/((float)(1<<13)),
	         ((int)(short)(*(((unsigned int*)mapped_base_dma_addr)+4*i+3)&0xffff))/((float)(1<<13)),((int)(short)((*(((unsigned int*)mapped_base_dma_addr)+4*i+3)>>16)&0xffff))/((float)(1<<13)));
	         if(flag){
	         		flag = 0;
	         		//printf("\n");
	         	}
	         else
	         		flag= 1;
	       fflush(stdout);
	    }

	    if (munmap(mapped_base_dma_addr, SIZE_BEO) == -1)
	    {
	        fprintf(stderr,"Can't unmap the memory to user space.\n");
	        exit(0);
	    }
	    fprintf(stderr,"unmap successed.\n");
	    return 0;
}



