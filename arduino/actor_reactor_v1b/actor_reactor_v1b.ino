
#include <OSCMessage.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <OnewireKeypad.h>
#include <AccelStepper.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>

#define DIR_SENSOR_MIN 0
#define DIR_SENSOR_MAX 5
#define DIR_ACTUATOR_MIN 10
#define DIR_ACTUATOR_MAX 15

#define DIR_BEZIER_A 20
#define DIR_BEZIER_B 25
#define DIR_BEZIER_C 30
#define DIR_BEZIER_D 35

#define ID 1
#define WLAN_ADDR  "192.168.0.255"
#define PORT  1112
#define WLAN_SSID  "cactus"
#define WLAN_PASS  "nicamembrillo"

float min_sensor = 0, min_actuator = 0, max_sensor = 0, max_actuator = 0;
float bezier_A = 0, bezier_B = 0, bezier_C = 0, bezier_D = 0, motor_pos = 0;
float EPSILON = 9.999999747378752E-5f;

//EEPROM.get(DIR_SENSOR_MIN, min_sensor);
//EEPROM.get(DIR_SENSOR_MAX, max_sensor);
//EEPROM.get(DIR_ACTUATOR_MAX, max_actuator);
char insert[10];
int i = 0;
double normalize;
boolean mainmenu_disp = 0, resp;
boolean salir = 0;
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
AccelStepper stepper(AccelStepper::DRIVER, 8, 9); // CLK+(paso) -> pin 8 --- CW+(direccion) -> pin 9

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
  mySerial.begin(57600);

  Serial.setTimeout(10000);
  digitalWrite(4, 0);
  pinMode(13, OUTPUT);
  delay(10);
  pinMode(13, INPUT);
  resp = mySerial.find("ready\r\n");
  mySerial.println("AT+CWMODE=1");
  resp = mySerial.find("OK\r\n");
  do {
    mySerial.print("AT+CWJAP=\"");
    mySerial.print(WLAN_SSID);
    mySerial.print("\",\"");
    mySerial.print(WLAN_PASS);
    mySerial.println("\"");
    resp = mySerial.find("OK\r\n");

  } while (!resp);
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
  stepper.setMaxSpeed(500.0);
  stepper.setAcceleration(10000.0);
  stepper.setCurrentPosition(0);
  stepper.moveTo(motor_pos);
  previous_millis = 0;
  actual_millis = millis();
  KP2.SetKeypadVoltage(4.7);
  EEPROM.get(DIR_SENSOR_MIN, min_sensor);
  EEPROM.get(DIR_SENSOR_MAX, max_sensor);
  EEPROM.get(DIR_ACTUATOR_MIN, min_actuator);
  EEPROM.get(DIR_ACTUATOR_MAX, max_actuator);

  EEPROM.get(DIR_BEZIER_A, bezier_A);
  EEPROM.get(DIR_BEZIER_B, bezier_B);
  EEPROM.get(DIR_BEZIER_C, bezier_C);
  EEPROM.get(DIR_BEZIER_D, bezier_D);

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

void adjust() {
  mainmenu_disp = !mainmenu_disp;
  display.clearDisplay();
  display.display();
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0, 0);
  display.println("Sensor Range");
  display.print("   Min: "); display.println(min_sensor, 0);
  display.print("   Max: "); display.println(max_sensor, 0);
  display.println("Act. Range");
  display.print("   Min: "); display.println(min_actuator, 0);
  display.print("   Max: "); display.println(max_actuator, 0);
  display.display();
  while (!salir == 1) {

    if ( char key = KP2.Getkey() ) {
      if (KP2.Key_State() == PRESSED) {
        switch (key) {
          case '0':
            salir = !salir;
            break;
          case 'A':
            adjust_sensor();
            salir = !salir;
            break;
          case 3:
            break;
          case 4:
            break;
        }
      }

    }
  }
  salir = !salir;
}

void mainmenu() {
  mainmenu_disp = !mainmenu_disp;
  display.clearDisplay();
  display.display();
  display.setContrast(60);
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0, 0);
  display.println("Main Menu");
  display.println("name");
  display.print("Sensor: "); display.println(sonar_read);
  display.display();
}

void manual() {
  mainmenu_disp = !mainmenu_disp;
  char key2;
  display.clearDisplay();
  display.display();

  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0, 0);
  display.print(stepper.currentPosition());
  display.display();
  while (!salir == 1) {
    key2 = KP2.Getkey();
    stepper.run();
    if (key2 == '6') {
      //stepper.setSpeed(100.0);
      display.clearDisplay();
      display.display();
      display.print(stepper.currentPosition());
      display.display();
      motor_pos++;
      stepper.moveTo(motor_pos);
      stepper.run();
    }
    if (key2 == '4') {
      //stepper.setSpeed(-100.0);
      display.clearDisplay();
      display.display();
      display.print(stepper.currentPosition());
      display.display();
      motor_pos--;
      stepper.moveTo(motor_pos);
      stepper.run();
    }
    if (key2 == '0') {
      salir = !salir;
    }
  }
  salir = !salir;
}

void automatic() {
  while (!salir == 1) {
    acum_sonar_read += analogRead(A2);
    analog_counter++;
    actual_millis = millis();
    diferencia = actual_millis - previous_millis;
    if (diferencia > 300) {
      previous_millis = actual_millis;
      sonar_read = acum_sonar_read / analog_counter * 1.26;
      acum_sonar_read = 0;
      analog_counter = 0;
    }
    sonar_read = constrain(sonar_read, min_sensor, max_sensor);
    normalize = map(sonar_read, min_sensor, max_sensor, 0, 10000);
    normalize = normalize / 10000;
    send_data();
    //motor_pos = DoubleQuadraticBezier(sonar_read, bezier_A, bezier_B, bezier_C, bezier_D);
    motor_pos = constrain(motor_pos, min_actuator, max_actuator);
    motor_pos = map(sonar_read, min_sensor, max_sensor, min_actuator, max_actuator);
    //Serial.println(motor_pos);

    //Serial.println(motor_pos);
    stepper.moveTo(motor_pos);
    stepper.run();
    if ( char key = KP2.Getkey() ) {
      if (KP2.Key_State() == PRESSED) {
        switch (key) {
          case '0':
            salir = !salir;
            break;
        }
      }

    }
  }
  salir = !salir;
}

void send_data() {
  OSCMessage msg("/bang/s01");
  msg.add((float)normalize);
  mySerial.println("AT+CIPSEND=4,16");
  mySerial.find(">");
  msg.send(mySerial);
  msg.empty();
}

