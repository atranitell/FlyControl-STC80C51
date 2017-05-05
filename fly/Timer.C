
#include "STC15W4K60S4.H"
#include "Timer.h"

#include "CONSTANT.H"

void Time0_Init(void)		//8ms@28MHz 定时器0 16位12T自动重载
{
	TR0 = 0;
	AUXR &= 0x7F;	// AUXR &= ~0x80;	// 12T模式,
	TMOD &= 0xF0;	// TMOD &= 0xf0;	// 定时模式, 16位自动重装
	IE  = 0x92;	    // 1001 0010    	// EA = 1, ES = 1, ET0 = 1
	TL0 = _TIME0_VALUE & 0x00FF;
	TH0 = _TIME0_VALUE >> 8 & 0x00FF;
	TF0 = 0;
	TR0 = 1;
}

// void Time2_Init(void)
// {
// 	AUXR |= 0x04;	//AUXR |=  (1<<2);	//1T
// 	T2L = 0xEB;
// 	T2H = 0xFF;
// 	AUXR |= 0x10;	//AUXR |=  (1<<4);	//开始运行
// }