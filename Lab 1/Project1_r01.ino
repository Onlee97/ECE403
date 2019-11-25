// Switch Bounce Counter
//
#define btnSTARTFall 2 // Pin to Ready Bounce Counter
#define btnSTARTRise 7 // Pin to Ready Bounce Counter
#define swHIT 3 // External Interrupt 1
#define swHIT18 18 // External Interrupt 2
#define ledTriggerBlue 4 //Led Indicator 1
#define ledTriggerRed 5 //Led Indicator 2

volatile int bounceCountFall = 0;
volatile int bounceCountRise = 0;

int breakWhileCount = 1000;
bool testingFall = true;

void setup() {
  // setup the switches and LED
  pinMode(btnSTARTFall, INPUT_PULLUP);
  pinMode(btnSTARTRise, INPUT_PULLUP);
  pinMode(swHIT, INPUT);
  pinMode(swHIT18, INPUT);
  pinMode(ledTriggerBlue, OUTPUT);
  pinMode(ledTriggerRed, OUTPUT);

  // If you don't use pin 3, you will need to change the 1 below to the new interrupt number
  attachInterrupt(1, bounceFall, FALLING);
  attachInterrupt(5, bounceRise, RISING);
  // By using FALLING, we capture everytime the pin transisions from HIGH to LOW
  
  // Turn off the LED
  digitalWrite(ledTriggerBlue, HIGH);
  digitalWrite(ledTriggerRed, HIGH);

  // Start the Serial port
  Serial.begin(9600);
  
  Serial.println("Arduino Switch Debounce");
  Serial.println();
}

// All the interrupt routine needs to do is increment bounceCount
// By keeping this routine small we maximize the chances of catching every bounce
void bounceFall() {
  bounceCountFall++;
}

void bounceRise() {
  bounceCountRise++;
}


void loop() {

  Serial.println("Press START button when ready");
  Serial.println("If Blue LED lights, the pressing test is ready.");
  Serial.println("If Red LED lights, the releasing test is ready.");

  // Wait for one of the START buttons
  while (digitalRead(btnSTARTFall) && digitalRead(btnSTARTRise)) {}
  
  testingFall = true;
  
  // Then wait for it to be released
  delay(10);
  if(!digitalRead(btnSTARTFall) && digitalRead(btnSTARTRise)){
    testingFall = true;
    //Serial.println("Test1");
    while(!digitalRead(btnSTARTFall)){}
  } else if (digitalRead(btnSTARTFall) && !digitalRead(btnSTARTRise)){
    testingFall = false;
    //Serial.println("Test2");
    while(!digitalRead(btnSTARTRise)){}
  }
  delay(1000);

  // Start the testing
  bounceCountFall = 0;
  bounceCountRise = 0;

  // Light the corresponding LED
  if (testingFall){
    digitalWrite(ledTriggerBlue, LOW);    
    Serial.println("Ready for testing Fall...");

    // Wait for the switch to close
    while (bounceCountFall == 0) {}
    
  } else{
    digitalWrite(ledTriggerRed, LOW);
    Serial.println("Ready for testing Rise...");
    
    // Wait for the switch to open
    while (bounceCountRise == 0) {}
  }
  // If you are here, the switch was thrown
  // Wait a second to collect the bounces

  delay(1000);

  // Output the results

  digitalWrite(ledTriggerBlue, HIGH);
  digitalWrite(ledTriggerRed, HIGH);
  Serial.print("The switch bounced ");
  if (testingFall){
    Serial.print(bounceCountFall);  
  } else {   
    Serial.print(bounceCountRise);  
  }
  Serial.println(" times.");
  Serial.println();
}
