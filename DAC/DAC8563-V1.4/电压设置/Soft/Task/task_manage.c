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


u8 Firt_In = 1;
u8 Task_Index = 0;
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
	LCD_Show_CEStr(0,6,"ADF4351");
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

void Copybuf2dis(u8 *source, u8 dis[10], u8  dispoint)
{
	dis[0] = *source + 		 '0';
	dis[1] = *(source+1) + '0';
	dis[2] = *(source+2) + '0';
	dis[3] = *(source+3) + '0';
	dis[4] = *(source+4) + '0';
	dis[5] = 0;

	dis[dispoint] += 128;
}

void Copybuf2disbai(u8 *source, u8 dis[10])//无光标
{
		dis[0] = *source + 		 '0';
	dis[1] = *(source+1) + '0';
	dis[2] = *(source+2) + '0';
	dis[3] = *(source+3) + '0';
	dis[4] = *(source+4) + '0';
	dis[5] = 0;

}

void Set_PointFre(u32 Key_Value, u8* Task_ID)
{
	static u8 P_Index1 = 0,P_Index2 = 0, R_Index = 1,modAB=0;
	static u32 Fre1 = 65535,Fre2 = 65535;
	
	u8 fre_buf1[6],fre_buf2[6];
	u8 display[15];


	if(Firt_In) 
	{
		Key_Value = K_2_S;
		OLED_ShowString(38, 0, (u8*)"DAC8563");
	  OLED_ShowString(23, 2, (u8*)"OUTA");
		OLED_ShowString(83, 2, (u8*)"OUTB");
		LCD_Show_CEStr(0,6,"    康威科技    ");
		//LCD_Show_CEStr(0,6,"kvdz.taobao.com");
		Firt_In = 0;
		_return=1;		
	}
	fre_buf1[0] = Fre1%100000/10000;
	fre_buf1[1] = Fre1%10000/1000;
	fre_buf1[2] = Fre1%1000/100;
	fre_buf1[3] = Fre1%100/10;
	fre_buf1[4] = Fre1%10;
	
	fre_buf2[0] = Fre2%100000/10000;
	fre_buf2[1] = Fre2%10000/1000;
	fre_buf2[2] = Fre2%1000/100;
	fre_buf2[3] = Fre2%100/10;
	fre_buf2[4] = Fre2%10;
	
	switch(Key_Value)
	{
		case K_4_S: if(R_Index==0)
									fre_buf1[P_Index1]++;
								else if(R_Index==1)
									fre_buf2[P_Index2]++;break;
		case K_4_L: if(R_Index==0)
									fre_buf1[P_Index1]++;
								else if(R_Index==1)
									fre_buf2[P_Index2]++;break;	
								
		case K_5_L: if(R_Index==0)P_Index1++;
								else if(R_Index==1)P_Index2++;
								break;
		case K_5_S: if(R_Index==0)P_Index1++;
								else if(R_Index==1)P_Index2++;
								break;
		case K_1_L:if(R_Index==0)P_Index1--;
								else if(R_Index==1)P_Index2--;
								break;
		case K_1_S: if(R_Index==0)P_Index1--;
								else if(R_Index==1)P_Index2--;
								break;
								
		case K_3_S:  if(R_Index==0)
									fre_buf1[P_Index1]--;
								else if(R_Index==1)
									fre_buf2[P_Index2]--;break;	
		case K_3_L:  if(R_Index==0)
									fre_buf1[P_Index1]--;
								else if(R_Index==1)
									fre_buf2[P_Index2]--;break;	
		case K_2_L: modAB++;break;
		case K_2_S: R_Index^=1;break;
	}
	modAB %=3;
	P_Index1 %= 5;
	P_Index2 %= 5;
	if(R_Index==0)
	{
		if(fre_buf1[P_Index1] == 255) fre_buf1[P_Index1] = 9;
		if(fre_buf1[P_Index1] ==  10) fre_buf1[P_Index1] = 0;
	}
	else if(R_Index==1)
	{
		if(fre_buf2[P_Index2] == 255) fre_buf2[P_Index2] = 9;
		if(fre_buf2[P_Index2] ==  10) fre_buf2[P_Index2] = 0;
	}
	
	if(Key_Value != K_NO)
	{
		Fre1 = fre_buf1[0]*10000 + fre_buf1[1]*1000 + fre_buf1[2]*100+ fre_buf1[3]*10 + fre_buf1[4];
		Fre2 = fre_buf2[0]*10000 + fre_buf2[1]*1000 + fre_buf2[2]*100+ fre_buf2[3]*10 + fre_buf2[4];
		if(Fre1>65535) Fre1=65535;
		if(Fre2>65535) Fre2=65535;
		fre_buf1[0] = Fre1%100000/10000;
		fre_buf1[1] = Fre1%10000/1000;
		fre_buf1[2] = Fre1%1000/100;
		fre_buf1[3] = Fre1%100/10;
		fre_buf1[4] = Fre1%10;
	
		fre_buf2[0] = Fre2%100000/10000;
		fre_buf2[1] = Fre2%10000/1000;
		fre_buf2[2] = Fre2%1000/100;
		fre_buf2[3] = Fre2%100/10;
		fre_buf2[4] = Fre2%10;
		if(R_Index==0)
		{
			Copybuf2disbai(fre_buf2, display);
			OLED_ShowString(74, 4, display);
			Copybuf2dis(fre_buf1, display, P_Index1);
			OLED_ShowString(15, 4, display);
			//设置电压
			if(modAB==0)
			{
				DAC8563_WRITE(CMD_GAIN, DATA_GAIN_B2_A1);		// Set Gains A:+10 B:±10
				LCD_Show_CEStr(15,2,"+");
				LCD_Show_CEStr(66,2,"±");
			}			
			else if(modAB==1)
			{
				DAC8563_WRITE(CMD_GAIN, DATA_GAIN_B1_A1);		// Set Gains A:+10 B:+10
				LCD_Show_CEStr(15,2,"+");
				LCD_Show_CEStr(75,2,"+");
			}
			else if(modAB==2)
			{
				DAC8563_WRITE(CMD_GAIN, DATA_GAIN_B2_A2);		// Set Gains A:±10 B:±10
				LCD_Show_CEStr(6,2,"±");
				LCD_Show_CEStr(66,2,"±");
			}
				
			DAC_OutAB(Fre1,Fre2);
		}
		else if(R_Index==1)
		{
			Copybuf2disbai(fre_buf1, display);
			OLED_ShowString(15, 4, display);
			Copybuf2dis(fre_buf2, display, P_Index2);
			OLED_ShowString(74, 4, display);
			//设置电压
			if(modAB==0)
			{
				DAC8563_WRITE(CMD_GAIN, DATA_GAIN_B2_A1);		// Set Gains A:+10 B:±10
				LCD_Show_CEStr(15,2,"+");
				LCD_Show_CEStr(66,2,"±");
			}			
			else if(modAB==1)
			{
				DAC8563_WRITE(CMD_GAIN, DATA_GAIN_B1_A1);		// Set Gains A:+10 B:+10
				LCD_Show_CEStr(15,2,"+");
				LCD_Show_CEStr(75,2,"+");
			}
			else if(modAB==2)
			{
				DAC8563_WRITE(CMD_GAIN, DATA_GAIN_B2_A2);		// Set Gains A:±10 B:±10
				LCD_Show_CEStr(6,2,"±");
				LCD_Show_CEStr(66,2,"±");
			}
			
			DAC_OutAB(Fre1,Fre2);
		}
		
		_return=1;
	}
}


