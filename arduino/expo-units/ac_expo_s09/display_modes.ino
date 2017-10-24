/* 0 */
void mainmenu() {
  mainmenu_disp = !mainmenu_disp;
  display.clearDisplay();
  display.display();
  display.setContrast(60);
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0, 0);
  display.println("Actor Reactor");
  display.println("F1 - auto");
  display.println("F2 - manual");
  display.print("sensor: "); display.println(sonar_read);
  display.display();
}

/* F1 */
void automatic() {
  //!back = 1;
  starting = false; // se apaga el switch de inicio
  mainmenu_disp = !mainmenu_disp;

  while (!back == 1) {
    acum_sonar_read += analogRead(A2);
    analog_counter++;
    actual_millis = millis();
    diferencia = actual_millis - previous_millis;
    last_motor_pos = motor_pos;

    if (diferencia > 300) {
      previous_millis = actual_millis;
      sonar_read = acum_sonar_read / analog_counter * 1.26;
      acum_sonar_read = 0;
      analog_counter = 0;
    }

    sonar_read = constrain(sonar_read, min_sensor, max_sensor);
    normalize = map(sonar_read, min_sensor, max_sensor, 0, 10000);
    normalize = normalize / 10000.0;

    if (wifi) send_data();

    //motor_pos = DoubleQuadraticBezier(sonar_read, bezier_A, bezier_B, bezier_C, bezier_D);

    last_motor_pos = stepper.currentPosition();

    motor_pos = map(sonar_read, max_sensor, min_sensor, min_actuator, max_actuator);
    motor_pos = constrain(motor_pos, min_actuator, max_actuator);
    softenMotorPos = soften(motor_pos, last_motor_pos);
    /*
      display.clearDisplay();
      display.display();
      display.println(NAME);
      display.println("\n");
      display.print("sonar ");
      display.print(sonar_read);
      display.print("/");
      display.print(soften(sonar_read));
      display.print("motor ");
      display.print(stepper.currentPosition());
      display.print("/");
      display.print(motor_pos);
      display.display();
    */
   

    /* movimiento original */
    stepper.moveTo(motor_pos);
    stepper.run();

    /* movimiento suavizado */
    // stepper.moveTo(soften(motor_pos));
    // stepper.run();

    /* movimiento de bloqueo */
    //stepper.runToNewPosition(motor_pos);


    if ( Serial.available() ) {
      getSerial();
      switch (serialdata) {
        case 'A':
          memset(serialdata, 0, sizeof(serialdata));
          back = !back;
          break;
        case 'B':
          memset(serialdata, 0, sizeof(serialdata));
          back = !back;
          break;
        case 'C':
          memset(serialdata, 0, sizeof(serialdata));
          back = !back;
          break;
        case 'D':
          memset(serialdata, 0, sizeof(serialdata));
          back = !back;
          break;
        case '0':
          memset(serialdata, 0, sizeof(serialdata));
          back = !back;
          break;
      }
    }
  }
  back = !back;
}

/* F2 */
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
  while (!back == 1) {

    if (Serial.available()) {
      getSerialMotor();
      char_2_int = atof(serialdata_motor);
    }

    display.clearDisplay();
    display.display();
    display.print(stepper.currentPosition());
    display.display();
    last_motor_pos = motor_pos;
    if (char_2_int != 9999)
      motor_pos = char_2_int;
   
    
    stepper.moveTo(motor_pos);
    stepper.run();
    //Para salir, escribir 9999 como posicion de motor
    if (char_2_int == 9999) {
      back = !back;
    }
  }
  back = !back;
}

