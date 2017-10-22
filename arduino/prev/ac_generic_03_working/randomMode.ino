void randomMode(){
  float newPos = random(max_actuator);
  stepper.runToNewPosition(newPos);
  delay(random(5000, 30000)); 
}

