//Code for the In Ear Monitor transmitter

#include <WiFi.h>
#include "SoundData.h"

//Network credentials
const char* ssid = "In-Ear-Transmitter";
const char* password = "123456789";

//Set server port number to 80
WiFiServer server(80);

void setup() {
  //Activate red LED
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  Serial.begin(115200);
 
  //WIFI SETUP SECTION
  //Activate WiFi network
  Serial.print("Setting up AP. . .");
  WiFi.softAP(ssid, password);
    //We can remove the password parameter if we want the network to be open
  IPAddress IP = WiFi.softAPIP();
  Serial.print("IP address: ");
  Serial.println(IP);
  server.begin();
  
  
}

void loop() {
  WiFiClient client = server.available();     //Listen for devices

  if(client){
    Serial.println("Connected.");
    client.setTimeout(20);
    //uint8_t audio[256];
    while(client.connected()){
      for(int i = 0; i < 40923; i++){
        /*for(int j = 0; j < 256; j++){
          audio[j] = Force[i+j];
        }*/
        client.write(Force[i]);
      }
      delay(1000);
    }
    //Close the connection
    client.stop();
    Serial.println("Disconnected.");
  }
}
