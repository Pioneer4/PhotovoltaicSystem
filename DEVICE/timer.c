/*************************************************************
*  Open source but not allowed to modify(All rights reserved)
*File name      : timer.c
*Auther         : ����
*Version        : v1.0
*Date           : 2017-08-05
*Description    : ���ļ��а����˶Զ�ʱ���ĳ�ʼ�����ж�����
*Function List  : 
*************************************************************/
#include "myinclude.h"

/*************************************************************
*Function Name  : TIM6_Init
*Auther         : ����
*Vertion        : v1.0
*Date           : 2017-08-05
*Description    : TIM6(APB1 84MHz)  ��ʱʱ��: 50ms
*Input          ��
*Output         ��
*Return         ��
*************************************************************/
void TIM6_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 100 - 1;             /* 100 * 0.5 = 50ms */
	TIM_TimeBaseInitStruct.TIM_Prescaler = 42000 - 1;  		 /* Ԥ��Ƶֵ  0.5ms */
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseInitStruct);
	
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);	//�ж�ʹ��
	
	NVIC_InitStruct.NVIC_IRQChannel = TIM6_DAC_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x01;/* ��ռ���ȼ�1 */
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x03;       /* �����ȼ�1 */
	NVIC_Init(&NVIC_InitStruct);

	TIM_Cmd(TIM6, ENABLE);
}


/*************************************************************
*Function Name  : TIM7_Init
*Auther         : ����
*Vertion        : v1.0
*Date           : 2017-08-05
*Description    : ADCģ��ת�����ɼ���ѹ�ź�   ��ʱʱ��: 10ms
*Input          ��
*Output         ��
*Return         ��
*************************************************************/
void TIM7_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
	
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 20 - 1;             /* 20 * 0.5 = 10ms */
	TIM_TimeBaseInitStruct.TIM_Prescaler = 42000 - 1;  		 /* Ԥ��Ƶֵ  0.5ms */
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseInitStruct);
	
	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);	//�ж�ʹ��
	
	NVIC_InitStruct.NVIC_IRQChannel = TIM7_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;/* ��ռ���ȼ�0 */
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x03;       /* �����ȼ�3 */
	NVIC_Init(&NVIC_InitStruct);

	TIM_Cmd(TIM7, ENABLE);
}

/*************************************************************
*Function Name  : TIM6_DAC_IRQHandler
*Author         : ����
*Version        : v1.0
*Date           : 2017-08-05
*Description    : ÿ50ms����һ���ж�
				  ��Ҫ����
				  1. ˢ��LCD��ʾ����
				  2. LED3��1S��˸����ʾϵͳ���У�
                  3. �������Ź�1Sι��
*Input          ��
*Output         ��
*Return         ��
*************************************************************/
void TIM6_DAC_IRQHandler(void)
{
	static u8 TIM6_Led_Flag = 0;
	//char charBuff[8];                                 //�ַ������
	
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) !=  RESET) //����ж�
	{
		TIM6_Led_Flag++;
		
		/************ LED3��1S��˸ ************/
	    if (TIM6_Led_Flag == 20)
		{
		    PAout(7) = !PAin(7);
			IwdgFeed();		                            //ι��
			TIM6_Led_Flag = 0;
			printf("voltage: %.3f \r\n",p_vawPV->voltage);
		}
		
		HmiChatGame();
	}
	TIM_ClearFlag(TIM6, TIM_IT_Update);	            //�����־λ
}

/*************************************************************
*Function Name  : TIM7_IRQHandler
*Author         : ����
*Version        : v1.0
*Date           : 2018-12-23
*Description    : ÿ10ms����һ���ж�
				  ��Ҫ����
				  1. ADC��ѹ����
*Input          ��
*Output         ��
*Return         ��
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
	
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) !=  RESET) /* ����ж� */
	{
		#if 1
		vPV = GetAdc(2);       /* (PA4)ADC����ֵ */
		vMT = GetAdc(3); 		 /* (PA5)ADC����ֵ */
		
		p_vawPV->voltage = vPV * (float)3.3 / 4096; 			/* ������������ѹ��λV */
		if (p_vawPV->voltage <= (float)0.001)
		{
			p_vawPV->voltage = 0;
		}
		p_vawPV->power = p_vawPV->voltage * p_vawPV->voltage / (float)4.7 * coefficient;  /* ÿƽ���׹��� */
		p_vawPV->wh += p_vawPV->power / 360000;
		
		p_vawMT->voltage = vMT * (float)3.3 / 4096;				/* ������������ѹ��λV */
		if (p_vawMT->voltage <= (float) 0.001)
		{
			p_vawMT->voltage = 0;
		}
		p_vawMT->power = p_vawMT->voltage * p_vawMT->voltage / (float)4.7 * coefficient;  /* ÿƽ���׹��� */
		p_vawMT->wh += p_vawMT->power / 360000;
		
		#endif
		
		#if 0
		count++;
		
		v = GetAdc(4);       /* (PA4)ADC����ֵ */
		c = GetAdc(5); 		 /* (PA5)ADC����ֵ */
		if (v * c > maxP)
		{
			maxP = v * c;
			maxPVoltage = v; /* ���������ʱ������ѹADCֵ */
			maxPCurrent = c; /* ���������ʱ��������ADCֵ */
		}
		
		PwmMosControl(count);
		
		/* 1000ms����һ�� */
		if (count >= 100)
		{
			p_vawPV->voltage = maxPVoltage * (float)3.3 / 4096 * (float)3.63333333; /* ��ѹ��λV */
			if (p_vawPV->voltage < (float)0.1 )
			{
				p_vawPV->voltage = 0;
			}
			
			p_vawPV->current = maxPCurrent * (float)3.3 / 4096 / 2 * 1000;          /* ������λmA */
			if (p_vawPV->current < 5)
			{
				p_vawPV->current = 0;
			}
			
			p_vawPV->power = p_vawPV->voltage * p_vawPV->current / 1000 * coefficient;  /* ÿƽ���׹��� */
			p_vawPV->wh += p_vawPV->power / 3600;
			
			brightControl(p_vawPV->power/10 + 5);									  /* �źŵ����ȿ��� */
			
			maxPVoltage = 0;
			maxPCurrent = 0;
			maxP = 0;
			count = 0;
		}
		#endif
	}
	
	TIM_ClearFlag(TIM7, TIM_IT_Update);	                /* �����־λ */
}

/******************************* draft���ݸ����� *******************************/
		#if 0
		v = GetAdc(4);
		//p_vawPV->voltage = v * (float)3.28 / 4096 * (float)3.687;
		
		/* ��ֵ�˲� */
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
		
		/****** ������·��ѹ���� ******/
		if (p_vawPV->voltage > (float)12)         /* 12 / 3.71(����ϵ��) = 3.2345 */
		{
			if (volCount == 1)                  /* ��ֹ�����ѹ���µ�·�л�,��ֵ��ѹ����50ms������Ϊ���������ѹ����ȡ���� */
			{
				LedSwitch(OPEN_CONTACT);  /* ���Թ���������ѹ����12V�����жϲ�����· */
				volCount = 0;
			    sampCircuitFlag = SAMP_CIRCUIT_1;
			} 
			else
			{
				volCount++;
			}
		} 
		
		/****** 1�ź�2�Ų�����·֮�����ʵ�ʵ��������л� ******/
		switch (sampCircuitFlag)
		{
			case SAMP_CIRCUIT_1:
			{
				WarmSwitch(CLOSE_CONTACT);
				v = GetAdc(5);
				p_vawPV->current = v * (float)3.3 / 4096 / (float)4.7 * 1000;
				p_vawPV->power = p_vawPV->voltage * p_vawPV->current / 1000 * coefficient;  /* ÿƽ���׹��� */
				p_vawPV->wh += p_vawPV->power * (float)0.05 / 3600;
				
				if (p_vawPV->current > 638)                  /* �������� 3V / 4.7�� = 0.63829A */
				{
					if (curCount == 1)
					{
						LedSwitch(OPEN_CONTACT);     /* ���Թ���������������638mA�����жϲ�����· */
						curCount = 0;
					}
					else 
					{
						curCount++;
					}
				}
					
				
				if (p_vawPV->current < 30)                   /* ��������С��30mA */
				{
					if (curCount == 20)
					{
						curCount = 0;
						sampCircuitFlag = SAMP_CIRCUIT_2;  /* �л�Ϊ2�Ų�����· */
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
				p_vawPV->power = p_vawPV->voltage * p_vawPV->current / 1000 * coefficient;  /* ÿƽ���׹��� */
				p_vawPV->wh += p_vawPV->power * (float)0.05 / 3600;
				
				if (p_vawPV->current > (float)42.553)                /* 2V / 47��= 0.042553A */ 
				{
					if (curCount == 20)                      /* ������������1S����42.553mA�����л���1�Ų�����· */
					{
						curCount = 0;
						sampCircuitFlag = SAMP_CIRCUIT_1;   /* �л�Ϊ1�Ų�����· */
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
		
		/* �����źŵ� */
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
