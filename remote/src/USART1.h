
/*------------------------------------------------------------------*/
/* --- STC MCU International Limited -------------------------------*/
/* --- STC 1T Series MCU RC Demo -----------------------------------*/
/* --- Mobile: (86)13922805190 -------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ---------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ---------------------*/
/* --- Web: www.GXWMCU.com -----------------------------------------*/
/* --- QQ:  800003751 ----------------------------------------------*/
/* If you want to use the program or the program referenced in the  */
/* article, please specify in which data and procedures from STC    */
/*------------------------------------------------------------------*/
/*本库函数只限于STCunio R1，无定时器1，定时器2做波特率发生器*/

#ifndef __USART_H
#define __USART_H	 

#include	"config.h"


#define	UART_8bit_BRTx	(1<<6)	//8位数据,可变波特率
#define	UART1_SW_P30_P31	0
#define	BRT_Timer2	2

void Serialbegin(u32 baud);
//char _getkey (void);
char putchar (char c);
void putbuf (char c);


#endif

