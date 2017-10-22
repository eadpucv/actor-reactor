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
  display.println("F3 - ajust");
  display.println("F4 - bezier");
  display.print("sensor: "); display.println(sonar_read);
  display.display();
}

/* F1 */
void automatic() {

  //!back = 1;
  starting = false; // se apaga el switch de inicio
  
  mainmenu_disp = !mainmenu_disp;
  display.clearDisplay();
  display.display();
  display.println("Automatic");
  display.println(" 0-F to exit");
  display.display();

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

      Serial.print("motor pos ");
      Serial.println(motor_pos);
      Serial.print("soft pos ");
      Serial.println(softenMotorPos);
    }

    sonar_read = constrain(sonar_read, min_sensor, max_sensor);
    normalize = map(sonar_read, min_sensor, max_sensor, 0, 10000);
    normalize = normalize / 10000;

    if (wifi) send_data();

    //motor_pos = DoubleQuadraticBezier(sonar_read, bezier_A, bezier_B, bezier_C, bezier_D);

    last_motor_pos = stepper.currentPosition();

    motor_pos = map(sonar_read, min_sensor, max_sensor, min_actuator, max_actuator);
    motor_pos = constrain(motor_pos, min_actuator, max_actuator);
    softenMotorPos = soften(motor_pos);

    endstop = digitalRead(12);
    if (!endstop)
      endstop_action();

    // stepper.moveTo(soften(motor_pos));
    // stepper.run();

    stepper.runToNewPosition(motor_pos);

    
    if ( char key = KP2.Getkey() ) {
      if (KP2.Key_State() == PRESSED) {
        switch (key) {
          case 'A':
            back = !back;
            break;
          case 'B':
            back = !back;
            break;
          case 'C':
            back = !back;
            break;
          case 'D':
            back = !back;
            break;
          case '0':
            back = !back;
            break;
        }
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

    key2 = KP2.Getkey();
    stepper.run();
    if (key2 == '6') {

      display.clearDisplay();
      display.display();
      display.print(stepper.currentPosition());
      display.display();
      last_motor_pos = motor_pos;
      motor_pos++;
      endstop = digitalRead(12);
      Serial.println(endstop);
      if (!endstop)
        endstop_action();

      stepper.moveTo(motor_pos);
      stepper.run();
    }
    if (key2 == '4') {

      display.clearDisplay();
      display.display();
      display.print(stepper.currentPosition());
      display.display();
      last_motor_pos = motor_pos;
      motor_pos--;
      endstop = digitalRead(12);
      Serial.println(endstop);
      if (!endstop)
        endstop_action();
      stepper.moveTo(motor_pos);
      stepper.run();
    }
    if (key2 == '0') {
      back = !back;
    }
  }
  back = !back;
}

/* F3 */
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
  while (!back == 1) {

    if ( char key = KP2.Getkey() ) {
      if (KP2.Key_State() == PRESSED) {
        switch (key) {
          case '0':
            back = !back;
            break;
          case 'A':
            adjust_sensor();
            back = !back;
            break;
          case 3:
            break;
          case 4:
            break;
        }
      }

    }
  }
  back = !back;
}



