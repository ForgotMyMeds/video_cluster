/*
 * read_res.h
 *
 *  Created on: 2017年11月22日
 *      Author: root
 */

#ifndef INCLUDE_READ_RES_H_
#define INCLUDE_READ_RES_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/time.h>

#define ADDR_BEO	0x20010000UL
#define SIZE_BEO	0x400UL

int read_res(char * feature);



#endif /* INCLUDE_READ_RES_H_ */
