#ifndef	__INTERRUPT_H
#define	__INTERRUPT_H
#include"config.h"
//外部中断0、1的两种中断触发方式
//外部中断2、3、4只能下降沿中断触发
#define	CHANGE	0	//上升沿/下降沿中断
#define	FALLING		1	//下降沿中断
/*   interrupt vector */
#define		INT0		0
#define		TIMER0	1
#define		INT1		2
#define		TIMER1	3
#define		UART1	  4
#define		ADC		  5
#define		LVD		  6
#define		PCA		  7
#define		UART2	  8
#define		SPI		  9
#define		INT2		10
#define		INT3		11
#define		TIMER212
#define		INT4		16
#define		UART3	  17
#define		UART4	  18
#define		TIMER3	19
#define		TIMER4	20
#define   CMP     21
#define   PWM     22
#define   PWMFAULT 23

#define interrupts( ) EA=1
#define noInterrupts( ) EA=0

void  setHighPolity(u8 interruptNum);
void  setLowPolity(u8 interruptNum);
void	attachInterrupt(u8 interruptNum ,void(*userfun)(),u8 mode);
void	Timer0_attachInterrupt(u16 time,void(*userfun)());
void  Timer0_detachInterrupt();
void  Timer1_attachInterrupt(u16 time,void(*userfun)());
void  Timer1_detachInterrupt();
void  Timer2_attachInterrupt(u16 time,void(*userfun)());
void  Timer2_detachInterrupt();
void  Timer3_attachInterrupt(u16 time,void(*userfun)());
void  Timer3_detachInterrupt();
void  Timer4_attachInterrupt(u16 time,void(*userfun)());
void  Timer4_detachInterrupt();
#endif