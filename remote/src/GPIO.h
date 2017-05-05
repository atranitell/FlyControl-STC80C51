//========================================================================
//	STCunio Studio淘宝http://shop112966725.taobao.com
//	STCunio Studio QQ群:117112538
//	作者：mingbozhu
//	联系方式：13724273732
//	邮箱：mingbozhu@yeah.net
//	修改日期2014.11.7
//	版本V1.1
//========================================================================


#ifndef	__GPIO_H
#define	__GPIO_H
#include	"config.h"
#define	PullUp		0	//上拉准双向口
#define pullup		0 //上拉准双向口
#define PULLUP		0 //上拉准双向口

#define	HighZ		1	//浮空输入
#define	highz		1	//浮空输入
#define	HIGHZ		1	//浮空输入

#define	OpenD		2	//开漏
#define	opend		2	//开漏
#define	OPEND		2	//开漏

#define	OutPP		3	//推挽输出
#define outpp		3 //推挽输出
#define OUTPP		3 //推挽输出

#define LOW			 0
#define low			 0
#define HIGH     1
#define high		 1
#define NOR      2
#define nor			 2
#define allpin   0xFF
#define ALLPIN	 0XFF
#define LSBFIRST 0
#define lsbfirst 0
#define MSBFIRST 1
#define msbfirst 1

//数字引脚的另一种宏定义
#define D0       0
#define D1       1
#define D2       2
#define D3       3
#define D4       4
#define D5       5
#define D6       6
#define D7       7
#define D8       8
#define D9       9
#define D10      10
#define D11      11
#define D12      12
#define D13      13
#define D14      14
#define D15      15
#define D16      16
#define D17      17
#define D18      18
#define D19      19
#define D20      20
#define D21      21
#define D22      22
#define D23      23
#define D24      24
#define D25      25
#define D26      26
#define D27      27
#define D28      28
#define D29      29
#define D30      30
#define D31      31
#define D32      32
#define D33      33
#define D34      34
#define D35      35
#define D36      36
#define D37      37
#define D38      38
#define D39      39
#define D40      40
#define D41      41
#define D42      42
#define D43      43
#define D44      44
#define D45      45
#define D46      46
#define D47      47
#define D48      48
#define D49      49
#define D50      50
#define D51      51
#define D52      52
#define D53      53
#define D54      54
#define D55      55
#define D56      56
#define D57      57
#define D58      58
#define D59      59
#define D60      60
#define D61      61
#define D62      62
#define D63      63
//模拟引脚的另一种宏定义
#define A0       56
#define A1       57
#define A2       58
#define A3       59
#define A4       60
#define A5       61
#define A6       62
#define A7       63



void pinMode(u8 pin,u8 mode);
void digitalWrite(u8 pin,u8 value);
void pinInvert(u8 pin);
u8 digitalRead(u8 pin);
void shiftOut(u8 dataPin,u8 clockPin,u8 bitOrder, u8 value);
u8 shiftIn(u8 dataPin,u8 clockPin,u8 bitOrder);
void tone(u8 pin,u16 frequency);
void noTone(u8 pin);
#endif
