void bounce() {
  stepper.runToNewPosition(max_actuator);
  delay(500);
  stepper.runToNewPosition(-max_actuator);
  delay(500);
}

void randomMode() {
  float newPos = random(max_actuator);
  stepper.runToNewPosition(newPos);
  delay(random(5000, 20000));
}

void sonarMode() {
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
  softenMotorPos = soften(motor_pos);

  stepper.runToNewPosition(motor_pos);
  delay(random(1000, 20000));
}

