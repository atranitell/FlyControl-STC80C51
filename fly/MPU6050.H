#ifndef __MPU6050_H
#define __MPU6050_H
							
#include "CONSTANT.H"

void  InitMPU6050(void);												
void  Delay2us(void);
void  I2C_Start(void);
void  I2C_Stop(void);
bit   I2C_RecvACK(void);
void  I2C_SendByte(u8 dat);
u8	  I2C_RecvByte(void);
void  I2C_ReadPage(void);
void  I2C_WritePage(void);
u8	  Single_ReadI2C(u8 REG_Address);					
void  Single_WriteI2C(u8 REG_Address,u8 REG_data);
void  Read_MPU6050(u8 *buf);

#endif  