//========================================================================
//	STCunio Studio淘宝http://shop112966725.taobao.com
//	STCunio Studio QQ群:117112538
//	作者：mingbozhu
//	联系方式：13724273732
//	邮箱：mingbozhu@yeah.net
//	修改日期2014.11.7
//	版本V1.1
//========================================================================


#include	"spi.h"
#include  "GPIO.h"
void 	SPIsetGroup(u8 Group)
{
	AUXR1 = (AUXR1 & ~(3<<2)) | Group;	
}
void	SPIbegin(void)
{
	SPCTL |=  (1<<6);//SPI enable	
	SPCTL |=  (1<<7);	//enable SS, conform Master or Slave by SS pin.
	SPCTL |=  (1<<4);	//master	
	
}
void SPIsetBitOrder(u8 bitOrder)
{
		if(bitOrder==LSBFIRST)SPCTL |= ~(1<<5);	//LSB first
		else SPCTL &= ~(1<<5);	//MSB first			
}
void SPIsetClockDivider(u8 rate)
{
		SPCTL = (SPCTL & ~3) | (rate & 3);					//set speed

}
//模式0 上升沿采样 下降沿移位 SCK闲置时为0 SPI_MODE0 SPCTL &= ~(1<<3);SPCTL  &= ~(1<<2)
//模式1 上升沿移位 下降沿采样							 SPI_MODE1 SPCTL &= ~(1<<3);SPCTL  |=  (1<<2)
//模式2 下降沿采样 上升沿移位 SCK闲置时为1 SPI_MODE2 SPCTL |=  (1<<3);SPCTL  &= ~(1<<2)
//模式3 下降沿移位 上升沿采样							 SPI_MODE2 SPCTL |=  (1<<3);SPCTL  |=  (1<<2)
void SPIsetDataMode(u8 mode)
{
		SPCTL =(SPCTL & ~(3<<2))|(mode<<2);
}
u8 SPItransfer(u8 _data)
{
	  SPDAT=_data;
		while (!(SPSTAT & 0x80));
		SPSTAT &=0x80;
    return SPDAT;
}
void SPIend()
{
      SPCTL &= ~(1<<6);
}