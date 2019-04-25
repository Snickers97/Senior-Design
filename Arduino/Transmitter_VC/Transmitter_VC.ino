//Code for the In Ear Monitor transmitter

#include <WiFi.h>
#include <SPI.h>

//Sets the SPI settings for reading from the ADC
SPISettings adcSettings(705600, MSBFIRST, SPI_MODE0);

int bufsize = 256;

volatile int dVal=128;  //internal power-on sets wiper to midscale 
int stepSize = 1, potPin, pinA, pinB;
//        Variables above
//  pinB      -   output B from rotary encoder
//  pinA      -   output A from rotary encoder

//  potPin    -   need to connect GND, Vdd, A1, W1, B1, and SPI stuffs
//                  V_a = 5 V
//                  V_b = 0 V
//                  V_w = V_out
//                  will be using wiper reg 1 -> A=0 from Func Blk Diag

//  stepSize  -   how much we want to increase/decrease each time

//  dVal      -   decimal value to be transfered to pot
//                declared as volatile as will change in ISRs
//                  255 ->  full scale
//                  128 ->  midscale
//                    1 ->  1 LSB
//                    0 ->  zero scale


//MISO = 19  (third, yellow wire)
//SCK = 18    (second)
//SS = 5      (first)

//Network credentials
const char* ssid = "In-Ear-Transmitter";
const char* password = "123456789";

//Set server port number to 80
WiFiServer server(80);


//Interrupt Service Routines
void isrPinA(){
  //read in current value on pinA
  int valA = digitalRead(pinA); //there may be a faster way to read this in
  
  //read in value on pinB
  int valB = digitalRead(pinB);
  
  //conditional statements
  if(valA != valB){
    //decrease volume
      //adjust pot
      dVal -= stepSize;
  }
  else{
    //increase volume
      //adjust pot
      dVal += stepSize;
  }
  
  //write val to pot
  potWrite(dVal);
  
} //end of isr on pinA


void isrPinB(){  
  //read in current value on pinB
  int valB = digitalRead(pinB);
  
  //read in value on pinA
  int valA = digitalRead(pinA);

  //conditional statements
  if(valA == valB){
    //decrease volume
      //adjust pot
      dVal -= stepSize;
  }
  else{
    //increase volume
      //adjust pot
      dVal += stepSize;
  }

  //write val to pot
  potWrite(dVal);
  
} //end of isr on pinB


void setup() {
  //Activate red LED
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  Serial.begin(230400);
 
  //WIFI SETUP SECTION
  //Activate WiFi network
  Serial.print("Setting up AP. . .");
  WiFi.softAP(ssid, password);
    //We can remove the password parameter if we want the network to be open
  IPAddress IP = WiFi.softAPIP();
  Serial.print("IP address: ");
  Serial.println(IP);
  server.begin();

  //initialize rotary encoder pins
  pinMode(pinA, INPUT);
  pinMode(pinB, INPUT);
    
  //attach interrupts for rotary encoder
  attachInterrupt(pinA, isrPinA, CHANGE);  //interrupt on change for pin A
  attachInterrupt(pinB, isrPinB, CHANGE);  //interrupt on change for pin B

  //SPI SETUP SECTION
  pinMode(SS, OUTPUT);
  pinMode(MISO, INPUT);
  pinMode(SCK, OUTPUT);
  pinMode(potPin, OUTPUT);
  SPI.begin();

  
  
  
}

void loop() {
  WiFiClient client = server.available();     //Listen for devices

  if(client){
    Serial.println("Connected.");
    client.setTimeout(20);
    uint8_t val;
    uint16_t audio[bufsize/2];
    while(client.connected()){
      for(int i = 0; i < bufsize; i++){
        SPI.beginTransaction(adcSettings);
        digitalWrite(SS,LOW);
        val = SPI.transfer(0);    //garbage data, throw away
        //audio[i] = SPI.transfer(0);   //upper 8 bits
        //audio[i+1] = SPI.transfer(0); //lower 8 bits
        audio[i] = SPI.transfer16(0);
        Serial.println(audio[i]);
        //Serial.print("      ");
        //Serial.println(audio[i+1]);
        digitalWrite(SS,HIGH);
        SPI.endTransaction();
      }
      //client.write(audio,bufsize/2);
    }
    //Close the connection
    client.stop();
    Serial.println("Disconnected.");
  }
}



int potWrite(int val){
  digitalWrite(SS, LOW);
  
  //select pin pot is on, the transfer value
  //note 9-bit serial word used, must be loaded MSB first
  SPI.transfer(potPin);
  SPI.transfer(dVal);
  
  digitalWrite(SS, HIGH);
}
