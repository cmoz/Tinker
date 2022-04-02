/* MODOFIED VERSION
 *  
 *  Includes additional code: speaking Warning / Danger
 *  
 *  This code is for a circuit created for the Circuit Playground boards
 *  It uses the SLIDE SWITCH to choose which program to run
 *  
 *  NeoPixels will change color for UV values / Temperature values
 *  
 *  Program 1: Wearable UV sensor on a cap to detect UV rating outside
 *             Circuit: UV sensor (SDA/SCL pins) vibration motor (pin 1)
 *             If UV rating is higher than an indoor rating:
 *             Hat will vibrate, sound will happen
 *        
 *  Program 2: Temperature sensor with axial fan that will spin for indoor use
 *             to cool the person wearing the hat
 *             Circuit: axial fan (pin 10) servo (pin 12)
 *             If temperature reaches a certain level:
 *             The servo will move into position as it has the fan on it
 *             The fan will go on to cool the person
 *             
 */


#include <Adafruit_CircuitPlayground.h>
#include <Wire.h> // needed for speach
#include <SPI.h>  // needed for speech
#include "Adafruit_SI1145.h"
#include "Servo.h"

const uint8_t spWARNING[]           PROGMEM = {0x6A,0x15,0x71,0x5C,0xDC,0x23,0x89,0x8D,0xD5,0x36,0xB5,0x8C,0x2D,0x36,0xDA,0xC6,0xCC,0xDD,0x91,0x5A,0xB9,0xBF,0x51,0xC7,0xCC,0x69,0x76,0x6A,0xA6,0x12,0xD2,0xB6,0xCE,0xDB,0x6B,0x0F,0xCC,0xBA,0xAA,0x10,0x2E,0xBC,0x29,0xED,0x2A,0xA2,0x5B,0xF3,0xA5,0x2C,0x2B,0x4F,0xA9,0x25,0x9A,0xBC,0xAE,0xBC,0x74,0xD3,0xF0,0x70,0xD6,0x8A,0xE4,0xC4,0x8A,0x3D,0xEE,0x6A,0x9B,0x57,0x35,0x8F,0xD4,0x6B,0xE8,0x3E,0xD8,0x22,0x16,0xAE,0x71,0xF8,0x20,0x4D,0x5F,0xB4,0xE6,0xE1,0x83,0xB4,0x7C,0xD6,0x58,0x46,0x0A,0x94,0xF1,0x99,0x73,0x09,0x47,0xAF,0x28,0x9D,0x16,0x34,0x1C,0xD5,0x61,0xA5,0xAA,0x13,0x73,0x65,0x43,0x15,0xE2,0x71,0xD2,0x95,0x0D,0x67,0x72,0xD8,0x4E,0x77,0xB1,0x5C,0xCE,0xEE,0xA4,0x3C,0x2D,0xBA,0xA4,0x5A,0xE5,0xF2,0x74,0x57,0x6C,0xA9,0xAB,0xFE,0x1F};
const uint8_t spDANGER[]            PROGMEM = {0x0A,0x28,0x30,0x6C,0xD4,0xC7,0x54,0x19,0xAA,0x9B,0xD5,0x34,0x6B,0xCA,0xEE,0x6B,0x56,0xDD,0xBD,0xA9,0x44,0xAC,0x59,0xD5,0x08,0xC1,0x9C,0xB1,0x7A,0x65,0x33,0x24,0x72,0xE7,0xAC,0x95,0x8C,0x64,0xE0,0x13,0x95,0x57,0xD4,0xBD,0x50,0x8D,0x45,0x19,0x51,0xB3,0x98,0x61,0xAE,0x64,0xA4,0xCD,0x60,0x94,0x85,0xD2,0x90,0x47,0x41,0x35,0xEA,0xEA,0x5A,0x73,0x4A,0x99,0xA0,0x38,0x09,0xC0,0x76,0x42,0xA3,0x9C,0xDA,0x94,0x35,0xF2,0x8C,0xB1,0x66,0x25,0x0D,0xDB,0x3C,0x86,0x9A,0x4B,0xA0,0x35,0xFB,0x98,0x4B,0x72,0xE2,0xB3,0x6C,0x6D,0xCB,0x43,0xC8,0x47,0xF2,0xA5,0x23,0x6F,0x21,0x6F,0xCE,0x16,0x8E,0xBC,0x89,0x2B,0xB5,0x6F,0x38,0xCB,0x32,0xD2,0x96,0x6C,0xEE,0x2C,0xDB,0x48,0x5B,0xF2,0xB9,0xAB,0x2C,0x11,0x2F,0xEE,0x1D,0xEE,0x52,0x07,0x25,0x43,0xFB,0xFF,0x03};

Servo myServo;

Adafruit_SI1145 uv = Adafruit_SI1145();  

bool slideSwitch;                    // Slide switch is + or - in it's position

int vibePin = 1;
int servoPin = 12;
int axialFanPin = 10;

float UVthresholdIndoors = 2;        // UV level set for indoors
int servoMovement = 0;              // distance to move the servo
int pos = 0;
const float alertTemp = 26.4;

// +++++++++++++++++++++++++ SETUP +++++++++++++++++++++++++++++++++++++
void setup() {
  Serial.begin(9600);
  CircuitPlayground.begin();
  myServo.attach(servoPin);
  
  Serial.println("Adafruit SI1145 test");

  if (! uv.begin()) {
  Serial.println("Didn't find Si1145");
  while (1);
  }
  
  Serial.println("OK!");
  pinMode(vibePin, OUTPUT);
  pinMode(axialFanPin, OUTPUT);
}

// +++++++++++++++++++++++++ LOOP +++++++++++++++++++++++++++++++++++++
void loop() {
  slideSwitch = CircuitPlayground.slideSwitch();
  
  int red, green, blue;
  float tempC;
  float UVindex = uv.readUV(); 

// +++++++++++++++++++++++ POSITION 1 for SLIDE SWITCH +++++++++++++++++++++
if(slideSwitch) { 
          
  Serial.println("hello UV sensor");
  digitalWrite(vibePin, LOW);

// ++++ Mapping the value of the UV 
// to reflect color for NeoPixels to display 

  if (UVindex <= 689) {
    red = 0;
    green = map(UVindex,0,689,0,255);
    blue = map(UVindex,0,689,255,0) ;
  }
   else if ((UVindex >=690 ) && (UVindex <=1839 )) {
    red = map(UVindex,690,1839,0,255);
    green = map(UVindex,690,1839,255,0);
    blue = 0;
  } else {
    red = map(UVindex,1840,2760,255,0);
    green = 0;
    blue = map(UVindex,1840,2760,0,255);
  }

  neoColor(red, green, blue); 

  if(uv.readUV()>= UVthresholdIndoors && uv.readUV()<689 )
  {
  Serial.println("LOW");
  digitalWrite(vibePin, LOW);
  } else 

  if(uv.readUV()>=690 && uv.readUV()<1379)
  {
  Serial.println("Moderate");
  CircuitPlayground.playTone(345, 200);
  vibrationWarn(200, 10);
  CircuitPlayground.speaker.say(spWARNING);
  delay(2000);
  } else 

  if(uv.readUV()>=1380 && uv.readUV()<1839) 
  {
  Serial.println("High");
  CircuitPlayground.playTone(445, 200);
  vibrationWarn(300, 15);
  CircuitPlayground.speaker.say(spWARNING);
  delay(2000);
  CircuitPlayground.speaker.say(spWARNING);
  } else
  
  if(uv.readUV()>=1839 && uv.readUV()<2529) 
  {
  Serial.println("Very High");
  CircuitPlayground.playTone(655, 400);
  vibrationWarn(400, 20);
  CircuitPlayground.speaker.say(spDANGER);
  delay(2000);
  CircuitPlayground.speaker.say(spDANGER);
  }

  if(uv.readUV()>=2530 && uv.readUV()<2760)
  {
  Serial.println("Extreme high");
  CircuitPlayground.playTone(755, 600);
  vibrationWarn(450, 25);
  CircuitPlayground.speaker.say(spWARNING);
  CircuitPlayground.speaker.say(spDANGER);
  delay(2000);
  CircuitPlayground.speaker.say(spDANGER);
  CircuitPlayground.speaker.say(spWARNING);
  }

delay(1000);
}

// +++++++++++++++++++++++ POSITION 2 for SLIDE SWITCH ++++++++++++++++++++++++
else {    
  // Get the temperature value from the sensor on the CP
  tempC = CircuitPlayground.temperature();

  //tempC = 12;  // to test the temperature color
  
  Serial.print("tempC: ");
  Serial.println(tempC);

// ++++ Mapping the value of the temperature, to reflect color for
//      the NeoPixels to display 

  if (tempC <= 15) {
    red = 0;
    green = map(tempC,0,15,0,255);
    blue = map(tempC,0,15,255,0) ;
  }
   else if ((tempC >=16 ) && (tempC <=26 )) {
    red = map(tempC,16,26,0,255);
    green = map(tempC,16,26,255,0);
    blue = 0;
  } else {
    red = map(tempC,26,60,255,0);
    green = 0;
    blue = map(tempC,26,60,0,255);;
  }

  neoColor(red, green, blue); 
           
  Serial.println("hello temperature");
  digitalWrite(axialFanPin, LOW);

// ++++ Moving the servo it the temperature is too hot
//      It will move slowly back and forth to cool the person

  if( tempC > alertTemp ) {
    for (pos = 110; pos >= 60; pos -= 1) { 
    myServo.write(pos);              
    delay(30);   
    }
    delay(1000);
    for (pos = 60; pos <= 110; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myServo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(30);      
    }
    digitalWrite(axialFanPin, HIGH);              
    delay(500);
    }

// ++++ If the temperature osn't too hot, it will move the servo in the up
//      position, and just wait. 

  if( tempC < alertTemp ) {
    myServo.write(160);              // tell servo to go to position in variable 'pos'
    delay(2000);
    }
  
  }
} 


// ++++++++++++++++++++ FUNCTIONS ++++++++++++++++++++++++++++

void neoColor(int r, int g,int b){
  for (int neoPin = 0; neoPin <=10; neoPin++){
  CircuitPlayground.setPixelColor(neoPin,r,g,b);
  delay(20);
  }
  Serial.println("===== neoColor() executing");
}

void vibrationWarn(int pulseDuration, int amount){
  for (int loops = 0; loops <= amount; loops++){
      digitalWrite(vibePin, HIGH);
      delay(pulseDuration);
      digitalWrite(vibePin, LOW);
      delay(pulseDuration);
  }
  Serial.println("===== vibrationWarn() executing");
}
