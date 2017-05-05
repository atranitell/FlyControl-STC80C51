#include <SoftwareSerial.h>

const int echopin=3; // echo接3端口
const int trigpin=2; // trig接2端口
int led1 = 6; // led灯接10端口
int led2 = 7;
int led3 = 8;
int last_distance = 0;
SoftwareSerial portZigbee(4,5) ;

unsigned int ADCValue;

void setup()
{
  Serial.begin(9600);  
  portZigbee.begin(9600) ;
  pinMode(echopin,INPUT); //设定echo为输入模式
  pinMode(trigpin,OUTPUT);//设定trig为输出模式
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(led3,OUTPUT);//设定led为输出模式
}

void loop()
{
  digitalWrite(trigpin,LOW);
  delayMicroseconds(2);
  digitalWrite(trigpin,HIGH);
  delayMicroseconds(15);
  digitalWrite(trigpin,LOW); //发一个10ms的高脉冲去触发TrigPin
  
  int distance = pulseIn(echopin,HIGH);//接收高电平时间
  distance = distance/58;//计算距离
  Serial.print((unsigned char)distance);  //输出距离
  Serial.println("cm");  //输出单位
  
  if(abs(last_distance - distance) > 40);
  else if(distance == 0)
    portZigbee.write(distance);
  else
    {
      last_distance = distance ;
      portZigbee.write(distance);
    }
  
  if(distance<50)
  {digitalWrite(led1,HIGH);}
  else{digitalWrite(led1,LOW);}//距离小于10cm时，led写入高电平
  if(distance=50)
  {digitalWrite(led2,HIGH);}
  else{digitalWrite(led2,LOW);}//距离小于10cm时，led写入高电平
  if(distance>50)
  {digitalWrite(led3,HIGH);}
  else{digitalWrite(led3,LOW);}//距离小于10cm时，led写入高电平
  
  
  
  delay(60);   //循环间隔100MS
}
