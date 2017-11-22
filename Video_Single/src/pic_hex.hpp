/*
 * pic_hex.hpp
 *
 *  Created on: 2017年11月20日
 *      Author: root
 */

#ifndef PIC_HEX_HPP_
#define PIC_HEX_HPP_
#include <iostream>
#include <stdlib.h>
#include <string>

#define g_Q_D 14;

int wFpgaPic_hex(const char *sFloatFilename);
int LoadFloatFile1D(const char *sFilename,float *pFloatDat,int nCount);


#endif /* PIC_HEX_HPP_ */
