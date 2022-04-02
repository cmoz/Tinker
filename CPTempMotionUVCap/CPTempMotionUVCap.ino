/* 
 *  This code is for a circuit created for the Circuit Playground boards
 *  It usese the SLIDE SWITCH to choose which program to run
 *  
 *  Program 1: Wearable UV sensor on a cap to detect UV rating outside
 *             Circuit, UV sensor (SDA/SCL pins)and vibration motor (pin XXX)
 *             If UV rating is higher than an indoor rating:
 *             Hat will vibrate, NeoPixels will change color, sound will happen
 *        
 *        
 *  Program 2: Temperature sensor with axial fan that will spin for indoor use
 *             to cool the person wearing the hat
 *             Circuit uses axial fan (A1) servo (12)
 *             If temperature reaches a certain level:
 *             The servo will move into position as it has the fan on it
 *             The fan will go on to cool the person
 *             NeoPixel color goes on
 *             
 */


#include <Adafruit_CircuitPlayground.h>
#include "Adafruit_SI1145.h"
#include "Servo.h"

Servo myServo;

Adafruit_SI1145 uv = Adafruit_SI1145();

bool slideSwitch;                    // Slide switch is + or - in it's position

float UVthresholdIndoors = 2;        // UV level set for indoors
int vibePin = A1;
int servoPin = 12;
int servoMovement = 0;              // distance to move the servo
int axialFanPin = 10;
int pos = 0;
const float alertTemp = 26.4;

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

void loop() {
  slideSwitch = CircuitPlayground.slideSwitch();
  
  float tempC;
  float UVindex = uv.readUV(); 

// +++++++++++++++++++++++ POSITION 1 for SLIDE SWITCH +++++++++++++++++++++
if(slideSwitch) { 
          
  Serial.println("hello UV sensor");
  digitalWrite(vibePin, LOW);

  if(uv.readUV()>= UVthresholdIndoors && uv.readUV()<689 ) //
  {
  Serial.println("LOW");
  neoColor(0,255,100);
  digitalWrite(vibePin, HIGH);
  } else 

  if(uv.readUV()>=690 && uv.readUV()<1379) // //
  {
  Serial.println("Moderate");
  CircuitPlayground.playTone(345, 200);
  neoColor(255,191,0);
  digitalWrite(vibePin, HIGH);
  } else 

   if(uv.readUV()>=1380 && uv.readUV()<1839) //  
  {
  Serial.println("High");
  CircuitPlayground.playTone(445, 200);
  neoColor(255,0,255);
  digitalWrite(vibePin, HIGH);
  } else

  if(uv.readUV()>=1839 && uv.readUV()<2529) //
  {
  Serial.println("Very High");
  CircuitPlayground.playTone(655, 400);
  neoColor(244,30,30);
  digitalWrite(vibePin, HIGH);
  }

  if(uv.readUV()>=2530 && uv.readUV()<2760)
  {
  Serial.println("Extreme high");
  CircuitPlayground.playTone(755, 600);
  neoColor(255,0,0);
  }

delay(1000);
}

// +++++++++++++++++++++++ POSITION 2 for SLIDE SWITCH ++++++++++++++++++++++++
else {    
    
 // tempC = CircuitPlayground.temperature();

  tempC = 32;
  
  Serial.print("tempC: ");
  Serial.println(tempC);

  int red, green, blue;

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

  if( tempC < alertTemp ) {
    myServo.write(160);              // tell servo to go to position in variable 'pos'
    delay(2000);
    }
  
  }
} 


// +++++++++++ FUNCTIONS ++++++++++++++++++++++++++++

void neoColor(int r, int g,int b){
  for (int neoPin = 0; neoPin <=10; neoPin++){
  CircuitPlayground.setPixelColor(neoPin,r,g,b);
  delay(20);
  }
}
