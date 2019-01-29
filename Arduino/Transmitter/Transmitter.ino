//Code for the In Ear Monitor project

//The WiFi library we'll be using
#include <WiFi.h>

//Network credentials
const char* ssid = "In-Ear Transmitter";
const char* password = "123456789";

//Set server port number to 80
WiFiServer server(80);

void setup() {
  Serial.begin(115200);

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
    while(client.connected()){
      //Send audio data to the client
    }
    //Close the connection
    client.stop();
    Serial.println("Disconnected.");
  }
}
