#include <Adafruit_CircuitPlayground.h>
#include <Servo.h>

Servo myservo;

float tempC, tempF;
int pos = 0; 

void setup() {
  Serial.begin(9600);
  CircuitPlayground.begin();
  myservo.attach(9);
}

void loop() {
  tempC = CircuitPlayground.temperature();
  tempF = CircuitPlayground.temperatureF();

  Serial.print("tempC: ");
  Serial.print(tempC);
  Serial.print("  tempF: ");
  Serial.println(tempF);

  delay(1000);

    if( tempC > 19.5 ) {

    for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
    }
    int colorR = random(255);   // chooses a value 0-255
    int colorG = random(255);
    int colorB = random(255); 
    neoColor(colorR, colorG, colorB);          // color is set from 3 random values  
                            
    delay(1000);
    }
}


void neoColor(int r, int g,int b){
  for (int neoPin = 0; neoPin <=10; neoPin++){
  CircuitPlayground.setPixelColor(neoPin,r,g,b);
  delay(20);
  }
}
