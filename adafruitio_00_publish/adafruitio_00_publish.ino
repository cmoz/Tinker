// Adafruit IO Publish Example
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

/************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"

/************************ Example Starts Here *******************************/
int threshold = 40;
bool touch1detected = false;
bool touch2detected = false;
bool touch3detected = false;

// set up the 'counter' feed
AdafruitIO_Feed *mydata = io.feed("scanItem");

void setup() {

  // start the serial connection
  Serial.begin(115200);

  // wait for serial monitor to open
  while(! Serial);
  Serial.println("ESP32 Touch Interrupt Test");
  touchAttachInterrupt(T5, gotTouch1, threshold);
  touchAttachInterrupt(T3, gotTouch2, threshold);
  touchAttachInterrupt(T0, gotTouch3, threshold);

  Serial.print("Connecting to Adafruit IO");

  // connect to io.adafruit.com
  io.connect();

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());

}

void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();

   if(touch1detected){
    touch1detected = false;
    Serial.println("Touch 1 detected: GPIO 12");
    Serial.print("sending -> ");
    Serial.println(mydata);
    mydata->save("credit card");
  }
  if(touch2detected){
    touch2detected = false;
    Serial.println("Touch 2 detected : GPIO 15");
    Serial.print("sending -> ");
    Serial.println(mydata);
    mydata->save("house keys");
  }
  if(touch3detected){
    touch3detected = false;
    Serial.println("Touch 3 detected: GPIO 4");
    Serial.print("sending -> ");
    Serial.println(mydata);
    mydata->save("mobile phone");
  }

  // Adafruit IO is rate limited for publishing, so a delay is required in
  // between feed->save events. In this example, we will wait three seconds
  // (1000 milliseconds == 1 second) during each loop.
  delay(3000);

}
