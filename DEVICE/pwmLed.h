/*************************************************************
*  Open source but not allowed to modify(All rights reserved)
*File name      : pwmLed.h
*Author         : 张沁
*Version        : v1.0
*Date           : 2018-12-02
*Description    : 该文件中包含了对pwmLed.c中所有的函数原型
*Function List  : 
*************************************************************/
#ifndef __PWMLED_H
#define __PWMLED_H

#include "myinclude.h"


void PwmLedInit(void);
void brightControl(u16 val);



#endif
