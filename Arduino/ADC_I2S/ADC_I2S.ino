
#include "driver/spi_master.h"  //allow use of spi master, is master
#include "driver/adc.h" //allow use of on board adc
#include "driver/i2s.h"

const int sysOn = 13;

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
  
  Serial.begin(115200);

  pinMode(sysOn, OUTPUT);
  digitalWrite(sysOn, HIGH);    //Enable power LED

    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM_0, &pin_config);

    //ADC config
  adc1_config_width(ADC_WIDTH_BIT_12);  //configure precision
  adc1_config_channel_atten(ADC1_CHANNEL_0,ADC_ATTEN_DB_0); //0<=V_in<=1.1 @0dB attenution
    //ADC1_CHANNEL_0 => GPIO 36
}

void loop() {
  uint8_t audio;
  void *src;
  src = &audio;
  uint16_t val = 0;
  while(1){
    val = adc1_get_raw(ADC1_CHANNEL_0); //read in from ADC
    audio = (val >> 4);    //right shift by 4, only use MS 8 bits
    i2s_write(I2S_NUM_0, src, len, &len, 100);
  }
}
