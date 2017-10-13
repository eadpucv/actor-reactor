/*
 *  arduino - puredata communication
 *  
 */

void send_data() {
  OSCMessage msg("/bang/");
  msg.add(HOSTNAME);
<<<<<<< HEAD
// msg.add("/");
=======
  //msg.add("/");
>>>>>>> 1f8546508dcf0d57884f806b5198789727ca9be8
  msg.add((float)normalize);
  wifiLink.println("AT+CIPSEND=4,16");
  wifiLink.find(">");
  msg.send(wifiLink);
  msg.empty();
}
