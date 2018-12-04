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
*Date           : 2017-08-05
*Description    : ÿ50ms����һ���ж�
				  ��Ҫ����
				  1. ADC��ѹ����
*Input          ��
*Output         ��
*Return         ��
*************************************************************/
void TIM7_IRQHandler(void)
{
	u16 v = 0;
	
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) !=  RESET) //����ж�
	{
		v = GetAdc(5);
		p_vaw->voltage = v * (float)3.3 / 4096;
	}
	
	TIM_ClearFlag(TIM7, TIM_IT_Update);	            //�����־λ
}

