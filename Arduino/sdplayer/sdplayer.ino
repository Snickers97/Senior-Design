#include <TimerOne.h>
#include <SPI.h>
#include <SD.h>


const int BUFF_SIZE = 512;
uint8_t samples[BUFF_SIZE];
static volatile int buf_read = 0x00, buf_write = 0x00;

// Frequency modes for TIMER4
#define PWM187k 1   // 187500 Hz
#define PWM94k  2   //  93750 Hz
#define PWM47k  3   //  46875 Hz
#define PWM23k  4   //  23437 Hz
#define PWM12k  5   //  11719 Hz
#define PWM6k   6   //   5859 Hz
#define PWM3k   7   //   2930 Hz

// Direct PWM change variables
#define PWM6        OCR4D
#define PWM13       OCR4A

// Terminal count
#define PWM6_13_MAX OCR4C

// Configure the PWM clock
// The argument is one of the 7 previously defined modes
void pwm613configure(int mode)
{
// TCCR4A configuration
TCCR4A=0;

// TCCR4B configuration
TCCR4B=mode;

// TCCR4C configuration
TCCR4C=0;

// TCCR4D configuration
TCCR4D=0;

// TCCR4D configuration
TCCR4D=0;

// PLL Configuration
// Use 96MHz / 2 = 48MHz
PLLFRQ=(PLLFRQ&0xCF)|0x30;
// PLLFRQ=(PLLFRQ&0xCF)|0x10; // Will double all frequencies

// Terminal count for Timer 4 PWM
OCR4C=255;
}

// Set PWM to D6 (Timer4 D)
// Argument is PWM between 0 and 255
void pwmSet6(int value)
{
  OCR4D=value;   // Set PWM value
  DDRD|=1<<7;    // Set Output Mode D7
  TCCR4C|=0x09;  // Activate channel D
}

//Main timer interrupt, this feeds samples from our buffer to the output
void outputAudio() { 
   
   //Buffer is empty.
   if (buf_read == buf_write) 
      return;

   //output the sample to our PWM hardware
    OCR4D = samples[buf_read]; 

  //increment buf_read with modulo arithmetic
   buf_read = (buf_read +1) % BUFF_SIZE;
}



File myFile;
void setup() {
    // Open serial communications and wait for port to open:
  Serial.begin(9600);
 // while (!Serial) {
 //   ; // wait for serial port to connect. Needed for native USB port only
 // }


  Serial.println("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
  }

  Serial.println("Opening file");
  myFile = SD.open("sound2.raw", FILE_READ);
  if (!myFile.available())
   Serial.println("File is empty");


  Serial.println("initialization done.");
  
  // put your setup code here, to run once:
  pwm613configure(PWM187k);
  pwmSet6(0); 

  
  //Enable main sampling interrupt
  Timer1.initialize(56); // 56 microseconds is close to 18khz
  Timer1.attachInterrupt(outputAudio);

  Serial.println("Playing!");
}

bool sentStop = false;
void loop() {
  // put your main code here, to run repeatedly:
  //Any data to read?
  if (!myFile.available())
    return;

    int remaining = (buf_write > buf_read)
                ? buf_write-buf_read
                : BUFF_SIZE - buf_write + buf_read;
  
  if(remaining < 128)
  {
    return; //no room
  }
  // read more raw audio samples from the file
  uint8_t buff[128];
  myFile.read(buff,128);

  for(int i=0;i<128;i++)
  {
    samples[buf_write] = buff[i];
  //increment buf_read with modulo arithmetic
   buf_write = (buf_write +1) % BUFF_SIZE;
  }
}

