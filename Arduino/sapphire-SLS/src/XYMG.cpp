//
//
//

#include "XYMG.h"

bool IsMoving_XY; // 是否正在移动_XY
bool NeedOpenLaser_bool_XYMG; // 是否需要打开激光器_bool_XYMG

int Times_Now; // 当前次数

int Times_LinearLnterpolation_int_XYMG; // 线性插值次数_int_XYMG
unsigned short* DAValMoveX_ushortPointer_XYMG; // 移动X轴的DA值_ushort指针_XYMG
unsigned short* DAValMoveY_ushortPointer_XYMG; // 移动Y轴的DA值_ushort指针_XYMG


void Init_XYMG()
{
	IsMoving_XY = false; // 设置移动状态为false
	pinMode(PIN_XY, OUTPUT); // 设置XY引脚为输出模式
	pinMode(PIN_LASER, OUTPUT); // 设置激光器引脚为输出模式
	digitalWrite(PIN_XY, HIGH); // 设置XY引脚为高电平
	SPI.begin(); // 开始SPI通信
	SPI.beginTransaction(SPISettings(SPEED_SPI, MSBFIRST, SPI_MODE1)); // 设置SPI通信参数
	XYMG_Write(32767 * 5 / BASE_VOLTAGE, 32767 * 5 / BASE_VOLTAGE); // 写入XY轴的初始DA值
	analogWrite(PIN_LASER, LASER_OFF_POWER); // 设置激光器为关闭状态
	Timer1.attachInterrupt(XYMG_Move); // 设置定时器5的中断函数为XYMG_Move

}


/*
DAC8552
http://www.ti.com/product/DAC8552
Vout = (Din/(2^n))*Vref
Din is straight binary
0 <= Din <= 65535
n = 16
=> Vout = (Din/65536)*Vref

|            COMMAND              |                      DATA							|
|D23 D22 D21 D20 D19 D18 D17 D16  |D15 D14 D13 D12 D11 D10 D9 D8 D7 D6 D5 D4 D3 D2 D1 D0|
|  0   0   1   1   0   0   0   0  |=>WR Buffer A w/Data and Load DACs A and B  (0x30)
|  0   0   1   1   0   1   0   0  |=>WR Buffer B w/Data and Load DACs A and B  (0x34)

*/

inline void XYMG_Write(unsigned short x, unsigned short y)//向DAC8552写入DAVal
{
	digitalWrite(PIN_XY, LOW); // 设置XY引脚为低电平
	SPI.transfer(0x30); // 发送0x30指令
	SPI.transfer16(x); // 发送X轴的DA值
	digitalWrite(PIN_XY, HIGH); // 设置XY引脚为高电平
	digitalWrite(PIN_XY, LOW); // 设置XY引脚为低电平
	SPI.transfer(0x34); // 发送0x34指令
	SPI.transfer16(y); // 发送Y轴的DA值
	digitalWrite(PIN_XY, HIGH); // 设置XY引脚为高电平
}

void XYMG_Move()
//根据数组中的DA数据移动
{
	MYSERIAL.println("XY is moving"); // 输出信息

	if (Times_Now != Times_LinearLnterpolation_int_XYMG) { // 如果当前次数不等于线性插值次数
		XYMG_Write(DAValMoveX_ushortPointer_XYMG[Times_Now], DAValMoveY_ushortPointer_XYMG[Times_Now]); // 写入XY轴的当前DA值
		Times_Now++; // 当前次数加一
	}
	else { // 否则
		Timer1.stop(); // 停止定时器5
		analogWrite(PIN_LASER, LASER_OFF_POWER); // 关闭激光器
		IsMoving_XY = false; // 设置移动状态为false
	}
}

void XYMG_StartMove()
//根据数组中的DA数据移动
{
	Times_Now = 0; // 设置当前次数为零
	IsMoving_XY = true; // 设置移动状态为true
	Timer1.initialize(1000000/CYCLE_LNTERPOLARION);
	Timer1.start(); // 设置定时器5的频率并启动
	if (NeedOpenLaser_bool_XYMG) { // 如果需要打开激光器
		analogWrite(PIN_LASER, LASER_ON_POWER); // 打开激光器
	}
}

