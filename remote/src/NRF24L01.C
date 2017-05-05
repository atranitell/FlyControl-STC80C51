
#include <intrins.h>

#include "config.h"
#include "spi.h"
#include "NRF24L01.H"
#include"delay.h"
//*********************************************NRF24L01*************************************
#define TX_ADR_WIDTH    5   	// 5 uints TX address width
#define RX_ADR_WIDTH    5   	// 5 uints RX address width
#define TX_PLOAD_WIDTH  20  	// 20 uints TX payload
#define RX_PLOAD_WIDTH  20  	// 20 uints TX payload
u8 code TX_ADDRESS[TX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01};	//本地地址
u8 code RX_ADDRESS[RX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01};	//接收地址

//***************************************NRF24L01寄存器指令*******************************************************
#define READ_REG        0x00  	// 读寄存器指令
#define WRITE_REG       0x20 	// 写寄存器指令
#define RD_RX_PLOAD     0x61  	// 读取接收数据指令
#define WR_TX_PLOAD     0xA0  	// 写待发数据指令
#define FLUSH_TX        0xE1 	// 冲洗发送 FIFO指令
#define FLUSH_RX        0xE2  	// 冲洗接收 FIFO指令
#define REUSE_TX_PL     0xE3  	// 定义重复装载数据指令
//#define NOP             0xFF  	// 保留
//*************************************SPI(nRF24L01)寄存器地址****************************************************
#define CONFIG2         0x00  // 配置收发状态，CRC校验模式以及收发状态响应方式
#define EN_AA           0x01  // 自动应答功能设置
#define EN_RXADDR       0x02  // 可用信道设置
#define SETUP_AW        0x03  // 收发地址宽度设置
#define SETUP_RETR      0x04  // 自动重发功能设置
#define RF_CH           0x05  // 工作频率设置
#define RF_SETUP        0x06  // 发射速率、功耗功能设置
#define STATUS          0x07  // 状态寄存器
#define OBSERVE_TX      0x08  // 发送监测功能
#define CD              0x09  // 地址检测           
#define RX_ADDR_P0      0x0A  // 频道0接收数据地址
#define RX_ADDR_P1      0x0B  // 频道1接收数据地址
#define RX_ADDR_P2      0x0C  // 频道2接收数据地址
#define RX_ADDR_P3      0x0D  // 频道3接收数据地址
#define RX_ADDR_P4      0x0E  // 频道4接收数据地址
#define RX_ADDR_P5      0x0F  // 频道5接收数据地址
#define TX_ADDR         0x10  // 发送地址寄存器
#define RX_PW_P0        0x11  // 接收频道0接收数据长度
#define RX_PW_P1        0x12  // 接收频道0接收数据长度
#define RX_PW_P2        0x13  // 接收频道0接收数据长度
#define RX_PW_P3        0x14  // 接收频道0接收数据长度
#define RX_PW_P4        0x15  // 接收频道0接收数据长度
#define RX_PW_P5        0x16  // 接收频道0接收数据长度
#define FIFO_STATUS     0x17  // FIFO栈入栈出状态寄存器设置

//******************************************************************************************
u8 		bdata sta;   //状态标志
sbit	RX_DR	=sta^6;
sbit	TX_DS	=sta^5;
sbit	MAX_RT	=sta^4;
sbit  TX_FULL =sta^0;

//****************************************************************************************
/*NRF24L01初始化
//***************************************************************************************/
void init_NRF24L01(void)
{
 	CE=0;    // chip enable
 	CSN=1;   // Spi  disable 
 	SCK=0;   // 
	SPI_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);    // 写本地地址	
	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH); // 写接收端地址
	SPI_RW_Reg(WRITE_REG + EN_AA, 0x01);      //  频道0自动	ACK应答允许	
	SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);  //  允许接收地址只有频道0，如果需要多频道可以参考Page21  
	SPI_RW_Reg(WRITE_REG + RF_CH, 0x6e);        //   设置信道工作为2.4GHZ，收发必须一致
	SPI_RW_Reg(WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH); //设置接收数据长度，本次设置为32字节
	SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x27);   		//设置发射速率为1MB/S，发射功率为最大值+7dB，由于有X2401L功放，实际+21dbm输出
}
/****************************************************************************************************
/*函数：u8 SPI_RW(u8 uchar)
/*功能：NRF24L01的SPI写时序
/****************************************************************************************************/
u8 SPI_RW(u8 dat)
{
    B = dat;
	P_24L01_MOSI = B7;	/*_nop_();*/	P_24L01_SCK = 1;	B7 = P_24L01_MISO;	P_24L01_SCK = 0;
	P_24L01_MOSI = B6;	/*_nop_();*/	P_24L01_SCK = 1;	B6 = P_24L01_MISO;	P_24L01_SCK = 0;
	P_24L01_MOSI = B5;	/*_nop_();*/	P_24L01_SCK = 1;	B5 = P_24L01_MISO;	P_24L01_SCK = 0;
	P_24L01_MOSI = B4;	/*_nop_();*/	P_24L01_SCK = 1;	B4 = P_24L01_MISO;	P_24L01_SCK = 0;
	P_24L01_MOSI = B3;	/*_nop_();*/	P_24L01_SCK = 1;	B3 = P_24L01_MISO;	P_24L01_SCK = 0;
	P_24L01_MOSI = B2;	/*_nop_();*/	P_24L01_SCK = 1;	B2 = P_24L01_MISO;	P_24L01_SCK = 0;
	P_24L01_MOSI = B1;	/*_nop_();*/	P_24L01_SCK = 1;	B1 = P_24L01_MISO;	P_24L01_SCK = 0;
	P_24L01_MOSI = B0;	/*_nop_();*/	P_24L01_SCK = 1;	B0 = P_24L01_MISO;	P_24L01_SCK = 0;
	P_24L01_MOSI = 0;
    return(B);           		  // return read byte
}
/*u8 SPI_RW(u8 dat)
{
		return(SPItransfer(dat));

}*/
/****************************************************************************************************
/*函数：uchar SPI_Read(uchar reg)
/*功能：NRF24L01的SPI时序
/****************************************************************************************************/

u8 SPI_Read(u8 reg)
{
	u8 reg_val;
	
	CSN = 0;                // CSN low, initialize SPI communication...
	delay_us(12);
	SPI_RW(reg);            // Select register to read from..
	delay_us(12);
	reg_val = SPI_RW(0);    // ..then read registervalue
	delay_us(12);
	CSN = 1;                // CSN high, terminate SPI communication
	
	return(reg_val);        // return register value
}


/****************************************************************************************************/
/*功能：NRF24L01读写寄存器函数
/****************************************************************************************************/
u8 SPI_RW_Reg(u8 reg, u8 value)
{
	u8 status;
	
	CSN = 0;                   // CSN low, init SPI transaction
	status = SPI_RW(reg);      // select register
	SPI_RW(value);             // ..and write value to it..
	CSN = 1;                   // CSN high again
	
	return(status);            // return nRF24L01 status
}
/****************************************************************************************************/
/*函数：u8 SPI_Read_Buf(u8 reg, u8 *pBuf, u8 uchars)
/*功能: 用于读数据，reg：为寄存器地址，pBuf：为待读出数据地址，uchars：读出数据的个数
/****************************************************************************************************/
u8 SPI_Read_Buf(u8 reg, u8 *pBuf, u8 uchars)
{
	u8 status,uchar_ctr;
	
	CSN = 0;                    		// Set CSN low, init SPI tranaction
	status = SPI_RW(reg);       		// Select register to write to and read status
	
	for(uchar_ctr=0; uchar_ctr<uchars; uchar_ctr++)
		pBuf[uchar_ctr] = SPI_RW(0);    // 
	
	CSN = 1;                           
	
	return(status);                    // return nRF24L01 status
}


/*********************************************************************************************************
/*函数：u8 SPI_Write_Buf(uchar reg, uchar *pBuf, uchar uchars)
/*功能: 用于写数据：为寄存器地址，pBuf：为待写入数据地址，uchars：写入数据的个数
/*********************************************************************************************************/
u8 SPI_Write_Buf(u8 reg, u8 *pBuf, u8 uchars)
{
	u8 status,uchar_ctr;
	
	CSN = 0;            //SPI使能       
	status = SPI_RW(reg);   
	for(uchar_ctr=0; uchar_ctr<uchars; uchar_ctr++) //
	SPI_RW(*pBuf++);
	CSN = 1;           //关闭SPI
	return(status);    // 
}
/****************************************************************************************************/
/*函数：void SetRX_Mode(void)
/*功能：数据接收配置 
/****************************************************************************************************/
void SetRX_Mode(void)
{
	CE=0;
	SPI_RW_Reg(WRITE_REG + CONFIG2, 0x0f);   		// IRQ收发完成中断响应，16位CRC	，主接收
	CE = 1; 
}
/******************************************************************************************************/
/*函数：u8 nRF24L01_RxPacket(u8* rx_buf)
/*功能：数据读取后放如rx_buf接收缓冲区中
/******************************************************************************************************/

u8 nRF24L01_RxPacket(u8* rx_buf)
{
  u8 revale=0;
	
	sta=SPI_Read(STATUS);	// 读取状态寄存其来判断数据接收状况
	CE = 0; 			
	if(RX_DR)				// 判断是否接收到数据
	{
	    
		SPI_Read_Buf(RD_RX_PLOAD,rx_buf,TX_PLOAD_WIDTH);// read receive payload from RX_FIFO buffer
		revale =1;			//读取数据完成标志
	}
	SPI_RW_Reg(WRITE_REG+STATUS,sta);   //接收到数据后RX_DR,TX_DS,MAX_PT都置高为1，通过写1来清楚中断标志
	SPI_RW_Reg(0xE2,0xff);//清楚RX FIFO的数据
	CE=1;                                                                     //！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
	return revale;
}


/***********************************************************************************************************
/*函数：void nRF24L01_TxPacket(u8 * tx_buf)
/*功能：发送 tx_buf中数据
/**********************************************************************************************************/

void nRF24L01_TxPacket(u8 * tx_buf)
{
	//SPI_RW_Reg(WRITE_REG+STATUS,0xff);
	SPI_RW_Reg(0xE1,0xff);//清楚TX FIFO的数据
	CE=0;		

	SPI_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH); 			     
	SPI_RW_Reg(WRITE_REG + CONFIG2, 0x0e); 
	SPI_RW_Reg(WRITE_REG+STATUS,0xff);
	CE=1;		 
	delay_us(10);   //CE高电平大于10us才能进入发射模式
}


