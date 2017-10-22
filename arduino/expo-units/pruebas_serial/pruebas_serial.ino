char serialdata[100];
boolean ans = 0;

void setup()
{
  Serial.begin(57600);  
}




void loop(){
  getSerial();
  ans = strcmp(serialdata, "automatic");
  if(!ans)
    Serial.println("Bien CTM!");
}




