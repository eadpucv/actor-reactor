#include <AccelStepper.h>
#include <MultiStepper.h>

AccelStepper stepper(2, 9, 8);   // EN+, CLK+, CW+, LIB ACCEL
//Variables
float cm = 0.00;                          //conversión valor de lectura
float pcm = cm;                           //cm previo = cm actual
int SonarPin = A0;                        //pin de lectura
int sensorValue;                          //valor de lectura
float newPos;                             //valor de escritura
int pasos;
int u = 13;                               //valor aprox. mínimo de lectura

void setup() {
  pinMode(SonarPin, INPUT);
  Serial.begin(9600);
  // Stepper config
  stepper.setMaxSpeed(5000.0);
  stepper.setSpeed(1000.0);
  stepper.setAcceleration(2000.0);
  pinMode(10, OUTPUT);                  //alimentación positivo
  pinMode(3, INPUT);                    //inicio de carrera
  pinMode(4, INPUT);                    //fin de carrera
  digitalWrite(10, HIGH);           //devolución al inicio hasta que presiona el botón de inicio de carrea
  for (long i = 0; i < 5000; i++) {
    stepper.runToNewPosition(i * 50);
    if (digitalRead(3) == HIGH) {
      break;
    }
  }
}

void loop() {
  readSonar();

  if (abs(cm - pcm < 10)) {               //calcula delta entre lectura actual y lectura anterior
    gotoPosition(cm);                     //si es menor a 10, va a la posición leída
  }
  if (digitalRead(3) == HIGH) {
    cm = u;
  }
}


void readSonar() {
  pcm = cm;
  sensorValue = analogRead(SonarPin);     //lee el valor de entrada del sensor
  delay(50);
  // Inch = (sensorValue * 0.497);           //calculo en pulgadas
  //cm = Inch * 2.54;                       //calculo en cms
  cm = (sensorValue * 1.262);
  Serial.print("cm = ");
  Serial.print(cm);
  Serial.print(" - new position = ");
  Serial.println(newPos);
}

void gotoPosition(float target) {
  float newPos = map(target, u, 200, 0, 400); //400 pasos = 90°
  stepper.runToNewPosition(newPos);
}

