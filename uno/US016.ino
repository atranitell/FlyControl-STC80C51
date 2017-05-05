#include <stdio.h>
#include <SoftwareSerial.h>

SoftwareSerial portSTC(4,5) ;
// *5-R 4-T

unsigned int ADCValue;
unsigned char data;
double Bs;
double p_now;
double q1,q3;

void KMfilter( double x_last, double p_last, double Q, double R, double y_now ) {
    double x_now = x_last ;
    p_now = p_last + Q ;
    double Kg_now = p_now/(p_now+R) ;
    Bs = x_now + Kg_now * (y_now - x_now) ;
    p_now = p_now * (1 - Kg_now) ;
}

void setup() {
  Serial.begin(9600);
  portSTC.begin(115200) ;
}

char tmp ;

void loop() { 
  ADCValue = analogRead(0);
  double v = ADCValue/3.37 ;
  q1 = 0.01 ;
  q3 = 0.03 ;
  KMfilter(Bs, p_now, q3, q1, v) ;
  data = (int)Bs ;
  Serial.println((int)data) ;
  portSTC.write(data) ;
  //tmp = portSTC.read() ;
  //Serial.println(tmp) ;
  delay(60);
  
}
