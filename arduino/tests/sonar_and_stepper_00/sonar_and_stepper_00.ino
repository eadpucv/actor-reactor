#include <AccelStepper.h>
#include <MultiStepper.h>

AccelStepper stepper(2, 9, 8);   // LIB ACCEL

float Inch = 0.00;
float cm = 0.00;
float pcm = cm;
int SonarPin = A0;
int sensorValue;


void setup() {
  pinMode(SonarPin, INPUT);
  Serial.begin(9600);
  // Stepper config
  stepper.setMaxSpeed(5000.0);
  stepper.setSpeed(900.0);
  stepper.setAcceleration(9000.0);
}

void loop() {
  readSonar();
  if (abs(cm - pcm < 10)) {
    gotoPosition(cm);
  }
}

void readSonar() {
  pcm = cm;
  sensorValue = analogRead(SonarPin);
  delay(10);
  Inch = (sensorValue * 0.497);
  cm = Inch * 2.54;
}

void gotoPosition(float target) {
  float newPos = map(target, 0, 100, 0, 20000);
  Serial.print("cm = ");
  Serial.print(cm);
  Serial.print(" - new position = ");
  Serial.println(newPos);
  stepper.runToNewPosition(newPos);
}


