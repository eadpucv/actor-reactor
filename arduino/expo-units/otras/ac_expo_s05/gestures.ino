void bounce() {
  Serial.println("running...");
  stepper.runToNewPosition(max_actuator);
  delay(10);
  stepper.runToNewPosition(-max_actuator);
  delay(10);
  stepper.runToNewPosition(max_actuator);
  delay(10);
  stepper.runToNewPosition(-max_actuator);
  Serial.println("done!");
}

void breathe() {

  /* desde poco y lento a mucho y rápido */
  // más cerca, más lento
  maxvel = map(sonar_read, min_sensor, max_sensor, 100, 500);

  stepper.setMaxSpeed(maxvel);
  stepper.setSpeed(vel);
  stepper.setAcceleration(accel);

  stepper.runToNewPosition(max_actuator);
  delay(500);
  stepper.runToNewPosition(-max_actuator);
  delay(500);

}


