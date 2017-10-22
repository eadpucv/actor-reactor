void adjust_actuator_max() {
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
  max_actuator = atof(insert);
  EEPROM.put(DIR_ACTUATOR_MAX, max_actuator);
  for ( int i = 0; i < sizeof(insert);  ++i )
    insert[i] = (char)0;
  back = !back;
}


void adjust_actuator_min() {
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
  min_actuator = atof(insert);
  EEPROM.put(DIR_ACTUATOR_MIN, min_actuator);
  for ( int i = 0; i < sizeof(insert);  ++i )
    insert[i] = (char)0;
  back = !back;
}
