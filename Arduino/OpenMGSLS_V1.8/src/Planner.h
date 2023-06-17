// Planner.h
/*DA值是用于控制XY轴的运动控制器的参数，它表示了XY轴的角度信息。
根据G代码参数计算出DA值，并存入缓冲区，然后通过SPI通信将DA值发送给运动控制器，从而实现XY轴的精确移动。*/

#include <Arduino.h> // 引入Arduino核心库，包含了基本的输入输出函数
#include "config.h" // 引入配置文件，包含了一些常量和变量的定义
#include "Command.h" // 引入Command头文件，包含了命令解析和处理函数

extern int DAValBufferPlanCouter_int_Planner; // 声明一个外部变量，用于记录DA值缓冲区的规划计数器，表示已经规划了多少个DA值
extern int DAValBufferMoveCouter_int_Planner; // 声明一个外部变量，用于记录DA值缓冲区的移动计数器，表示已经执行了多少个DA值
extern int DAValBufferLen_int_Planner; // 声明一个外部变量，用于记录DA值缓冲区的长度，表示还有多少个DA值待执行

extern bool NeedOpenLaser_boolArry_Planner[DA_VAL_BUFF_SIZE]; // 声明一个外部数组，用于存储每个DA值是否需要打开激光的标志
extern int Times_LinearLnterpolation_intArry_Planner[DA_VAL_BUFF_SIZE]; // 声明一个外部数组，用于存储每个DA值需要进行多少次线性插补的次数
extern unsigned short DAValBufferX_ushortPointer_Planner[DA_VAL_BUFF_SIZE][DA_VAL_ARRY_SIZE]; // 声明一个外部二维数组，用于存储每个DA值对应的X轴的DA值数组的指针
extern unsigned short DAValBufferY_ushortPointer_Planner[DA_VAL_BUFF_SIZE][DA_VAL_ARRY_SIZE]; // 声明一个外部二维数组，用于存储每个DA值对应的Y轴的DA值数组的指针

void PlaneXY_Planner(); // 声明一个函数，用于进行XY轴平面插补，根据G代码参数计算出DA值并存入缓冲区
unsigned short DisToDA_X(float x, float y); // 声明一个函数，用于将X轴的距离转换为DA值，根据X轴和Y轴的距离计算出X轴的角度并转换为对应的数字量
unsigned short DisToDA_Y(float y); // 声明一个函数，用于将Y轴的距离转换为DA值，根据Y轴的距离计算出Y轴的角度并转换为对应的数字量

