//========================================================================
//	STCunio Studio淘宝http://shop112966725.taobao.com
//	STCunio Studio QQ群:117112538
//	作者：mingbozhu
//	联系方式：13724273732
//	邮箱：mingbozhu@yeah.net
//	修改日期2014.11.17
//	版本V1.1
//========================================================================

#include	"GPIO.h"
u8 code digital_pin_to_bit_mask_PGM[]={
    0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,//D0-D7掩码   P3
    0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,//D8-D15掩码  P2
    0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,//D16-D23掩码 P4
    0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,//D24-D31掩码 P5
    0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,//D32-D39掩码 P6
    0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,//D40-D47掩码 P7
    0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,//D48-D55掩码 P0
    0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80 //D56-D63掩码 P1
};
u8 code digital_pin_to_bit_PGM[]={
    0,1,2,3,4,5,6,7,//D0-D7   P3
    0,1,2,3,4,5,6,7,//D8-D15  P2
    0,1,2,3,4,5,6,7,//D16-D23 P4
    0,1,2,3,4,5,6,7,//D24-D31 P5
    0,1,2,3,4,5,6,7,//D32-D39 P6
    0,1,2,3,4,5,6,7,//D40-D47 P7
    0,1,2,3,4,5,6,7,//D48-D55 P0
    0,1,2,3,4,5,6,7 //D56-D63 P1
};
u8 code digital_pin_to_port_PGM[]={
    0,0,0,0,0,0,0,0,//D0-D7   P3
    1,1,1,1,1,1,1,1,//D8-D15  P2
    2,2,2,2,2,2,2,2,//D16-D23 P4
    3,3,3,3,3,3,3,3,//D24-D31 P5
    4,4,4,4,4,4,4,4,//D32-D39 P6
    5,5,5,5,5,5,5,5,//D40-D47 P7
    6,6,6,6,6,6,6,6,//D48-D55 P0
    7,7,7,7,7,7,7,7 //D56-D63 P1
}; 
u8 code digital_pin_mode_PxM1_PGM[4][8]={
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//上拉准双向口
    0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,//浮空输入
    0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,//开漏输出
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//推挽输出
};
u8 code digital_pin_mode_PxM0_PGM[4][8]={
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//上拉准双向口
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//浮空输入
    0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,//开漏输出
    0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,//推挽输出    
};
//========================================================================
//	设置Digital IO的工作模式
//	STCunio有4种IO工作模式
//	1.准双向模式（内部上拉）PullUp
//	2.高阻输入模式 HighZ
//	3.开漏模式(内部断开上拉)OpenD
//	4.推挽输出模式 OutPP
//========================================================================   
void pinMode(u8 pin,u8 mode)
{
	u8 Bitmask,Bit,Port,PxM1,PxM0;
	Port = digital_pin_to_port_PGM[pin];
  Bitmask  = digital_pin_to_bit_mask_PGM[pin];
  Bit  = digital_pin_to_bit_PGM[pin];
  PxM1 = digital_pin_mode_PxM1_PGM[mode][Bit];
  PxM0 = digital_pin_mode_PxM0_PGM[mode][Bit];
	switch(Port)
	{
		case 0://P3 0-7
            P3M1  = (P3M1&~Bitmask)|PxM1;
            P3M0  = (P3M0&~Bitmask)|PxM0;
						break;
		case 1://P2 8-15	
            P2M1  = (P2M1&~Bitmask)|PxM1;
            P2M0  = (P2M0&~Bitmask)|PxM0;
						break;
		case 2://P4 16-23
            P4M1  = (P4M1&~Bitmask)|PxM1;
            P4M0  = (P4M0&~Bitmask)|PxM0;
						break;
		case 3://P5 24-31
            P5M1  = (P5M1&~Bitmask)|PxM1;
            P5M0  = (P5M0&~Bitmask)|PxM0;
						break;
		case 4://P6 32-39
            P6M1  = (P6M1&~Bitmask)|PxM1;
            P6M0  = (P6M0&~Bitmask)|PxM0;
						break;
		case 5://P7 40-47
            P7M1  = (P7M1&~Bitmask)|PxM1;
            P7M0  = (P7M0&~Bitmask)|PxM0;
						break;
		case 6://P0 48-55
            P0M1  = (P0M1&~Bitmask)|PxM1;
            P0M0  = (P0M0&~Bitmask)|PxM0;
						break;
		case 7://P1 56-63
            P1M1  = (P1M1&~Bitmask)|PxM1;
            P1M0  = (P1M0&~Bitmask)|PxM0;
						break;
		default:
						break;
	}	
}

//写IO口
void digitalWrite(u8 pin,u8 value)
{
	u8 Bitmask,Port;
	
	Port = digital_pin_to_port_PGM[pin];
  Bitmask  = digital_pin_to_bit_mask_PGM[pin];
	switch(Port)
	{
		case 0://P3 0-7			
				if(value)P3|=Bitmask;
				else P3&=~Bitmask;			
				break;
		case 1://P2 8-15	
			  if(value)P2|=Bitmask;
			  else P2&=~Bitmask;				
				break;
		case 2://P4 16-23
			  if(value)P4|=Bitmask;
			  else P4&=~Bitmask;
				break;
		case 3://P5
			  if(value)P5|=Bitmask;
			  else P5&=~Bitmask;
				break;
		case 4://P6
			  if(value)P6|=Bitmask;
			  else P6&=~Bitmask;
				break;
		case 5://P7
			  if(value)P7|=Bitmask;
			  else P7&=~Bitmask;
				break;
	  case 6://P0
			  if(value)P0|=Bitmask;
			  else P0&=~Bitmask;
				break;
		case 7://P1
			  if(value)P1|=Bitmask;
			  else P1&=~Bitmask;
				break;
		default:
				break;	
	}
}
//将Digital IO取反
void pinInvert(u8 pin)
{	
	u8 Bitmask,Port;
	Port = digital_pin_to_port_PGM[pin];
  Bitmask  = digital_pin_to_bit_mask_PGM[pin];
	switch(Port)
	{
		case 0:
				P3^=Bitmask;
				break;
		case 1:
				P2^=Bitmask;
				break;
		case 2:
				P4^=Bitmask;
				break;
		case 3:
				P5^=Bitmask;
				break;
		case 4:
				P6^=Bitmask;
				break;
		case 5:
				P7^=Bitmask;
				break;
		case 6:
				P0^=Bitmask;
				break;
		case 7:
				P1^=Bitmask;
				break;
		default:
				break;
	}
}
//读IO的状态

u8 digitalRead(u8 pin)
{
	u8 Bitmask,Port;
	
	Port = digital_pin_to_port_PGM[pin];
  Bitmask  = digital_pin_to_bit_mask_PGM[pin];
	switch(Port)
	{
		case 0:
				
				if(P3&Bitmask)return HIGH;
				else return LOW;				
				break;
		case 1:
				if(P2&Bitmask)return HIGH;
				else return LOW;
				break;
		case 2:
				if(P4&Bitmask)return HIGH;
				else return LOW;
				break;
		case 3:
				if(P5&Bitmask)return HIGH;
				else return LOW;
				break;
		case 4:
				if(P6&Bitmask)return HIGH;
				else return LOW;
				break;
		case 5:
				if(P7&Bitmask)return HIGH;
				else return LOW;
				break;
		case 6:
				if(P0&Bitmask)return HIGH;
				else return LOW;
				break;
		case 7:
				if(P1&Bitmask)return HIGH;
				else return LOW;
				break;
		default:
				return LOW;
				break;
	}

}
//========================================================================
// 函数: shiftOut(u8 dataPin,u8 clockPin,u8 bitOrder,u8 value)
// 描述: 串行发送数据。
// 参数: dataPin，选择需要的数据线所在的引脚。
// 参数：clockPin，选择需要的时钟线所在的引脚。
// 参数：bitOrder，选择发送数据的顺序，包括MSBFIRST最高位先发和LSBFIRST最低位先发
// 参数：value，发送的数据
// 返回: none.
// 版本: VER1.1
// 日期: 2014-11-17
// 备注: 
//========================================================================  
void shiftOut(u8 dataPin,u8 clockPin,u8 bitOrder,u8 value)
{
		unsigned char	i;
	for(i=0; i<8; i++)
	{ 
		if(bitOrder==MSBFIRST)
		{
				if(value & 0x80)digitalWrite(dataPin,HIGH);
				else			digitalWrite(dataPin,LOW);
			  value = value << 1;
		}
		else
		{
				if(value & 0x01)digitalWrite(dataPin,HIGH);
				else			digitalWrite(dataPin,LOW);
			  value = value >> 1;
		}					
		digitalWrite(clockPin,HIGH);
		digitalWrite(clockPin,LOW);		
	}
}
//========================================================================
// 函数: shiftIn(u8 dataPin,u8 clockPin,u8 bitOrder)
// 描述: 串行接收数据。
// 参数: dataPin，选择需要的数据线所在的引脚。
// 参数：clockPin，选择需要的时钟线所在的引脚。
// 参数：bitOrder，选择接收数据的顺序，包括先收MSBFIRST最高位和先收LSBFIRST最低位
// 返回: value，接收的数据
// 版本: VER1.1
// 日期: 2014-11-17
// 备注: 
//========================================================================
u8 shiftIn(u8 dataPin,u8 clockPin,u8 bitOrder)

{
		char	i;
	  unsigned char	value=0;
  if(bitOrder==MSBFIRST)
  {
    for(i=7; i>=0; i--)
    { 
        digitalWrite(clockPin,HIGH);
        digitalWrite(clockPin,LOW);
			  value |= (digitalRead(dataPin)<<i);					        				
    }
  }
  else	
  {
    for(i=0; i<=7; i++)
    {
        digitalWrite(clockPin,HIGH);											  					
        digitalWrite(clockPin,LOW);	  
        value |= (digitalRead(dataPin)<<i);
    }
  }
	return value;
}
//========================================================================
// 函数: tone(u8 pin,u16 frequency)
// 描述: 发出一定频率的函数。
// 参数: pin，选择需要引脚。
// 参数：frequency，设置发出的频率。
// 返回: none
// 版本: VER1.1
// 日期: 2014-11-17
// 备注: 
//========================================================================
void tone(u8 pin,u16 frequency)
{
    u8 TH_TEMP,TL_TEMP;
    u16 TIM_Value;
    switch(pin)
    {
      case 5: //T0
              TMOD &= ~0x03;	//工作模式,0: 16位自动重装, 1: 16位定时/计数, 2: 8位自动重装, 3: 16位自动重装, 不可屏蔽中断
              AUXR &= ~0x80;	//12T
              TMOD &= ~0x04;	//定时	
              INT_CLKO |= 0x01;	//输出时钟
              TIM_Value = 65536UL - (MAIN_Fosc /frequency/24+0.5);		//初值
              TH_TEMP = (u8)(TIM_Value >> 8);
              TL_TEMP = (u8)TIM_Value;
              TH0=TH_TEMP;
              TL0=TL_TEMP;
              TR0=1;
              break;
      case 4: //T1

              TMOD &= ~0x30;	//工作模式,0: 16位自动重装, 1: 16位定时/计数, 2: 8位自动重装, 3: 16位自动重装, 不可屏蔽中断
              AUXR &= ~0x40;	//12T
              TMOD &=  0x40;	//定时	
              INT_CLKO |= 0x02;	//输出时钟
              TIM_Value     = 65536UL -(MAIN_Fosc /frequency/24+0.5);		//初值
              TH_TEMP = (u8)(TIM_Value >> 8);
              TL_TEMP = (u8)TIM_Value;
              TH1=TH_TEMP;
              TL1=TL_TEMP;
              TR1 = 1;	//开始运行
              break;
      case 0: //T2
                  //定时器2被固定为16位重装模式
              AUXR &= ~0x04;	//12T
              AUXR &= ~0x08;	//定时	
              INT_CLKO |= 0x04;	//输出时钟
              TIM_Value     = 65536UL - (MAIN_Fosc /frequency/24+0.5);		//初值
              TH_TEMP = (u8)(TIM_Value >> 8);
              TL_TEMP = (u8)TIM_Value;
              T2H=TH_TEMP;
              T2L=TL_TEMP;
              AUXR |= 0x10;	//开始运行
              break;
      case 52:  //T3
              
              T4T3M &= ~0x02;	//12T
              T4T3M &= ~0x04;	//定时	
              T4T3M |=  0x01;	//输出时钟
              TIM_Value     = 65536UL - (MAIN_Fosc /frequency/24+0.5);		//初值
              TH_TEMP = (u8)(TIM_Value >> 8);
              TL_TEMP = (u8)TIM_Value;
              T3H=TH_TEMP;
              T3L=TL_TEMP;
              T4T3M |= 0x08;	//开始运行
              break;
      case 54://T4
        
              T4T3M &= ~0x20;	//12T
              T4T3M &= ~0x40;	//定时	
              T4T3M |=  0x10;	//输出时钟
              TIM_Value     = 65536UL - (MAIN_Fosc /frequency/24+0.5);		//初值
              TH_TEMP = (u8)(TIM_Value >> 8);
              TL_TEMP = (u8)TIM_Value;
              T4H=TH_TEMP;
              T4L=TL_TEMP;
              T4T3M |= 0x80;	//开始运行
              break;
      default:
              break;
    }
}
//========================================================================
// 函数: noTone(u8 pin)
// 描述: 结束频率发出的函数。
// 参数: pin，选择需要引脚。
// 返回: none
// 版本: VER1.1
// 日期: 2014-11-17
// 备注: 
//========================================================================
void noTone(u8 pin)
{
   
   switch(pin)
   {
      case 5: //T0
              INT_CLKO &= ~0x01;	//不输出时钟
              TR0=0;//关闭定时器0
              break;
      case 4: //T1	
              INT_CLKO &= ~0x02;	//不输出时钟
              TR1 =0;//关闭定时器1
              break;
      case 0: //T2
              INT_CLKO &= ~0x04;	//不输出时钟
              AUXR &= ~0x10;	//关闭定时器2
              break;
      case 52:  //T3
              T4T3M &= ~0x01;	//不输出时钟
              T4T3M &= ~0x08;	//关闭定时器3
              break;
      case 54://T4
              T4T3M &= ~0x10;	//不输出时钟
              T4T3M &= ~0x80;	//开始运行
              break;
      default:
              break; 	
   }
   digitalWrite(pin,HIGH);
}