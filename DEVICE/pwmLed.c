/*************************************************************
*  Open source but not allowed to modify(All rights reserved)
*File name      : pwmLed.c
*Author         : 张沁
*Version        : v1.0
*Date           : 2018-12-02
*Description    : 该文件中包含了对PWM输出的初始化
*Function List  : 
*************************************************************/
#include "myinclude.h"

/*************************************************************
*Function Name  : pwmLedInit
*Auther         : 张沁
*Vertion        : v1.0
*Date           : 2018-12-02
*Description    : 初始化pwm,用于驱动MOS管，控制灯，显示电量信息
			      TIM3用于生产PWM（1KHz）
				  PC6 对应通道（CH1）
*Input          ：
*Output         ：
*Return         ：
*************************************************************/
void PwmLedInit(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  	  //TIM3时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); 	  //使能PORTC时钟	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;              //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	      //速度100MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);                    //初始化PC6,PC7,PC8,PC9
	
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);   //GPIOC6复用为定时器3
	
	TIM_TimeBaseStructure.TIM_Prescaler = 840 - 1;            //定时器分频 84M/840 = 100kHz
	TIM_TimeBaseStructure.TIM_Period = 100 - 1;               //自动重装载值 100kHz/100 = 1kHz
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;         //选择定时器模式:TIM脉冲宽度调制模式1
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OCInitStructure.TIM_Pulse = 0;                        //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//比较输出使能
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  


	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR1上的预装载寄存器
 
    TIM_ARRPreloadConfig(TIM3, ENABLE);//ARPE使能 
	
	TIM_Cmd(TIM3, ENABLE);             //使能TIM3
}


/*************************************************************
*Function Name  : brightControl
*Author         : 张沁
*Version        : v1.0
*Date           : 2018-12-02
*Description    : 亮度控制
*Input          : val  @arg 0~99  
*                 占空比 = （val+1）/ 100
*Output         : 
*Return         : 
*************************************************************/
void brightControl(u16 val)
{
	TIM_SetCompare1(TIM3, val);
}

