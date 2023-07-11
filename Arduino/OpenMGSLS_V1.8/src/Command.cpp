//
//
//

#include "Command.h"

String GCodeBuff_StringArry_Command[GCODE_BUFF_SIZE];

char Serial_Char_Command;

int GCodeBuffGetCouter_int_Command;
int GCodeBuffProcessCouter_int_Commmand;
int GCodeBuffLen_int_Command;


int KeyStart_int_Command;
int KeyEnd_int_Command;


float GCodeVal_floatArry_Command[6];//X,Y,Z,A,B,C

bool NeedMove_boolArry_Command[6];//XY,E,Z,A,B,C
bool CommentMode_bool_Command;

struct
{
	String S;
	float F;
    long L;
}KeyVal_struct;

void Init_Command()
{
	MYSERIAL.begin(SPEED_SERIAL);
}


// 定义一个函数，没有返回值，也没有参数
void Get_Command()
{
	/**
	* 当串口有字符输入，并且队列没有满时，循环执行以下操作
	*/
	while (MYSERIAL.available() > 0 && (GCodeBuffLen_int_Command < GCODE_BUFF_SIZE-1))
	{
		// 读取一个字符，并赋值给Serial_Char_Command变量
		Serial_Char_Command = MYSERIAL.read();
		// 如果字符是回车、换行或者冒号（并且不在注释模式下），则执行以下操作
		if (Serial_Char_Command == '\r' ||
			Serial_Char_Command == '\n' ||
			(Serial_Char_Command == ':' && CommentMode_bool_Command == false)) {
 			// 如果当前队列的字符串长度为零，说明没有有效的指令，就退出函数
			if (!GCodeBuff_StringArry_Command[GCodeBuffGetCouter_int_Command].length()) {
				CommentMode_bool_Command = false;
				return;
			}
			// 如果不在注释模式下，就将注释模式设为假，将队列的计数器加一（取余以防越界），将队列的长度加一
			if (!CommentMode_bool_Command) {
				CommentMode_bool_Command = false;
				GCodeBuffGetCouter_int_Command = (GCodeBuffGetCouter_int_Command + 1) % GCODE_BUFF_SIZE;
				GCodeBuffLen_int_Command++;
			}
			// 将当前队列的字符串设为空
			GCodeBuff_StringArry_Command[GCodeBuffGetCouter_int_Command] = "";
		}
		else {
			// 如果字符是分号，就将注释模式设为真
			if (Serial_Char_Command == ';') {
				CommentMode_bool_Command = true;
			}
			// 如果不在注释模式下，就将字符追加到当前队列的字符串中
			if (!CommentMode_bool_Command) {
				GCodeBuff_StringArry_Command[GCodeBuffGetCouter_int_Command] += Serial_Char_Command;
			}
		}
	}
	delay(15);
}

// 定义一个函数，用于处理命令
void Process_Command() {

	// 如果命令中有'G'键
	if (Command_Key('G')) {

		// 根据KeyVal_struct.F的值进行不同的操作
		switch ((int)KeyVal_struct.F)
		{
		case 1:
			// 如果命令中有'Z'键，那么设置GCodeVal_floatArry_Command[2]为KeyVal_struct.F的值，并将NeedMove_boolArry_Command[2]设为真
			if (Command_Key('Z')) {
				GCodeVal_floatArry_Command[2] = KeyVal_struct.F;
				NeedMove_boolArry_Command[2] = true;
			}
			// 如果命令中有'X'键，那么设置GCodeVal_floatArry_Command[0]为KeyVal_struct.F的值，并将NeedMove_boolArry_Command[0]设为真
			if (Command_Key('X')) {
				GCodeVal_floatArry_Command[0] = KeyVal_struct.F;
				NeedMove_boolArry_Command[0] = true;

			}
			// 如果命令中有'Y'键，那么设置GCodeVal_floatArry_Command[1]为KeyVal_struct.F的值，并将NeedMove_boolArry_Command[0]设为真
			if (Command_Key('Y')) {
				GCodeVal_floatArry_Command[1] = KeyVal_struct.F;
				NeedMove_boolArry_Command[0] = true;
			}
			// 如果命令中有'A'键，那么设置GCodeVal_floatArry_Command[3]为KeyVal_struct.F的值，并将NeedMove_boolArry_Command[3]设为真
			else if (Command_Key('A')) {
				GCodeVal_floatArry_Command[3] = KeyVal_struct.F;
				NeedMove_boolArry_Command[3] = true;
			}
			// 如果命令中有'B'键，那么设置GCodeVal_floatArry_Command[4]为KeyVal_struct.F的值，并将NeedMove_boolArry_Command[4]设为真
			else if (Command_Key('B')) {
				GCodeVal_floatArry_Command[4] = KeyVal_struct.F;
				NeedMove_boolArry_Command[4] = true;
			}
			// 如果命令中有'C'键，那么设置GCodeVal_floatArry_Command[5]为KeyVal_struct.F的值，并将NeedMove_boolArry_Command[5]设为真
			else if (Command_Key('C')) {
				GCodeVal_floatArry_Command[5] = KeyVal_struct.F;
				NeedMove_boolArry_Command[5] = true;
			}
			break;
		case 90:
			// 如果KeyVal_struct.F的值为90，那么打印"start"
			MYSERIAL.println("start");
			break;
		}
		// 如果命令中有'E'键，那么将NeedMove_boolArry_Command[1]设为真
		if (Command_Key('E')) {
			NeedMove_boolArry_Command[1] = true;
		}
	}
	// 如果命令中有'M'键
	else if (Command_Key('M')) {
		// 根据KeyVal_struct.F的值进行不同的操作
		switch ((int)KeyVal_struct.F)
		{
		case 105:
			break;
		case 888:
		    // 如果KeyVal_struct.F的值为888，那么调用Start_Heating()函数
		    Start_Heating();
			break;
		case 889:
		    // 如果KeyVal_struct.F的值为889，那么调用Stop_Heating()函数
		    Stop_Heating();
			break;
		}
	}
    // 将GCodeBuffProcessCouter_int_Commmand加1并取模GCODE_BUFF_SIZE，将GCodeBuffLen_int_Command减1
	GCodeBuffProcessCouter_int_Commmand = (GCodeBuffProcessCouter_int_Commmand + 1) % GCODE_BUFF_SIZE;
	GCodeBuffLen_int_Command--;
    // 打印"ok"
	MYSERIAL.println("ok");
}




//定义一个函数，参数是一个字符，返回值是一个布尔值
inline bool Command_Key(char key)
{
	//在GCodeBuff_StringArry_Command数组中查找key字符的位置，赋值给KeyStart_int_Command变量
	 KeyStart_int_Command = GCodeBuff_StringArry_Command[GCodeBuffProcessCouter_int_Commmand].indexOf(key);
	 //如果找到了key字符，说明有对应的命令
	 if (KeyStart_int_Command >= 0) {   //当没有找到关键字时，indexOf函数返回-1
	    //在key字符后面查找空格字符的位置，赋值给KeyEnd_int_Command变量
		KeyEnd_int_Command = GCodeBuff_StringArry_Command[GCodeBuffProcessCouter_int_Commmand].substring(KeyStart_int_Command).indexOf(' ');
		//如果找到了空格字符，说明命令有参数
		if (KeyEnd_int_Command >= 0) {
		    //截取key字符和空格字符之间的字符串，赋值给KeyVal_struct.S变量
			KeyVal_struct.S = GCodeBuff_StringArry_Command[GCodeBuffProcessCouter_int_Commmand].substring(KeyStart_int_Command + 1,KeyStart_int_Command + KeyEnd_int_Command);
		}
        //如果没有找到空格字符，说明命令没有参数
		else {
		    //截取key字符后面的所有字符串，赋值给KeyVal_struct.S变量
			KeyVal_struct.S = GCodeBuff_StringArry_Command[GCodeBuffProcessCouter_int_Commmand].substring(KeyStart_int_Command + 1);

		}
		//将KeyVal_struct.S变量转换为整数，赋值给KeyVal_struct.L变量
		KeyVal_struct.L = KeyVal_struct.S.toInt();
		//将KeyVal_struct.S变量转换为浮点数，赋值给KeyVal_struct.F变量
		KeyVal_struct.F = KeyVal_struct.S.toFloat();
		//返回真值，表示成功执行了命令
		return true;
	}
	//如果没有找到key字符，说明没有对应的命令
	else {
	    //返回假值，表示没有执行任何命令
		return false;
	}

}

