#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <WiFiUDP.h>
#include <ESP8266WebServer.h>

extern "C"{
  #include "user_interface.h"
}

ESP8266WebServer webServer(80);
WiFiUDP udpServer; 


// trigger state is a simple mechanism that allows us to 
// repeat the same packet multiple times, and giving the client 
// a chance to figure out that it shouldn't retake another image. 
// we send only the characters 'a' to 'z' 
uint8_t triggerCount = 'a'; 


void setupServer(){
  WiFi.softAP(ACCESSPOINT_SSID, ACCESSPOINT_PASS, 9);
//  webServer.on("/", handleRoot);
}

void updateServer(){
//  webServer.handleClient();  
}


// informs all connected clients that now is the time to take a picture! 
void triggerServer(){
/*  udpServer.beginMulticast(WiFi.localIP(), {224, 0, 0, 1}, 10000);
  udpServer.write("NOW");
  udpServer.endPacket();  */
  // found here: http://www.esp8266.com/viewtopic.php?f=32&t=5669
  unsigned char softap_stations_cnt;
  struct station_info *stat_info;
  struct ip_addr *ip;
  uint32 uintaddress;

  triggerCount ++; 
  if(triggerCount>='z') triggerCount = 'a'; 
  

  softap_stations_cnt = wifi_softap_get_station_num(); // Count of stations which are connected to ESP8266 soft-AP
  stat_info = wifi_softap_get_station_info();
  while (stat_info != NULL) {
    ip = &stat_info->ip;
    uintaddress = ip->addr;
    for(int i = 0; i < 5; i++){
      udpServer.beginPacket(IPAddress(uintaddress), 7475);
      udpServer.write(&triggerCount,1); 
      udpServer.endPacket(); 
    }
    stat_info = STAILQ_NEXT(stat_info, next);
  }
}

void handleRoot() {
  webServer.send(200, "text/html", "<h1>You are connected</h1>");
}
