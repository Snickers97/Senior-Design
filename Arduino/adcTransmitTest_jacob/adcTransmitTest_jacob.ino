
#include <WiFi.h>
#include "driver/spi_master.h"  //allow use of spi master, is master
#include "driver/adc.h" //allow use of on board adc

//Network credentials
const char* ssid = "In-Ear-Transmitter";
const char* password = "123456789";

int bufsize = 1024;

//LED setup
const int sysOn = 13;

//Set server port number to 80
WiFiServer server(80);

void setup() {
  
  Serial.begin(115200);

  pinMode(sysOn, OUTPUT);
  digitalWrite(sysOn, HIGH);    //Enable power LED
 
  //WIFI SETUP SECTION
    //Activate WiFi network
  Serial.print("Setting up AP. . .");
  WiFi.softAP(ssid, password);
    //We can remove the password parameter if we want the network to be open
  IPAddress IP = WiFi.softAPIP();
  Serial.print("IP address: ");
  Serial.println(IP);
  server.begin();
  
  //ADC config
  adc1_config_width(ADC_WIDTH_BIT_12);  //configure precision
  adc1_config_channel_atten(ADC1_CHANNEL_0,ADC_ATTEN_DB_0); //0<=V_in<=1.1 @0dB attenution
    //ADC1_CHANNEL_0 => GPIO 36
}

void loop() {
  WiFiClient client = server.available();     //Listen for devices

  if(client){
    Serial.println("Connected.");
    client.setTimeout(20);
    uint16_t val = 0;
    uint8_t audio[bufsize];
    while(client.connected()){
      for(int i = 0; i < bufsize; i++){
        val = adc1_get_raw(ADC1_CHANNEL_0); //read in from ADC
        audio[i] = (val >> 4);    //right shift by 4, only use MS 8 bits
        //Serial.println(audio[i]);
      }
      client.write(audio,bufsize);
    }
    //Close the connection
    client.stop();
    Serial.println("Disconnected.");
  }
}
