
#ifndef __NRF24L01_H
#define __NRF24L01_H

sbit	CE	    = P2^0;
sbit	CSN		= P4^4;
sbit	IRQ		= P3^2;
sbit	SCK	    = P2^1;
//sbit 	MOSI	= P2^2;//修改前
//sbit 	MISO	= P2^3;
sbit 	MOSI	= P2^3;//修改后
sbit 	MISO	= P2^2;

sbit P_24L01_CE   = P2^0;
sbit P_24L01_CSN  = P4^4;
sbit P_24L01_SCK  = P2^1;
//sbit P_24L01_MOSI = P2^2;//修改前
//sbit P_24L01_MISO = P2^3;
sbit P_24L01_MOSI = P2^3;//修改后
sbit P_24L01_MISO = P2^2;

sbit P_24L01_IRQ  = P3^2;

sbit ACC0 = ACC^0;
sbit ACC1 = ACC^1;
sbit ACC2 = ACC^2;
sbit ACC3 = ACC^3;
sbit ACC4 = ACC^4;
sbit ACC5 = ACC^5;
sbit ACC6 = ACC^6;
sbit ACC7 = ACC^7;

sbit B0 = B^0;
sbit B1 = B^1;
sbit B2 = B^2;
sbit B3 = B^3;
sbit B4 = B^4;
sbit B5 = B^5;
sbit B6 = B^6;
sbit B7 = B^7;
void nRF24L01_TxPacket(u8 * tx_buf);
void init_NRF24L01(void);
u8 SPI_RW(u8 dat);
u8 SPI_Read(u8 reg);
void SetRX_Mode(void);
u8 SPI_RW_Reg(u8 reg, u8 value);
u8 SPI_Read_Buf(u8 reg, u8 *pBuf, u8 uchars);
u8 SPI_Write_Buf(u8 reg, u8 *pBuf, u8 uchars);
u8 nRF24L01_RxPacket(u8 *rx_buf);

#endif  