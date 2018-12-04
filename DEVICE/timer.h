/*************************************************************
*  Open source but not allowed to modify(All rights reserved)
*File name      : timer.h
*Auther         : 张沁
*Version        : v1.0
*Date           : 2017-08-05
*Description    : 该文件中包含了time.c中所有的函数原型
*Function List  : 
*************************************************************/
#ifndef __TIMER_H
#define __TIMER_H

#include "myinclude.h"

void TIM6_Init(void);  /* 用于与HMI通信、LDE3的1S闪烁、独立看门狗喂狗 */

void TIM7_Init(void);  /* ADC模数转换 */

#endif
