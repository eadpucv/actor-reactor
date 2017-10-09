/*
 *  arduino - puredata communication
 *  
 */

void send_data() {
  OSCMessage msg("/b02s");
  msg.add((float)normalize);
  wifiLink.println("AT+CIPSEND=4,16");
  wifiLink.find(">");
  msg.send(wifiLink);
  msg.empty();
}
