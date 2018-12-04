/*************************************************************
*  Open source but not allowed to modify(All rights reserved)
*File name      : hmi_uart.h
*Author         : ����
*Version        : v1.0
*Date           : 2018-11-24
*Description    : ���ļ�������hmi_uart.cz�����еĺ���ԭ��
*Function List  : 
*************************************************************/

#ifndef __HMI_UART_H_
#define	__HMI_UART_H_


#include "myinclude.h"


void UART3_Init(u32 bound);
static char *itoa( int value, char *string, int radix );
void USART3_printf(USART_TypeDef* USARTx, char *Data, ...);
void USART3_Send_Data(u8 *data, u8 len);

void SendHmiCmd(u8 *cmd);

#endif 
