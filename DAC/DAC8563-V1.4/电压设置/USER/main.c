/**********************************************************
                       �����Ƽ�
������������뵽�Ա��꣬�������ӽ߳�Ϊ������ ^_^
https://kvdz.taobao.com/
**********************************************************/


#include "stm32_config.h"
#include "stdio.h"
#include "led.h"
#include "DAC8563.h"
#include "key.h"
#include "timer.h"
#include "task_manage.h"

char str[30];	//��ʾ����
extern u8 _return;
uint16_t dacValue = 0;
int main(void)
{

	MY_NVIC_PriorityGroup_Config(NVIC_PriorityGroup_2);	//�����жϷ���
	delay_init(72);	//��ʼ����ʱ����
	LED_Init();	//��ʼ��LED�ӿ�
	key_init();
	initial_lcd();
	LCD_Clear();
	DAC8563_Init();
	delay_ms(100);
	LCD_Refresh_Gram();
	delay_ms(50);
  DAC_OutAB(65535,65535);
	delay_ms(100);
	while(1)
	{
		KeyRead();
		Set_PointFre(Keycode, 0);
		if(_return){_return=0;LCD_Refresh_Gram();}
		KEY_EXIT();

	}
}

