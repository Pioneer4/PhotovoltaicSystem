/*************************************************************
*  Open source but not allowed to modify(All rights reserved)
*File name      : MyTypeDef.H
*Auther         : 张沁
*Version        : v1.0
*Date           : 2018-11-22
*Description    : 通用数据类型定义区(如全局变量等)
*Function List  : 
*************************************************************/
#ifndef _MYTYPEDEF_H_
#define _MYTYPEDEF_H_

#include "ff.h"

/*** 全局变量定义区 ***/
#ifdef CPP_GLOBALS
#define EXTERN
#else
#define EXTERN extern
#endif

EXTERN unsigned char flagTP;

/*
接收状态
bit15 接收完成
bit14 接收到0x0d '\r'
bit13~0 接收到的有效字符数目
*/
EXTERN unsigned short int USART_RX_STA; /* 接收状态标记为0 */
EXTERN unsigned char UART_RX_BUF[200];  /* 接收缓冲 */

/*
byte0: 0x55 HMI发送的用户指令
byte1: 0x01 控制命令 关于单片机与hmi数据交互状态机的运行控制
       0x02 控制命令 关于使用CAN总线发送数据的控制指令
byte2: 具体指令区域
*/
#define  UART3_REC_LEN  3                         /* UART3接收缓冲区长度 */
EXTERN unsigned char point3;                      /* UART3接收缓冲区数据计数器 */
EXTERN unsigned char UART3_RX_BUF[UART3_REC_LEN]; /* UART3接收缓冲区 */

#define  UART4_REC_LEN  15						  /* UART4接收缓冲区长度 */
EXTERN unsigned char UART4_RX_BUF[UART4_REC_LEN]; /* UART4接收缓冲区 */


/*
与HMI通信的状态机参数
*/

#define PAGE_LOGO			 0
#define	PAGE_MENU 	 	     1
#define PAGE_CONTROL         2
#define	PAGE_INFO       	 3
#define PAGE_SURE	         4
EXTERN unsigned char state;

#define REFRESH_PHOTOV_INFO   20   /* 光伏模组基本信息刷新时间（1000ms） */
#define REFRESH_MENU          20   /* 菜单界面（1000ms） */
#define REFRESH_CURVE_INFO    1    /* 曲线信息刷新时间（50ms） */
EXTERN unsigned char timeIndex;    /* 计时索引 */


typedef struct ENVIR_PARAM
{
	float lightIntensity;        /* 光照强度 */
	float temp;					 /* 温度 */
	float humi;                  /* 湿度 */
	float pressure;              /* 气压 */
	short altitude;              /* 海拔 */
} _ENVIR_PARAM;
EXTERN _ENVIR_PARAM envirParam, *p_envirParam;

typedef struct vawPV
{
	float voltage;               /* 电压 */
	float current;			     /* 电流 */
	float power;                 /* 功率 */
	float wh;                    /* 累计发电量 （瓦时） */
} _vawPV;
EXTERN _vawPV vawPV, *p_vawPV;
EXTERN _vawPV vawMT, *p_vawMT;

EXTERN float coefficient;        /* 功率归一化系数 */

/* FATFS相关变量 */
EXTERN FIL fil;
EXTERN FRESULT res;
EXTERN UINT bww;

EXTERN unsigned char moduleNum;
EXTERN unsigned char newFileFlag;

EXTERN unsigned char sdInitFlag;

EXTERN unsigned char sampCircuitFlag;
#define SAMP_CIRCUIT_1   1       /* 1号采样电路 */
#define SAMP_CIRCUIT_2   2       /* 2号采样电路 */

#endif//_MYTYPEDEF_H_
