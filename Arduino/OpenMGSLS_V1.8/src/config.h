//Command
#define MYSERIAL Serial // 定义MYSERIAL为Serial，用于串口通信
#define SPEED_SERIAL 250000 // 定义串口通信的波特率为250000
#define GCODE_BUFF_SIZE 3 // 定义G代码缓冲区的大小为3，表示可以存储3条G代码

//XYMG
#define SPEED_SPI 50000000 // 定义SPI通信的速率为50000000，用于与XY轴运动控制器通信
#define BASE_VOLTAGE 4.8// 定义基准电压为4.8伏，用于计算DA值
#define LASER_OFF_POWER 10//0-255  correspond 0-5W (when the laser max power is 5W) // 定义激光器关闭时的功率为10，对应0-5瓦的范围（当激光器最大功率为5瓦时）
#define LASER_ON_POWER 255 // 定义激光器打开时的功率为255，对应0-5瓦的范围（当激光器最大功率为5瓦时）
#define PIN_XY 16 // 定义XY轴运动控制器的片选引脚为16
#define PIN_LASER 32 // 定义激光器的控制引脚为32

//Stepper
#define STEPPER_A_STEP_PER_UNIT 13.3333 // 定义步进电机A每个单位距离需要走的步数为13.3333
#define STEPPER_A_ACCELERATION 300 // 定义步进电机A的加速度为300
#define STEPPER_A_TIMER Timer4 // 定义步进电机A使用的定时器为Timer4
#define STEPPER_A_SPEED 250 // 定义步进电机A的速度为250
#define STEPPER_A_PIN_EN 24 // 定义步进电机A的使能引脚为24
#define STEPPER_A_PIN_STEP 47 // 定义步进电机A的步进引脚为47
#define STEPPER_A_PIN_DIR 45 // 定义步进电机A的方向引脚为45
#define STEPPER_A_NEGATE false // 定义步进电机A的方向是否取反为false

#define STEPPER_B_STEP_PER_UNIT 200 // 定义步进电机B每个单位距离需要走的步数为200
#define STEPPER_B_ACCELERATION 100 // 定义步进电机B的加速度为100
#define STEPPER_B_TIMER Timer4 // 定义步进电机B使用的定时器为Timer4
#define STEPPER_B_SPEED 50 // 定义步进电机B的速度为50
#define STEPPER_B_PIN_EN 0 // 定义步进电机B的使能引脚为0
#define STEPPER_B_PIN_STEP 43 // 定义步进电机B的步进引脚为43
#define STEPPER_B_PIN_DIR 41 // 定义步进电机B的方向引脚为41
#define STEPPER_B_NEGATE true // 定义步进电机B的方向是否取反为true


#define STEPPER_C_STEP_PER_UNIT 200 // 定义步进电机C每个单位距离需要走的步数为200
#define STEPPER_C_ACCELERATION 100 // 定义步进电机C的加速度为100
#define STEPPER_C_TIMER Timer4 // 定义步进电机C使用的定时器为Timer4
#define STEPPER_C_SPEED 50 // 定义步进电机C的速度为50
#define STEPPER_C_PIN_EN 0 // 定义步进电机C的使能引脚为0
#define STEPPER_C_PIN_STEP 39 // 定义步进电机C的步进引脚为39
#define STEPPER_C_PIN_DIR 37 // 定义步进电机C的方向引脚为37
#define STEPPER_C_NEGATE true // 定义步进电机C的方向是否取反为true

#define Stepper_A_DISTANCE_MAX 400 // 定义步进电机A的最大移动距离为400

//Planner
#define DA_VAL_BUFF_SIZE 3 // 定义DA值缓冲区的大小为3，表示可以存储3个DA值
#define DA_VAL_ARRY_SIZE 6000 // 定义DA值数组的大小为6000，表示每个DA值可以存储6000个数字量
#define SPEED_XY 3000 //  mm per seconds // 定义XY轴的移动速度为3000毫米每秒
#define DIS_F_THETA 260//distance between F-Theta and plane // 定义F-Theta镜头和平面之间的距离为260毫米
#define DIS_XYMOTOR 8 // 定义XY轴运动控制器之间的距离为8毫米
#define RAD_MG_MAX 0.349065 //the Mirror galvanometer max rad  (20/180*pi) // 定义镜片振镜的最大角度为0.349065弧度（20/180*π）
#define CYCLE_LNTERPOLARION 3000// per seconds // 定义线性插补的周期为3000次每秒

//heating
#define HEATING_PIN 35 // 定义加热器的控制引脚为35
#define HEATING_CYCLE 4000000 //mirosecond // 定义加热器的控制周期为4000000微秒
#define HEATING_PROPORTION 0.43 // 定义加热器的控制占空比为0.43
#define HEATING_TIMER Timer6 // 定义加热器使用的定时器为Timer6
