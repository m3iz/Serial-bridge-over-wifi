#include <WiFi.h>
#include <WiFiUdp.h>
 
// Set WiFi credentials
#define WIFI_SSID "define"
#define WIFI_PASS "11223345"
#define UDP_PORT 4210

bool connected = false;
int strl=0;

IPAddress broadcastIp;
// UDP
WiFiUDP UDP;
char packet[255];
char reply[] = "Packet received!";
char data_rcvd[1024];
void setup() {
  // Setup serial port
  Serial.begin(115200);
  Serial.println();
 
  // Begin WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASS);
 
  // Connecting to WiFi...
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  // Loop continuously while WiFi is not connected
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    Serial.print(".");
  }
 
  // Connected to WiFi
  Serial.println();
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());

  // Begin listening to UDP port
  UDP.begin(UDP_PORT);
  Serial.print("Listening on UDP port ");
  Serial.println(UDP_PORT);
  broadcastIp = WiFi.localIP();
  broadcastIp[3] = 255;
 
}

void loop() {
  if(!connected){
    delay(1);
    UDP.beginPacket(broadcastIp, UDP_PORT);
    UDP.print("t");
    UDP.endPacket();
  }
  // If packet received...
  int packetSize = UDP.parsePacket();
  if (packetSize) {
    Serial.print("Received packet! Size: ");
    Serial.println(packetSize); 
    int len = UDP.read(packet, 255);
    if (len > 0)
    {
      packet[len] = '\0';
    }
    if(packet[0]=='k'){
      connected = true;
      Serial.print("Connected\n");
    }
    if(packet[0]=='t'){
      UDP.beginPacket(UDP.remoteIP(), UDP.remotePort());
      UDP.print("kk");
      UDP.endPacket();
      Serial.print("Connecting packet recieved\n");
    }
    Serial.print("Packet received: ");
    Serial.println(packet);
    //Serial.write();
   // if(connected){
    // Send return packet
  //  UDP.beginPacket(UDP.remoteIP(), UDP.remotePort());
    //UDP.print("hello");
    //UDP.endPacket();
   // }

  }
  if(connected){
  bool ready = false;

    while(Serial.available()){
    ready= true;
    data_rcvd[strl] = Serial.read(); 
    delay(2);
    strl++;
    //Serial.print("Serial data recieved\n");
    }
    
  if(ready){
    Serial.print(data_rcvd);
    UDP.beginPacket(UDP.remoteIP(), UDP.remotePort());
    UDP.print(data_rcvd);
    UDP.endPacket();
    memset(data_rcvd, 0, 1024);
    ready = false;
    strl = 0;
  }
  
}
}