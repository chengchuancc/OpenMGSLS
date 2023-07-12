// Stepper.h


#include "config.h" // 引入配置文件，包含了一些常量和变量的定义
#include "TimerThree.h"
#include <Arduino.h>
void Init_Stepper(); // 声明一个函数，用于初始化步进电机

void StepperA_Moving(); // 声明一个函数，用于控制步进电机A的运动
void StepperB_Moving(); // 声明一个函数，用于控制步进电机B的运动
void StepperC_Moving(); // 声明一个函数，用于控制步进电机C的运动

void StepperA_StartMove(float dis); // 声明一个函数，用于让步进电机A开始移动一定的距离
void StepperB_StartMove(float dis); // 声明一个函数，用于让步进电机B开始移动一定的距离
void StepperC_StartMove(float dis); // 声明一个函数，用于让步进电机C开始移动一定的距离
extern float Z_Position_Old; // 声明一个外部变量，用于存储Z轴的旧位置
extern bool IsMoving_A; // 声明一个外部变量，用于判断步进电机A是否在运动中
extern bool IsMoving_B; // 声明一个外部变量，用于判断步进电机B是否在运动中
extern bool IsMoving_C; // 声明一个外部变量，用于判断步进电机C是否在运动中
