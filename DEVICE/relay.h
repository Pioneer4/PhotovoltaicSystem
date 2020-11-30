/*************************************************************
*  Open source but not allowed to modify(All rights reserved)
*File name      : relay.c
*Auther         : 张沁
*Vertion        : v1.0
*Date           : 2018-12-25
*Description    : 该文件包含了relay.c中所有的函数原型
*Function List  : 
*************************************************************/
#ifndef __RELAY_H
#define __RELAY_H

#include "myinclude.h"



void RelayInit(void);  	
void LedSwitch(BitAction state);
void WarmSwitch(BitAction state);

#endif
