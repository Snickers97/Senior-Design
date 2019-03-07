int freq = 2000;
int channel = 0;
int resolution = 16;
 
void setup() {
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
 
  ledcSetup(channel, freq, resolution);
  ledcAttachPin(12, channel);
 
}
 
void loop() {
 
  //ledcWriteTone(channel, 2000);
 
  //ledcWrite(channel, 125);
 
  for (int freq = 255; freq < 10000; freq = freq + 250){
 
 
     ledcWriteTone(channel, freq);
     delay(1000);
  }
 
}
