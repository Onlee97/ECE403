
#include "TimerOne.h"
#include <Wire.h>
#include <Adafruit_MCP4725.h>

Adafruit_MCP4725 dac;

//storage variables
boolean toggle0 = 0;
uint32_t counter;
const int COUNTEND = 1023;
const int COUNTBEG = 0;

void setup(void) {
  noInterrupts();//stop interrupts

  //pinMode(13,OUTPUT);

  interrupts();//allow interrupts
  TWBR = 12;// 400khz

  Timer1.initialize(1000);
  Timer1.attachInterrupt(Blink);

  counter = 0;

  Serial.begin(9600);
  Serial.println("Hello!");

  // For Adafruit MCP4725A1 the address is 0x62 (default) or 0x63 (ADDR pin tied to VCC)
  // For MCP4725A0 the address is 0x60 or 0x61
  // For MCP4725A2 the address is 0x64 or 0x65
  dac.begin(0x62);
}

void Blink(){
  digitalWrite(13, digitalRead(13) ^ 1);//Toggle pin 10 for blink
  
  if (!toggle0 && counter < COUNTEND) {
    counter++;
  }
  else if (toggle0 && counter > COUNTBEG) {
    counter--;
  }
  if (!toggle0 && counter >= COUNTEND) {
    toggle0 = 1;
  } else if (toggle0 && counter <= COUNTBEG) {
    toggle0 = 0;
  }
}


void loop(void) {
  dac.setVoltage(counter, false);
}
