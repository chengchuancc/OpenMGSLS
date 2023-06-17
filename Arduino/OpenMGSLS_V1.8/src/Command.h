// Command.h
#include <Arduino.h> // 引入Arduino核心库，包含了基本的输入输出函数
#include "config.h" // 引入配置文件，包含了一些常量和变量的定义
#include "Heating.h" // 引入Heating头文件，包含了加热控制函数
extern float GCodeVal_floatArry_Command[6]; // 声明一个外部数组，用于存储G代码的参数值，共有6个元素，分别对应X、Y、Z、A、B、C轴的目标位置
extern bool NeedMove_boolArry_Command[6]; // 声明一个外部数组，用于存储是否需要移动的标志，共有6个元素，分别对应XY轴、Z轴、A轴、B轴、C轴的移动以及激光器的开关


extern int GCodeBuffLen_int_Command; // 声明一个外部变量，用于记录G代码缓冲区的长度，表示还有多少条G代码待处理
extern int GCodeBuffGetCouter_int_Command; // 声明一个外部变量，用于记录G代码缓冲区的获取计数器，表示已经获取了多少条G代码
extern int GCodeBuffProcessCouter_int_Commmand; // 声明一个外部变量，用于记录G代码缓冲区的处理计数器，表示已经处理了多少条G代码


void Init_Command(); // 声明一个函数，用于初始化命令模块，设置串口通信参数和缓冲区变量
void Get_Command(); // 声明一个函数，用于获取命令，从串口读取一行G代码并存入缓冲区
void Process_Command(); // 声明一个函数，用于处理命令，从缓冲区取出一条G代码并解析其参数和功能
inline bool Command_Key(char key); // 声明一个内联函数，用于判断字符是否为G代码的关键字（G、M、X、Y、Z、A、B、C）
