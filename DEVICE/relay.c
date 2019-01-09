/*************************************************************
*  Open source but not allowed to modify(All rights reserved)
*File name      : realy.c
*Author         : 张沁
*Version        : v1.0
*Date           : 2018-12-25
*Description    : 该文件包含了继电器控制接口的相关函数
*Function List  : 
*************************************************************/

#include "myinclude.h"


/*************************************************************
*Function Name  : RelayInit
*Author         : 张沁
*Version        : v1.0
*Date           : 2018-12-25
*Description    : 初始化RelayPhov(PE0)和RelaySamp(PE1)
				  1.RelayPhov继电器：用于保护采样电路，输入电流过大，
				  继电器断开，测试光伏板断路
				  2.RelaySamp继电器：用于采样电路切换
*Input          ：
*Output         ：
*Return         ：
*************************************************************/
void RelayInit(void)
{
	GPIO_InitTypeDef GPIOE_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIOE_InitStructure.GPIO_Mode = GPIO_Mode_OUT;           /* 输出模式 */
	GPIOE_InitStructure.GPIO_OType = GPIO_OType_PP;          /* 推挽输出 */
	GPIOE_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;  /* GPIOE0和GPIOE1 */
	GPIOE_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;            /* 上拉 */
	GPIOE_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;      /* 速度100M */
	
	GPIO_Init(GPIOE, &GPIOE_InitStructure);                  /* 初始化PE0，PE1 */
	
	GPIO_ResetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_1);          /* 电平拉低 （继电器常闭触点闭合，常开触点断开） */
}

/*************************************************************
*Function Name  : RelayPhovSwitch
*Author         : 张沁
*Version        : v1.0
*Date           : 2018-12-25
*Description    : 控制继电器（采样保护电路）动作
*Input          ：BitAction: 继电器动作方式
				  @arg OPEN_CONTACT : 常闭触点闭合，常开触点断开
				  @arg CLOSE_CONTACT: 常闭触点断开，常开触点闭合
*Output         ：
*Return         ：
*************************************************************/
void RelayPhovSwitch(BitAction state) 
{
	if (state == OPEN_CONTACT)         /* 常闭触点闭合，常开触点断开 */
	{
		GPIO_ResetBits(GPIOE, GPIO_Pin_0);
	}
	else if (state == CLOSE_CONTACT)   /* 常闭触点断开，常开触点闭合 */
	{
		GPIO_SetBits(GPIOE, GPIO_Pin_0);
	}
}

/*************************************************************
*Function Name  : RelaySampSwitch
*Author         : 张沁
*Version        : v1.0
*Date           : 2018-12-25
*Description    : 控制继电器（采样切换电路）动作
*Input          ：BitAction: 继电器动作方式
				  @arg OPEN_CONTACT : 常闭触点闭合，常开触点断开
				  @arg CLOSE_CONTACT: 常闭触点断开，常开触点闭合
*Output         ：
*Return         ：
*************************************************************/
void RelaySampSwitch(BitAction state) 
{
	if (state == OPEN_CONTACT)         /* 常闭触点闭合，常开触点断开 */
	{
		GPIO_ResetBits(GPIOE, GPIO_Pin_1);
	}
	else if (state == CLOSE_CONTACT)   /* 常闭触点断开，常开触点闭合 */
	{
		GPIO_SetBits(GPIOE, GPIO_Pin_1);
	}
}

