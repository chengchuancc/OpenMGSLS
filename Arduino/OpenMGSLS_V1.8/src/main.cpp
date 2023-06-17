/*
 Name:		OpenMGSLS.ino
 Created:	2018/1/6 21:51:00
 Author:	QiayuanLiao
 Version:	1.8
*/
#include <Arduino.h>
#include <SPI.h>
#include "XYMG.h"
#include "Planner.h"
#include "Command.h"
#include "Stepper.h"
#include "Heating.h"
// the setup function runs once when you press reset or power the board
void setup() {
	Init_Command(); // 调用初始化命令函数，设置串口通信参数和缓冲区变量
	Init_XYMG();// 调用初始化XY轴运动控制函数，设置定时器中断和引脚模式
	Init_Stepper();// 调用初始化步进电机控制函数，设置引脚模式和初始位置
//	Init_Heating();
}
// the loop function runs over and over again until power down or reset
void loop() {
	if (GCodeBuffLen_int_Command < (GCODE_BUFF_SIZE - 1)) {// 如果命令缓冲区长度小于最大值减一，说明还有空间存储新的命令
		Get_Command(); // 调用获取命令函数，从串口读取一行G代码并存入缓冲区
	}
	if (GCodeBuffLen_int_Command && (DAValBufferLen_int_Planner < (DA_VAL_BUFF_SIZE - 1))) {// 如果命令缓冲区长度大于零，说明有待处理的命令；如果DA值缓冲区长度小于最大值减一，说明还有空间存储新的DA值
		Process_Command();// 调用处理命令函数，从缓冲区取出一条G代码并解析其参数和功能
		if (NeedMove_boolArry_Command[0]) { // 如果需要移动标志数组的第一个元素为真，说明需要进行XY轴的移动
			NeedMove_boolArry_Command[0] = false;// 将需要移动标志数组的第一个元素置为假，表示已经处理过该标志
			NeedOpenLaser_boolArry_Planner[DAValBufferPlanCouter_int_Planner] = NeedMove_boolArry_Command[1];// 将需要移动标志数组的第二个元素赋值给需要打开激光标志数组的当前计数器位置，表示是否需要在移动过程中打开激光
			NeedMove_boolArry_Command[1] = false;// 将需要移动标志数组的第二个元素置为假，表示已经处理过该标志
			PlaneXY_Planner();// 调用XY轴平面插补函数，根据G代码参数计算出DA值并存入缓冲区
		}
		else if (NeedMove_boolArry_Command[2]) { // 如果需要移动标志数组的第三个元素为真，说明需要进行Z轴的移动
			NeedMove_boolArry_Command[2] = false;
			if (abs(GCodeVal_floatArry_Command[2] - Z_Position_Old) < 0.1)// 如果G代码参数数组的第三个元素（Z轴目标位置）和Z轴旧位置的差值的绝对值小于0.1，说明移动距离很小
				StepperB_StartMove(0.2);// 调用步进电机B开始移动函数，让步进电机B移动0.2个单位
			else
				StepperB_StartMove((GCodeVal_floatArry_Command[2] - Z_Position_Old)*1.5);// 调用步进电机B开始移动函数，让步进电机B移动G代码参数数组的第三个元素和Z轴旧位置的差值乘以1.5个单位
			while (IsMoving_B)// 当步进电机B还在运动中时
				delay(1);

			StepperC_StartMove(-(GCodeVal_floatArry_Command[2] - Z_Position_Old));// 调用步进电机C开始移动函数，让步进电机C移动G代码参数数组的第三个元素和Z轴旧位置的差值的相反数个单位
			while (IsMoving_C)
				delay(1);

			StepperA_StartMove(Stepper_A_DISTANCE_MAX);// 调用步进电机A开始移动函数，让步进电机A移动到最大距离
			while (IsMoving_A)
				delay(1);

			StepperA_StartMove(-Stepper_A_DISTANCE_MAX);// 调用步进电机A开始移动函数，让步进电机A移动到最小距离
			while (IsMoving_A)
				delay(1);

			Z_Position_Old = GCodeVal_floatArry_Command[2];// 将Z轴旧位置更新为G代码参数数组的第三个元素（Z轴目标位置）
		}
		else if (NeedMove_boolArry_Command[3]) {// 如果需要移动标志数组的第四个元素为真，说明需要进行A轴的移动
			NeedMove_boolArry_Command[3] = false;

			StepperA_StartMove(Stepper_A_DISTANCE_MAX);
			while (IsMoving_A) // 调用步进电机A开始移动函数，让步进电机A移动到最大距离
				delay(1);
			StepperA_StartMove(-Stepper_A_DISTANCE_MAX);
			while (IsMoving_A)
				delay(1);
		}
		else if (NeedMove_boolArry_Command[4]) {
			NeedMove_boolArry_Command[4] = false;
			StepperB_StartMove(GCodeVal_floatArry_Command[4]);
			while (IsMoving_B)
				delay(1);
		}
		else if (NeedMove_boolArry_Command[5]) {
			NeedMove_boolArry_Command[5] = false;
			StepperC_StartMove(GCodeVal_floatArry_Command[5]);
			while (IsMoving_C)
				delay(1);
		}

	}
	if (DAValBufferLen_int_Planner && !IsMoving_XY) { // 如果DA值缓冲区长度大于零，说明有待执行的DA值；如果XY轴没有在运动中，说明可以开始新的移动
		DAValMoveX_ushortPointer_XYMG = DAValBufferX_ushortPointer_Planner[DAValBufferMoveCouter_int_Planner]; // 将DA值缓冲区X轴的当前计数器位置的指针赋值给XY轴运动控制的X轴移动指针，表示要移动的X轴DA值
		DAValMoveY_ushortPointer_XYMG = DAValBufferY_ushortPointer_Planner[DAValBufferMoveCouter_int_Planner]; // 将DA值缓冲区Y轴的当前计数器位置的指针赋值给XY轴运动控制的Y轴移动指针，表示要移动的Y轴DA值
		NeedOpenLaser_bool_XYMG = NeedOpenLaser_boolArry_Planner[DAValBufferMoveCouter_int_Planner]; // 将需要打开激光标志数组的当前计数器位置的值赋值给XY轴运动控制的需要打开激光标志，表示是否需要在移动过程中打开激光
		Times_LinearLnterpolation_int_XYMG = Times_LinearLnterpolation_intArry_Planner[DAValBufferMoveCouter_int_Planner]; // 将线性插补次数数组的当前计数器位置的值赋值给XY轴运动控制的线性插补次数，表示要进行多少次线性插补
		XYMG_StartMove(); // 调用XY轴开始移动函数，根据上述参数控制XY轴的运动
		DAValBufferMoveCouter_int_Planner = (DAValBufferMoveCouter_int_Planner + 1) % DA_VAL_BUFF_SIZE; // 将DA值缓冲区的移动计数器加一并取余，表示已经执行了一个DA值
		DAValBufferLen_int_Planner--; // 将DA值缓冲区长度减一，表示已经执行了一个DA值
	}
}
