/*************************************************************
*  Open source but not allowed to modify(All rights reserved)
*File name      : relay.c
*Auther         : ����
*Vertion        : v1.0
*Date           : 2018-12-25
*Description    : ���ļ�������relay.c�����еĺ���ԭ��
*Function List  : 
*************************************************************/
#ifndef __RELAY_H
#define __RELAY_H

#include "myinclude.h"



void RelayInit(void);  	
void LedSwitch(BitAction state);
void WarmSwitch(BitAction state);

#endif
