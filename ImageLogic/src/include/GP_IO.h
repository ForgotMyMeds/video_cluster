/*
 * GP_IO.h
 *
 *  Created on: 2017年11月22日
 *      Author: root
 */

#ifndef INCLUDE_GP_IO_H_
#define INCLUDE_GP_IO_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <time.h>
#include <memory.h>
#include <malloc.h>
#include <sys/mman.h>

int GP_IO(char* command, char* address, char* data);
uint64_t _GP_IO(char* command, char* address);
int hex2dec(char *hex, uint64_t *O_DATA);
int char2int(char ch);


#endif /* INCLUDE_GP_IO_H_ */
