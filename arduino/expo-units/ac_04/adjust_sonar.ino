void adjust_sensor_min() {
  display.clearDisplay(); 
  display.display();
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0, 0);
  display.println("Insert number");
  display.display();
  while (!back == 1) {

    if ( Serial.available() ) {
      getSerial();
      switch (serialdata) {

          case 'Y':
          memset(serialdata, 0, sizeof(serialdata));
            i = 0;
            back = !back;
            break;
          case '1':
          memset(serialdata, 0, sizeof(serialdata));
            insert[i] = serialdata;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '2':
          memset(serialdata, 0, sizeof(serialdata));
            insert[i] = serialdata;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '3':
          memset(serialdata, 0, sizeof(serialdata));
            insert[i] = serialdata;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '4':
          memset(serialdata, 0, sizeof(serialdata));
            insert[i] = serialdata;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '5':
          memset(serialdata, 0, sizeof(serialdata));
            insert[i] = serialdata;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '6':
          memset(serialdata, 0, sizeof(serialdata));
            insert[i] = serialdata;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '7':
          memset(serialdata, 0, sizeof(serialdata));
            insert[i] = serialdata;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '8':
          memset(serialdata, 0, sizeof(serialdata));
            insert[i] = serialdata;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '9':
          memset(serialdata, 0, sizeof(serialdata));
            insert[i] = serialdata;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '0':
          memset(serialdata, 0, sizeof(serialdata));
            insert[i] = serialdata;
            display.print(insert[i]);
            display.display();
            i++;
            break;
        }

    }
  }
  min_sensor = atof(insert);
  EEPROM.put(DIR_SENSOR_MIN, min_sensor);
  for ( int i = 0; i < sizeof(insert);  ++i )
    insert[i] = (char)0;
  back = !back;
}

void adjust_sensor_max() {
  display.clearDisplay();
  display.display();

  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0, 0);
  display.println("Insert number");
  display.display();
  while (!back == 1) {

    if ( Serial.available() ) {
      getSerial();
      switch (serialdata) {

          case 'Y':
          memset(serialdata, 0, sizeof(serialdata));
            i = 0;
            back = !back;
            break;
          case '1':
          memset(serialdata, 0, sizeof(serialdata));
            insert[i] = serialdata;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '2':
          memset(serialdata, 0, sizeof(serialdata));
            insert[i] = serialdata;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '3':
          memset(serialdata, 0, sizeof(serialdata));
            insert[i] = serialdata;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '4':
          memset(serialdata, 0, sizeof(serialdata));
            insert[i] = serialdata;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '5':
          memset(serialdata, 0, sizeof(serialdata));
            insert[i] = serialdata;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '6':
          memset(serialdata, 0, sizeof(serialdata));
            insert[i] = serialdata;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '7':
          memset(serialdata, 0, sizeof(serialdata));
            insert[i] = serialdata;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '8':
          memset(serialdata, 0, sizeof(serialdata));
            insert[i] = serialdata;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '9':
          memset(serialdata, 0, sizeof(serialdata));
            insert[i] = serialdata;
            display.print(insert[i]);
            display.display();
            i++;
            break;
          case '0':
          memset(serialdata, 0, sizeof(serialdata));
            insert[i] = serialdata;
            display.print(insert[i]);
            display.display();
            i++;
            break;
        }

    }
  }
  
  max_sensor = atof(insert);
  EEPROM.put(DIR_SENSOR_MAX, max_sensor);
  for ( int i = 0; i < sizeof(insert);  ++i )
    insert[i] = (char)0;
  back = !back;
}

void adjust_sensor() {

  while (!back == 1) {
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

    if ( Serial.available() ) {
      getSerial();
      switch (serialdata) {

          case '0':
          memset(serialdata, 0, sizeof(serialdata));
            back = !back;
            break;
          case 'B':
          memset(serialdata, 0, sizeof(serialdata));
            adjust_sensor_max();
            show_adjust = 0;
            break;
          case 'A':
          memset(serialdata, 0, sizeof(serialdata));
            adjust_sensor_min();
            show_adjust = 0;
            break;
          case 'C':
          memset(serialdata, 0, sizeof(serialdata));
            adjust_actuator_min();
            show_adjust = 0;
            break;
          case 'D':
          memset(serialdata, 0, sizeof(serialdata));
            adjust_actuator_max();
            show_adjust = 0;
            break;
        }
    }
  }
  back = !back;
}
