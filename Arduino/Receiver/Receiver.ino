//Code for the In Ear Monitor receiver

#include <WiFi.h>
#include "driver/i2s.h"

//The ssid and password set on the transmitter
const char* ssid = "In-Ear-Transmitter";
const char* password = "123456789";

//The IP address of the transmitter and the port it's using
const char * host = "192.168.4.1";
const uint16_t port = 80;

//LED pins, may change later
const int sysOn = 16;
const int WiFiOn = 17;
const int WiFiConnected = 18;

void setup() {
  digitalWrite(sysOn, HIGH);    //enables "system on" power indicator
  digitalWrite(WiFiOn, LOW);   //ensures wifi indicator is off initially
  digitalWrite(WiFiConnected, LOW);   //ensures the connection indicator is off initially
  Serial.begin(115200);
  //Initializes wifi with the settings we want to use
  WiFi.begin(ssid, password);
  //Runs until receiver is connected
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("...");
  }
  digitalWrite(WiFiOn, HIGH);   //enables the wifi indicator
  Serial.print("Connected: ");
  Serial.println(WiFi.localIP());

}

void loop() {
  //Declares WiFiClient object, used to establish connection
  WiFiClient client;
  //Runs until a connection is established
  if(!client.connect(host, port)){
    digitalWrite(WiFiConnected, LOW);
    delay(1000);
    return;
  }
  digitalWrite(WiFiConnected, HIGH);      //enables the connection indicator
  //Reads a byte sent from the transmitter and stores it in the variable "audio"
  uint16_t audio = client.read();
  //Send using I2S - seems extremely complicated

}
