
#include <intrins.h>
#include "STC15W4K60S4.H"
#include "EEPROM.h"
#include "NRF24L01.H"
#include "MPU6050.H"

#include "CONSTANT.H"

#define CMD_IDLE    0
#define CMD_READ    1
#define CMD_PROGRAM 2
#define CMD_ERASE   3

#define ENABLE_IAP 0x80           //if SYSCLK<30MHz
extern int   xdata g_x, g_y, g_z;		//陀螺仪矫正参数
extern float xdata a_x, a_y;			//角度矫正参数
extern float data Angle, Angley;

u8	xdata	tmp[20];

//========================================================================
// 函数: void	DisableEEPROM(void)
// 描述: 禁止访问ISP/IAP.
// 参数: non.
// 返回: non.
// 版本: V1.0, 2012-10-22
//========================================================================
void	DisableEEPROM(void)
{
	IAP_CONTR = 0;			//禁止ISP/IAP操作
	IAP_CMD   = 0;			//去除ISP/IAP命令
	IAP_TRIG  = 0;			//防止ISP/IAP命令误触发
	IAP_ADDRH = 0xff;		//清0地址高字节
	IAP_ADDRL = 0xff;		//清0地址低字节，指向非EEPROM区，防止误操作
}

//========================================================================
// 函数: void EEPROM_read_n(u16 EE_address,u8 *DataAddress,u16 number)
// 描述: 从指定EEPROM首地址读出n个字节放指定的缓冲.
// 参数: EE_address:  读出EEPROM的首地址.
//       DataAddress: 读出数据放缓冲的首地址.
//       number:      读出的字节长度.
// 返回: non.
// 版本: V1.0, 2012-10-22
//========================================================================

void EEPROM_read_n(u16 EE_address, u8 *DataAddress, u16 number)
{
	F0 = EA;
	EA = 0;		//禁止中断
	IAP_CONTR = ENABLE_IAP;
	IAP_CMD   = CMD_READ;
	do
	{
		IAP_ADDRH = EE_address / 256;		//送地址高字节（地址需要改变时才需重新送地址）
		IAP_ADDRL = EE_address % 256;		//送地址低字节
		IAP_TRIG  = 0x5a;	IAP_TRIG  = 0xa5;
		_nop_();
		*DataAddress = IAP_DATA;			//读出的数据送往
		EE_address++;
		DataAddress++;
	} while (--number);

	DisableEEPROM();
	EA = F0;		//重新允许中断
}


/******************** 扇区擦除函数 *****************/
//========================================================================
// 函数: void EEPROM_SectorErase(u16 EE_address)
// 描述: 把指定地址的EEPROM扇区擦除.
// 参数: EE_address:  要擦除的扇区EEPROM的地址.
// 返回: non.
// 版本: V1.0, 2013-5-10
//========================================================================
void EEPROM_SectorErase(u16 EE_address)
{
	F0 = EA;
	EA = 0;		//禁止中断
	IAP_ADDRH = EE_address / 256;			//送扇区地址高字节（地址需要改变时才需重新送地址）
	IAP_ADDRL = EE_address % 256;			//送扇区地址低字节
	IAP_CONTR = ENABLE_IAP;
	IAP_CMD   = CMD_ERASE;
	IAP_TRIG  = 0x5a;	IAP_TRIG  = 0xa5;
	_nop_();
	DisableEEPROM();
	EA = F0;		//重新允许中断
}

//========================================================================
// 函数: void EEPROM_write_n(u16 EE_address,u8 *DataAddress,u16 number)
// 描述: 把缓冲的n个字节写入指定首地址的EEPROM.
// 参数: EE_address:  写入EEPROM的首地址.
//       DataAddress: 写入源数据的缓冲的首地址.
//       number:      写入的字节长度.
// 返回: non.
// 版本: V1.0, 2012-10-22
//========================================================================
void EEPROM_write_n(u16 EE_address, u8 *DataAddress, u16 number)
{
	F0 = EA;
	EA = 0;		//禁止中断
	IAP_CONTR = ENABLE_IAP;
	IAP_CMD   = CMD_PROGRAM;
	do
	{
		IAP_ADDRH = EE_address / 256;		//送地址高字节（地址需要改变时才需重新送地址）
		IAP_ADDRL = EE_address % 256;		//送地址低字节
		IAP_DATA  = *DataAddress;			//送数据到IAP_DATA，只有数据改变时才需重新送
		IAP_TRIG  = 0x5a;	IAP_TRIG  = 0xa5;
		_nop_();
		EE_address++;
		DataAddress++;
	} while (--number);

	DisableEEPROM();
	EA = F0;		//重新允许中断
}

extern	u8	xdata	tp[];

void IAP_Gyro(void)   //陀螺仪校准
{
	u8	i, j;
	g_x = ((int *)&tp)[4];  //读取陀螺仪数据
	g_y = ((int *)&tp)[5];
	g_z = ((int *)&tp)[6];

	((int *)&tmp)[0] = g_x;
	((int *)&tmp)[1] = g_y;
	((int *)&tmp)[2] = g_z;
	((int *)&tmp)[3] = 0x55aa;

	((float *)&tmp)[2] = Angle;
	((float *)&tmp)[3] = Angley;
	a_x = Angle;
	a_y = Angley;


	for (i = 0, j = 0; i < 16; i++)	j += tmp[i];
	j = ~j + 1;
	tmp[i] = j;

	EEPROM_SectorErase(_EF_PROM); //扇区擦除
	EEPROM_write_n(_EF_PROM, tmp, 17);
}

void IAPRead(void)		//读取陀螺仪静差
{
	u8	i, j;

	EEPROM_read_n(_EF_PROM, tmp, 17);
	for (i = 0, j = 0; i < 17; i++)	j += tmp[i];

	if ((j == 0) && (((int *)&tmp)[3] == 0x55aa))		//判断数据是否已经更新过，更新过才读取数据
	{
		g_x = ((int *)&tmp)[0];
		g_y = ((int *)&tmp)[1];
		g_z = ((int *)&tmp)[2];
		a_x = ((float *)&tmp)[2];
		a_y = ((float *)&tmp)[3];
	}
	else
	{
		g_x = 0;
		g_y = 0;
		g_z = 0;
		a_x = 0;
		a_y = 0;
	}

}

