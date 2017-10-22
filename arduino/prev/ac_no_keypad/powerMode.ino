void powerMode() {
  display.clearDisplay();
  display.display();
  display.setContrast(60);
  display.setTextSize(2);
  display.setTextColor(BLACK);
  display.setCursor(0, 0);
  display.println("Actor Reactor");
  display.println("automatic");
  display.print("sensor: "); display.println(sonar_read);
  display.display();

  acum_sonar_read += analogRead(A2);
  analog_counter++;

  previous_millis = actual_millis;
  sonar_read = acum_sonar_read / analog_counter * 1.26;
  acum_sonar_read = 0;
  analog_counter = 0;


  sonar_read = constrain(sonar_read, min_sensor, max_sensor);
  normalize = map(sonar_read, min_sensor, max_sensor, 0, 10000);
  normalize = normalize / 10000;

  if (wifi) send_data();

  last_motor_pos = stepper.currentPosition();
  motor_pos = map(sonar_read, min_sensor, max_sensor, min_actuator, max_actuator);
  motor_pos = constrain(motor_pos, min_actuator, max_actuator);


  Serial.print("motor pos ");
  Serial.println(motor_pos);
  Serial.print("soft pos ");
  Serial.println(softenMotorPos);

  softenMotorPos = soften(motor_pos);
  //stepper.moveTo(softenMotorPos);

  stepper.moveTo(motor_pos);
  stepper.run();
}
