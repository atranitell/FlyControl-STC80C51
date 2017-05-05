
/*------------------------------------------------------------------*/
/********STCunio Studio淘宝http://shop112966725.taobao.com***********/
/*********STCunio Studio QQ群:117112538******************************/
/********作者：朱明波************************************************/
/********联系方式：13724273732***************************************/
/********邮箱：mingbozhu@yeah.net************************************/
/********修改日期2014.11.17******************************************/
/********版本V1.1****************************************************/
/*------------------------------------------------------------------*/


#include	"PCA.h"
#include "gpio.h"
#include<stdio.h>
 
	 xdata u8	PCA0_mode=0xff;
   xdata u8 PCA1_mode=0xff;
   xdata volatile UINT32 Capture0Value;
   xdata volatile UINT32 Capture1Value;
   xdata volatile u16 Capture0Value_High=0;
   xdata volatile u16 Capture1Value_High=0;
   bit Capture0FirstPulse;
   bit Capture1FirstPulse;
   bit B_Capture0;
   bit B_Capture1;
//========================================================================
// 函数: UpdatePwm(u8 PCA_id, u8 pwm_value)
// 描述: 更新PWM值. 
// 参数: PCA_id: PCA序号. 取值 PCA0,PCA1,PCA2,PCA_Counter
//		 pwm_value: pwm值, 这个值是输出低电平的时间.
// 返回: none.
// 版本: V1.0, 2012-11-22
//========================================================================
void	UpdatePwm(u8 PCA_id, u8 pwm_value)
{
    if(PCA_id == PCA0) CCAP0H = pwm_value;
    else if(PCA_id == PCA1)	CCAP1H = pwm_value;
}
//========================================================================
// 函数: void	analogWrite(pin,u8 value)
// 描述: 模拟写操作，并非正在的模拟量，是可调占空比的PWM输出信号.
// 参数: pin：只限数字5、数字6、数字13、数字14
//		   value：0-255
// 返回: none.
// 版本: V1.1, 2014-11-17
//========================================================================

void	analogWrite(pin,u8 value)
{
      CMOD=(CMOD  & ~(7<<1))|PCA_Clock_12T|DISABLE;//初始化时钟以及CF中断使能位不考虑
      if(pin==5||pin==6)   //若选择5、6脚则转移CCAPn到PCA_P34_P35_P36_P37
      AUXR1 = (AUXR1 & ~(3<<4)) | PCA_P34_P35_P36_P37;
      else if(pin==13||pin==14)//若选择13、14脚则转移CCAPn到PCA_P24_P25_P26_P27
      AUXR1 = (AUXR1 & ~(3<<4)) | PCA_P24_P25_P26_P27;
      else return;
      if(pin==5||pin==13)
      {
          PCA0_mode=PCA_Mode_PWM;
          PCA_PWM0  = (PCA_PWM0 & ~(3<<6))|PCA_PWM_8bit;//8位宽度的PWM
          CCAPM0 |=0x42;//打开比较器，并设为PWM输出模式
          CCAP0L=~value;
          CCAP0H=~value;
    
          if(value==0)//解决PWM0不为0和1的情况
          {
              PWM0_OUT_0();		
          }
          else if(value==255)
          {
              PWM0_OUT_1();
          }
          else		
              PWM0_NORMAL();    
      }
      else if(pin==6||pin==14)
      {	
          PCA1_mode=PCA_Mode_PWM;
          PCA_PWM1  = (PCA_PWM1 & ~(3<<6))|PCA_PWM_8bit;//8位宽度的PWM
          CCAPM1 |=0x42;//打开比较器，并设为PWM输出模式
          CCAP1L=~value;
          CCAP1H=~value;
          if(value==0)//解决PWM1不为0和1的情况
          {
              PWM1_OUT_0();		
          }
          else if(value==255)
          {
              PWM1_OUT_1();
          }
          else		
              PWM1_NORMAL();
      }
    CR=1;//最后别忘了打开计数器
}
//========================================================================
// 函数: Capturebegin(pin,pulsemode)
// 描述: 捕捉电平准备
// 参数: pin：只限数字5、数字6、数字13、数字14
//		   pulsemode：PCA_Rise_Active、PCA_Fall_Active
// 返回: none.
// 版本: V1.1, 2014-11-17
//========================================================================
void Capturebegin(pin,pulsemode)
{
      CMOD=(CMOD  & ~(7<<1))|PCA_Clock_12T|ENABLE;//初始化时钟以及CF中断使能
      if(pin==5||pin==6)
      AUXR1 = (AUXR1 & ~(3<<4)) | PCA_P34_P35_P36_P37;
      else if(pin==13||pin==14)
      AUXR1 = (AUXR1 & ~(3<<4)) | PCA_P24_P25_P26_P27;
      else return;
      if((pin==5)||(pin==13))
      {
          PCA0_mode=PCA_Mode_Capture;//设置为捕捉模式
          CCAPM0=pulsemode|ENABLE;//允许中断
          Capture0FirstPulse=1;
      }
      else if((pin==6)||(pin==14))
      {
          PCA1_mode=PCA_Mode_Capture;//设置为捕捉模式
          CCAPM1=pulsemode|ENABLE;//允许中断
          Capture1FirstPulse=1;
      }
      else return;
}
//========================================================================
// 函数: CaptureAvailable()
// 描述: 查询是否捕捉成功
// 参数: 无
// 返回: true OR false.
// 版本: V1.1, 2014-11-17
//========================================================================
u8 CaptureAvailable()
{
    if(PCA0_mode == PCA_Mode_Capture)//若检测到PCA0为捕捉模式
    {
        if(B_Capture0==1)//检测PCA0捕捉是否成功
        {
          B_Capture0=0;//清除标识位
          return true;
        }
        else return false;
    }
    else if(PCA1_mode ==PCA_Mode_Capture)//若检测到PCA1为捕捉模式
    {
        if(B_Capture1==1)//检测PCA1捕捉是否成功
        {
          B_Capture1=0;//清除标识位
          return true;
        }
        else 
          return false;
    }
    else
    return false;
}
//========================================================================
// 函数: GetCapture()
// 描述: 获得捕捉电平的宽度
// 参数: 无
// 返回: 电平的宽度.
// 版本: V1.1, 2014-11-17
//========================================================================
u32 GetCapture()
{ 
  xdata  UINT32 value0;
  xdata u32 value_temp=0;
  static xdata  u32 value1=0;
  value0.all=0;
  if(PCA0_mode == PCA_Mode_Capture)
  {   
        value0.all=Capture0Value.all;
        value_temp=value0.all;
        value0.all=value0.all-value1;
        value1=value_temp&0xffff;      
        return (value0.all);
  }
  else if(PCA1_mode == PCA_Mode_Capture)
  {              
        value0.all=Capture1Value.all;
        value_temp=value0.all;
        value0.all=value0.all-value1;
        value1=value_temp&0xffff;       
        return (value0.all);
  }  
  return 0;
}
void CaptureEnd()
{
    if(PCA0_mode == PCA_Mode_Capture)
    {
        CCAPM0 &=0xcf;
    }
    else if(PCA1_mode == PCA_Mode_Capture)
    {
        CCAPM1 &=0xcf;
    }
}

//========================================================================
// 函数: void	PCA_Handler (void) interrupt PCA_VECTOR
// 描述: PCA中断处理程序.
// 参数: None
// 返回: none.
// 版本: V1.1, 2014-11-17
//========================================================================
void	PCA_Handler (void) interrupt PCA_VECTOR
{
	if(CCF0)		//PCA模块0中断
	{
      CCF0 = 0;		//清PCA模块0中断标志

      if(PCA0_mode == PCA_Mode_Capture)
      {       
          Capture0Value.BYTES.byte2 = CCAP0H;	//读CCAP0H
          Capture0Value.BYTES.byte1 = CCAP0L;  //读CCAP0L
          Capture0Value.WORDS.int2=0;         //将Capture0Value的高16位清0
          B_Capture0 = 1;                     //捕捉成功标识置1
          if(Capture0FirstPulse==1)           //若PCA0第1次捕捉到了信号，那么忽略这次
          {
              CR=1;                           //需要打开计数器
              Capture0FirstPulse=0;  
              CCAPM0 |=0x30;                  //同时打开正负捕获，若只需测周期，只需打开正捕获或负捕获
              Capture0Value_High=0;           //第一次捕捉只需记录CCAP0H、CCAP0L的位置，高16位清0，以作下次捕捉长度的减数
          }
          else
          {
              if(CF==1)
              {
                  CF=0;
              Capture0Value_High++;           //若CCF0中断处理过程中出现CF中断，那么需要处理一次计数，否则将会高位会少1
              } 
              Capture0Value.WORDS.int2=Capture0Value_High;	//记录高16位
              Capture0Value_High = 0;         
          }           
      }
      else return;
	}

	if(CCF1)	//PCA模块1中断
	{
      CCF1 = 0;		//清PCA模块1中断标志
    
      if(PCA1_mode == PCA_Mode_Capture)
      {        
          Capture1Value.BYTES.byte2 = CCAP1H;	//读CCAP1H
          Capture1Value.BYTES.byte1 = CCAP1L;  //读CCAP1L
          Capture1Value.WORDS.int2=0;         //将Capture1Value的高16位清0
          B_Capture1 = 1;                     //捕捉成功标识置1      
          if(Capture1FirstPulse==1)           //若是捕捉1第1次捕捉到了信号
          {
              CR=1;                           //需要打开计数器
              Capture1FirstPulse=0;
              CCAPM1 |=0x30;                  //同时打开正负捕获
              Capture1Value_High=0;           //第一次捕捉只需记录CCAP1H、CCAP1L的位置，高16位清0，以作下次捕捉长度的减数
          }
          else
          {
              if(CF==1)
              {
                  CF=0;
                  Capture1Value_High++;                   //若CCF1中断处理过程中出现CF中断，那么需要处理一次计数，否则将会高位会少1
              }        
              Capture1Value.WORDS.int2=Capture1Value_High;//记录高16位
              Capture1Value_High=0;               
          }   
      }
	}
	if(CF)	//PCA溢出中断
	{
      CF = 0;			//清PCA溢出中断标志
      if(PCA0_mode == PCA_Mode_Capture)
      {       
          if(Capture0FirstPulse==0)  
              Capture0Value_High++;
      }
      else if(PCA1_mode == PCA_Mode_Capture)
      {      
          if(Capture1FirstPulse==0)  
              Capture1Value_High++;      
      }   
	}
}