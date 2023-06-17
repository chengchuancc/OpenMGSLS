/*
  DueTimer.h - DueTimer header file, definition of methods and attributes...
  For instructions, go to https://github.com/ivanseidel/DueTimer

  Created by Ivan Seidel Gomes, March, 2013.
  Modified by Philipp Klaus, June 2013.
  Released into the public domain.
*/

// 这是一个头文件，用于定义DueTimer类的方法和属性
// 如果想了解更多使用说明，可以访问https://github.com/ivanseidel/DueTimer
// 这个库是由Ivan Seidel Gomes在2013年3月创建，由Philipp Klaus在2013年6月修改
// 这个库是公共领域的，可以自由使用和修改

#ifdef __arm__

#ifndef DueTimer_h
#define DueTimer_h

#include "Arduino.h"

#include <inttypes.h>

/*
	This fixes compatibility for Arduono Servo Library.
	Uncomment to make it compatible.

	Note that:
		+ Timers: 0,2,3,4,5 WILL NOT WORK, and will
				  neither be accessible by Timer0,...
*/
// #define USING_SERVO_LIB	true

#ifdef USING_SERVO_LIB
	#warning "HEY! You have set flag USING_SERVO_LIB. Timer0, 2,3,4 and 5 are not available"
#endif


#define NUM_TIMERS  9

// 这里是一些预处理指令，用于检查是否是在ARM架构的Arduino上编译，以及是否已经定义了DueTimer_h
// 如果是的话，就包含Arduino.h和inttypes.h这两个头文件，否则报错
// 这里还定义了一个宏USING_SERVO_LIB，用于修复和Arduino Servo库的兼容性问题
// 如果定义了这个宏，就会发出警告，并且禁用掉0,2,3,4,5这几个定时器
// 这里还定义了一个常量NUM_TIMERS，表示有9个定时器可用

class DueTimer
{
protected:

	// Represents the timer id (index for the array of Timer structs)
	const unsigned short timer;

	// Stores the object timer frequency
	// (allows to access current timer period and frequency):
	static double _frequency[NUM_TIMERS];

	// Picks the best clock to lower the error
	static uint8_t bestClock(double frequency, uint32_t& retRC);

  // Make Interrupt handlers friends, so they can use callbacks
  friend void TC0_Handler(void);
  friend void TC1_Handler(void);
  friend void TC2_Handler(void);
  friend void TC3_Handler(void);
  friend void TC4_Handler(void);
  friend void TC5_Handler(void);
  friend void TC6_Handler(void);
  friend void TC7_Handler(void);
  friend void TC8_Handler(void);

	static void (*callbacks[NUM_TIMERS])();

	struct Timer
	{
		Tc *tc;
		uint32_t channel;
		IRQn_Type irq;
	};

	// Store timer configuration (static, as it's fixed for every object)
	static const Timer Timers[NUM_TIMERS];

public:

	static DueTimer getAvailable(void);

	DueTimer(unsigned short _timer);
	DueTimer& attachInterrupt(void (*isr)());
	DueTimer& detachInterrupt(void);
	DueTimer& start(long microseconds = -1);
	DueTimer& stop(void);
	DueTimer& setFrequency(double frequency);
	DueTimer& setPeriod(unsigned long microseconds);

	double getFrequency(void) const;
	long getPeriod(void) const;
};

// 这里是定义DueTimer类的主体部分，包括以下内容：
// - 定义了一个受保护的成员变量timer，表示定时器的id（也是定时器结构体数组的索引）
// - 定义了一个静态数组_frequency，用于存储每个定时器对象的频率（也可以用来获取当前定时器的周期和频率）
// - 定义了一个静态方法bestClock，用于根据给定的频率选择最佳的时钟源，以降低误差，并返回相应的RC值（计数器上限）
// - 定义了9个友元函数，分别是TC0到TC8的中断处理函数，这样它们就可以使用回调函数
// - 定义了一个静态数组callbacks，用于存储每个定时器对象的回调函数指针
// - 定义了一个结构体Timer，用于存储每个定时器的相关信息，包括TC指针，通道号，中断号
// - 定义了一个静态常量数组Timers，用于存储每个定时器对象的配置信息（这是静态的，因为对于每个对象都是固定的）
// - 定义了一些公共的方法，包括：
//   - getAvailable：静态方法，用于获取一个可用的定时器对象
//   - DueTimer：构造函数，根据给定的定时器id创建一个定时器对象
//   - attachInterrupt：绑定一个回调函数到当前定时器对象，并开启中断
//   - detachInterrupt：解绑当前定时器对象的回调函数，并关闭中断
//   - start：开始当前定时器对象的计数，可以指定一个微秒数作为周期
//   - stop：停止当前定时器对象的计数
//   - setFrequency：设置当前定时器对象的频率
//   - setPeriod：设置当前定时器对象的周期（单位是微秒）
//   - getFrequency：获取当前定时器对象的频率
//   - getPeriod：获取当前定时器对象的周期（单位是微秒）

// Just to call Timer.getAvailable instead of Timer::getAvailable() :
extern DueTimer Timer;

extern DueTimer Timer1;
// Fix for compatibility with Servo library
#ifndef USING_SERVO_LIB
	extern DueTimer Timer0;
	extern DueTimer Timer2;
	extern DueTimer Timer3;
	extern DueTimer Timer4;
	extern DueTimer Timer5;
#endif
extern DueTimer Timer6;
extern DueTimer Timer7;
extern DueTimer Timer8;

#endif

#else
	#error Oops! Trying to include DueTimer on another device?
#endif

// 这里是定义一些全局变量，用于方便地访问每个定时器对象，比如Timer1, Timer2等
// 这里也考虑了和Servo库的兼容性问题，如果定义了USING_SERVO_LIB宏，就不会定义0,2,3,4,5这几个定时器对象
