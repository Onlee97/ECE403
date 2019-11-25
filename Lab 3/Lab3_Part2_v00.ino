
#include "TimerOne.h"
#include <Wire.h>
#include <Adafruit_MCP4725.h>

/*
 * AVG Settings
 * Freq = 200 Hz
 * Amp = 2.5
 * Offset +1.25
 */

Adafruit_MCP4725 dac;

//storage variables
const int F = 200;//Hz
const int interuptTime = 1000;//us
const int HzDiff = 1136;
//const int nullingFV = 2*PI*F*(interuptTime*10^-6);
const int nullingFV = 2*PI*F*(HzDiff*10^-6);
long x1;
long x2;
long x3;
long y;

int analogPin = A14; // potentiometer wiper (middle terminal) connected to analog pin 3
                    // outside leads to ground and +5V
int val = 0;  // variable to store the value read

void setup(void) {
  noInterrupts();//stop interrupts

  //pinMode(13,OUTPUT);

  interrupts();//allow interrupts
  TWBR = 12;// 400khz

  Timer1.initialize(interuptTime);
  Timer1.attachInterrupt(Blink);


  Serial.begin(9600);
  Serial.println("Hello!");

  // For Adafruit MCP4725A1 the address is 0x62 (default) or 0x63 (ADDR pin tied to VCC)
  // For MCP4725A0 the address is 0x60 or 0x61
  // For MCP4725A2 the address is 0x64 or 0x65
  dac.begin(0x62);
}

void Blink(){  
  SetTheVoltage();
}

void SetTheVoltage(){
  
  val = analogRead(analogPin);  // read the input pin
  //TODO: Convert Offset to Digital 10 Bit, Remove Offset, Convert to 12 Bit, add at the end
  x3=x2;
  x2=x1;
  x1=val;
  y=x1-2*cos(nullingFV)*x2+x3;
  //dac.setVoltage(val, false);
  
}


void loop(void) {
  //sSerial.println(y);
  //dac.setVoltage(val, false);//withoutFilter
  dac.setVoltage(y, false);//withFilter
}
