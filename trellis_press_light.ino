#include <Wire.h>
#include "Adafruit_Trellis.h"
#define MOMENTARY 0
#define LATCHING 1
#define MODE LATCHING 

Adafruit_Trellis matrix0 = Adafruit_Trellis();
Adafruit_TrellisSet trellis =  Adafruit_TrellisSet(&matrix0);

#define NUMTRELLIS 1

#define numKeys (NUMTRELLIS * 16)

#define INTPIN A2


void setup() {
  Serial.begin(9600);

  pinMode(INTPIN, INPUT);
  digitalWrite(INTPIN, HIGH);

  trellis.begin(0x70);  

  //light up all the LEDs in order
  for (uint8_t i=0; i<numKeys; i++) {
    trellis.setLED(i);
    trellis.writeDisplay();    
    delay(50);
  }
  // then turn them off
  for (uint8_t i=0; i<numKeys; i++) {
    trellis.clrLED(i);
    trellis.writeDisplay();    
    delay(50);
  }
}


void loop() {
  delay(30);
  
  if (MODE == MOMENTARY) {
    //button was just pressed or released
    if (trellis.readSwitches()) {
      // go through every button
      for (uint8_t i=0; i<numKeys; i++) {
	//pressed, turn it on
	if (trellis.justPressed(i)) {
	  Serial.print("v"); Serial.println(i);
	  trellis.setLED(i);
	} 
	// if released, turn off
	if (trellis.justReleased(i)) {
	  Serial.print("^"); Serial.println(i);
	  trellis.clrLED(i);
	}
      }
      trellis.writeDisplay();
    }
  }

  if (MODE == LATCHING) {
    //button was just pressed or released
    if (trellis.readSwitches()) {
      // go through every button
      for (uint8_t i=0; i<numKeys; i++) {
        //pressed
	if (trellis.justPressed(i)) {
	  Serial.print("v"); Serial.println(i);
	  //alternate the LED
	  if (trellis.isLED(i))
	    trellis.clrLED(i);
	  else
	    trellis.setLED(i);
        } 
      }
      trellis.writeDisplay();
    }
  }
}
