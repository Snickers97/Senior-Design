#include<SPI.h>

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


//Interrupt Service Routines
void isrPinA(){
  //read in current value on pinA
  valA = digitalRead(pinA); //there may be a faster way to read this in
  
  //read in value on pinB
  valB = digitalRead(pinB);
  
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
  valB = digitalRead(pinB);
  
  //read in value on pinA
  valA = digitalRead(pinA);

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


void setup(){
  //initialize rotary encoder pins
  pinMode(pinA, INPUT);
  pinMode(pinB, INPUT);
    
  //attach interrupts for rotary encoder
  attachInterrupt(pinA, isrPinA, CHANGE);  //interrupt on change for pin A
  attachInterrupt(pinB, isrPinB, CHANGE);  //interrupt on change for pin B
  
  //SPI setup section
  pinMode(potPin, OUTPUT);
  SPI.begin();
}

void loop(){
  //main functionality
}

int potWrite(int val){
  digitalWrite(SS, LOW);
  
  //select pin pot is on, the transfer value
  //note 9-bit serial word used, must be loaded MSB first
  SPI.transfer(potPin);
  SPI.transfer(dVal);
  
  digitalWrite(SS, HIGH);
}
