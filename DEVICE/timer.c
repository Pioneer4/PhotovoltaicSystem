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
*Description    : ADCģ��ת�����ɼ���ѹ�ź�   ��ʱʱ��: 50ms
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
	TIM_TimeBaseInitStruct.TIM_Period = 100 - 1;             /* 100 * 0.5 = 50ms */
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
		
		/************LED3��1S��˸************/
	    if (TIM6_Led_Flag == 20)
		{
		    PAout(7) = !PAin(7);
			IwdgFeed();		                            //ι��
			TIM6_Led_Flag = 0;
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
*Description    : ÿ50ms����һ���ж�
				  ��Ҫ����
				  1. ADC��ѹ����
*Input          ��
*Output         ��
*Return         ��
*************************************************************/
void TIM7_IRQHandler(void)
{
	static unsigned int curCount = 0;
	static unsigned int volCount = 0;
	
	u16 v = 0;
	
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) !=  RESET) /* ����ж� */
	{
		v = GetAdc(4);
		p_vaw->voltage = v * (float)3.3 / 4096 * (float)3.71;
		
		/****** ������·��ѹ���� ******/
		if (p_vaw->voltage > (float)12)        /* 12 / 3.71(����ϵ��) = 3.2345 */
		{
			if (volCount == 1)                  /* ��ֹ�����ѹ���µ�·�л�,��ֵ��ѹ����50ms������Ϊ���������ѹ����ȡ���� */
			{
				RelayPhovSwitch(OPEN_CONTACT);  /* ���Թ���������ѹ����12V�����жϲ�����· */
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
				RelaySampSwitch(CLOSE_CONTACT);
				v = GetAdc(5);
				p_vaw->current = v * (float)3.3 / 4096 / (float)4.7 * 1000;
				p_vaw->power = p_vaw->voltage * p_vaw->current / 1000;
				p_vaw->wh += p_vaw->power * (float)0.05 / 3600;
				
				if (p_vaw->current > 638)                  /* �������� 3V / 4.7�� = 0.63829A */
				{
					if (curCount == 1)
					{
						RelayPhovSwitch(OPEN_CONTACT);     /* ���Թ���������������638mA�����жϲ�����· */
						curCount = 0;
					}
					else 
					{
						curCount++;
					}
				}
					
				
				if (p_vaw->current < 30)                   /* ��������С��30mA */
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
				RelaySampSwitch(OPEN_CONTACT);
				v = GetAdc(5);
				p_vaw->current = v * (float)3.3 / 4096 /47 * 1000;
				p_vaw->power = p_vaw->voltage * p_vaw->current / 1000;
				p_vaw->wh += p_vaw->power * (float)0.05 / 3600;
				
				if (p_vaw->current > (float)42.553)                /* 2V / 47��= 0.042553A */ 
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
		
		if (p_vaw->power > 8)
		{
			brightControl(100);
		}
		else
		{
			brightControl(p_vaw->power * 12);
		}
		
	}
	
	TIM_ClearFlag(TIM7, TIM_IT_Update);	            //�����־λ
}

