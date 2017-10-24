void write_eeprom() {
  min_sensor = 80;
  max_sensor = 270;

  min_actuator = 0;
  max_actuator = 160;

  bezier_A = 0;
  bezier_B = 0;
  bezier_C = 1;
  bezier_D = 1;
  
  EEPROM.put(DIR_SENSOR_MIN, min_sensor);
  EEPROM.put(DIR_SENSOR_MAX, max_sensor);
  EEPROM.put(DIR_ACTUATOR_MIN, min_actuator);
  EEPROM.put(DIR_ACTUATOR_MAX, max_actuator);

  EEPROM.put(DIR_BEZIER_A, bezier_A);
  EEPROM.put(DIR_BEZIER_B, bezier_B);
  EEPROM.put(DIR_BEZIER_C, bezier_C);
  EEPROM.put(DIR_BEZIER_D, bezier_D);
}
