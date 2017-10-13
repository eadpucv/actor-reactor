void write_eeprom(){

  /* smart defaults */ 
  min_sensor   = 5;
  min_actuator = 0;
  max_sensor   = 450;
  max_actuator = 400;

  bezier_A = .5;
  bezier_B = .5;
  bezier_C = .5;
  bezier_D = .5;

  EEPROM.put(DIR_SENSOR_MIN, min_sensor);
  EEPROM.put(DIR_SENSOR_MAX, max_sensor);
  EEPROM.put(DIR_ACTUATOR_MIN, min_actuator);
  EEPROM.put(DIR_ACTUATOR_MAX, max_actuator);  

  EEPROM.put(DIR_BEZIER_A, bezier_A);
  EEPROM.put(DIR_BEZIER_B, bezier_B);
  EEPROM.put(DIR_BEZIER_C, bezier_C);
  EEPROM.put(DIR_BEZIER_D, bezier_D);
}
