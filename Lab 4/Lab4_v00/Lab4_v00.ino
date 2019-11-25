
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
const int interuptTime = 500;//us
const int HzDiff = 1136;
//const int nullingFV = 2*PI*F*(interuptTime*10^-6);
const int nullingFV = 2*PI*F*(HzDiff*10^-6);
long x1;
long x2;
long y;
float bitToVoltage = 5.0/1023.0;
float inputoffSet = 2.5/bitToVoltage;
//long outoffSet = 912; 

/*
 * 5 = 1023
 * offset (2.5) = 1023/2
 */

/*
 * First remove offset
 * Read Input
 * Read feedback
 * Output = K*(Input-Feeback)
 * Add Back Offset
 */

bool openLoop = false;

const long K = 2.5;

int analogPin = A1;
int feedBackPin = A0;
int val = 0;  // variable to store the value read

void setup(void) {
  noInterrupts();//stop interrupts

  pinMode(analogPin, INPUT);
  pinMode(feedBackPin, INPUT);

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
  
  val = analogRead(analogPin)- inputoffSet;  // read the input pin
 
  if(openLoop)
  {
    y = val + inputoffSet;
  } else{
    x2 = analogRead(feedBackPin) - inputoffSet;
    float error = val-x2;
    if (error <0){
      y = 0;
    } else{
      y = K*((error) + inputoffSet);
    }
    
  }  
  
}


void loop(void) {
  //Serial.println(analogRead(analogPin)*bitToVoltage);//voltageConversion);
  //Serial.println(analogRead(feedBackPin)*bitToVoltage);//voltageConversion);
  Serial.println(y*4);
  dac.setVoltage(y*4, false);//
  //dac.setVoltage(512, false);//withFilter
}
