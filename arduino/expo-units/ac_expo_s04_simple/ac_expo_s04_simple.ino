/**

  Actor Reactor - CLAC
  Taller de Espacios Expositivos e[ad]
  Octubre 2017


  "Vínculo Sinuoso"         s01     Javier, César

  "Descalce Ampliado"       s02     Catalina C, Bastián

  "Expansión Híbrida"       s03     Consuelo, Melisa, Carolina

  "Arritmia Dispar"         s04     Doyma, Javiera

  "Distension Palpitante"   s05     Bastián M, Monserrat

  "Elegancia Segmentada"    s06     MªIgnacia, Sofía S, Paul, Sam

  "Fragmentada Extension"   s07     Alejandro, Dominique, Javier

  "Impulso Acorazado"       s08     Consuelo, Marcelo

  "Asincronia Elevada"      s09     Sofía V. Catalina M.

*/

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <AccelStepper.h>

float min_sensor = 30;
float max_sensor = 450;

float min_actuator = 0;
float max_actuator = 300 * 26 * 2;

float sonar_read = 0;
float triggerDist  = 100;    // distancia de activación

String NAME = "Arritmia\nDispar";

Adafruit_PCD8544 display = Adafruit_PCD8544(7, 5, 6, 4, 3);
AccelStepper stepper(2, 9, 8);

void setup() {
  Serial.begin(57600);
  pinMode(2, OUTPUT);     // turn on screen backlight 1
  pinMode(12, INPUT);

  digitalWrite(4, 0);
  digitalWrite(2, 0);     // turn on screen backlight 2

  stepper.setMaxSpeed(10000);
  stepper.setSpeed(8000);
  stepper.setAcceleration(1000);

  display.begin();
  display.clearDisplay();
  display.display();
  display.setContrast(60);
  display.setRotation(2);
  display.println(NAME);
  display.display();
}

void loop() {
  sonar_read = analogRead(A2) * 1.26;
  sonar_read = constrain(sonar_read, min_sensor, max_sensor);
  Serial.println(sonar_read);
  
  
  if (sonar_read < triggerDist) {
    //sinusoidal();
    bounce();
  }
  
}

void sinusoidal() {
  Serial.println("sinusoidal...");
  for (float i = 0; i < PI; i += 0.01) {

    float newPos = sin(i) * max_actuator;
    stepper.moveTo(newPos);
    stepper.run();
    //Serial.print("*  ");
    //Serial.println(newPos);
  }
  Serial.println("listo...");
}

void bounce() {
  Serial.println("running...");
  stepper.runToNewPosition(max_actuator);
  delay(100);
  stepper.runToNewPosition(0);
  Serial.println("done!");
}

