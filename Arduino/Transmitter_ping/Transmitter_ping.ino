//Code for the In Ear Monitor transmitter

#include <WiFi.h>
#include <SPI.h>

//Sets the SPI settings for reading from the ADC
SPISettings adcSettings(705600, MSBFIRST, SPI_MODE0);
  //These values are rough educated guesses, will adjust once we have the hardware

//Network credentials
const char* ssid = "In-Ear-Transmitter";
const char* password = "123456789";

//Set server port number to 80
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
 
  //WIFI SETUP SECTION
  //Activate WiFi network
  Serial.print("Setting up AP. . .");
  WiFi.softAP(ssid, password);
    //We can remove the password parameter if we want the network to be open
  IPAddress IP = WiFi.softAPIP();
  Serial.print("IP address: ");
  Serial.println(IP);
  server.begin();

  //SPI SETUP SECTION
  pinMode(SS, OUTPUT);
  //Ensures the ADC starts disabled (high signal disables devices)
  digitalWrite(SS, HIGH);
  SPI.begin();
  
  
}

void loop() {
  WiFiClient client = server.available();     //Listen for devices

  if(client){
    unsigned long time1, time2, diff;
    Serial.println("Connected.");
    uint8_t x[256] = {15};
    uint8_t y = 15;
    client.setTimeout(20);
    time1 = millis();
    client.write(x,256);
    while(client.connected()){
      if(client.read() == y){
        time2 = millis();
        diff = time2-time1;
        Serial.println(diff);
        //Serial.println(x);
        //delay(10);
        time1 = millis();
        client.write(x,16);
      }
    }
    //Close the connection
    client.stop();
    Serial.println("Disconnected.");
  }
}
