/**********************************************************
                       康威科技
更多电子需求，请到淘宝店，康威电子竭诚为您服务 ^_^
https://kvdz.taobao.com/
**********************************************************/


#include "task_manage.h"
#include "delay.h"
#include "DAC8563.h"
#include "key.h"
#include <stdio.h>
#include <ctype.h>
#include <cstring>

#define OUT_KEY  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)//读取按键0
#define FLASH_SAVE_ADDR  0x0801F000  				//设置FLASH 保存地址(必须为偶数)


u8 Task_Index = 0;//界面切换
u8 Param_Mode = 0;//参数类型切换
u8 fre_buf[StrMax]; //参数转换字符缓存
u8 display[StrMax]; //参数显示缓存
u8 P_Index = 0;
u8 Task_First=1;//第一次进入标记
u8 _return=0;


u8 Task_Delay(u32 delay_time, u8* delay_ID)
{
	static u8 Time_Get = 0;
	static u32 Time_Triger;
    if(Time_Get == 0)
    {
      Time_Triger = SysTimer + delay_time;
      Time_Get = 1;
    }
    if(SysTimer >= Time_Triger)
    { 
      Time_Get = 0;
			*delay_ID = 1;		//	后续代码已被执行一遍
			return 1;
    }
		return 0;
}

//u8 TaskOneToIdel(void)
//{
//	static u8 delay_ID0 = 0;
//	static u8 delay_ID1 = 0;
//	static u8 delay_ID2 = 0;
//	static u8 delay_ID3 = 0;
//	u8 delay_arrive0 = 0;
//	u8 delay_arrive1 = 0;
//	u8 delay_arrive2 = 0;
//	u8 delay_arrive3 = 0;
//	
//	delay_arrive0 = Task_Delay(100, &delay_ID0);
//	delay_arrive1 = Task_Delay(100, &delay_ID1);
//	delay_arrive2 = Task_Delay(100, &delay_ID2);
//	delay_arrive3 = Task_Delay(100, &delay_ID3);
//	if((delay_arrive0 == 0) && (delay_ID0 == 0))
//		return 0;
//	else if((delay_arrive0) && (delay_ID0))
//	{
//			//执行本延时后代码
//	}
//		
//	
//	
//	if(delay_ID0&&delay_ID1&&delay_ID2&&delay_ID3)
//	{
//		delay_ID0 = 0;
//		delay_ID1 = 0;
//		delay_ID2 = 0;
//		delay_ID3 = 0;
//	}
//}

u8 TaskCycleDelay(u32 delay_time, u8* Last_delay_ID, u8* Self_delay_ID)
{
	static u8 Time_Get = 0;
	static u32 Time_Triger;
	
	if(!(*Last_delay_ID))
		return 0;
	if(Time_Get == 0)
	{
		Time_Triger = SysTimer + delay_time;
		Time_Get = 1;
	}
	if(SysTimer >= Time_Triger)
	{ 
		Time_Get = 0;
		*Last_delay_ID = 0;
		*Self_delay_ID = 1;		//	后续代码已被执行一遍
		return 1;
	}
	return 0;
}
void welcome_KW(void)
{
	u8 delay_ID[11] = {0,0,0,0,0,0,0,0,0,0};
	u8 delay_ID_Welcome[3] = {0,0,0};
	u16 Wel_time0 = 50,Wel_time1 = 100;
	delay_ID[10] = 1;
	delay_ID_Welcome[2] = 1;
	LCD_Show_CEStr(0,6,"DAC5863");
	LCD_Show_CEStr(56,6,"初始化");
	while(1)
	{
	if(TaskCycleDelay(Wel_time0, &delay_ID[10], &delay_ID[0]))
		LCD_Show_CEStr(32,0,"康");
	if(TaskCycleDelay(Wel_time0, &delay_ID[0], &delay_ID[1]))
		LCD_Show_CEStr(48,0,"威");
	if(TaskCycleDelay(Wel_time0, &delay_ID[1], &delay_ID[2]))
		LCD_Show_CEStr(64,0,"电");
	if(TaskCycleDelay(Wel_time0, &delay_ID[2], &delay_ID[3]))
		LCD_Show_CEStr(80,0,"子");
	if(TaskCycleDelay(Wel_time0, &delay_ID[3], &delay_ID[4]))
		LCD_Show_CEStr(16,2,"竭");
	if(TaskCycleDelay(Wel_time0, &delay_ID[4], &delay_ID[5]))
		LCD_Show_CEStr(32,2,"诚");
	if(TaskCycleDelay(Wel_time0, &delay_ID[5], &delay_ID[6]))
		LCD_Show_CEStr(48,2,"为");
	if(TaskCycleDelay(Wel_time0, &delay_ID[6], &delay_ID[7]))
		LCD_Show_CEStr(64,2,"您");
	if(TaskCycleDelay(Wel_time0, &delay_ID[7], &delay_ID[8]))
		LCD_Show_CEStr(80,2,"服");
	if(TaskCycleDelay(Wel_time0, &delay_ID[8], &delay_ID[9]))
		LCD_Show_CEStr(96,2,"务");
	if(TaskCycleDelay(Wel_time0, &delay_ID[9], &delay_ID[0]))
		break;
	if(TaskCycleDelay(Wel_time1, &delay_ID_Welcome[2], &delay_ID_Welcome[0]))
		LCD_Show_CEStr(104,6,".  ");
	if(TaskCycleDelay(Wel_time1, &delay_ID_Welcome[0], &delay_ID_Welcome[1]))
		LCD_Show_CEStr(112,6,".");
	if(TaskCycleDelay(Wel_time1, &delay_ID_Welcome[1], &delay_ID_Welcome[2]))
		LCD_Show_CEStr(120,6,".");
		LCD_Refresh_Gram();
	}
}
//把数据放到另一个缓存，显示，FloatNum小数点位数，CursorEn光标使能
void Copybuf2dis(u8 *source, u8 dis[StrMax], u8 dispoint, u8 FloatNum, u8 CursorEn)
{
	int i, len;
	
	len = strlen(source);
	i = len - FloatNum;//整数个数
	if(FloatNum>0)dis[i] = '.';
	for (i = 0; i < len; i++)
	{
		if(i < (len-FloatNum)) dis[i] = source[i];
		else 
		{ dis[i+1] = source[i]; }
	}
	
	if(CursorEn)
	{
		if(dispoint < (len-FloatNum)) dis[dispoint] += 128;
		else dis[dispoint+1] += 128;	
	}
}
//
void Set_PointFre(u32 Key_Value, u8* Task_ID)
{
	if(Task_First) 
	{
		Key_Value = K_2_L;
		LCD_Show_CEStr(20,0,"  DAC8563   ");
		LCD_Show_CEStr(0,6,"↑          ↓");
		Task_First = 0;
		_return=1;		
	}
	//按键判断
	switch(Key_Value)
	{
		case K_4_S: P_Index++;break;
		case K_4_L: P_Index++;break;
		case K_3_S: P_Index--;break;
		case K_3_L: P_Index--;break;
		default:break;
	}

	if(P_Index ==255) P_Index=1;
	if(P_Index >= 2) P_Index=0;
	
	
	if(P_Index==0)
	{
		//LCD_Show_CEStr(30,2,"正弦波");
		DAC_OutAB(65535,65535);
	}		
	else if(P_Index==1) 
	{
		//LCD_Show_CEStr(30,2,"三角波");
		DAC_OutAB(0,0);
	}
	
	
	
	_return=1;
	
}


