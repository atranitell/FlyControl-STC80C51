/*------------------------------------------------------------------*/
//========================================================================
//	STCunio Studio淘宝http://shop112966725.taobao.com
//	STCunio Studio QQ群:117112538
//	作者：mingbozhu
//	联系方式：13724273732
//	邮箱：mingbozhu@yeah.net
//	修改日期2014.11.17
//	版本V1.1
//========================================================================
#include "USART.h"
#include<stdarg.h>
//串口1
#define  OLEN  32                      /* 串口1输出缓冲器大小*/
volatile unsigned char  ostart;        /* 输出缓冲器起始地址 */
volatile unsigned char  oend;          /* 输出缓冲器终止地址 */
xdata  volatile  char  outbuf[OLEN];           /* 输出缓冲器*/
#define  ILEN  16                      /* 串口1输入缓冲器大小*/
volatile unsigned char  istart;        /* 输入缓冲器起始地址 */
volatile unsigned char  iend;          /* 输入缓冲器终止地址 */
xdata  volatile char  inbuf[ILEN];            /* 输入缓冲器*/
#define   CTRL_Q  0x11                 /* Control+Q 字符码*/
#define   CTRL_S  0x13                 /* Control+S 字符码*/
volatile bit   sendfull;               /* 输出缓冲器已满标识位*/
volatile bit   sendactive;             /* 输出进行中标识位*/
volatile bit   sendstop;               /* 输出缓冲器已空标识位*/
volatile bit   bufempty;
//串口2
#define  OLEN2  32                     
volatile unsigned char  ostart2;      
volatile unsigned char  oend2;         
xdata    volatile char  outbuf2[OLEN2];         
#define  ILEN2  16                     
volatile unsigned char  istart2;       
volatile unsigned char  iend2;         
xdata    volatile char  inbuf2[ILEN2];          
#define   CTRL_Q  0x11                 
#define   CTRL_S  0x13                 
volatile bit   sendfull2;              
volatile bit   sendactive2;            
volatile bit   sendstop2;              
volatile bit   bufempty2;
//串口3
#define  OLEN3  32                     
volatile unsigned char  ostart3;       
volatile unsigned char  oend3;         
xdata    char  outbuf3[OLEN3];        
#define  ILEN3  16                     
volatile unsigned char  istart3;       
volatile unsigned char  iend3;         
xdata    char  inbuf3[ILEN3];          
#define   CTRL_Q  0x11                 
#define   CTRL_S  0x13                 
volatile bit   sendfull3;              
volatile bit   sendactive3;            
volatile bit   sendstop3;             
volatile bit   bufempty3;
//串口4
#define  OLEN4  32                     
volatile unsigned char  ostart4;                
volatile unsigned char  oend4;                  
xdata    char  outbuf4[OLEN4];         
#define  ILEN4  16                      
volatile unsigned char  istart4;                
volatile unsigned char  iend4;                  
xdata    char  inbuf4[ILEN4];           
#define   CTRL_Q  0x11                
#define   CTRL_S  0x13                
volatile bit   sendfull4;                      
volatile bit   sendactive4;                     
volatile bit   sendstop4;
volatile bit   bufempty4;
xdata 	char COMx;
//========================================================================
// 函数: print(char comx)
// 描述: 将printf重定向到其中一个串口。
// 参数: comx，COM1：串口1；COM2：串口2；COM3：串口3； COM4：串口4。
// 返回: none.
// 版本: VER1.1
// 日期: 2014-11-17
// 备注: 
//========================================================================
void print(char comx)
{
    COMx=comx;
}
//========================================================================
// 函数: putbuf (char c) 
// 描述: 发送一个字节数据到串口。
// 参数: c，8位数据
// 返回: none.
// 版本: VER1.1
// 日期: 2014-11-17
// 备注: 
//========================================================================
void putbuf (char c)  
{	
	switch (COMx)
	{
		case COM1:
    default  ://默认串口1
							//P55=0;
              while(sendfull);
							//P55=1;
              if (!sendfull)
              {                                      // transmit only if buffer not full  
                  if (!sendactive && !sendstop)      // if transmitter not active:
                  {                                            
                      sendactive = 1;                // transfer the first character direct
                      SBUF = c;                      // to SBUF to start transmission      
                  }
                  else                               // otherwize:
                  {                                                  
                      outbuf[oend] = c; 
                      if(++oend>= OLEN)	oend = 0;
                      bufempty = 0;
                      if (oend==ostart)  sendfull = 1;

                  }                               
              }
              break;
    case COM2:
              while(sendfull2);
              if (!sendfull2)  
              {                                       // transmit only if buffer not full  
                  if (!sendactive2 && !sendstop2) 
                  {                                   // if transmitter not active:        
                      sendactive2 = 1;                // transfer the first character direct
                      S2BUF = c;                      //to SBUF to start transmission      
                  }
                  else        
                  {                                   // otherwize:                      
                      outbuf2[oend2] = c; 
                      if(++oend2>= OLEN2)	oend2 = 0;
                      bufempty2 = 0;
                      if (oend2==ostart2)  sendfull2 = 1;

                   }                               
              }
              break;
    case COM3:
              while(sendfull3);
              if (!sendfull3)
              {                                       // transmit only if buffer not full  
                  if (!sendactive3 && !sendstop3)  
                  {                                   // if transmitter not active:        
                      sendactive3 = 1;                // transfer the first character direct
                      S3BUF = c;                      //to SBUF to start transmission      
                  }
                  else  
                  {                                   // otherwize:                      
                      outbuf3[oend3] = c; 
                      if(++oend3>= OLEN3)	oend3 = 0;
                      bufempty3 = 0;
                      if (oend3==ostart3)  sendfull3 = 1;

                  }                               
              }
              break;
    case COM4:
              while(sendfull4);
              if (!sendfull4) 
              {                                       // transmit only if buffer not full  
                  if (!sendactive4 && !sendstop4) 
                  {                                   // if transmitter not active:        
                      sendactive4 = 1;                // transfer the first character direct
                      S4BUF = c;                      //to SBUF to start transmission      
                  }
                  else  
                  {                                   // otherwize:                      
                      outbuf4[oend4] = c; 
                      if(++oend4>= OLEN4)	oend4 = 0;
                      bufempty4 = 0;
                      if (oend4==ostart4)  sendfull4 = 1;
                  }                               
              }
              break;
   }
}

//========================================================================
// 函数: putchar (char c) 
// 描述: 发送一个字节数据到串口，需调用putbuf。
// 参数: c，8位数据
// 返回: none.
// 版本: VER1.1
// 日期: 2014-11-17
// 备注: 
//=======================================================================
char putchar (char c)  
{
    if (c == '\n')  
    {                                   /* expand new line character:         */
        putbuf (0x0D);                  /* send CR before LF for <new line>   */
    }
    putbuf (c);                         /* send character                     */
    return (c);                         /* return character: ANSI requirement */
}


/****************************************************************************/
/*       _getkey:  interrupt controlled _getkey                             */
/****************************************************************************/
/*char _getkey (void)  {

char c;

  while (!RI);
  c = SBUF;
  RI = 0;
  return (c);
}*/

/***************************/
/*       串口1中断服务程序  */
/***************************/
void serial () interrupt 4  
{     /* use registerbank 1 for interrupt    */
    unsigned char c;
    bit   start_trans = 0;
    static char cnt7f = 0;          //7f的计数器  
    if (RI)  
    {                               /* 检测是否为接收中断 */
        c = SBUF;                   /* 读SBUF */
        RI = 0;                     /* 清除RI */
				if (SBUF == 0x7f)				    /* 检测接受数据是否为0x7f */
        {								//
            if (++cnt7f >= 16)			/*若连续检测到16个0x7f，那么认为是下载流 */
            {							//
                IAP_CONTR=0X60; 		/* 跳转到ISP监控区 */
            }										  								
        }							
        else						    
        {							   
            cnt7f = 0;				    
        }
        switch (c) 
        {                                       /* process character                   */
            case CTRL_S:
                        sendstop = 1;           /* if Control+S stop transmission      */
                        break;
            case CTRL_Q:
                        start_trans = sendstop; /* if Control+Q start transmission     */
                        sendstop = 0;
                        break;

            default:                       /* read all other characters into inbuf*/
            /* if (istart + ILEN != iend)  {
                inbuf[iend] = c;
                if(++iend>=ILEN)iend=0;
            }*/                                  
                        break;
       }
    }
   /* if (TI || start_trans)               // 若是发送中断 
    {                     
        TI = 0;                           // 清除TI        
        if(!bufempty&&!sendstop) 
        {
            SBUF = outbuf[ostart];
            if(++ostart>= OLEN)	ostart = 0;
            if(ostart == oend) bufempty = 1;
            sendfull = 0;
        }
        else sendactive = 0;             //if all transmitted clear 'sendactive
    }*/
}
void serial2 () interrupt 8
{
		unsigned char c;
    bit   start_trans2 = 0;

    if (RI2)  
    {                         
        c = S2BUF;                        
        CLR_RI2();                          /* clear interrupt request flag        */
        switch (c)  
        {                   
            case CTRL_S:
                          sendstop2 = 1;                
                          break;

            case CTRL_Q:
                          start_trans2 = sendstop2;      
                          sendstop2 = 0;
                          break;

            default:                       
          /* if (istart + ILEN != iend)  {
              inbuf[iend] = c;
              if(++iend>=ILEN)iend=0;
            }*/                                  
            break;
        }
    }
    if (TI2 || start_trans2)  
    {                    
        CLR_TI2();                                
        if(!bufempty2&&!sendstop2)
        {
            S2BUF = outbuf2[ostart2];
            if(++ostart2>= OLEN2)	ostart2 = 0;
            if(ostart2 == oend2) bufempty2 = 1;
            sendfull2 = 0;
        }
        else sendactive2 = 0;             
    }
}
void serial3 () interrupt 17
{
		unsigned char c;
    bit   start_trans3 = 0;
    if (RI3)  
    {                                    
        c = S3BUF;                       
        CLR_RI3();                        
        switch (c)  
        {                         
            case CTRL_S:
                        sendstop3 = 1;                   
                        break;
            case CTRL_Q:
                        start_trans3 = sendstop3;         
                        sendstop3 = 0;
                        break;

            default:                            
       /* if (istart + ILEN != iend)  {
          inbuf[iend] = c;
					if(++iend>=ILEN)iend=0;
        }*/
                        break;
       }
    }
    if (TI3 || start_trans3)  
    {                      
        CLR_TI3();                                 
        if(!bufempty3&&!sendstop3) 
        {
            S3BUF = outbuf3[ostart3];
            if(++ostart3>= OLEN3)	ostart3 = 0;
            if(ostart3 == oend3) bufempty3 = 1;
            sendfull3 = 0;
        }
        else sendactive3 = 0;            
    }
}
void serial4 () interrupt 18
{
		unsigned char c;
    bit   start_trans4 = 0;

    if (RI4)  
    {                         
        c = S4BUF;                        
        CLR_RI4();                          /* clear interrupt request flag        */
        switch (c)  
        {                    
            case CTRL_S:
                        sendstop4 = 1;               
                        break;

            case CTRL_Q:
                        start_trans4 = sendstop4;      
                        sendstop4 = 0;
                        break;

            default:                       /* read all other characters into inbuf*/
              /* if (istart + ILEN != iend)  {
                  inbuf[iend] = c;
                  if(++iend>=ILEN)iend=0;
                }*/                                   
            break;
        }
    }
    if (TI4 || start_trans4)  
    {          // if transmitter interrupt            
         CLR_TI4();                         // clear interrupt request flag        
         if(!bufempty4&&!sendstop4) 
         {
              S4BUF = outbuf4[ostart4];
              if(++ostart4>= OLEN4)	ostart4 = 0;
              if(ostart4 == oend4) bufempty4 = 1;
              sendfull4 = 0;
         }
         else sendactive4 = 0;             
    }
}
void Serialbegin(u32 baud,u8 sum,...)
{
	  u8	i,n;
	  u32	j;
	  va_list args;
		va_start(args, sum);
		
		j = (MAIN_Fosc / 4) / baud;	//按1T计算
		if(j >= 65536UL)	return ;	//错误
		j = 65536UL - j;
		
		AUXR &= ~(1<<4);	//Timer stop		
		AUXR &= ~(1<<3);	//Timer2 set As Timer
		AUXR |=  (1<<2);	//Timer2 set as 1T mode
		T2H = (u8)(j>>8);
		T2L = (u8)j;
		IE2  &= ~(1<<2);	//禁止中断
		AUXR &= ~(1<<3);	//定时
		AUXR |=  (1<<4);	//Timer run enable
	
		
		for(n=0;n<sum;n++)
    { 
        switch(va_arg(args,char))
        {			
            case COM1:			
                        for(i=0; i<OLEN; i++)	outbuf[i] = 0;
                        for(i=0; i<ILEN; i++)	inbuf[i] = 0;
                        PS = 1;	//高优先级中断
                        SCON = (SCON & 0x3f) | UART_8bit_BRTx;
                        AUXR |= 0x01;		//S1 BRT Use Timer2;	
                        REN = 1;	//允许接收
                        P_SW1 = (P_SW1 & 0x3f) | (UART1_SW_P30_P31 & 0xc0);	//切换IO
                        CLK_DIV &= ~(1<<4);//串口1正常工作方式
                        ES = 1;	//允许串口1中断
                        break;
            case COM2:
                        for(i=0; i<OLEN2; i++)	outbuf[i] = 0;
                        for(i=0; i<ILEN2; i++)	inbuf[i] = 0;
                        for(i=0; i<OLEN; i++)	outbuf[i] = 0;
                        for(i=0; i<ILEN; i++)	inbuf[i] = 0;
                        S2CON=0X50;//8位可变波特率，允许接收
                        IE2 |=0x01;//允许串口2中断
                        break;		
            case COM3:
                        for(i=0; i<OLEN3; i++)	outbuf[i] = 0;
                        for(i=0; i<ILEN3; i++)	inbuf[i] = 0;
                        S3CON=0x10;//8位可变波特率，允许接收，使用定时器2作为波特率发生器
                        IE2 |=0x08;//允许串口3中断
                        break;
		
            case COM4:  
                        for(i=0; i<OLEN4; i++)	outbuf[i] = 0;
                        for(i=0; i<ILEN4; i++)	inbuf[i] = 0;
                        S4CON=0x10;//8位可变波特率，允许接收，使用定时器2作为波特率发生器
                        IE2 |=0x10;//允许串口4中断

                        break;
            default:
                        break;		
        }		
    }
		EA=1;//开总中断
		va_end(args);
}
