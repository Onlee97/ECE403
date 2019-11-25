#define echoPin 10
#define trigPin 9

long duration;
long distance;

void setup() {
  Serial.begin(9600);
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  //Setting up digital display for shits and giggles
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  duration=pulseIn(echoPin, HIGH);
  //Distance in CM conversion to inches
  distance=(duration/58.2)/2.54;


  Serial.println(distance);

  delay(50);
}
