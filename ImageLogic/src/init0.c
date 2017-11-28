#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/time.h>

#define ADDR_BEO	0x20000000UL
#define SIZE_BEO	0x5000000UL

int init0()
{
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

	int num=0;
    for(i=0;i<SIZE_BEO/4;i++)
    {
    	if ((i & 0x3)==0)
    		num=i>>2;
    	else
    		num=0;
        *(((unsigned int*)mapped_base_dma_addr)+i) = num;
        //if ((i & 0xffff)==0)
        	//printf ("%d\n", i);
    }

    if (munmap(mapped_base_dma_addr, SIZE_BEO) == -1)
    {
        fprintf(stderr,"Can't unmap the memory to user space.\n");
        exit(0);
    }
    fprintf(stderr,"unmap successed.\n");

   // printf("\n\nInit finished !\n\n");fflush(stdout);
    return 0;
}
