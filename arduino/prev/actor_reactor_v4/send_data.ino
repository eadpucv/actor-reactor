/*
 *  arduino - puredata communication
 *  
 */

void send_data() {
  OSCMessage msg("/bang/s01");
  msg.add((float)normalize);
  mySerial.println("AT+CIPSEND=4,16");
  mySerial.find(">");
  msg.send(mySerial);
  msg.empty();
}
