/*************************************************************
*  Open source but not allowed to modify(All rights reserved)
*File name      : timer.c
*Auther         : 张沁
*Version        : v1.0
*Date           : 2017-08-05
*Description    : 该文件中包含了对定时器的初始化和中断配置
*Function List  : 
*************************************************************/
#include "myinclude.h"

/*************************************************************
*Function Name  : TIM6_Init
*Auther         : 张沁
*Vertion        : v1.0
*Date           : 2017-08-05
*Description    : TIM6(APB1 84MHz)  定时时长: 50ms
*Input          ：
*Output         ：
*Return         ：
*************************************************************/
void TIM6_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 100 - 1;             /* 100 * 0.5 = 50ms */
	TIM_TimeBaseInitStruct.TIM_Prescaler = 42000 - 1;  		 /* 预分频值  0.5ms */
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseInitStruct);
	
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);	//中断使能
	
	NVIC_InitStruct.NVIC_IRQChannel = TIM6_DAC_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x01;/* 抢占优先级1 */
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x03;       /* 子优先级1 */
	NVIC_Init(&NVIC_InitStruct);

	TIM_Cmd(TIM6, ENABLE);
}


/*************************************************************
*Function Name  : TIM7_Init
*Auther         : 张沁
*Vertion        : v1.0
*Date           : 2017-08-05
*Description    : ADC模数转换，采集电压信号   定时时长: 50ms
*Input          ：
*Output         ：
*Return         ：
*************************************************************/
void TIM7_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
	
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 100 - 1;             /* 100 * 0.5 = 50ms */
	TIM_TimeBaseInitStruct.TIM_Prescaler = 42000 - 1;  		 /* 预分频值  0.5ms */
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseInitStruct);
	
	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);	//中断使能
	
	NVIC_InitStruct.NVIC_IRQChannel = TIM7_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;/* 抢占优先级0 */
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x03;       /* 子优先级3 */
	NVIC_Init(&NVIC_InitStruct);

	TIM_Cmd(TIM7, ENABLE);
}

/*************************************************************
*Function Name  : TIM6_DAC_IRQHandler
*Author         : 张沁
*Version        : v1.0
*Date           : 2017-08-05
*Description    : 每50ms进入一次中断
				  主要任务：
				  1. 刷新LCD显示界面
				  2. LED3的1S闪烁（表示系统运行）
                  3. 独立看门狗1S喂狗
*Input          ：
*Output         ：
*Return         ：
*************************************************************/
void TIM6_DAC_IRQHandler(void)
{
	static u8 TIM6_Led_Flag = 0;
	//char charBuff[8];                                 //字符串输出
	
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) !=  RESET) //溢出中断
	{
		TIM6_Led_Flag++;
		
		/************LED3的1S闪烁************/
	    if (TIM6_Led_Flag == 20)
		{
		    PAout(7) = !PAin(7);
			IwdgFeed();		                            //喂狗
			TIM6_Led_Flag = 0;
			
		}
		
		HmiChatGame();
	}
	TIM_ClearFlag(TIM6, TIM_IT_Update);	            //清除标志位
}

/*************************************************************
*Function Name  : TIM7_IRQHandler
*Author         : 张沁
*Version        : v1.0
*Date           : 2017-08-05
*Description    : 每50ms进入一次中断
				  主要任务：
				  1. ADC电压采样
*Input          ：
*Output         ：
*Return         ：
*************************************************************/
void TIM7_IRQHandler(void)
{
	u16 v = 0;
	
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) !=  RESET) //溢出中断
	{
		v = GetAdc(5);
		p_vaw->voltage = v * (float)3.3 / 4096;
	}
	
	TIM_ClearFlag(TIM7, TIM_IT_Update);	            //清除标志位
}

