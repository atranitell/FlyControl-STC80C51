
#include "STC15W4K60S4.H"
#include "STC15W4KPWM.H"
#include "NRF24L01.H"
#include "Timer.h"
#include "CONSTANT.H"

void PWMGO(u8 _mode_ctr)
{
	//设置需要使用的PWM输出口为强推挽模式
	P2M0 = 0x0e;
	P2M1 = 0x00;
	P3M0 = 0x80;
	P3M1 = 0x00;

	//使用定时器2作为时钟源
	// Time2_Init();

	//最高位置1才能访问和PWM相关的特殊寄存器
	P_SW2 = 0x80;
	PWMCFG = 0xb0;
	PWMCKS = 0x10;
	PWMIF = 0x00;
	PWMFDCR = 0x00;

	// 以下为每个PWM输出口单独设置
	if ( _mode_ctr == 0 )
		initEachPWM( _PWM_S_1H, _PWM_S_1L, _PWM_S_2H, _PWM_S_2L ) ;
	else if ( _mode_ctr == 1 )
		initEachPWM( _PWM_B_1H, _PWM_B_1L, _PWM_B_2H, _PWM_B_2L ) ;
}

void PWM(u16 PWMa, u16 PWMb, u16 PWMc, u16 PWMd)
{
	PWM2T1H = PWMa >> 8;
	PWM2T1L = PWMa;

	PWM3T1H = PWMb >> 8;
	PWM3T1L = PWMb;

	PWM4T1H = PWMc >> 8;
	PWM4T1L = PWMc;

	PWM5T1H = PWMd >> 8;
	PWM5T1L = PWMd;
}

void initEachPWM( int PWM_1H, int PWM_1L, int PWM_2H, int PWM_2L) {
	PWMCH = PWM_2H;
	PWMCL = PWM_2L;

	PWM2CR = 0x00;
	PWM3CR = 0x00;
	PWM4CR = 0x00;
	PWM5CR = 0x00;

	PWM2T1H = PWM_1H;
	PWM2T1L = PWM_1L;
	PWM2T2H = PWM_2H;
	PWM2T2L = PWM_2L;

	PWM3T1H = PWM_1H;
	PWM3T1L = PWM_1L;
	PWM3T2H = PWM_2H;
	PWM3T2L = PWM_2L;

	PWM4T1H = PWM_1H;
	PWM4T1L = PWM_1L;
	PWM4T2H = PWM_2H;
	PWM4T2L = PWM_2L;

	PWM5T1H = PWM_1H;
	PWM5T1L = PWM_1L;
	PWM5T2H = PWM_2H;
	PWM5T2L = PWM_2L;

	PWMCR = 0x8f;
}