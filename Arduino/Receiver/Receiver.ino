//Code for the In Ear Monitor receiver

#include <WiFi.h>
#include "driver/i2s.h"
#include "freertos/ringbuf.h"

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

//The buffer audio data will be stored in
//RingbufHandle_t buf = xRingbufferCreate(1024, RINGBUF_TYPE_NOSPLIT);

//i2s configuration
int i2s_num = 0; // i2s port number
i2s_config_t i2s_config = {
  .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
  .sample_rate = 16000,
  .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
  .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
  .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S),
  .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1, // high interrupt priority
  .dma_buf_count = 32, //number of buffers
  .dma_buf_len = 16   //size of each buffer
};

i2s_pin_config_t pin_config = {
  .bck_io_num = 26, //this is BCK pin
  .ws_io_num = 25, // this is LRCK pin
  .data_out_num = 22, // this is DATA output pin
  .data_in_num = -1   //Not used
};

//Length of the data to send through I2S in bytes
size_t len = 2;


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
  //A pointer to the mem location of audio
  //const void* ptr = &audio;
  //Length of the audio data sample
  //size_t audio_size = 16;
  //Stores the audio data in at the end of the ring buffer
  //xRingbufferSend(buf, ptr, audio_size, 1);
  //In this section, something will need to happen with timing, but I'm not sure what yet
  //Receive audio data from the beginning of the ring buffer
  //uint16_t *item = (uint16_t *)xRingbufferReceive(buf, &audio_size, 1);
  i2s_write(I2S_NUM_0, &audio, len, &len, 1);

}
