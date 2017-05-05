/*-------------------------------------------------------*/
/*本库函数只限于STCunio R1，无定时器1，定时器2做波特率发生器*/
/*-------------------------------------------------------*/
#include "USART1.h"

#define  OLEN  32                      /* size of serial transmission buffer */
volatile unsigned char  ostart;                /* transmission buffer start index    */
volatile unsigned char  oend;                  /* transmission buffer end index      */

idata    char  outbuf[OLEN];          /* storage for transmission buffer    */

#define  ILEN  16                      /* size of serial receiving buffer    */
volatile unsigned char  istart;                /* receiving buffer start index       */
volatile unsigned char  iend;                  /* receiving buffer end index         */
idata    char  inbuf[ILEN];           /* storage for receiving buffer       */

#define   CTRL_Q  0x11                /* Control+Q character code           */
#define   CTRL_S  0x13                /* Control+S character code           */

volatile bit   sendfull;                       /* flag: marks transmit buffer full   */
volatile bit   sendactive;                     /* flag: marks transmitter active     */
volatile bit   sendstop;                       /* flag: marks XOFF character         */
volatile bit   bufempty;

/****************************************************************************/
/*       putbuf:  write a character to SBUF or transmission buffer          */
/****************************************************************************/
void putbuf (char c)  {
	
	while(sendfull);

  if (!sendfull)  {                   // transmit only if buffer not full  
    if (!sendactive && !sendstop)  { // if transmitter not active:        
      sendactive = 1;                 // transfer the first character direct
      SBUF = c;                       //to SBUF to start transmission      
    }
    else  {                           // otherwize:                      
			outbuf[oend] = c; 
			if(++oend>= OLEN)	oend = 0;
			bufempty = 0;
			if (oend==ostart)  sendfull = 1;

		}                               
  }
}

/****************************************************************************/
/*       putchar:  interrupt controlled putchar function                    */
/****************************************************************************/
char putchar (char c)  {
  if (c == '\n')  {                   /* expand new line character:         */
      putbuf (0x0D);                    /* send CR before LF for <new line>   */
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

/****************************************************************************/
/*       serial:  serial receiver / transmitter interrupt                   */
/****************************************************************************/
void serial () interrupt 4 using 1  {     /* use registerbank 1 for interrupt    */
  unsigned char c;
  bit   start_trans = 0;
	static char cnt7f = 0;              //7f的计数器

  if (RI)  {                         /* if receiver interrupt               */
    c = SBUF;                        /* read character                      */
    RI = 0;                          /* clear interrupt request flag        */
					if (SBUF == 0x7f)				//检测接受数据是否为0x7f
        	{								//
            	if (++cnt7f >= 16)			//若连续检测到16个0x7f，那么认为是下载流
            	{							//
                 	IAP_CONTR=0X60; 		//跳转到ISP监控区
            	}										  								
        	}							
        	else						    
        	{							   
            	cnt7f = 0;				    
        	}
    switch (c)  {                    /* process character                   */
      case CTRL_S:
        sendstop = 1;                /* if Control+S stop transmission      */
        break;

      case CTRL_Q:
        start_trans = sendstop;      /* if Control+Q start transmission     */
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

  if (TI || start_trans)  {          // if transmitter interrupt            
    TI = 0;                         // clear interrupt request flag        
		if(!bufempty&&!sendstop) {
			SBUF = outbuf[ostart];
			if(++ostart>= OLEN)	ostart = 0;
			if(ostart == oend) bufempty = 1;
			sendfull = 0;
		}
    else sendactive = 0;             //if all transmitted clear 'sendactive
  }

}


void Serialbegin(u32 baud)
{
	u8	i;
	u32	j;
	  
		for(i=0; i<OLEN; i++)	outbuf[i] = 0;
		for(i=0; i<ILEN; i++)	inbuf[i] = 0;
		PS = 1;	//高优先级中断
		SCON = (SCON & 0x3f) | UART_8bit_BRTx;
		j = (MAIN_Fosc / 4) / baud;	//按1T计算
		if(j >= 65536UL)	return ;	//错误
		j = 65536UL - j;
		AUXR &= ~(1<<4);	//Timer stop
		AUXR |= 0x01;		//S1 BRT Use Timer2;
		AUXR &= ~(1<<3);	//Timer2 set As Timer
		AUXR |=  (1<<2);	//Timer2 set as 1T mode
		T2H = (u8)(j>>8);
		T2L = (u8)j;
		IE2  &= ~(1<<2);	//禁止中断
		AUXR &= ~(1<<3);	//定时
		AUXR |=  (1<<4);	//Timer run enable
		ES = 1;	//允许中断
		REN = 1;	//允许接收
		P_SW1 = (P_SW1 & 0x3f) | (UART1_SW_P30_P31 & 0xc0);	//切换IO
		PCON2 &= ~(1<<4);
		EA=1;//开总中断
}
