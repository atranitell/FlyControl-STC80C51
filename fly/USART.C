#include <STC15W4K60S4.H>
#include <USART.h>
#include <intrins.h>
#include "CONSTANT.H"

bit busy;
extern unsigned int Height ;

void Usart_Init() {
    EA = 0;
    SCON = 0x50;
    AUXR |= 0x40;
    AUXR &= 0xFE;
    TMOD &= 0x0F;
    TL1 = _UART_T1 & 0x00FF;
    TH1 = _UART_T1 >> 8 & 0x00FF;
    ET1 = 0;
    TR1 = 1;
    EA = 1;
    ES = 1;
}

void Uart() interrupt 4 using 1 {
    if (RI) {
        RI = 0;
        Height = SBUF;
    }
    if (TI) {
        TI = 0;
        busy = 0;
    }
}

void SendData(unsigned char dat)
{
    while (busy);
    ACC = dat;
    busy = 1;
    SBUF = ACC;
}