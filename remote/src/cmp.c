#include"cmp.h"

void CMPbegin(u8 CMP_P,u8 CMP_N,u8 CMP_OUT_EN)
{    
    CMPCR1  = (CMPCR1&0xf7)|CMP_P;//比较器正极输入选择
    CMPCR1  = (CMPCR1&0xfb)|CMP_N;//比较器负极输入选择
    CMPCR1  = (CMPCR1&0xfd)|(CMP_OUT_EN<<1);//选择比较器输出到P1.2 
  
    CMPCR2  = 0x10;//比较器输出不取反，输出有0.1us的滤波器，持续16个时钟周期      
    CMPCR1  |= 0x80;
}
u8 CMPresult()
{
    u8 value;
    value=CMPCR1&0x01;
    return value;
}