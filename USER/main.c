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
function:   pwm周期修改
param   :   u8 mode  0增加   1减少
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
	DutyRatio = PwmDutyRatio * PwmPeriod / temp; // 保持占空比不变
	TIM4_PWM_Init(PwmPeriod - 1, 84-1);
	TIM_SetCompare1(TIM4, DutyRatio);
	TIM_SetCompare2(TIM4, DutyRatio);
	LCD_ShowNum(101, 70, PwmPeriod, 5, 16);    // 显示周期
	PwmFreq = 1000 / PwmPeriod;     
	LCD_ShowNum(111, 110, PwmFreq, 4, 16);   // 显示频率  kHz
}

/*
function:  pwm占空比修改
param   :  u8 mode  0增加   1减少
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
	LCD_ShowNum(151, 90,  PwmDutyRatio * 100 / PwmPeriod, 3, 16);   // 显示占空比
}




int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2s
	delay_init(168);  //初始化延时函数
	uart_init(115200);		//初始化串口波特率为115200	
	LCD_Init();					//LCD初始化 
	tp_dev.init();				//触摸屏初始化
//	LCD_Clear(WHITE);
	POINT_COLOR = RED;            //设置字体为红色
	LCD_ShowString(30,  70, 72, 16, 16, "TIM4 arr:");	     // 周期
	LCD_ShowString(30,  90, 120, 16, 16, "PWM Duty Ratio:"); // 占空比  
	LCD_ShowString(190, 90, 8, 16, 16, "%");                 //    
	LCD_ShowString(30, 110, 80, 16, 16, "PWM Freq:");        // 频率
	LCD_ShowString(142,110, 24, 16, 16, "KHz");              //  
	
	POINT_COLOR = GREEN;            //设置字体为绿色
	LCD_ShowString(10, 142, 48, 16, 16, "Period");
	LCD_ShowString(70, 142, 24, 16, 16, "+10");
	LCD_DrawRectangle(60, 130, 110, 170);       // 周期 增按钮
	LCD_ShowString(160, 142, 24, 16, 16, "-10");
	LCD_DrawRectangle(150, 130, 200, 170);      // 周期 减按钮
	
	LCD_ShowString(22, 212, 24, 16, 16, "Duty");
	LCD_ShowString(70, 212, 24, 16, 16, "+1%");
	LCD_DrawRectangle(60, 200, 110, 240);       // 占空比 增按钮
	LCD_ShowString(160, 212, 24, 16, 16, "-1%");
	LCD_DrawRectangle(150, 200, 200, 240);      // 占空比 减按钮
	
    // 84M/84=1Mhz的计数频率,重装载值50，所以PWM频率为 1M/50=20Khz.     
	TIM4_PWM_Init(PwmPeriod-1, 84-1);
	TIM_SetCompare1(TIM4, PwmDutyRatio);
	TIM_SetCompare2(TIM4, PwmDutyRatio);
	
	POINT_COLOR = RED;
 	LCD_ShowNum(101, 70,  PwmPeriod, 5, 16);    // 显示周期
	LCD_ShowNum(151, 90,  PwmDutyRatio * 100 / PwmPeriod, 3, 16);   // 显示占空比
	LCD_ShowNum(111, 110, PwmFreq, 4, 16);   // 显示频率
	
	while(1) //实现比较值从0-300递增，到300后从300-0递减，循环
	{
		if(tp_dev.scan(0))
		{
			delay_ms(200);
			if(tp_dev.x[0] > 60 && tp_dev.x[0] < 110 && tp_dev.y[0] > 130 && tp_dev.y[0] < 170)  // 周期 增按钮
			{
				PWM_PeriodModify(0);
	//			LCD_ShowNum(100, 70,  1000, 5, 16);    // 显示周期
			}
			else if(tp_dev.x[0] > 150 && tp_dev.x[0] < 200 && tp_dev.y[0] > 130 && tp_dev.y[0] < 170) // 周期 减按钮
			{
				PWM_PeriodModify(1);
			}
			else if(tp_dev.x[0] > 60 && tp_dev.y[0] > 200 && tp_dev.x[0] < 110 && tp_dev.y[0] < 240) // 占空比 增按钮
			{
				PWM_DutyRatioModify(0);
			}
			else if(tp_dev.x[0] > 150 && tp_dev.y[0] > 200 && tp_dev.x[0] < 200 && tp_dev.y[0] < 240) // 占空比 减按钮
			{
				PWM_DutyRatioModify(1);
			}
		}
	}
}
#endif

 
