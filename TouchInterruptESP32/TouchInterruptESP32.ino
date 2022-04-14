/*
This is un example howto use Touch Intrrerupts
The bigger the threshold, the more sensible is the touch
*/

int threshold = 40;
bool touch1detected = false;
bool touch2detected = false;
bool touch3detected = false;

void gotTouch1(){
 touch1detected = true;
}

void gotTouch2(){
 touch2detected = true;
}

void gotTouch3(){
 touch3detected = true;
}

void setup() {
  Serial.begin(115200);
  delay(1000); // give me time to bring up serial monitor
  Serial.println("ESP32 Touch Interrupt Test");
  touchAttachInterrupt(T5, gotTouch1, threshold);
  touchAttachInterrupt(T3, gotTouch2, threshold);
  touchAttachInterrupt(T0, gotTouch3, threshold);
}

void loop(){
  if(touch1detected){
    touch1detected = false;
    Serial.println("Touch 1 detected: GPIO 12");
  }
  if(touch2detected){
    touch2detected = false;
    Serial.println("Touch 2 detected : GPIO 15");
  }
  if(touch3detected){
    touch3detected = false;
    Serial.println("Touch 3 detected: GPIO 4");
  }
}
