
#include <Wire.h>
#include <Adafruit_MCP4725.h>

Adafruit_MCP4725 dac;

//storage variables
boolean toggle0 = 0;
uint32_t counter;
const int COUNTEND = 1024;
const int COUNTBEG = 0;

void setup(void) {
  cli();//stop interrupts

  //set timer0 interrupt at 1000Hz
  TCCR0A = 0;// set entire TCCR0A register to 0
  TCCR0B = 0;// same for TCCR0B
  TCNT0  = 0;//initialize counter value to 0
  // set compare match register for 1kHz increments
  OCR0A = 249;// = (16*10^6) / (1000*64) - 1 (must be <256)
  // turn on CTC mode
  TCCR0A |= (1 << WGM01);
  // Set CS01 and CS00 bits for 64 prescaler
  TCCR0B |= (1 << CS01) | (1 << CS00);   
  // enable timer compare interrupt
  TIMSK0 |= (1 << OCIE0A);
  
  sei();//allow interrupts

  counter = 0;
  
  Serial.begin(9600);
  Serial.println("Hello!");

  // For Adafruit MCP4725A1 the address is 0x62 (default) or 0x63 (ADDR pin tied to VCC)
  // For MCP4725A0 the address is 0x60 or 0x61
  // For MCP4725A2 the address is 0x64 or 0x65
  dac.begin(0x62);
}

ISR(TIMER0_COMPA_vect){//timer0 interrupt 1kHz toggles pin 13
  //dac.setVoltage(counter, false);
  if (!toggle0 && counter < COUNTEND){
    counter++;
  }
  else if(toggle0 && counter > COUNTBEG){
    counter--;
  }
  if(!toggle0 && counter >= COUNTEND){
    toggle0 = 1;
  } else if(toggle0 && counter <= COUNTBEG){
    toggle0 = 0;
  } 
  
}

void loop(void) {
  dac.setVoltage(0, false);
  /*
    // Run through the full 12-bit scale for a triangle wave
    for (counter = 0; counter < 4095; counter++)
    {
      dac.setVoltage(counter, false);
    }
    for (counter = 4095; counter > 0; counter--)
    {
      dac.setVoltage(counter, false);
    }*/
}
