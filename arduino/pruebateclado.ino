#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <OnewireKeypad.h>
#include <AccelStepper.h>
#include <EEPROM.h>

#define DIR_SENSOR_MIN 0
#define DIR_SENSOR_MAX 10
#define DIR_ACTUATOR_MAX 20

float min_sensor = 0, min_actuator = 0, max_sensor = 0, max_actuator = 0;
//EEPROM.get(DIR_SENSOR_MIN, min_sensor);
//EEPROM.get(DIR_SENSOR_MAX, max_sensor);
//EEPROM.get(DIR_ACTUATOR_MAX, max_actuator);
char insert[10];
int i = 0;
boolean mainmenu_disp = 0;
boolean salir = 0;
boolean show_adjust = 0;
long motor_pos = 0, motor_pos_previous = 0;
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
AccelStepper stepper(AccelStepper::DRIVER, 8, 9); // CLK+ -> pin 8 --- CW+ -> pin 9   

char KEYS[] = {
  '1', '2', '3', 'A',
  '4', '5', '6', 'B',
  '7', '8', '9', 'C',
  'Y', '0', 'N', 'D'
};


OnewireKeypad <Print, 16 > KP2(Serial, KEYS, 4, 4, A0, 4700, 1000, ExtremePrec );

void setup () {
  Serial.begin(115200);
  display.begin();
  display.setContrast(60);
  display.display();
  stepper.setMaxSpeed(1000.0);
  stepper.setAcceleration(10000.0);
  stepper.setCurrentPosition(0);
  stepper.moveTo(motor_pos);
  previous_millis = 0;
  actual_millis = millis();
  KP2.SetKeypadVoltage(5.0);
  EEPROM.get(DIR_SENSOR_MIN, min_sensor);
  EEPROM.get(DIR_SENSOR_MAX, max_sensor);
  EEPROM.get(DIR_ACTUATOR_MAX, max_actuator);
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
          case 'D':
            i = 0;
            salir = !salir;
            break;
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
          case 'D':
            i = 0;
            salir = !salir;
            break;
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
          case 'D':
            salir = !salir;
            break;
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
      display.println("   Max: F3");
      display.display();
      show_adjust = 1;
    }

    if ( char key = KP2.Getkey() ) {
      if (KP2.Key_State() == PRESSED) {
        switch (key) {
          case 'D':
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
  display.print("   Max: "); display.println(max_actuator, 0);
  display.display();
  while (!salir == 1) {

    if ( char key = KP2.Getkey() ) {
      if (KP2.Key_State() == PRESSED) {
        switch (key) {
          case 'D':
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
      stepper.setSpeed(100.0);
      display.clearDisplay();
      display.display();
      display.print(stepper.currentPosition());
      display.display();
      motor_pos++;
      stepper.moveTo(motor_pos);
      stepper.run();
    }
    if (key2 == '4') {
      stepper.setSpeed(-100.0);
      display.clearDisplay();
      display.display();
      display.print(stepper.currentPosition());
      display.display();
      motor_pos--;
      stepper.moveTo(motor_pos);
      stepper.run();
    }
    if (key2 == 'Y') {
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
    motor_pos = map(sonar_read, min_sensor, max_sensor, 0, max_actuator);
    //Serial.println(motor_pos);
    stepper.moveTo(motor_pos);
    stepper.run();
    if ( char key = KP2.Getkey() ) {
      if (KP2.Key_State() == PRESSED) {
        switch (key) {
          case 'D':
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
        case 'A':
          adjust();
          break;
        case 'C':
          manual();
          break;
        case 'D':
          automatic();
          break;
        case 4:
          break;
      }
    }

  }


}

