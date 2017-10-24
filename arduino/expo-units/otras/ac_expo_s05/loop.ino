
void loop() {
  // lectura del sensor
  sonar_read = analogRead(A2) * 1.26;
  actual_millis = millis();
  diferencia = actual_millis - previous_millis;
  sonar_read = constrain(sonar_read, min_sensor, max_sensor);

  // modificar datos para pure data
  normalize = map(sonar_read, min_sensor, max_sensor, 0, 10000);
  normalize = normalize / 10000;
  if (wifi) send_data();
  
  Serial.print(sonar_read);
  Serial.print("\t");
  Serial.println(normalize);
  
  if(sonar_read < triggerDist){
    bounce();
  }
  
  if ( Serial.available() ) {
    getSerial();
    Serial.println(serialdata);
    switch (serialdata) {

      /* F1 */
      case 'A':
        Serial.println("Automatic mode");
        memset(serialdata, 0, sizeof(serialdata));
        automatic();
        break;

      /* F2 */
      case 'B':
        Serial.println("Manual mode");
        memset(serialdata, 0, sizeof(serialdata));
        manual();
        break;
    }
  }
}

