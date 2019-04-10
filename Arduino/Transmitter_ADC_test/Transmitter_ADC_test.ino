//Code for the In Ear Monitor transmitter

#include <WiFi.h>
#include <SPI.h>

//Sets the SPI settings for reading from the ADC
SPISettings adcSettings(16000, MSBFIRST, SPI_MODE0);
  //These values are rough educated guesses, will adjust once we have the hardware

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

  //SPI SETUP SECTION
  pinMode(SS, OUTPUT);
  pinMode(MISO, INPUT);
  pinMode(SCK, OUTPUT);
  SPI.begin();

  
  
  
}

void loop() {
  uint8_t val;
  uint16_t audio;
  SPI.beginTransaction(adcSettings);
  digitalWrite(SS,LOW);
  val = SPI.transfer(0);
  audio = SPI.transfer16(0);
  digitalWrite(SS,HIGH);
  SPI.endTransaction();
  Serial.println(audio);
}
