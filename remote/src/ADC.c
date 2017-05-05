//========================================================================
//	STCunio Studio淘宝http://shop112966725.taobao.com
//	STCunio Studio QQ群:117112538
//	作者：mingbozhu
//	联系方式：13724273732
//	邮箱：mingbozhu@yeah.net
//	修改日期2014.11.7
//	版本V1.1
//========================================================================
#include	"adc.h"

void analogbegin(u8 pin)
{
     if(pin > ADC_CH7)	return;	//错误,返回1024,调用的程序判断
     P1ASF=1<<pin;//设置模拟通道
     ADC_CONTR=(ADC_CONTR & ~ADC_90T) | ADC_90T;//设置ADC的时钟
     ADC_CONTR |= 0x80;//打开ADC电源
    //PCON2 |=  (1<<5);	//10位AD结果的高2位放ADC_RES的低2位，低8位在ADC_RESL。
     PCON2 &= ~(1<<5);	//10位AD结果的高8位放ADC_RES，低2位在ADC_RESL的低2位。     
}
//========================================================================
// 函数: analogResult(u8 pin)
// 描述: 获取模拟采样结果函数。
// 参数: 无
// 返回: adc，10位AD的值
// 版本: VER1.1
// 日期: 2014-11-17
// 备注: AD中断服务程序调用
//========================================================================
u16 analogResult()
{
    u16 adc;
    ADC_CONTR &= ~ADC_FLAG;
    adc = (u16)ADC_RES;//10位AD结果的高8位放ADC_RES,低2位放在ADC_RESL。
    adc = (adc << 2) | (ADC_RESL & 3);
    ADC_RES = 0;
    ADC_RESL = 0;
    return	adc; 
}
//========================================================================
// 函数: analogSetup(u8 pin)
// 描述: 获取模拟采样结果函数。
// 参数: pin，选择需要的模拟引脚。
// 返回: adc，10位AD的值
// 版本: VER1.1
// 日期: 2014-11-17
// 备注: AD中断服务程序调用
//========================================================================
void analogStart(u8 pin)
{
    if(pin > ADC_CH7)	return;	
    if(PWMCFG&0xbf) ADC_CONTR = (ADC_CONTR & 0xe0) | pin;//若是CBTADC为1，则ADC_START无需置1
    else ADC_CONTR = (ADC_CONTR & 0xe0) | ADC_START | pin; 
}
//========================================================================
// 函数: analogRead(u8 pin)
// 描述: 模拟采样函数。
// 参数: pin，选择需要的模拟引脚。
// 返回: adc，10位AD的值
// 版本: VER1.1
// 日期: 2014-11-17
// 备注: 不能用在AD中断服务程序调用
//========================================================================
u16 analogRead(u8 pin)
{
    u16	adc;
    u8	i;
    analogbegin(pin);
    analogStart(pin);
    for(i=0; i<250; i++)		//超时
    {
      if(ADC_CONTR & ADC_FLAG)
      {
        adc = analogResult();
        return	adc;
      }
    }    
   return	1024;	//错误,返回1024,调用的程序判断
}