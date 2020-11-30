/*************************************************************
*  Open source but not allowed to modify(All rights reserved)
*File name      : MyTypeDef.H
*Auther         : ����
*Version        : v1.0
*Date           : 2018-11-22
*Description    : ͨ���������Ͷ�����(��ȫ�ֱ�����)
*Function List  : 
*************************************************************/
#ifndef _MYTYPEDEF_H_
#define _MYTYPEDEF_H_

#include "ff.h"

/*** ȫ�ֱ��������� ***/
#ifdef CPP_GLOBALS
#define EXTERN
#else
#define EXTERN extern
#endif

EXTERN unsigned char flagTP;

/*
����״̬
bit15 �������
bit14 ���յ�0x0d '\r'
bit13~0 ���յ�����Ч�ַ���Ŀ
*/
EXTERN unsigned short int USART_RX_STA; /* ����״̬���Ϊ0 */
EXTERN unsigned char UART_RX_BUF[200];  /* ���ջ��� */

/*
byte0: 0x55 HMI���͵��û�ָ��
byte1: 0x01 �������� ���ڵ�Ƭ����hmi���ݽ���״̬�������п���
       0x02 �������� ����ʹ��CAN���߷������ݵĿ���ָ��
byte2: ����ָ������
*/
#define  UART3_REC_LEN  3                         /* UART3���ջ��������� */
EXTERN unsigned char point3;                      /* UART3���ջ��������ݼ����� */
EXTERN unsigned char UART3_RX_BUF[UART3_REC_LEN]; /* UART3���ջ����� */

#define  UART4_REC_LEN  15						  /* UART4���ջ��������� */
EXTERN unsigned char UART4_RX_BUF[UART4_REC_LEN]; /* UART4���ջ����� */


/*
��HMIͨ�ŵ�״̬������
*/

#define PAGE_LOGO			 0
#define	PAGE_MENU 	 	     1
#define PAGE_CONTROL         2
#define	PAGE_INFO       	 3
#define PAGE_SURE	         4
EXTERN unsigned char state;

#define REFRESH_PHOTOV_INFO   20   /* ���ģ�������Ϣˢ��ʱ�䣨1000ms�� */
#define REFRESH_MENU          20   /* �˵����棨1000ms�� */
#define REFRESH_CURVE_INFO    1    /* ������Ϣˢ��ʱ�䣨50ms�� */
EXTERN unsigned char timeIndex;    /* ��ʱ���� */


typedef struct ENVIR_PARAM
{
	float lightIntensity;        /* ����ǿ�� */
	float temp;					 /* �¶� */
	float humi;                  /* ʪ�� */
	float pressure;              /* ��ѹ */
	short altitude;              /* ���� */
} _ENVIR_PARAM;
EXTERN _ENVIR_PARAM envirParam, *p_envirParam;

typedef struct vawPV
{
	float voltage;               /* ��ѹ */
	float current;			     /* ���� */
	float power;                 /* ���� */
	float wh;                    /* �ۼƷ����� ����ʱ�� */
} _vawPV;
EXTERN _vawPV vawPV, *p_vawPV;
EXTERN _vawPV vawMT, *p_vawMT;

EXTERN float coefficient;        /* ���ʹ�һ��ϵ�� */

/* FATFS��ر��� */
EXTERN FIL fil;
EXTERN FRESULT res;
EXTERN UINT bww;

EXTERN unsigned char moduleNum;
EXTERN unsigned char newFileFlag;

EXTERN unsigned char sdInitFlag;

EXTERN unsigned char sampCircuitFlag;
#define SAMP_CIRCUIT_1   1       /* 1�Ų�����· */
#define SAMP_CIRCUIT_2   2       /* 2�Ų�����· */

#endif//_MYTYPEDEF_H_
