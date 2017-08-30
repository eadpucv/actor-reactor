#include <AccelStepper.h>
// #include <MultiStepper.h>

AccelStepper stepper(2, 9, 8);   // EN+, CLK+, CW+, LIB ACCEL
//Variables
float cm = 0.00;                          //conversión valor de lectura
float pcm = cm;                           //cm previo = cm actual
int SonarPin = A0;                        //pin de lectura
int sensorValue;                          //valor de lectura
float newPos;                             //valor de escritura
int pasos;                                //valor de movimiento
int u = 13;                               //valor aprox. mínimo de lectura
boolean debug = false;                    //boolean para consola

void setup() {
  pinMode(SonarPin, INPUT);
  Serial.begin(9600);
  // Stepper config
  stepper.setMaxSpeed(6000.0);
  stepper.setSpeed(2000.0);
  stepper.setAcceleration(2000.0);
  pinMode(10, OUTPUT);                  //alimentación positivo
  pinMode(3, INPUT);                    //fin de carrera
  digitalWrite(10, HIGH);               //devolución al inicio hasta que presiona el botón de inicio de carrea
  long target1 = -1600;
  while (digitalRead(3) == LOW) {
    stepper.runToNewPosition(target1);
    target1 -= 1600;
    Serial.println(target1);
  }
  stepper.setCurrentPosition(0);
  stepper.runToNewPosition(20);
  
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
  float newPos = map(target, u, 200, 0, 1600); //1600 pasos = 360°
  stepper.runToNewPosition(newPos);
}

