/**

    Actor Reactor - CLAC
    Taller de Espacios Expositivos e[ad]
    Octubre 2017

*/
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <AccelStepper.h>

Adafruit_PCD8544 display = Adafruit_PCD8544(7, 5, 6, 4, 3);
AccelStepper stepper(AccelStepper::DRIVER, 9, 8); // CLK+(paso) -> pin 9 --- CW+(direccion) -> pin 8

int stepToGo;
float bounce;

void setup () {
  Serial.begin(57600);
  pinMode(12, INPUT);
  Serial.setTimeout(10000);
  digitalWrite(4, 0);
  pinMode(13, OUTPUT);
  delay(10);
  pinMode(13, INPUT);

  stepper.setMaxSpeed(4000.0);
  stepper.setAcceleration(2000.0);

  display.clearDisplay();
  display.display();

  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0, 0);
  display.println("vamos!");
  display.display();
}




int halfMaxSteps = 400;

void loop() {

  bounce = sin(float(millis()) / 300.0) * halfMaxSteps;
  stepToGo = halfMaxSteps + round(bounce);
  stepper.moveTo(stepToGo);
  stepper.run();
  Serial.println(stepToGo);
  
  display.clearDisplay();
  display.display();
  display.println("solo motor");
  display.display();

  /*
    // If at the end of travel go to the other end
    if (stepper.distanceToGo() == 0)
    stepper.moveTo(-stepper.currentPosition());
  
  */
}

