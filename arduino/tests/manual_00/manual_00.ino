/*
   Prueba Manual - Expo Autómata
*/

#include <AccelStepper.h>
AccelStepper stepper(AccelStepper::DRIVER, 8, 9);
// CLK+(paso) -> pin 8 --- CW+ (direccion) -> pin 9

int cw          = 11;   // pin boton para accionar en sentido cw
int ccw         = 12;   // pin boton para accionar en sentido ccw

int pos         = 0;    // posición de inicio del motor
int inc         = 400;  // incremento por loop del botón presionado

int CW, CCW;            // variables de almacenamiento de los botones

void setup() {
  Serial.begin(9600);
  pinMode(cw, INPUT);
  pinMode(ccw, INPUT);

  stepper.setMaxSpeed(5000);
  stepper.setSpeed(800);
  stepper.setAcceleration(3000);
}

void loop() {

  CW = digitalRead(cw);
  CCW = digitalRead(ccw);
  
  printButtons();
  // checkButtons();
}

void printButtons() {
  Serial.print("CW = ");
  Serial.println(CW);
  Serial.print("CCW = ");
  Serial.println(CCW);
}

void checkButtons() {
  if (CW != 0) {
    pos += inc;
  }
  if (CCW != 0) {
    pos -= inc;
  }
  if (CW != CCW) {
    stepper.moveTo(pos);
    stepper.run();
    Serial.println(pos);
  }
}


