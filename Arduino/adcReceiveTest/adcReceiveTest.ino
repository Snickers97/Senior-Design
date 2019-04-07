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
const int sysOn = 13;           //Onboard LED
const int WiFiOn = 15;          //Red LED
const int latency = 32;   //Yellow LED
  

//i2s configuration
static const int i2s_num = 0; // i2s port number
i2s_config_t i2s_config = {
  .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
  .sample_rate = 24000,
  .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
  .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
  .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
  .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1, // high interrupt priority
  .dma_buf_count = 128, //number of buffers
  .dma_buf_len = 8,   //size of each buffer
  .use_apll = false,
  .tx_desc_auto_clear = false
};

i2s_pin_config_t pin_config = {
  .bck_io_num = 12, //this is BCLK pin
  .ws_io_num = 27, // this is LRC pin
  .data_out_num = 33, // this is DATA output pin
  .data_in_num = -1   //Not used
};

//Length of the data to send through I2S in bytes
size_t len = 2;


void setup() {
  pinMode(sysOn, OUTPUT);
  pinMode(WiFiOn, OUTPUT);
  pinMode(latency, OUTPUT);
  digitalWrite(sysOn, HIGH);    //enables "system on" power indicator
  digitalWrite(WiFiOn, LOW);   //ensures wifi indicator is off initially
  digitalWrite(latency, LOW);   //ensures the connection indicator is off initially
  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM_0, &pin_config);
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
    digitalWrite(latency, LOW);
    delay(1000);
    return;
  }
  client.setTimeout(20);
  uint8_t audio;
  void *src;
  src = &audio;
  while(client.connected()){
    audio = client.read();
    i2s_write(I2S_NUM_0, src, len, &len, 100);
    i2s_write(I2S_NUM_0, src, len, &len, 100);
  }
  client.stop();
  if(WiFi.status() == WL_CONNECTION_LOST){
    digitalWrite(WiFiOn, LOW);
  }

}
