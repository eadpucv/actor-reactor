/**

    Actor Reactor - CLAC
    Taller de Espacios Expositivos e[ad]
    Octubre 2017

*/


#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <OnewireKeypad.h>
#include <AccelStepper.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>
#include <OSCMessage.h>

#define DIR_SENSOR_MIN 5
#define DIR_SENSOR_MAX 444
#define DIR_ACTUATOR_MIN 1
#define DIR_ACTUATOR_MAX 800

#define DIR_BEZIER_A 20
#define DIR_BEZIER_B 25
#define DIR_BEZIER_C 30
#define DIR_BEZIER_D 35

#define ID 1
#define WLAN_ADDR  "224.0.0.1" //"192.168.1.139"        // Dirección IP del PC que recibe
#define PORT  1112

// #define WLAN_SSID  "VTR-6517395"       // SSID de la red Wi-Fi
// #define WLAN_PASS  "dhPxnntG8yvs"      // Password de la red Wi-Fi

#define WLAN_SSID  "AC"                // SSID de la red Wi-Fi
#define WLAN_PASS  "actor-reactor"     // Password de la red Wi-Fi


float min_sensor = 0, min_actuator = 0, max_sensor = 0, max_actuator = 0;
float bezier_A = 0, bezier_B = 0, bezier_C = 0, bezier_D = 0, motor_pos = 0, last_motor_pos;
float EPSILON = 9.999999747378752E-5f;
boolean variable;

//EEPROM.get(DIR_SENSOR_MIN, min_sensor);
//EEPROM.get(DIR_SENSOR_MAX, max_sensor);
//EEPROM.get(DIR_ACTUATOR_MAX, max_actuator);

char insert[10];
int i = 0;
double normalize;
boolean mainmenu_disp = 0, resp;
boolean salir = 0;
boolean endstop, endstop_activation = 0, endstop_position = 0;
boolean show_adjust = 0;
long motor_pos_previous = 0;
float sonar_read, acum_sonar_read;
int analog_counter = 0;
word previous_millis, actual_millis, diferencia;

// Software SPI (slower updates, more flexible pin options):
// pin 7 - Serial clock out (SCLK)
// pin 6 - Serial data out (DIN)
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin 3 - LCD reset (RST)

Adafruit_PCD8544 display = Adafruit_PCD8544(7, 5, 6, 4, 3);
AccelStepper stepper(AccelStepper::DRIVER, 9, 8); // CLK+(paso) -> pin 9 --- CW+(direccion) -> pin 8

char KEYS[] = {
  '1', '2', '3', 'A',
  '4', '5', '6', 'B',
  '7', '8', '9', 'C',
  'Y', '0', 'N', 'D'
};


OnewireKeypad <Print, 16 > KP2(Serial, KEYS, 4, 4, A0, 4700, 1000, ExtremePrec );
SoftwareSerial mySerial(11, 10);


void setup () {
  Serial.begin(57600);
  pinMode(12, INPUT);
  mySerial.begin(57600);

  Serial.setTimeout(10000);

  digitalWrite(4, 0);
  pinMode(13, OUTPUT);
  delay(10);
  pinMode(13, INPUT);
  resp = mySerial.find("ready\r\n");
  mySerial.println("AT+CWMODE=1");
  resp = mySerial.find("OK\r\n");

  // connect to wifi
  
   
  mySerial.println("AT+CIPMUX=1");
  resp = mySerial.find("OK\r\n");
  mySerial.print("AT+CIPSTART=4,\"UDP\",\"");
  mySerial.print(WLAN_ADDR);
  mySerial.print("\",54321,");
  mySerial.print(PORT);
  mySerial.println(",0");
  resp = mySerial.find("OK\r\n");
  mySerial.setTimeout(1000);
  display.begin();
  display.setContrast(60);
  display.setRotation(2);
  display.display();
  display.println("Conectando...");
  display.display();
   /*
   do {
    mySerial.print("AT+CWJAP=\"");
    mySerial.print(WLAN_SSID);
    mySerial.print("\",\"");
    mySerial.print(WLAN_PASS);
    mySerial.println("\"");
    resp = mySerial.find("OK\r\n");
    Serial.println(resp);
    } while (!resp);
*/
  display.clearDisplay();
  display.display();

  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0, 0);
  display.println("Conectado!");
  display.display();
  stepper.setMaxSpeed(1600.0);
  stepper.setAcceleration(10000.0);
  stepper.setCurrentPosition(0);
  stepper.moveTo(motor_pos);
  previous_millis = 0;
  actual_millis = millis();
  KP2.SetKeypadVoltage(4.7);

  /*
  EEPROM.get(DIR_SENSOR_MIN, min_sensor);
  EEPROM.get(DIR_SENSOR_MAX, max_sensor);
  EEPROM.get(DIR_ACTUATOR_MIN, min_actuator);
  EEPROM.get(DIR_ACTUATOR_MAX, max_actuator);
  */

  EEPROM.put(DIR_SENSOR_MIN, 10);
  EEPROM.put(DIR_SENSOR_MAX, 400);
  EEPROM.put(DIR_ACTUATOR_MIN, 0);
  EEPROM.put(DIR_ACTUATOR_MAX, 400);

  
  EEPROM.get(DIR_BEZIER_A, bezier_A);
  EEPROM.get(DIR_BEZIER_B, bezier_B);
  EEPROM.get(DIR_BEZIER_C, bezier_C);
  EEPROM.get(DIR_BEZIER_D, bezier_D);
  Serial.println("done setup");
}

// position = 0 -> endstop al inicio
// position = 1 -> endstop al final

void endstop_action() {
  stepper.setCurrentPosition(0);
  if (last_motor_pos < motor_pos) {
    do {
      motor_pos--;
      stepper.moveTo(motor_pos);
      stepper.run();
    } while (digitalRead(12) == 0);
    stepper.runToNewPosition(-10);
    stepper.setCurrentPosition(0);
  }
  else if (last_motor_pos > motor_pos) {
    do {
      motor_pos++;
      stepper.moveTo(motor_pos);
      stepper.run();
    } while (digitalRead(12) == 0);
  }
  stepper.setCurrentPosition(0);
  motor_pos = 0;
}

void send_data() {
  OSCMessage msg("/b02s");
  msg.add((float)normalize);
  mySerial.println("AT+CIPSEND=4,16");
  mySerial.find(">");
  msg.send(mySerial);
  msg.empty();
}

void loop() {
  sonar_read = analogRead(A2) * 1.26;
  actual_millis = millis();
  diferencia = actual_millis - previous_millis;
  if (diferencia > 100) {
    previous_millis = actual_millis;
    mainmenu();
  }
  stepper.run();
  if ( char key = KP2.Getkey() ) {
    if (KP2.Key_State() == PRESSED) {
      switch (key) {
        case 'C':
          adjust();
          break;
        case 'B':
          manual();
          break;
        case 'A':
          automatic();
          break;
        case 'D':
          bezier();
          break;
      }
    }
  }
}

