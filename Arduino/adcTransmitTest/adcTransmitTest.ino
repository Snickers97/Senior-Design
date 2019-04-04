
#include <WiFi.h>
#include <driver/adc.h>   //allows use of onboard adc for testing
#include "driver/spi_master.h"  //allow use of spi master, is master
#include "driver/i2s.h" 
#include "driver/adc.h" //allow use of on board adc

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
    while(client.connected()){
//      digitalWrite(26, HIGH);
      uint16_t val = adc1_get_raw(ADC1_CHANNEL_0); //read in from ADC
//      for(int i = 0; i < 40923; i+=bufsize){
//        uint8_t audio[bufsize] = {0x80};
//        for(int j = 0; j < bufsize; j++){
//          if(i+j >= 40923){
//            break;
//          }
//          audio[j] = Force[i+j];
//        }
//        client.write(audio,bufsize);
//      }
      client.write(val);
      Serial.println(val);
//      digitalWrite(26, LOW);
      delay(1000);
    }
    //Close the connection
    client.stop();
    Serial.println("Disconnected.");
  }
}
