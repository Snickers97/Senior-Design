//Code for the In Ear Monitor transmitter

#include <WiFi.h>
#include "Force.h"

//Network credentials
const char* ssid = "In-Ear-Transmitter";
const char* password = "123456789";

//Buffer size for transmission
int bufsize = 64;    //The smaller this is, the smaller the latency, but smaller values increase risk of data unavailability on the Rx side
                    //So far I've found values in the range 16-256 to be most effective (powers of 2 only!)

//Set server port number
WiFiServer server(80);

void setup() {
  //Activate red LED
  pinMode(13, OUTPUT);
  pinMode(26, OUTPUT);    //LED for latency testing
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
    while(client.connected()){
      for(int i = 0; i < 87424; i+=bufsize){
        uint8_t audio[bufsize] = {0x80};
        for(int j = 0; j < bufsize; j++){
          if(i+j >= 87424){
            break;
          }
          audio[j] = Force[i+j];
        }
        client.write(audio,bufsize);
        digitalWrite(26, HIGH);
      }
      digitalWrite(26, LOW);
      delay(1000);
    }
    //Close the connection
    client.stop();
    Serial.println("Disconnected.");
  }
}
