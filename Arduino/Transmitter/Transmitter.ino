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
    Serial.println("Connected.");
    uint16_t audio;
    while(client.connected()){
      audio = adc_read();
      client.write(audio);    //This likely won't be the way it actually works
    }
    //Close the connection
    client.stop();
    Serial.println("Disconnected.");
  }
}

uint16_t adc_read(){
  SPI.beginTransaction(adcSettings);
  //Enable the ADC
  digitalWrite(SS, LOW);
  //stores the received data in variable "audio"
  //The data sent doesn't matter, as we are reading only
  uint16_t audio = SPI.transfer16(0);
  //digitalWrite(SS, HIGH);
  //SPI.endTransaction();
  return audio;
}
