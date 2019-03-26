#include "sys.h"
#include "delay.h"
#include "usart.h"
//#include "led.h"
#include "pwm.h"
#include "lcd.h"
#include "touch.h"

static u16 PwmPeriod = 500;
static u16 PwmDutyRatio = 250;
static u16 PwmFreq = 1000000 / 500 / 1000;
 
#if 1

/*
function:   pwm�����޸�
param   :   u8 mode  0����   1����
*/
void PWM_PeriodModify(u8 mode)
{
	u16 temp = PwmPeriod;
	u16 DutyRatio;
	switch(mode)
	{
		case 0:
			PwmPeriod += 10;
			break;
		case 1:
			PwmPeriod -= 10;
			break;
	}
	DutyRatio = PwmDutyRatio * PwmPeriod / temp; // ����ռ�ձȲ���
	TIM4_PWM_Init(PwmPeriod - 1, 84-1);
	TIM_SetCompare1(TIM4, DutyRatio);
	TIM_SetCompare2(TIM4, DutyRatio);
	LCD_ShowNum(101, 70, PwmPeriod, 5, 16);    // ��ʾ����
	PwmFreq = 1000 / PwmPeriod;     
	LCD_ShowNum(111, 110, PwmFreq, 4, 16);   // ��ʾƵ��  kHz
}

/*
function:  pwmռ�ձ��޸�
param   :  u8 mode  0����   1����
*/
void PWM_DutyRatioModify(u8 mode)
{
	u16 temp = PwmPeriod * 0.01;
	switch(mode)
	{
		case 0:
			PwmDutyRatio +=  temp;		
			break;
		case 1:
			PwmDutyRatio -=  temp;
			break;
	}
	TIM_SetCompare1(TIM4, PwmDutyRatio);
	TIM_SetCompare2(TIM4, PwmDutyRatio);
	LCD_ShowNum(151, 90,  PwmDutyRatio * 100 / PwmPeriod, 3, 16);   // ��ʾռ�ձ�
}




int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2s
	delay_init(168);  //��ʼ����ʱ����
	uart_init(115200);		//��ʼ�����ڲ�����Ϊ115200	
	LCD_Init();					//LCD��ʼ�� 
	tp_dev.init();				//��������ʼ��
//	LCD_Clear(WHITE);
	POINT_COLOR = RED;            //��������Ϊ��ɫ
	LCD_ShowString(30,  70, 72, 16, 16, "TIM4 arr:");	     // ����
	LCD_ShowString(30,  90, 120, 16, 16, "PWM Duty Ratio:"); // ռ�ձ�  
	LCD_ShowString(190, 90, 8, 16, 16, "%");                 //    
	LCD_ShowString(30, 110, 80, 16, 16, "PWM Freq:");        // Ƶ��
	LCD_ShowString(142,110, 24, 16, 16, "KHz");              //  
	
	POINT_COLOR = GREEN;            //��������Ϊ��ɫ
	LCD_ShowString(10, 142, 48, 16, 16, "Period");
	LCD_ShowString(70, 142, 24, 16, 16, "+10");
	LCD_DrawRectangle(60, 130, 110, 170);       // ���� ����ť
	LCD_ShowString(160, 142, 24, 16, 16, "-10");
	LCD_DrawRectangle(150, 130, 200, 170);      // ���� ����ť
	
	LCD_ShowString(22, 212, 24, 16, 16, "Duty");
	LCD_ShowString(70, 212, 24, 16, 16, "+1%");
	LCD_DrawRectangle(60, 200, 110, 240);       // ռ�ձ� ����ť
	LCD_ShowString(160, 212, 24, 16, 16, "-1%");
	LCD_DrawRectangle(150, 200, 200, 240);      // ռ�ձ� ����ť
	
    // 84M/84=1Mhz�ļ���Ƶ��,��װ��ֵ50������PWMƵ��Ϊ 1M/50=20Khz.     
	TIM4_PWM_Init(PwmPeriod-1, 84-1);
	TIM_SetCompare1(TIM4, PwmDutyRatio);
	TIM_SetCompare2(TIM4, PwmDutyRatio);
	
	POINT_COLOR = RED;
 	LCD_ShowNum(101, 70,  PwmPeriod, 5, 16);    // ��ʾ����
	LCD_ShowNum(151, 90,  PwmDutyRatio * 100 / PwmPeriod, 3, 16);   // ��ʾռ�ձ�
	LCD_ShowNum(111, 110, PwmFreq, 4, 16);   // ��ʾƵ��
	
	while(1) //ʵ�ֱȽ�ֵ��0-300��������300���300-0�ݼ���ѭ��
	{
		if(tp_dev.scan(0))
		{
			delay_ms(200);
			if(tp_dev.x[0] > 60 && tp_dev.x[0] < 110 && tp_dev.y[0] > 130 && tp_dev.y[0] < 170)  // ���� ����ť
			{
				PWM_PeriodModify(0);
	//			LCD_ShowNum(100, 70,  1000, 5, 16);    // ��ʾ����
			}
			else if(tp_dev.x[0] > 150 && tp_dev.x[0] < 200 && tp_dev.y[0] > 130 && tp_dev.y[0] < 170) // ���� ����ť
			{
				PWM_PeriodModify(1);
			}
			else if(tp_dev.x[0] > 60 && tp_dev.y[0] > 200 && tp_dev.x[0] < 110 && tp_dev.y[0] < 240) // ռ�ձ� ����ť
			{
				PWM_DutyRatioModify(0);
			}
			else if(tp_dev.x[0] > 150 && tp_dev.y[0] > 200 && tp_dev.x[0] < 200 && tp_dev.y[0] < 240) // ռ�ձ� ����ť
			{
				PWM_DutyRatioModify(1);
			}
		}
	}
}
#endif

 
