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

  mainmenu_disp = !mainmenu_disp;
  display.clearDisplay();
  display.display();
  display.println("Mode auto");
  display.println(" 0-F keys to exit");
  display.display();

  while (!exit == 1) {
    acum_sonar_read += analogRead(A2);
    analog_counter++;
    actual_millis = millis();
    diff = actual_millis - previous_millis;

    if (diff > 300) {
      previous_millis = actual_millis;
      sonar_read = acum_sonar_read / analog_counter * 1.26;
      acum_sonar_read = 0;
      analog_counter = 0;
    }

    sonar_read = constrain(sonar_read, min_sensor, max_sensor);
    normalize = map(sonar_read, min_sensor, max_sensor, 0, 10000);
    normalize = normalize / 10000;
    
    if(wifi) send_data(); // only send data if connection available 
    
    //motor_pos = DoubleQuadraticBezier(sonar_read, bezier_A, bezier_B, bezier_C, bezier_D);
    last_motor_pos = motor_pos;

    motor_pos = map(sonar_read, min_sensor, max_sensor, min_actuator, max_actuator);
    motor_pos = constrain(motor_pos, min_actuator, max_actuator);

    endstop = digitalRead(12);
    if (!endstop)
      endstop_action();

    stepper.moveTo(motor_pos);
    stepper.run();
    if ( char key = KP2.Getkey() ) {
      if (KP2.Key_State() == PRESSED) {
        switch (key) {
          /*
          case 'A':
            leave = !leave;
            break;
          case 'B':
            leave = !leave;
            break;
          case 'C':
            leave = !leave;
            break;
          case 'D':
            leave = !leave;
            break;
            */
          case '0':
            leave = !leave;
            break;
        }
      }
    }
  }
  leave = !leave;
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

  while (exit == 0) {
    key2 = KP2.Getkey();
    stepper.run();
    if (key2 == '6') {
      //stepper.setSpeed(100.0);
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
      //stepper.setSpeed(-100.0);
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
    switch (key2) {
      case 'A':
        leave = !leave;
        break;
      case 'B':
        leave = !leave;
        break;
      case 'C':
        leave = !leave;
        break;
      case 'D':
        leave = !leave;
        break;
      case '0':
        leave = !leave;
        break;
    }
  }
  leave = !leave;
}

/* F3 */
void adjust() {
  mainmenu_disp = !mainmenu_disp;
  display.clearDisplay();
  display.display();
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0, 0);
  display.println("sensor range");
  display.print("   min: "); display.println(min_sensor, 0);
  display.print("   max: "); display.println(max_sensor, 0);
  display.println("act range");
  display.print("   min: "); display.println(min_actuator, 0);
  display.print("   max: "); display.println(max_actuator, 0);
  display.display();
  while (!exit == 1) {

    if ( char key = KP2.Getkey() ) {
      if (KP2.Key_State() == PRESSED) {
        switch (key) {
          case '0':
            leave = !leave;
            break;
          case 'A':
            adjust_sensor();
            leave = !leave;
            break;
          case 3:
            break;
          case 4:
            break;
        }
      }

    }
  }
  leave = !leave;
}


/* F4 */
void bezier() {
  while (!exit == 1) {
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
            leave = !leave;
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
  leave = !leave;
}
