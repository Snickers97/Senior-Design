//Code for the In Ear Monitor transmitter

#include <WiFi.h>
#include <SPI.h>

//Sets the SPI settings for reading from the ADC
SPISettings adcSettings(705600, MSBFIRST, SPI_MODE0);

int bufsize = 256;

//MISO = 19  (third, yellow wire)
//SCK = 18    (second)
//SS = 5      (first)

//Network credentials
const char* ssid = "In-Ear-Transmitter";
const char* password = "123456789";

//Set server port number to 80
WiFiServer server(80);

void setup() {
  //Activate red LED
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  Serial.begin(230400);
 
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
  pinMode(MISO, INPUT);
  pinMode(SCK, OUTPUT);
  SPI.begin();

  
  
  
}

void loop() {
  WiFiClient client = server.available();     //Listen for devices

  if(client){
    Serial.println("Connected.");
    client.setTimeout(20);
    uint8_t val;
    uint16_t audio[bufsize/2];
    while(client.connected()){
      for(int i = 0; i < bufsize; i++){
        SPI.beginTransaction(adcSettings);
        digitalWrite(SS,LOW);
        val = SPI.transfer(0);    //garbage data, throw away
        //audio[i] = SPI.transfer(0);   //upper 8 bits
        //audio[i+1] = SPI.transfer(0); //lower 8 bits
        audio[i] = SPI.transfer16(0);
        Serial.println(audio[i]);
        //Serial.print("      ");
        //Serial.println(audio[i+1]);
        digitalWrite(SS,HIGH);
        SPI.endTransaction();
      }
      //client.write(audio,bufsize/2);
    }
    //Close the connection
    client.stop();
    Serial.println("Disconnected.");
  }
}
