/**********************************************************
                       康威科技
更多电子需求，请到淘宝店，康威电子竭诚为您服务 ^_^
https://kvdz.taobao.com/
**********************************************************/


#include "stm32_config.h"
#include "stdio.h"
#include "led.h"
#include "DAC8563.h"
#include "key.h"
#include "timer.h"
#include "task_manage.h"

char str[30];	//显示缓存
extern u8 _return;
uint16_t dacValue = 0;
int main(void)
{

	MY_NVIC_PriorityGroup_Config(NVIC_PriorityGroup_2);	//设置中断分组
	delay_init(72);	//初始化延时函数
	LED_Init();	//初始化LED接口
	key_init();
	initial_lcd();
	LCD_Clear();
	DAC8563_Init();
	delay_ms(100);
	OLED_ShowString(20, 3, (u8*)"DAC8563");
	OLED_ShowString(0, 5, (u8*)"Triangle TEST!");
	LCD_Refresh_Gram();
	delay_ms(50);
  DAC_OutAB(65535,65535);
	delay_ms(100);
	while(1)
	{
		DAC_OutAB(TriangleWave_Value[dacValue],TriangleWave_Value[dacValue]);
		dacValue++;
		if(dacValue >= 255) dacValue = 0;

	}
}

