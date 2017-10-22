char serialdata[100];
boolean ans = 0;

void setup()
{
  Serial.begin(57600);  
}




void loop(){
  if(Serial.available())
    getSerial();
  ans = strcmp(serialdata, "automatic");
  if(!ans){
    Serial.println("Bien CTM!");
    memset(serialdata, 0, sizeof(serialdata));
  }
}




