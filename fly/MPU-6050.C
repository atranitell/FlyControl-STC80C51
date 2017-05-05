
#include <intrins.h>

#include "STC15W4K60S4.H"
#include "MPU6050.H"
#include "NRF24L01.H"

sbit    SCL = P0 ^ 0;			//IIC时钟引脚定义    Rev8.0硬件
sbit    SDA = P4 ^ 6;			//IIC数据引脚定义
//sbit    SCL=P2^5;			//IIC时钟引脚定义      Rev7.0硬件
//sbit    SDA=P2^6;			//IIC数据引脚定义

void  InitMPU6050(void);	//初始化MPU6050
void  Delay2us(void);
void  I2C_Start(void);
void  I2C_Stop(void);

bit   I2C_RecvACK(void);

void  I2C_SendByte(u8 dat);
u8	  I2C_RecvByte(void);

void  I2C_ReadPage(void);
void  I2C_WritePage(void);
u8	  Single_ReadI2C(u8 REG_Address);						//读取I2C数据
void  Single_WriteI2C(u8 REG_Address, u8 REG_data);	//向I2C写入数据


//I^C时序中延时设置，具体参见各芯片的数据手册  6050推荐最小1.3us 但是会出问题，这里延时实际1.9us左右
void Delay2us(void)
{
	u8 i;
	i = 14;//11;
	while (--i);
}

//**************************************
//I2C起始信号
//**************************************
void I2C_Start(void)
{
	SDA = 1;                    //拉高数据线
	SCL = 1;                    //拉高时钟线
	Delay2us();                 //延时
	SDA = 0;                    //产生下降沿
	Delay2us();                 //延时
	SCL = 0;                    //拉低时钟线
}

//**************************************
//I2C停止信号
//**************************************
void I2C_Stop(void)
{
	SDA = 0;                    //拉低数据线
	SCL = 1;                    //拉高时钟线
	Delay2us();                 //延时
	SDA = 1;                    //产生上升沿
	Delay2us();                 //延时
}

//**************************************
//I2C接收应答信号
//**************************************
bit I2C_RecvACK(void)
{
	SCL = 1;                    //拉高时钟线
	Delay2us();                 //延时
	CY = SDA;                   //读应答信号
	SCL = 0;                    //拉低时钟线
	Delay2us();                 //延时
	return CY;
}

//**************************************
//向I2C总线发送一个字节数据
//**************************************
void I2C_SendByte(u8 dat)
{
	u8 i;
	for (i = 0; i < 8; i++)     //8位计数器
	{
		dat <<= 1;              //移出数据的最高位
		SDA = CY;               //送数据口
		SCL = 1;                //拉高时钟线
		Delay2us();             //延时
		SCL = 0;                //拉低时钟线
		Delay2us();             //延时
	}
	I2C_RecvACK();
}

//**************************************
//从I2C总线接收一个字节数据
//**************************************
u8 I2C_RecvByte(void)
{
	u8 i;
	u8 dat = 0;
	SDA = 1;                    //使能内部上拉,准备读取数据,
	for (i = 0; i < 8; i++)     //8位计数器
	{
		dat <<= 1;
		SCL = 1;                //拉高时钟线
		Delay2us();             //延时
		dat |= SDA;             //读数据
		SCL = 0;                //拉低时钟线
		Delay2us();             //延时
	}
	return dat;
}

//**************************************
//向I2C设备写入一个字节数据
//**************************************
void Single_WriteI2C(u8 REG_Address, u8 REG_data)
{
	I2C_Start();                  //起始信号
	I2C_SendByte(SlaveAddress);   //发送设备地址+写信号
	I2C_SendByte(REG_Address);    //内部寄存器地址，
	I2C_SendByte(REG_data);       //内部寄存器数据，
	I2C_Stop();                   //发送停止信号
}
//**************************************
//从I2C设备读取一个字节数据
//**************************************
/*
u8 Single_ReadI2C(u8 REG_Address)
{
	u8 REG_data;
	I2C_Start();                   //起始信号
	I2C_SendByte(SlaveAddress);    //发送设备地址+写信号
	I2C_SendByte(REG_Address);     //发送存储单元地址，从0开始
	I2C_Start();                   //起始信号
	I2C_SendByte(SlaveAddress+1);  //发送设备地址+读信号
	REG_data=I2C_RecvByte();       //读出寄存器数据

	SDA = 1;                    //写应答信号
	SCL = 1;                    //拉高时钟线
	Delay2us();                 //延时
	SCL = 0;                    //拉低时钟线
	Delay2us();                 //延时

	I2C_Stop();                    //停止信号
	return REG_data;
}
*/

//**************************************
//初始化MPU6050
//**************************************
void InitMPU6050(void)
{
	Single_WriteI2C(PWR_MGMT_1, 0x00);	//解除休眠状态
	Single_WriteI2C(SMPLRT_DIV, 0x07);  //陀螺仪125hz
	Single_WriteI2C(CONFIG, 0x04);      //21HZ滤波 延时A8.5ms G8.3ms  此处取值应相当注意，延时与系统周期相近为宜
	Single_WriteI2C(GYRO_CONFIG, 0x08); //陀螺仪500度/S 65.5LSB/g
	Single_WriteI2C(ACCEL_CONFIG, 0x08);//加速度+-4g  8192LSB/g
}

//**************************************
//合成数据
//**************************************
/*
int GetData(u8 REG_Address)
{
	char H,L;
	H = Single_ReadI2C(REG_Address);
	L = Single_ReadI2C(REG_Address+1);
//	return (H<<8)+L;   //合成数据
	return (((u16)H << 8) + L);   //合成数据
}
*/

void Read_MPU6050(u8 *buf)
{
	u8	i;

	I2C_Start();                  //起始信号
	I2C_SendByte(SlaveAddress);   //发送设备地址+写信号
	I2C_SendByte(ACCEL_XOUT_H);    //内部寄存器地址，
	I2C_Start();                   //起始信号
	I2C_SendByte(SlaveAddress + 1); //发送设备地址+读信号
	for (i = 0; i < 13; i++)
	{
		buf[i] = I2C_RecvByte();       //读出寄存器数据
		SDA = 0;                    //写应答信号
		SCL = 1;                    //拉高时钟线
		Delay2us();                 //延时
		SCL = 0;                    //拉低时钟线
		Delay2us();                 //延时
	}
	buf[i] = I2C_RecvByte();	//最后一个字节
	SDA = 1;                    //写非应答信号
	SCL = 1;                    //拉高时钟线
	Delay2us();                 //延时
	SCL = 0;                    //拉低时钟线
	Delay2us();                 //延时
	I2C_Stop();                    //停止信号
}
