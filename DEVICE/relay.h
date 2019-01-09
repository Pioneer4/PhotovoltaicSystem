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

#define OPEN_CONTACT  Bit_RESET
#define CLOSE_CONTACT Bit_SET

void RelayInit(void);  	
void RelayPhovSwitch(BitAction state);
void RelaySampSwitch(BitAction state);

#endif
