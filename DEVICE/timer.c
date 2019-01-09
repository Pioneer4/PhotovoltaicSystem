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
*Date           : 2018-12-23
*Description    : 每50ms进入一次中断
				  主要任务：
				  1. ADC电压采样
*Input          ：
*Output         ：
*Return         ：
*************************************************************/
void TIM7_IRQHandler(void)
{
	static unsigned int curCount = 0;
	static unsigned int volCount = 0;
	
	u16 v = 0;
	
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) !=  RESET) /* 溢出中断 */
	{
		v = GetAdc(4);
		p_vaw->voltage = v * (float)3.3 / 4096 * (float)3.71;
		
		/****** 采样电路电压保护 ******/
		if (p_vaw->voltage > (float)12)        /* 12 / 3.71(比例系数) = 3.2345 */
		{
			if (volCount == 1)                  /* 防止脉冲电压导致电路切换,峰值电压超过50ms，则认为不是脉冲电压，采取保护 */
			{
				RelayPhovSwitch(OPEN_CONTACT);  /* 测试光伏板输出电压超过12V，则切断采样电路 */
				volCount = 0;
			    sampCircuitFlag = SAMP_CIRCUIT_1;
			} 
			else
			{
				volCount++;
			}
		} 
		
		/****** 1号和2号采样电路之间根据实际电流进行切换 ******/
		switch (sampCircuitFlag)
		{
			case SAMP_CIRCUIT_1:
			{
				RelaySampSwitch(CLOSE_CONTACT);
				v = GetAdc(5);
				p_vaw->current = v * (float)3.3 / 4096 / (float)4.7 * 1000;
				p_vaw->power = p_vaw->voltage * p_vaw->current / 1000;
				p_vaw->wh += p_vaw->power * (float)0.05 / 3600;
				
				if (p_vaw->current > 638)                  /* 电流保护 3V / 4.7Ω = 0.63829A */
				{
					if (curCount == 1)
					{
						RelayPhovSwitch(OPEN_CONTACT);     /* 测试光伏板输出电流超过638mA，则切断采样电路 */
						curCount = 0;
					}
					else 
					{
						curCount++;
					}
				}
					
				
				if (p_vaw->current < 30)                   /* 采样电流小于30mA */
				{
					if (curCount == 20)
					{
						curCount = 0;
						sampCircuitFlag = SAMP_CIRCUIT_2;  /* 切换为2号采样电路 */
					}
					else 
					{
						curCount++;
					}
				}
				
				break;
			}
			
			case SAMP_CIRCUIT_2:
			{
				RelaySampSwitch(OPEN_CONTACT);
				v = GetAdc(5);
				p_vaw->current = v * (float)3.3 / 4096 /47 * 1000;
				p_vaw->power = p_vaw->voltage * p_vaw->current / 1000;
				p_vaw->wh += p_vaw->power * (float)0.05 / 3600;
				
				if (p_vaw->current > (float)42.553)                /* 2V / 47Ω= 0.042553A */ 
				{
					if (curCount == 20)                      /* 采样电流持续1S大于42.553mA，则切换至1号采样电路 */
					{
						curCount = 0;
						sampCircuitFlag = SAMP_CIRCUIT_1;   /* 切换为1号采样电路 */
					}
					else
					{
						curCount++;
					}
				}
				break;
			}
			
			default:
				break;
		}
		
		if (p_vaw->power > 8)
		{
			brightControl(100);
		}
		else
		{
			brightControl(p_vaw->power * 12);
		}
		
	}
	
	TIM_ClearFlag(TIM7, TIM_IT_Update);	            //清除标志位
}

