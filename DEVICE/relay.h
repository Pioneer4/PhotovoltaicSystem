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

#define OPEN_CONTACT  Bit_RESET
#define CLOSE_CONTACT Bit_SET

void RelayInit(void);  	
void RelayPhovSwitch(BitAction state);
void RelaySampSwitch(BitAction state);

#endif
