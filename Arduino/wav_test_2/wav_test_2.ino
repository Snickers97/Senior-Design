//Code for the In Ear Monitor transmitter

#include <WiFi.h>
#include "driver/i2s.h"
#include "Thrones.h"

//i2s configuration
static const int i2s_num = 0; // i2s port number
i2s_config_t i2s_config = {
  .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
  .sample_rate = 22050,
  .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
  .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
  .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
  .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1, // high interrupt priority
  .dma_buf_count = 32, //number of buffers
  .dma_buf_len = 16,   //size of each buffer
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
  //Activate red LED
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM_0, &pin_config);
  Serial.begin(115200);
 

}

void loop() {
  uint8_t audio;
  void *src;
  src = &audio;
  for(int i = 0; i < 87400; i++){
    audio = thrones[i];
    i2s_write(I2S_NUM_0, src, len, &len, 1000);
    //Serial.println(audio);
  }
  delay(1000);
}
