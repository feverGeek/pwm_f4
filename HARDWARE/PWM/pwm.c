#include "pwm.h"
#include "led.h"
#include "usart.h"
 
// GPIO 重映射  
// PB6 -> TIM4_CH1
// PB7 -> TIM4_CH2
//
void _GPIO_Configuration(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7; // PB6  PB7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;         // 复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   // 速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;       // 推挽复用输出
	GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_NOPULL;    // 不上拉 也不下拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);               // 初始化PB9
 
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_TIM4);
}


//TIM14 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM14_PWM_Init(u32 arr,u32 psc)
{		 					 
	//此部分需手动修改IO口设置
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);  	//TIM14时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE); 	//使能PORTF时钟	
	
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource9,GPIO_AF_TIM14); //GPIOF9复用为定时器14
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;           //GPIOF9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOF,&GPIO_InitStructure);              //初始化PF9
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM14,&TIM_TimeBaseStructure);//初始化定时器14
	
	//初始化TIM14 Channel1 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低
	TIM_OC1Init(TIM14, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 4OC1

	TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Enable);  //使能TIM14在CCR1上的预装载寄存器
 
  TIM_ARRPreloadConfig(TIM14,ENABLE);//ARPE使能 
	
	TIM_Cmd(TIM14, ENABLE);  //使能TIM14
 
										  
}  


// TIM4 双通道输出PWM
void TIM4_PWM_Init(u32 arr, u16 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	_GPIO_Configuration(); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , ENABLE); 
	
	
	TIM_TimeBaseStructure.TIM_Prescaler=psc;         //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;            //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0; // 重复寄存器，用于自动更新pwm占空比  	
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);//初始化定时器4
	
	//初始化TIM4 Channel1 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;             // 选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // 比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;      // 输出极性:TIM输出比较极性低
//	TIM_OCInitStructure.TIM_Pulse = 222;                          // 设置占空比时间
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);                      // 根据T指定的参数初始化外设TIM1 4OC1
	
	
	//初始化TIM4 Channel2 PWM模式	
//	TIM_OCInitStructure.TIM_Pulse = 222;                          // 设置占空比时间
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     // 输出极性:TIM输出比较极性高
  TIM_OC2Init(TIM4, &TIM_OCInitStructure);
	
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);             // 使能TIM4在CCR1上的预装载寄存器
	TIM_ARRPreloadConfig(TIM4,ENABLE);                            // ARPE使能 
	
	TIM_Cmd(TIM4, ENABLE);                                        //使能TIM4
	TIM_CtrlPWMOutputs(TIM4, ENABLE);
}




