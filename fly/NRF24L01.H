
#ifndef __NRF24L01_H
#define __NRF24L01_H

typedef unsigned char uchar;
typedef unsigned char uint;

sbit	CE	    = P6 ^ 1;
sbit	CSN		= P6 ^ 2;
sbit	SCK	    = P6 ^ 3;
sbit 	MOSI	= P1 ^ 0;
sbit 	MISO	= P1 ^ 1;
sbit	IRQ		= P4 ^ 7;

sbit P_24L01_CE   = P6 ^ 1;
sbit P_24L01_CSN  = P6 ^ 2;
sbit P_24L01_SCK  = P6 ^ 3;
sbit P_24L01_MOSI = P1 ^ 0;
sbit P_24L01_MISO = P1 ^ 1;
sbit P_24L01_IRQ  = P4 ^ 7;

void nRF24L01_TxPacket(u8 * tx_buf);
void Delay(unsigned int s);
void inerDelay_us(u8 n);
void init_NRF24L01(void);
u8 SPI_RW(u8 dat);
u8 SPI_Read(u8 reg);

u8 SPI_RW_Reg(u8 reg, u8 value);
u8 SPI_Read_Buf(u8 reg, u8 *pBuf, u8 uchars);
u8 SPI_Write_Buf(u8 reg, u8 *pBuf, u8 uchars);

void nrf_RxTx(uchar mod_nrf, uchar *buff) ;
int getTxDS() ;
int getRxDR() ;

void SetRX_Mode(void) ;
u8 nRF24L01_RxPacket(u8* rx_buf) ;

#endif