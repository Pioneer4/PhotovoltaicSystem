/*************************************************************
*  Open source but not allowed to modify(All rights reserved)
*File name      : pwmLed.c
*Author         : ����
*Version        : v1.0
*Date           : 2018-12-02
*Description    : ���ļ��а����˶�PWM����ĳ�ʼ��
*Function List  : 
*************************************************************/
#include "myinclude.h"

/*************************************************************
*Function Name  : pwmLedInit
*Auther         : ����
*Vertion        : v1.0
*Date           : 2018-12-02
*Description    : ��ʼ��pwm,��������MOS�ܣ����Ƶƣ���ʾ������Ϣ
			      TIM3��������PWM��1KHz��
				  PC6 ��Ӧͨ����CH1��
*Input          ��
*Output         ��
*Return         ��
*************************************************************/
void PwmLedInit(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  	  //TIM3ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); 	  //ʹ��PORTCʱ��	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;              //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	      //�ٶ�100MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);                    //��ʼ��PC6,PC7,PC8,PC9
	
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);   //GPIOC6����Ϊ��ʱ��3
	
	TIM_TimeBaseStructure.TIM_Prescaler = 840 - 1;            //��ʱ����Ƶ 84M/840 = 100kHz
	TIM_TimeBaseStructure.TIM_Period = 100 - 1;               //�Զ���װ��ֵ 100kHz/100 = 1kHz
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;         //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OCInitStructure.TIM_Pulse = 0;                        //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//�Ƚ����ʹ��
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  


	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR1�ϵ�Ԥװ�ؼĴ���
 
    TIM_ARRPreloadConfig(TIM3, ENABLE);//ARPEʹ�� 
	
	TIM_Cmd(TIM3, ENABLE);             //ʹ��TIM3
}


/*************************************************************
*Function Name  : brightControl
*Author         : ����
*Version        : v1.0
*Date           : 2018-12-02
*Description    : ���ȿ���
*Input          : val  @arg 0~99  
*                 ռ�ձ� = ��val+1��/ 100
*Output         : 
*Return         : 
*************************************************************/
void brightControl(u16 val)
{
	TIM_SetCompare1(TIM3, val);
}

