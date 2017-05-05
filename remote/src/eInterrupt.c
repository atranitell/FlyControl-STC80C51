#include"eInterrupt.h"

void  setHighPolity(u8 interruptNum)
{
     switch(interruptNum)
     {
        case INT0:   
                          PX0=PolityHigh;         //设置优先级
                          break;
        case TIMER0:
                          PT0=PolityHigh;
                          break;
        case INT1:   
                          PX1=PolityHigh;
                          break;
        case TIMER1:
                          PT1=PolityHigh;	//高优先级中断
                          break;
        case UART1:
                          PS =PolityHigh;	//高优先级中断
                          break;
        case ADC:
                          PADC=PolityHigh;
                          break;
        case LVD:
                          PLVD=PolityHigh;
                          break;
        case PCA:  
                          PPCA=PolityHigh;
                          break;
        case UART2:
                          IP2|=PolityHigh;
                          break;
        case SPI:         
                          IP2|=0x02;  
                          break;
        default:
                          break;
     }
}
void  setLowPolity(u8 interruptNum)
{
     switch(interruptNum)
     {
        case INT0:   
                          PX0=PolityLow;         //设置优先级
                          break;
        case TIMER0:
                          PT0=PolityLow;
                          break;
        case INT1:   
                          PX1=PolityLow;
                          break;
        case TIMER1:
                          PT1=PolityLow;	//高优先级中断
                          break;
        case UART1:
                          PS =PolityLow;	//高优先级中断
                          break;
        case ADC:
                          PADC=PolityLow;
                          break;
        case LVD:
                          PLVD=PolityLow;
                          break;
        case PCA:  
                          PPCA=PolityLow;
                          break;
        case UART2:
                          IP2&=0xFE;
                          break;
        case SPI:  
                          IP2&=0xFD;  
                          break;
        default:
                          break;
     }
}
void	attachInterrupt(u8 interruptNum ,u8 mode)//PolityHigh or PolityLow
{
	switch (interruptNum)
	{
		case INT0: 
						EX0=1;              //允许外部中断0中断
						IT0=mode;           //中断触发模式
						break;
    
		case INT1: 
	          EX1=1;
						IT1=mode;
						break;
    
		case INT2: 
            INT_CLKO |=  0x10;  //无优先级设置，允许外部中断2中断
						break;
    
		case INT3: 
            INT_CLKO |=  0x20;	//无优先级设置，允许外部中断3中断
						break;
    
		case INT4: 
            INT_CLKO |=  0x40;	//无优先级设置，允许外部中断4中断
						break;
		default: 
						break;
	}
}
void detachInterrupt(u8 interruptNum)
{
		switch(interruptNum)
	{
			case INT0: EX0=0;
							break;
			case INT1: EX1=0;
							break;
			case INT2: INT_CLKO &= 0xef;
							break;
			case INT3: INT_CLKO &= 0xdf;
							break;
			case INT4: INT_CLKO &= 0xbf;
							break;
			default:
							break;
	}
}
void	Timer0_attachInterrupt(u16 time)
{
    u8 TH0_TEMP,TL0_TEMP;
    u16 TIM_Value;
		TR0 = 0;		//停止计数
		ET0 = 1;	//允许中断

		TMOD &= ~0x03;	//工作模式,0: 16位自动重装, 1: 16位定时/计数, 2: 8位自动重装, 3: 16位自动重装, 不可屏蔽中断
		AUXR |=  0x80;	//1T
		TMOD &= ~0x04;	//定时
	
		INT_CLKO &= ~0x01;	//不输出时钟
		TIM_Value     = 65536UL - (MAIN_Fosc /1000*time/1000);		//初值
		TH0_TEMP = (u8)(TIM_Value >> 8);
		TL0_TEMP = (u8)TIM_Value;
		TH0=TH0_TEMP;
		TL0=TL0_TEMP;
		TR0 = 1;	//开始运行
}
void  Timer0_detachInterrupt()
{
    ET0 = 0;	//关闭中断
}
void  Timer1_attachInterrupt(u16 time)
{
    u8 TH1_TEMP,TL1_TEMP;
    u16 TIM_Value;
		TR1 = 0;		//停止计数
		ET1 = 1;	//允许中断		
		TMOD &= ~0x30;	//工作模式,0: 16位自动重装, 1: 16位定时/计数, 2: 8位自动重装, 3: 16位自动重装, 不可屏蔽中断
		AUXR |=  0x40;	//1T
		TMOD &= ~0x40;	//定时
	
		INT_CLKO &= ~0x02;	//不输出时钟
		TIM_Value     = 65536UL - (MAIN_Fosc /1000*time/1000);		//初值
		TH1_TEMP = (u8)(TIM_Value >> 8);
		TL1_TEMP = (u8)TIM_Value;
		TH1=TH1_TEMP;
		TL1=TL1_TEMP;
		TR1 = 1;	//开始运行
}
void  Timer1_detachInterrupt()
{
    ET1 = 0;	//关闭中断
}
void  Timer2_attachInterrupt(u16 time)
{
    u8 TH2_TEMP,TL2_TEMP;
    u16 TIM_Value;
		AUXR &= ~0x10;		//停止计数
		IE2  |= 0x04;	//允许中断
                  //定时器2无优先级设置
                  //定时器2被固定为16位重装模式
	
		AUXR |=  0x04;	//1T
		AUXR &= ~0x08;	//定时
	
		INT_CLKO &= ~0x04;	//不输出时钟
		TIM_Value     = 65536UL - (MAIN_Fosc /1000*time/1000);		//初值
		TH2_TEMP = (u8)(TIM_Value >> 8);
		TL2_TEMP = (u8)TIM_Value;
		T2H=TH2_TEMP;
		T2L=TL2_TEMP;
		AUXR |= 0x10;	//开始运行
}
void  Timer2_detachInterrupt()
{
    IE2  &= ~0x04;	//关闭中断
}
void  Timer3_attachInterrupt(u16 time)
{
    u8 TH3_TEMP,TL3_TEMP;
    u16 TIM_Value;
		T4T3M &= ~0x08;		//停止计数
		IE2  |= 0x20;	//允许中断
                  //定时器3无优先级设置
                  //定时器3被固定为16位重装模式
	
		T4T3M |=  0x02;	//1T
		T4T3M &= ~0x04;	//定时
	
		T4T3M &= ~0x01;	//不输出时钟
		TIM_Value     = 65536UL - (MAIN_Fosc /1000*time/1000);		//初值
		TH3_TEMP = (u8)(TIM_Value >> 8);
		TL3_TEMP = (u8)TIM_Value;
		T3H=TH3_TEMP;
		T3L=TL3_TEMP;
		T4T3M |= 0x08;	//开始运行
}
void  Timer3_detachInterrupt()
{
    IE2  &= ~0x20;	//关闭中断
}
void  Timer4_attachInterrupt(u16 time)
{
    u8 TH4_TEMP,TL4_TEMP;
    u16 TIM_Value;
		T4T3M &= ~0x80;		//停止计数
		IE2  |= 0x40;	//允许中断
                  //定时器4无优先级设置
                  //定时器4被固定为16位重装模式
	
		T4T3M |=  0x20;	//1T
		T4T3M &= ~0x40;	//定时
	
		T4T3M &= ~0x10;	//不输出时钟
		TIM_Value     = 65536UL - (MAIN_Fosc /1000*time/1000);		//初值
		TH4_TEMP = (u8)(TIM_Value >> 8);
		TL4_TEMP = (u8)TIM_Value;
		T4H=TH4_TEMP;
		T4L=TL4_TEMP;
		T4T3M |= 0x80;	//开始运行
}
void  Timer4_detachInterrupt()
{
    IE2  &= ~0x40;	//关闭中断
}