#include"epwm.h"
//========================================================================
// 函数: epwmnTcolck(u16 nTclock)
// 描述: pwm时钟分频函数。
// 参数: nTclock，1~16分频.
// 返回: none.
// 版本: VER1.1
// 日期: 2014-11-17
// 备注: 
//========================================================================
void epwmnTcolck(u8 nTclock)//pwm时钟
{
    P_SW2 |= 0x80;//使用外部扩展SFR
    PWMCKS = nTclock-1;//分频
    P_SW2 &= ~0x80;//退出外部扩展SFR 
}
//========================================================================
// 函数: epwmTwide(u16 Twide)
// 描述: pwm周期宽度设置。
// 参数: Twide，15位.
// 返回: none.
// 版本: VER1.1
// 日期: 2014-11-17
// 备注: 
//========================================================================
void epwmTwide(u16 Twide)
{
    P_SW2 |= 0x80;//使用外部扩展SFR 
    PWMC=Twide;
    P_SW2 &= ~0x80;//退出外部扩展SFR 
}
//========================================================================
// 函数: epwmStartADC(u8 choice)
// 描述: pwm触发AD事件函数。
// 参数: choice，1：触发AD；0：不触发AD.
// 返回: none.
// 版本: VER1.1
// 日期: 2014-11-17
// 备注: 
//========================================================================
void epwmStartADC(u8 StartADC_EN)
{
    PWMCFG = (PWMCFG&0x3f)|(StartADC_EN<<6);
}
//========================================================================
// 函数: epwmFaultSource(u8 FaultSource,u8 FaultCloseIO)
// 描述: 设置pwm故障源函数。
// 参数: FaultSource，1：检测比较器异常；2：检测P24异常；3：检测两者异常.
//       FaultCloseIO，1：pwm异常则关闭pwm输出；0：pwm异常不影响pwm输出
// 返回: none.
// 版本: VER1.1
// 日期: 2014-11-17
// 备注: 
//========================================================================
void epwmFaultSource(u8 FaultSource,u8 FaultCloseIO)
{
    PWMFDCR |= 0x20;//使能PWM异常检测
    PWMFDCR  = (PWMFDCR&0xf3)|FaultSource;   
    PWMFDCR  = (PWMFDCR&0xef)|FaultCloseIO;
}
//========================================================================
// 函数: epwmFirstSignalLevel(u8 pin,u8 value)
// 描述: 设置pwm初始电平函数。
// 参数: pin，选择需要的pwm所在的引脚.
//       value，1：pwm初始输出高；0：pwm初始输出低
// 返回: none.
// 版本: VER1.1
// 日期: 2014-11-17
// 备注: 
//========================================================================
void epwmFirstSignalLevel(u8 pin,u8 value)//设置初始电平
{   
    
    switch(pin)
    {
        case 7 :         //第1组PWM2
                P_SW2 |= 0x80;//使用外部扩展SFR 
                PWM2CR&= ~0x08;//选择P3.7为PWM2
                P_SW2 &= ~0x80;//退出外部扩展SFR 
                PWMCFG = (PWMCFG&0xfe)|value; 
                PWMCR |= 0x01;
                break;
        case 9 :         //第1组PWM3
                P_SW2 |= 0x80;//使用外部扩展SFR 
                PWM3CR&= ~0x08;//选择P2.1为PWM3
                P_SW2 &= ~0x80;//退出外部扩展SFR
                PWMCFG =(PWMCFG&0xfd)|(value<<1);
                PWMCR |= 0x02;
                break;
        case 10 :       //第1组PWM4
                P_SW2 |= 0x80;//使用外部扩展SFR 
                PWM4CR&= ~0x08;//选择P2.2为PWM4
                P_SW2 &= ~0x80;//退出外部扩展SFR
                PWMCFG =(PWMCFG&0xfb)|(value<<2);
                PWMCR |= 0x04;
                break;
        case 11 :       //第1组PWM5
                P_SW2 |= 0x80;//使用外部扩展SFR 
                PWM5CR&= ~0x08;//选择P2.3为PWM5
                P_SW2 &= ~0x80;//退出外部扩展SFR
                PWMCFG =(PWMCFG&0xf7)|(value<<3);
                PWMCR |= 0x08;
                break;
        case 62 :       //第1组PWM6
                P_SW2 |= 0x80;//使用外部扩展SFR 
                PWM6CR&= ~0x08;//选择P1.6为PWM6
                P_SW2 &= ~0x80;//退出外部扩展SFR 
                PWMCFG =(PWMCFG&0xef)|(value<<4);
                PWMCR |= 0x10;
                break;
        case 63 :       //第1组PWM7
                P_SW2 |= 0x80;//使用外部扩展SFR 
                PWM7CR&= ~0x08;//选择P1.7为PWM7
                P_SW2 &= ~0x80;//退出外部扩展SFR 
                PWMCFG =(PWMCFG&0xdf)|(value<<5);
                PWMCR |= 0x20;          
                break;
        case 15 :       //第2组PWM2
                P_SW2 |= 0x80;//使用外部扩展SFR 
                PWM2CR|= 0x08;//选择P2.7为PWM2
                P_SW2 &= ~0x80;//退出外部扩展SFR 
                PWMCFG =(PWMCFG&0xfe)|value;
                PWMCR |= 0x01;
                break;
        case 21 :       //第2组PWM3
                P_SW2 |= 0x80;//使用外部扩展SFR 
                PWM3CR|= 0x08;//选择P4.5为PWM3
                P_SW2 &= ~0x80;//退出外部扩展SFR 
                PWMCFG =(PWMCFG&0xfd)|(value<<1);
                PWMCR |= 0x02;
                break;
        
        case 20 :       //第2组PWM4
                P_SW2 |= 0x80;//使用外部扩展SFR 
                PWM4CR|= 0x08;//选择P4.4为PWM4
                P_SW2 &= ~0x80;//退出外部扩展SFR 
                PWMCFG =(PWMCFG&0xfb)|(value<<2);
                PWMCR |= 0x04;
                break;
        
        case 18 :       //第2组PWM5
                P_SW2 |= 0x80;//使用外部扩展SFR 
                PWM5CR|= 0x08;//选择P4.2为PWM5
                P_SW2 &= ~0x80;//退出外部扩展SFR 
                PWMCFG =(PWMCFG&0xf7)|(value<<3);
                PWMCR |= 0x08;
                break;
        
        case 47 :       //第2组PWM6
                P_SW2 |= 0x80;//使用外部扩展SFR 
                PWM6CR|= 0x08;//选择P0.7为PWM6
                P_SW2 &= ~0x80;//退出外部扩展SFR 
                PWMCFG =(PWMCFG&0xef)|(value<<4);
                PWMCR |= 0x10;
                break;
        
        case 46 :       //第2组PWM7
                P_SW2 |= 0x80;//使用外部扩展SFR 
                PWM7CR|= 0x08;//选择P0.6为PWM7
                P_SW2 &= ~0x80;//退出外部扩展SFR 
                PWMCFG =(PWMCFG&0xdf)|(value<<5);
                PWMCR |= 0x20;
                break;
                
        default:               
                break;
    }
    
}
//========================================================================
// 函数: epwmSetValue(u8 pin,u16 PWMT1,u16 PWMT2)
// 描述: 设置pwm的翻转值函数。
// 参数: pin，选择需要的pwm所在的引脚。
//       PWMT1，第一个翻转值，不可超过第二个翻转值。
//       PWMT2，第二个翻转值，不可超过周期。
// 返回: none.
// 版本: VER1.1
// 日期: 2014-11-17
// 备注: 
//========================================================================
void epwmSetValue(u8 pin,u16 PWMT1,u16 PWMT2)
{
    switch(pin)
    {
       case 7 :         //第1组PWM2
       case 15 :       //第2组PWM2
                P_SW2 |= 0x80;//使用外部扩展SFR 
                PWM2T1 = PWMT1;//
                PWM2T2 = PWMT2;                
                P_SW2 &= ~0x80;//退出外部扩展SFR
                break;
       
        case 9 :         //第1组PWM3
        case 21 :       //第2组PWM3
                P_SW2 |= 0x80;//使用外部扩展SFR 
                PWM3T1 = PWMT1;//
                PWM3T2 = PWMT2;
                P_SW2 &= ~0x80;//退出外部扩展SFR

                break;
        case 10 :       //第1组PWM4
        case 20 :       //第2组PWM4
                P_SW2 |= 0x80;//使用外部扩展SFR 
                PWM4T1 = PWMT1;//
                PWM4T2 = PWMT2;
                P_SW2 &= ~0x80;//退出外部扩展SFR

                break;
        case 11 :       //第1组PWM5
        case 18 :       //第2组PWM5 
                P_SW2 |= 0x80;//使用外部扩展SFR 
                PWM5T1 = PWMT1;//
                PWM5T2 = PWMT2;
                P_SW2 &= ~0x80;//退出外部扩展SFR
                break;
        case 62 :       //第1组PWM6
        case 47 :       //第2组PWM6
                P_SW2 |= 0x80;//使用外部扩展SFR 
                PWM6T1 = PWMT1;//
                PWM6T2 = PWMT2;
                P_SW2 &= ~0x80;//退出外部扩展SFR 
                break;
        case 63 :       //第1组PWM7
        case 46 :       //第2组PWM7
                P_SW2 |= 0x80;//使用外部扩展SFR 
                PWM7T1 = PWMT1;//
                PWM7T2 = PWMT2;
                P_SW2 &= ~0x80;//退出外部扩展SFR 
                break;
                
        default:               
                break;
    }
}
//========================================================================
// 函数: epwmCRbegin()
// 描述: 启动PWM计数器。
// 参数: 无
// 返回: none.
// 版本: VER1.1
// 日期: 2014-11-17
// 备注: 
//========================================================================
void epwmCRbegin()
{
     PWMCR |= 0x80;
}