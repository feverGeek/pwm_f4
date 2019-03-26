#include "pwm.h"
#include "led.h"
#include "usart.h"
 
// GPIO ��ӳ��  
// PB6 -> TIM4_CH1
// PB7 -> TIM4_CH2
//
void _GPIO_Configuration(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7; // PB6  PB7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;         // ���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   // �ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;       // ���츴�����
	GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_NOPULL;    // ������ Ҳ������
	GPIO_Init(GPIOB, &GPIO_InitStructure);               // ��ʼ��PB9
 
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_TIM4);
}


//TIM14 PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM14_PWM_Init(u32 arr,u32 psc)
{		 					 
	//�˲������ֶ��޸�IO������
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);  	//TIM14ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE); 	//ʹ��PORTFʱ��	
	
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource9,GPIO_AF_TIM14); //GPIOF9����Ϊ��ʱ��14
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;           //GPIOF9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
	GPIO_Init(GPIOF,&GPIO_InitStructure);              //��ʼ��PF9
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=arr;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM14,&TIM_TimeBaseStructure);//��ʼ����ʱ��14
	
	//��ʼ��TIM14 Channel1 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�
	TIM_OC1Init(TIM14, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 4OC1

	TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Enable);  //ʹ��TIM14��CCR1�ϵ�Ԥװ�ؼĴ���
 
  TIM_ARRPreloadConfig(TIM14,ENABLE);//ARPEʹ�� 
	
	TIM_Cmd(TIM14, ENABLE);  //ʹ��TIM14
 
										  
}  


// TIM4 ˫ͨ�����PWM
void TIM4_PWM_Init(u32 arr, u16 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	_GPIO_Configuration(); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , ENABLE); 
	
	
	TIM_TimeBaseStructure.TIM_Prescaler=psc;         //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=arr;            //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0; // �ظ��Ĵ����������Զ�����pwmռ�ձ�  	
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);//��ʼ����ʱ��4
	
	//��ʼ��TIM4 Channel1 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;             // ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // �Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;      // �������:TIM����Ƚϼ��Ե�
//	TIM_OCInitStructure.TIM_Pulse = 222;                          // ����ռ�ձ�ʱ��
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);                      // ����Tָ���Ĳ�����ʼ������TIM1 4OC1
	
	
	//��ʼ��TIM4 Channel2 PWMģʽ	
//	TIM_OCInitStructure.TIM_Pulse = 222;                          // ����ռ�ձ�ʱ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     // �������:TIM����Ƚϼ��Ը�
  TIM_OC2Init(TIM4, &TIM_OCInitStructure);
	
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);             // ʹ��TIM4��CCR1�ϵ�Ԥװ�ؼĴ���
	TIM_ARRPreloadConfig(TIM4,ENABLE);                            // ARPEʹ�� 
	
	TIM_Cmd(TIM4, ENABLE);                                        //ʹ��TIM4
	TIM_CtrlPWMOutputs(TIM4, ENABLE);
}




