#include <SoftwareSerial.h>
SoftwareSerial ESP(11, 10); // RX | TX

/* 

 
AT+UART_DEF=57600,8,1,0,0



*/
void setup()
  {  
    
     Serial.begin(57600);
     ESP.begin(57600);

      //Serial.begin(115200);
      //ESP.begin(115200);
  }

void loop()
  {
     // Repetir lo recibido por el ESP8266 hacia el monitor serial
     if (ESP.available())
         { char c = ESP.read() ;
           Serial.print(c);
         }

     // Repetir lo recibido por el monitor serial hacia el ESP8266
     if (Serial.available())
         {  char c = Serial.read();
            ESP.print(c);
         }
   }
