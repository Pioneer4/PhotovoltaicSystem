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
*Description    : ADC模数转换，采集电压信号   定时时长: 10ms
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
	TIM_TimeBaseInitStruct.TIM_Period = 20 - 1;             /* 20 * 0.5 = 10ms */
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
		
		/************ LED3的1S闪烁 ************/
	    if (TIM6_Led_Flag == 20)
		{
		    PAout(7) = !PAin(7);
			IwdgFeed();		                            //喂狗
			TIM6_Led_Flag = 0;
			printf("voltage: %.3f \r\n",p_vawPV->voltage);
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
*Description    : 每10ms进入一次中断
				  主要任务：
				  1. ADC电压采样
*Input          ：
*Output         ：
*Return         ：
*************************************************************/
void TIM7_IRQHandler(void)
{
//	static unsigned int count = 0;
//	static float maxPVoltage = 0;
//	static float maxPCurrent = 0;
//	
//	static u32 maxP = 0;
	u32 vPV = 0;
	u32 vMT = 0;
	
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) !=  RESET) /* 溢出中断 */
	{
		#if 1
		vPV = GetAdc(2);       /* (PA4)ADC采样值 */
		vMT = GetAdc(3); 		 /* (PA5)ADC采样值 */
		
		p_vawPV->voltage = vPV * (float)3.3 / 4096; 			/* 光伏采样电阻电压单位V */
		if (p_vawPV->voltage <= (float)0.001)
		{
			p_vawPV->voltage = 0;
		}
		p_vawPV->power = p_vawPV->voltage * p_vawPV->voltage / (float)4.7 * coefficient;  /* 每平方米功率 */
		p_vawPV->wh += p_vawPV->power / 360000;
		
		p_vawMT->voltage = vMT * (float)3.3 / 4096;				/* 电机采样电阻电压单位V */
		if (p_vawMT->voltage <= (float) 0.001)
		{
			p_vawMT->voltage = 0;
		}
		p_vawMT->power = p_vawMT->voltage * p_vawMT->voltage / (float)4.7 * coefficient;  /* 每平方米功率 */
		p_vawMT->wh += p_vawMT->power / 360000;
		
		#endif
		
		#if 0
		count++;
		
		v = GetAdc(4);       /* (PA4)ADC采样值 */
		c = GetAdc(5); 		 /* (PA5)ADC采样值 */
		if (v * c > maxP)
		{
			maxP = v * c;
			maxPVoltage = v; /* 保存最大功率时的最大电压ADC值 */
			maxPCurrent = c; /* 保存最大功率时的最大电流ADC值 */
		}
		
		PwmMosControl(count);
		
		/* 1000ms进入一次 */
		if (count >= 100)
		{
			p_vawPV->voltage = maxPVoltage * (float)3.3 / 4096 * (float)3.63333333; /* 电压单位V */
			if (p_vawPV->voltage < (float)0.1 )
			{
				p_vawPV->voltage = 0;
			}
			
			p_vawPV->current = maxPCurrent * (float)3.3 / 4096 / 2 * 1000;          /* 电流单位mA */
			if (p_vawPV->current < 5)
			{
				p_vawPV->current = 0;
			}
			
			p_vawPV->power = p_vawPV->voltage * p_vawPV->current / 1000 * coefficient;  /* 每平方米功率 */
			p_vawPV->wh += p_vawPV->power / 3600;
			
			brightControl(p_vawPV->power/10 + 5);									  /* 信号灯亮度控制 */
			
			maxPVoltage = 0;
			maxPCurrent = 0;
			maxP = 0;
			count = 0;
		}
		#endif
	}
	
	TIM_ClearFlag(TIM7, TIM_IT_Update);	                /* 清除标志位 */
}

/******************************* draft（草稿区） *******************************/
		#if 0
		v = GetAdc(4);
		//p_vawPV->voltage = v * (float)3.28 / 4096 * (float)3.687;
		
		/* 均值滤波 */
		volBuf[averCount++] = v * (float)3.28 / 4096 * (float)3.687;
		if (averCount == 3)
		{
			for (averCount=0; averCount<3; averCount++)
			{
				sum += volBuf[averCount];
				p_vawPV->voltage = sum / 3;
			}
			averCount = 0;
		}
		
		/****** 采样电路电压保护 ******/
		if (p_vawPV->voltage > (float)12)         /* 12 / 3.71(比例系数) = 3.2345 */
		{
			if (volCount == 1)                  /* 防止脉冲电压导致电路切换,峰值电压超过50ms，则认为不是脉冲电压，采取保护 */
			{
				LedSwitch(OPEN_CONTACT);  /* 测试光伏板输出电压超过12V，则切断采样电路 */
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
				WarmSwitch(CLOSE_CONTACT);
				v = GetAdc(5);
				p_vawPV->current = v * (float)3.3 / 4096 / (float)4.7 * 1000;
				p_vawPV->power = p_vawPV->voltage * p_vawPV->current / 1000 * coefficient;  /* 每平方米功率 */
				p_vawPV->wh += p_vawPV->power * (float)0.05 / 3600;
				
				if (p_vawPV->current > 638)                  /* 电流保护 3V / 4.7Ω = 0.63829A */
				{
					if (curCount == 1)
					{
						LedSwitch(OPEN_CONTACT);     /* 测试光伏板输出电流超过638mA，则切断采样电路 */
						curCount = 0;
					}
					else 
					{
						curCount++;
					}
				}
					
				
				if (p_vawPV->current < 30)                   /* 采样电流小于30mA */
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
				WarmSwitch(OPEN_CONTACT);
				v = GetAdc(5);
				p_vawPV->current = v * (float)3.3 / 4096 /47 * 1000;
				p_vawPV->power = p_vawPV->voltage * p_vawPV->current / 1000 * coefficient;  /* 每平方米功率 */
				p_vawPV->wh += p_vawPV->power * (float)0.05 / 3600;
				
				if (p_vawPV->current > (float)42.553)                /* 2V / 47Ω= 0.042553A */ 
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
		
		/* 功率信号灯 */
		if (p_vawPV->power > 8 * coefficient)
		{
			brightControl(100);
		}
		else if (p_vawPV->power > (float)0.09 * coefficient)
		{
			brightControl(p_vawPV->power / coefficient * 12);
		}
		else if (p_vawPV->power > (float)0.001 * coefficient)
		{
			brightControl(1);
		}
		else
		{
			brightControl(0);
		}
		#endif
