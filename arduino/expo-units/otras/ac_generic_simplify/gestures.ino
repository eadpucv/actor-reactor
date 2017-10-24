void bounce() {
  stepper.runToNewPosition(max_actuator);
  delay(500);
  stepper.runToNewPosition(-max_actuator);
  delay(500);
}

void wiggle() {

  /* desde poco y lento a mucho y rápido */

  maxvel = map(sonar_read, min_sensor, max_sensor, 

  normalize = map(sonar_read, min_sensor, max_sensor, 0, 10000);
  
  stepper.setMaxSpeed(maxvel);
  stepper.setSpeed(vel);
  stepper.setAcceleration(accel);

}

void breathe() {

  /* desde poco y rápido a mucho y lento */

}

