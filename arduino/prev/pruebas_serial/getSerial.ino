void getSerial(){
  memset(serialdata, 0, sizeof(serialdata));
  
  int n = 0;
  int inbyte = 0;
  
  while (inbyte != '/')
  {
    inbyte = Serial.read();  
    if (inbyte > 0 && inbyte != '/')
    { 
      serialdata[n] = (char)inbyte;
      n++;
    }
  }
  
  
  inbyte = 0;
}
