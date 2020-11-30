/*************************************************************
*  Open source but not allowed to modify(All rights reserved)
*File name      : pwmMos.h
*Author         : 张沁
*Version        : v1.0
*Date           : 2019-04-24
*Description    : 该文件中包含了对pwmMos.c中所有的函数原型
*Function List  : 
*************************************************************/
#ifndef __PWMMOS_H
#define __PWMMOS_H

#include "myinclude.h"


void PwmMosInit(void);
void PwmMosControl(u16 val);



#endif
