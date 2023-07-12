//
//
//

#include "Planner.h"
//声明一个布尔数组，用来存储每个缓冲区是否需要开启激光
bool NeedOpenLaser_boolArry_Planner[DA_VAL_BUFF_SIZE];

//声明一个整数数组，用来存储每个缓冲区的线性插补次数
int Times_LinearLnterpolation_intArry_Planner[DA_VAL_BUFF_SIZE];
//声明一个整数变量，用来存储当前的线性插补次数
int T_LinearLnterpolation_int;
//声明一个整数变量，用来存储缓冲区的计划指针
int DAValBufferPlanCouter_int_Planner;
//声明一个整数变量，用来存储缓冲区的移动指针
int DAValBufferMoveCouter_int_Planner;
//声明一个整数变量，用来存储缓冲区的长度
int DAValBufferLen_int_Planner;

//声明一个无符号短整数指针数组，用来存储计划好的X轴的DA值
unsigned short DAValBufferX_ushortPointer_Planner[DA_VAL_BUFF_SIZE][DA_VAL_ARRY_SIZE];
//声明一个无符号短整数指针数组，用来存储计划好的Y轴的DA值
unsigned short DAValBufferY_ushortPointer_Planner[DA_VAL_BUFF_SIZE][DA_VAL_ARRY_SIZE];

//声明四个浮点数变量，用来存储目标点和起始点的X和Y坐标
float Goal_X_float, Goal_Y_float, Start_X_float, Start_Y_float;
//声明两个浮点数变量，用来存储目标点和起始点的X和Y方向的距离
float Dis_X_float, Dis_Y_float;
//声明两个浮点数变量，用来存储每次线性插补的X和Y方向的增量
float Plus_X_float, Plus_Y_float;
//声明两个浮点数变量，用来存储当前的X和Y方向的位置
float Plane_X_float, Plane_Y_float;
//声明两个浮点数变量，用来存储X和Y方向的弧度值
float RadVal_X_float, RadVal_Y_float;


//定义一个函数，用于规划XY平面的运动数据，并转换为DA值
//并将XY数组的地址存放在两个指针中
void PlaneXY_Planner()
{
    //从GCodeVal_floatArry_Command数组中获取目标X和Y坐标的值，赋值给Goal_X_float和Goal_Y_float变量
	Goal_X_float = GCodeVal_floatArry_Command[0];
	Goal_Y_float = GCodeVal_floatArry_Command[1];

	//计算X和Y方向的位移，赋值给Dis_X_float和Dis_Y_float变量
	Dis_X_float = Goal_X_float - Start_X_float;
	Dis_Y_float = Goal_Y_float - Start_Y_float;
	//根据位移和速度计算需要的插补次数，赋值给Times_LinearLnterpolation_intArry_Planner数组
	Times_LinearLnterpolation_intArry_Planner[DAValBufferPlanCouter_int_Planner] = sqrt(Dis_X_float*Dis_X_float + Dis_Y_float*Dis_Y_float) / (float)SPEED_XY*(float)CYCLE_LNTERPOLARION;
	//如果插补次数不为零，说明有运动
	if (Times_LinearLnterpolation_intArry_Planner[DAValBufferPlanCouter_int_Planner] != 0) { //计算所有的线性插补
	    //如果X方向有位移，计算每次插补的增量，赋值给Plus_X_float变量
		if (Dis_X_float > 0.00001 || Dis_X_float < 0.00001)
			Plus_X_float = (Dis_X_float) / Times_LinearLnterpolation_intArry_Planner[DAValBufferPlanCouter_int_Planner];
		//如果X方向没有位移，将增量设为零
		else
			Plus_X_float = 0;

		//如果Y方向有位移，计算每次插补的增量，赋值给Plus_Y_float变量
		if (Dis_Y_float > 0.00001 || Dis_Y_float < 0.00001)
			Plus_Y_float = (Dis_Y_float) / Times_LinearLnterpolation_intArry_Planner[DAValBufferPlanCouter_int_Planner];
		//如果Y方向没有位移，将增量设为零
		else
			Plus_Y_float = 0;

        //将起始坐标赋值给Plane_X_float和Plane_Y_float变量
		Plane_X_float = Start_X_float;
		Plane_Y_float = Start_Y_float;

        //循环插补次数的次数
		while (T_LinearLnterpolation_int < Times_LinearLnterpolation_intArry_Planner[DAValBufferPlanCouter_int_Planner])
		{
		    //更新插补后的坐标，赋值给Plane_X_float和Plane_Y_float变量
			Plane_X_float = Plane_X_float + Plus_X_float;
			Plane_Y_float = Plane_Y_float + Plus_Y_float;

            //调用DisToDA_X函数，将坐标转换为DA值，并存放在DAValBufferX_ushortPointer_Planner指针指向的数组中
			DAValBufferX_ushortPointer_Planner[DAValBufferPlanCouter_int_Planner][T_LinearLnterpolation_int] = DisToDA_X(Plane_X_float, Plane_Y_float);
			//调用DisToDA_Y函数，将坐标转换为DA值，并存放在DAValBufferY_ushortPointer_Planner指针指向的数组中
			DAValBufferY_ushortPointer_Planner[DAValBufferPlanCouter_int_Planner][T_LinearLnterpolation_int] = DisToDA_Y(Plane_Y_float);

            //更新插补计数器
			T_LinearLnterpolation_int++;

		}
        //重置插补计数器
		T_LinearLnterpolation_int = 0;
        //更新起始坐标为目标坐标
		Start_X_float = Goal_X_float;
		Start_Y_float = Goal_Y_float;
	}
	//如果插补次数为零，说明位移很小
	else {
	    //将插补次数设为1
		Times_LinearLnterpolation_intArry_Planner[DAValBufferPlanCouter_int_Planner] = 1;
		//调用DisToDA_X函数，将目标坐标转换为DA值，并存放在DAValBufferX_ushortPointer_Planner指针指向的数组中
		DAValBufferX_ushortPointer_Planner[DAValBufferPlanCouter_int_Planner][0] = { DisToDA_X(Goal_X_float, Goal_Y_float) };
		//调用DisToDA_Y函数，将目标坐标转换为DA值，并存放在DAValBufferY_ushortPointer_Planner指针指向的数组中
		DAValBufferY_ushortPointer_Planner[DAValBufferPlanCouter_int_Planner][0] = { DisToDA_Y(Goal_Y_float) };
        //更新起始坐标为目标坐标
		Start_X_float = Goal_X_float;
		Start_Y_float = Goal_Y_float;
	}

    //更新规划缓冲区的计数器和长度
	DAValBufferPlanCouter_int_Planner = (DAValBufferPlanCouter_int_Planner + 1) % DA_VAL_BUFF_SIZE;
	DAValBufferLen_int_Planner++;

}

//定义一个函数，参数是两个浮点数，表示X和Y坐标，返回值是一个无符号短整数，表示X方向的DA值
unsigned short DisToDA_X(float x, float y) {
    //根据X和Y坐标计算X方向的旋转角度，赋值给RadVal_X_float变量
	RadVal_X_float = atan(x / (sqrt(DIS_F_THETA*DIS_F_THETA + y*y) + DIS_XYMOTOR));
	//根据Y坐标计算Y方向的旋转角度，赋值给RadVal_Y_float变量
	RadVal_Y_float = atan(y / DIS_F_THETA);
    //根据旋转角度和最大角度计算DA值，并返回
	return (atan((x - (DIS_F_THETA*tan(RadVal_X_float)*(1 / cos(RadVal_Y_float) - 1))) / (sqrt(DIS_F_THETA*DIS_F_THETA + y*y) + DIS_XYMOTOR)) / RAD_MG_MAX * 32768 + 32767)* 5/BASE_VOLTAGE;
}

//定义一个函数，参数是一个浮点数，表示Y坐标，返回值是一个无符号短整数，表示Y方向的DA值
unsigned short DisToDA_Y(float y) {
    //根据旋转角度和最大角度计算DA值，并返回
	return  (RadVal_Y_float / RAD_MG_MAX * 32768 + 32767)*5 / BASE_VOLTAGE;
}
