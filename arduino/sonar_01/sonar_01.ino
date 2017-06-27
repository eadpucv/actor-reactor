float Inch = 0.00;
float cm = 0.00;
float pcm = cm;
int SonarPin = A0;
int sensorValue;


void setup() {
  pinMode(SonarPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  readSonar();

  if(abs(cm - pcm > 5)){
    Serial.println("******************* tilt! *******************");
  }
    
  Serial.print(cm);
  Serial.print(" cm.-  ");
  Serial.println(millis());
}

void readSonar() {
  pcm = cm;
  sensorValue = analogRead(SonarPin);
  delay(10);
  Inch = (sensorValue * 0.497);
  cm = Inch * 2.54;
}
