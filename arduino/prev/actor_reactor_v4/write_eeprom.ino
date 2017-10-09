void write_eeprom(){
  min_sensor = 5;
  min_actuator = 0;
  max_sensor = 450;
  max_actuator = 800;

  EEPROM.put(DIR_SENSOR_MIN, min_sensor);
  EEPROM.put(DIR_SENSOR_MAX, max_sensor);
  EEPROM.put(DIR_ACTUATOR_MIN, min_actuator);
  EEPROM.put(DIR_ACTUATOR_MAX, max_actuator);  
}
