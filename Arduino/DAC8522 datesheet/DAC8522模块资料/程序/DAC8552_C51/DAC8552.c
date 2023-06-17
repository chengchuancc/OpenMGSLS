/******************************************************************************
				  DAC8552ģ��

		���ܣ�DAC8552˫·�����ѹ

		Ӳ��ԭ��ͼ��
              P0.0    -->   DAC8552_DIN
              P0.1    -->   DAC8552_SCLK
              P0.2    -->   DAC8552_SYNC

		Time��2012��10��1
		By��������ӹ�����
		shop��mcu-dream.taobao.com

******************************************************************************/

#include <reg51.h>

#define uchar unsigned char
#define uint unsigned int

sbit DAC8552_DIN  = P0^0;			/***********************************��Ҫ�޸�*******************************************/
sbit DAC8552_SCLK = P0^1;		 	/***********************************��Ҫ�޸�*******************************************/
sbit DAC8552_SYNC = P0^2;		  	/***********************************��Ҫ�޸�*******************************************/

void Write_DAC8552(uchar Command, uint Data);
uint Voltage_Convert(float Vref, float voltage);

void main(void)
{
	Write_DAC8552(0x30, Voltage_Convert(5.0,2));    	//Write channel A buffer (0x30)
    Write_DAC8552(0x34, Voltage_Convert(5.0,3.5));    	//Write channel B buffer (0x34)	
}

///*******************************************
//		�������ƣ�Write_DAC8552
//		��    �ܣ�ѡ��ͨ�����趨�����ѹ
//		��    ����Command -> ͨ��A/B��0x30=A��0x34=B   
//          	  Data -> �����ѹ
//		����ֵ  ����
//********************************************/
void Write_DAC8552(uchar Command, uint Data)
{
	uchar i;

	DAC8552_SYNC = 1;
	DAC8552_SYNC = 0;

	for (i=0; i<8; i++)              //Write 8 bit command 
	{
		DAC8552_SCLK = 1;
		if (Command & 0x80)
		{
			DAC8552_DIN = 1;
		}
		else
		{
			DAC8552_DIN = 0;
		}
		DAC8552_SCLK = 0;
		Command <<= 1;
	}

	for (i=0; i<16; i++)             //write 16 bit data
	{
		DAC8552_SCLK = 1;
		if (Data & 0x8000)
		{
			DAC8552_DIN = 1;
		}
		else
		{
			DAC8552_DIN = 0;
		}
		DAC8552_SCLK = 0;
		Data <<= 1;
	}
	DAC8552_SYNC = 1;
}

///*******************************************
//		�������ƣ�Voltage_Convert
//		��    �ܣ�����ο���ѹ����Ҫ����ĵ�ѹ���õ�DA��ֵ
//		��    ����Vref -> �ο���ѹ   
//          	  voltage -> ��Ҫ����ĵ�ѹ
//		����ֵ  ��DA��ֵ
//********************************************/
uint Voltage_Convert(float Vref, float voltage)
{
	uint _D_;
	_D_ = (uint)(65536 * voltage / Vref);
    
	return _D_;
}