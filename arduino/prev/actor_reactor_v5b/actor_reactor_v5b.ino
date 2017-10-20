#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <OnewireKeypad.h>
#include <AccelStepper.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>
#include <OSCMessage.h>

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
/*
#define WLAN_SSID  "MadLab"
#define WLAN_PASS  "12345678"
*/

#define WLAN_SSID  "AC"
#define WLAN_PASS  "actor-reactor"

float min_sensor = 0, min_actuator = 0, max_sensor = 0, max_actuator = 0;
float bezier_A = 0, bezier_B = 0, bezier_C = 0, bezier_D = 0, motor_pos = 0;
float EPSILON = 9.999999747378752E-5f;

// motor_soft 
float motor_soft = 0;
float A = 0.98;
float B = 0.02;

//EEPROM.get(DIR_SENSOR_MIN, min_sensor);
//EEPROM.get(DIR_SENSOR_MAX, max_sensor);
//EEPROM.get(DIR_ACTUATOR_MAX, max_actuator);
char insert[10];
int i = 0;
double normalize;
boolean mainmenu_disp = 0, resp;
boolean salir = 0;
boolean endstop = 1, endstop_activation = 0, endstop_position = 0;
boolean show_adjust = 0;
long motor_pos_previous = 0;
float sonar_read, acum_sonar_read, last_motor_pos;
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
  mySerial.begin(57600);

  Serial.setTimeout(10000);
  pinMode(12, INPUT);
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
    Serial.println(resp);
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
  
  stepper.setMaxSpeed(200.0);
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
  Serial.println("pase setup");

}


float DoubleQuadraticBezier (float x, float a, float b, float c, float d) {
  // functionName = "Double Quadratic Bezier";
  // also see http://engineeringtraining.tpub.com/14069/css/14069_150.htm
  // produces mysterious values when a=0,b=1,c=0.667,d=0.417

  float xmid = (a + c) / 2.0;
  float ymid = (b + d) / 2.0;
  xmid = constrain (xmid, EPSILON, 1.0 - EPSILON);
  ymid = constrain (ymid, EPSILON, 1.0 - EPSILON);

  float y = 0;
  float om2a;
  float t;
  float xx;
  float aa;
  float bb;

  if (x <= xmid) {
    xx = x / xmid;
    aa = a / xmid;
    bb = b / ymid;
    om2a = 1.0 - 2.0 * aa;
    if (om2a == 0) {
      om2a = EPSILON;
    }
    t = (sqrt(aa * aa + om2a * xx) - aa) / om2a;
    y = (1.0 - 2.0 * bb) * (t * t) + (2 * bb) * t;
    y *= ymid;
  }
  else {
    xx = (x - xmid) / (1.0 - xmid);
    aa = (c - xmid) / (1.0 - xmid);
    bb = (d - ymid) / (1.0 - ymid);
    om2a = 1.0 - 2.0 * aa;
    if (om2a == 0) {
      om2a = EPSILON;
    }
    t = (sqrt(aa * aa + om2a * xx) - aa) / om2a;
    y = (1.0 - 2.0 * bb) * (t * t) + (2 * bb) * t;
    y *= (1.0 - ymid);
    y += ymid;
  }

  return y;
}

void adjust_A() {
  display.clearDisplay();
  display.display();

  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0, 0);
  display.println("Insert number");
  display.display();
  while (!salir == 1) {

    if ( char key = KP2.Getkey() ) {
      if (KP2.Key_State() == PRESSED) {
        switch (key) {

          case 'Y':
            i = 0;
            salir = !salir;
            break;
          case '1':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '2':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '3':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '4':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '5':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '6':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '7':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '8':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '9':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '0':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
        }
      }

    }
  }
  bezier_A = atof(insert);
  bezier_A = bezier_A / 1000;
  EEPROM.put(DIR_BEZIER_A, bezier_A);
  for ( int i = 0; i < sizeof(insert);  ++i )
    insert[i] = (char)0;
  salir = !salir;
}

void adjust_B() {
  display.clearDisplay();
  display.display();

  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0, 0);
  display.println("Insert number");
  display.display();
  while (!salir == 1) {

    if ( char key = KP2.Getkey() ) {
      if (KP2.Key_State() == PRESSED) {
        switch (key) {

          case 'Y':
            i = 0;
            salir = !salir;
            break;
          case '1':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '2':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '3':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '4':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '5':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '6':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '7':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '8':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '9':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '0':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
        }
      }

    }
  }
  bezier_B = atof(insert) / 1000;
  EEPROM.put(DIR_BEZIER_B, bezier_B);
  for ( int i = 0; i < sizeof(insert);  ++i )
    insert[i] = (char)0;
  salir = !salir;
}

void adjust_C() {
  display.clearDisplay();
  display.display();

  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0, 0);
  display.println("Insert number");
  display.display();
  while (!salir == 1) {

    if ( char key = KP2.Getkey() ) {
      if (KP2.Key_State() == PRESSED) {
        switch (key) {

          case 'Y':
            i = 0;
            salir = !salir;
            break;
          case '1':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '2':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '3':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '4':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '5':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '6':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '7':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '8':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '9':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '0':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
        }
      }

    }
  }
  bezier_C = atof(insert) / 1000;
  EEPROM.put(DIR_BEZIER_C, bezier_C);
  for ( int i = 0; i < sizeof(insert);  ++i )
    insert[i] = (char)0;
  salir = !salir;
}

void adjust_D() {
  display.clearDisplay();
  display.display();

  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0, 0);
  display.println("Insert number");
  display.display();
  while (!salir == 1) {

    if ( char key = KP2.Getkey() ) {
      if (KP2.Key_State() == PRESSED) {
        switch (key) {

          case 'Y':
            i = 0;
            salir = !salir;
            break;
          case '1':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '2':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '3':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '4':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '5':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '6':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '7':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '8':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '9':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '0':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
        }
      }

    }
  }
  bezier_D = atof(insert) / 1000;
  EEPROM.put(DIR_BEZIER_D, bezier_D);
  for ( int i = 0; i < sizeof(insert);  ++i )
    insert[i] = (char)0;
  salir = !salir;
}



void bezier() {
  while (!salir == 1) {
    if (show_adjust == 0) {
      display.clearDisplay();
      display.display();

      display.setTextSize(1);
      display.setTextColor(BLACK);
      display.setCursor(0, 0);
      display.println("Set Variables");
      display.print("   A: F1 "); display.println(bezier_A, 3);
      display.print("   B: F2 "); display.println(bezier_B, 3);
      display.print("   C: F3 "); display.println(bezier_C, 3);
      display.print("   D: F4 "); display.println(bezier_D, 3);
      display.display();
      show_adjust = 1;
    }

    if ( char key = KP2.Getkey() ) {
      if (KP2.Key_State() == PRESSED) {
        switch (key) {
          case '0':
            show_adjust = 0;
            salir = !salir;
            break;
          case 'B':
            adjust_B();
            show_adjust = 0;
            break;
          case 'A':
            adjust_A();
            show_adjust = 0;
            break;
          case 'C':
            adjust_C();
            show_adjust = 0;
            break;
          case 'D':
            adjust_D();
            show_adjust = 0;
            break;
        }
      }

    }
  }
  salir = !salir;
}





void adjust_actuator_max() {
  display.clearDisplay();
  display.display();

  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0, 0);
  display.println("Insert number");
  display.display();
  while (!salir == 1) {

    if ( char key = KP2.Getkey() ) {
      if (KP2.Key_State() == PRESSED) {
        switch (key) {

          case 'Y':
            i = 0;
            salir = !salir;
            break;
          case '1':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '2':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '3':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '4':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '5':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '6':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '7':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '8':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '9':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '0':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
        }
      }

    }
  }
  max_actuator = atof(insert);
  EEPROM.put(DIR_ACTUATOR_MAX, max_actuator);
  for ( int i = 0; i < sizeof(insert);  ++i )
    insert[i] = (char)0;
  salir = !salir;
}


void adjust_actuator_min() {
  display.clearDisplay();
  display.display();

  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0, 0);
  display.println("Insert number");
  display.display();
  while (!salir == 1) {

    if ( char key = KP2.Getkey() ) {
      if (KP2.Key_State() == PRESSED) {
        switch (key) {

          case 'Y':
            i = 0;
            salir = !salir;
            break;
          case '1':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '2':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '3':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '4':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '5':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '6':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '7':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '8':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '9':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '0':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
        }
      }

    }
  }
  min_actuator = atof(insert);
  EEPROM.put(DIR_ACTUATOR_MIN, min_actuator);
  for ( int i = 0; i < sizeof(insert);  ++i )
    insert[i] = (char)0;
  salir = !salir;
}

void adjust_sensor_min() {
  display.clearDisplay();
  display.display();

  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0, 0);
  display.println("Insert number");
  display.display();
  while (!salir == 1) {

    if ( char key = KP2.Getkey() ) {
      if (KP2.Key_State() == PRESSED) {
        switch (key) {

          case 'Y':
            i = 0;
            salir = !salir;
            break;
          case '1':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '2':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '3':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '4':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '5':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '6':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '7':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '8':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '9':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '0':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
        }
      }

    }
  }
  min_sensor = atof(insert);
  EEPROM.put(DIR_SENSOR_MIN, min_sensor);
  for ( int i = 0; i < sizeof(insert);  ++i )
    insert[i] = (char)0;
  salir = !salir;
}




void adjust_sensor_max() {
  display.clearDisplay();
  display.display();

  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0, 0);
  display.println("Insert number");
  display.display();
  while (!salir == 1) {

    if ( char key = KP2.Getkey() ) {
      if (KP2.Key_State() == PRESSED) {
        switch (key) {
          case 'Y':
            i = 0;
            salir = !salir;
            break;
          case '1':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '2':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '3':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '4':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '5':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '6':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '7':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '8':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '9':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '0':
            insert[i] = key;
            display.print(insert[i]);
            display.display();
            i++;
            break;
        }
      }

    }
  }
  max_sensor = atof(insert);
  EEPROM.put(DIR_SENSOR_MAX, max_sensor);
  for ( int i = 0; i < sizeof(insert);  ++i )
    insert[i] = (char)0;
  salir = !salir;
}

void adjust_sensor() {

  while (!salir == 1) {
    if (show_adjust == 0) {
      display.clearDisplay();
      display.display();

      display.setTextSize(1);
      display.setTextColor(BLACK);
      display.setCursor(0, 0);
      display.println("Set Sensor");
      display.println("   Min: F1");
      display.println("   Max: F2");
      display.println("Set Motor");
      display.println("   Min: F3");
      display.println("   Max: F4");
      display.display();
      show_adjust = 1;
    }

    if ( char key = KP2.Getkey() ) {
      if (KP2.Key_State() == PRESSED) {
        switch (key) {
          case '0':
            salir = !salir;
            break;
          case 'B':
            adjust_sensor_max();
            show_adjust = 0;
            break;
          case 'A':
            adjust_sensor_min();
            show_adjust = 0;
            break;
          case 'C':
            adjust_actuator_min();
            show_adjust = 0;
            break;
          case 'D':
            adjust_actuator_max();
            show_adjust = 0;
            break;
        }
      }

    }
  }
  salir = !salir;
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

// position = 0 -> endstop al inicio
// position = 1 -> endstop al final
void endstop_action() {
  stepper.setCurrentPosition(0);
  if (last_motor_pos < motor_pos){
    stepper.runToNewPosition(-10);
    stepper.setCurrentPosition(0);
  }
  if (last_motor_pos > motor_pos){
    stepper.runToNewPosition(10);
    stepper.setCurrentPosition(0);
  }
  
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
      endstop = digitalRead(12);
      Serial.println(!endstop);
      if (!endstop)
        endstop_action();
        
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
      endstop = digitalRead(12);
      Serial.println(!endstop);
      if (!endstop)
        endstop_action();
        
      stepper.moveTo(motor_pos);
      stepper.run();
    }
    if (key2 == '0') {
      salir = !salir;
    }
  }
  salir = !salir;
}

void send_data() {
  Serial.println(normalize);
  OSCMessage msg("/b02s");
  //msg.add("/");
  msg.add((float)normalize);
  mySerial.println("AT+CIPSEND=4,16");
  mySerial.find(">");
  msg.send(mySerial);
  msg.empty();
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
    normalize = normalize / 10000.0;
    send_data();
    //motor_pos = DoubleQuadraticBezier(sonar_read, bezier_A, bezier_B, bezier_C, bezier_D);
    last_motor_pos = motor_pos;
    motor_pos = constrain(motor_pos, min_actuator, max_actuator);
    motor_pos = map(sonar_read, min_sensor, max_sensor, min_actuator, max_actuator);  
    
    motor_soft = last_motor_pos * A + motor_pos * B;

    last_motor_pos = motor_soft;// motor_pos;
    
    //Serial.println(motor_pos);
    endstop = digitalRead(12);
      if (!endstop)
        endstop_action();
        
    //Serial.println(motor_pos);
    stepper.moveTo(motor_soft);
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

