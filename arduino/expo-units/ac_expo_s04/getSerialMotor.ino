void getSerialMotor(){
  memset(serialdata_motor, 0, sizeof(serialdata));
  
  int n = 0;
  int inbyte = 0;
  
  while (inbyte != '/')
  {
    inbyte = Serial.read();  
    if (inbyte > 0 && inbyte != '/')
    { 
      serialdata_motor[n] = (char)inbyte;
      n++;
    }
  }
  
  //Serial.println(n);
  inbyte = 0;
}
