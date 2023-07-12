// XYMG.h

#include "config.h" // 引入配置文件，包含了一些常量和变量的定义
#include "TimerOne.h"
#include "Planner.h" // 引入Planner头文件，包含了路径规划和插补函数
#include <SPI.h> // 引入SPI库，用于串行外设接口通信

extern bool IsMoving_XY; // 声明一个外部变量，用于判断XY轴是否在运动中
extern bool NeedOpenLaser_bool_XYMG; // 声明一个外部变量，用于判断是否需要打开激光器
extern int Times_LinearLnterpolation_int_XYMG; // 声明一个外部变量，用于记录线性插补的次数
extern unsigned short* DAValMoveX_ushortPointer_XYMG; // 声明一个外部指针，用于指向要移动的X轴的DA值数组
extern unsigned short* DAValMoveY_ushortPointer_XYMG; // 声明一个外部指针，用于指向要移动的Y轴的DA值数组

void Init_XYMG(); // 声明一个函数，用于初始化XY轴运动控制模块，设置定时器中断和引脚模式
void XYMG_Move(); // 声明一个函数，用于控制XY轴的运动，根据DA值数组和线性插补次数发送数字量给运动控制器
inline void XYMG_Write(unsigned short x, unsigned short y); // 声明一个内联函数，用于向运动控制器写入X轴和Y轴的数字量
void XYMG_StartMove(); // 声明一个函数，用于开始XY轴的移动，启动定时器中断和激光器
