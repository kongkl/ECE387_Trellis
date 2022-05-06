//pin assign
//gray, 5V ---> 5V
//white, GND ---> GND
//black, SCL ---> A5
//brown, SDA ---> A4
//purple, INT ---> A2

#include <Wire.h>
#include "Adafruit_Trellis.h"

#define MOMENTARY 0
#define LATCHING 1
// set the mode here
#define MODE MOMENTARY

Adafruit_Trellis matrix0 = Adafruit_Trellis();

// Just one
Adafruit_TrellisSet trellis =  Adafruit_TrellisSet(&matrix0);

// set to however many you're working with here, up to 8
#define NUMTRELLIS 1

#define numKeys (NUMTRELLIS * 16)

// Connect Trellis Vin to 5V and Ground to ground.
// Connect the INT wire to pin #A2 (can change later!)
#define INTPIN A2
// Connect I2C SDA pin to your Arduino SDA line
// Connect I2C SCL pin to your Arduino SCL line
// All Trellises share the SDA, SCL and INT pin! 
// Even 8 tiles use only 3 wires max

//buzzer
const int buzzer = 9;
//notes are: D4, E4, FS4, GS4, A4, B4, CS5, D5, FS5, GS5, A5, B5, CS6, D6, E6, FS6
int buttonSounds[] = {294, 330, 370, 415, 440, 494, 554, 587, 740, 831, 880, 988, 1109, 1175, 1319, 1480};

//variables
//stores the pattern for simon says
//assign simon's pattern
int simonsPattern[4];
int j;
bool simonSet = false;
int simonTracker;
bool failed;
int currentStage;

void setup() {
  Serial.begin(9600);
  Serial.println("Trellis Demo");

  // INT pin requires a pullup
  pinMode(INTPIN, INPUT);
  digitalWrite(INTPIN, HIGH);
  
  // begin() with the addresses of each panel in order
  // I find it easiest if the addresses are in order
  trellis.begin(0x70);  // only one
  // trellis.begin(0x70, 0x71, 0x72, 0x73);  // or four!

  // light up all the LEDs in order
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

  j = 0;
  simonSet = true;
  simonTracker = 0;
  failed = false;
  currentStage = 0;

  //buzzer init
  pinMode(buzzer, OUTPUT);

  //getting new pattern
    for(int i = 0; i<sizeof(simonsPattern)/sizeof(int); i++) {
    simonsPattern[i] = random(1, 12);
    
    //what actual code should be if the LED soldering was done properly
    //simonsPattern[i] = random(16);
    }
}

void clearLEDS() {
  // then turn them off
  for (uint8_t i=0; i<numKeys; i++) {
    trellis.clrLED(i);
    trellis.writeDisplay();    
    delay(50);
  }
}


void loop() {
  delay(30); // 30ms delay is required, dont remove me!

  //temp
  //simonSet = false;
  //Serial.println(simonSet);
  if (simonSet == true) {
  switch(j+1) {
    case 1:
      trellis.setLED(simonsPattern[j]);
      trellis.writeDisplay();
      tone(buzzer, buttonSounds[simonsPattern[j]]);
      delay(1000);
      noTone(buzzer);
      clearLEDS();
      trellis.writeDisplay();
      //Serial.println(j);
      j++;
      
    break;
    case 2:
      if (currentStage > 0) {
      trellis.setLED(simonsPattern[j]);
      trellis.writeDisplay();
      tone(buzzer, buttonSounds[simonsPattern[j]]);
      delay(1000);
      noTone(buzzer);
      clearLEDS();
      trellis.writeDisplay();
       //Serial.println(j);
      j++;
      }
    break;
    case 3:
      if (currentStage > 1) {
      trellis.setLED(simonsPattern[j]);
      trellis.writeDisplay();
      tone(buzzer, buttonSounds[simonsPattern[j]]);
      delay(1000);
      noTone(buzzer);
      clearLEDS();
      noTone(buzzer);
      trellis.writeDisplay();
      //Serial.println(j);
      j++;
      }
    break;
    case 4:
      if (currentStage > 2) {
      trellis.setLED(simonsPattern[j]);
      trellis.writeDisplay();
      tone(buzzer, buttonSounds[simonsPattern[j]]);
      delay(1000);
      noTone(buzzer);
      clearLEDS();
      trellis.writeDisplay();
      //Serial.println(j);
      j++;
      }
    break;
    default:
      clearLEDS();
      trellis.writeDisplay();
      simonSet = false;
      //Serial.println(j);
    break;
  }
  }

  
  if (MODE == MOMENTARY) {

    
    // If a button was just pressed or released...
    if (trellis.readSwitches()) {
      // go through every button
      for (uint8_t i=0; i<numKeys; i++) {
        // if it was pressed, turn it on
        if (trellis.justPressed(i)) {
          Serial.print("v"); Serial.println(i);
          trellis.setLED(i);
          trellis.writeDisplay();
          tone(buzzer, buttonSounds[i]);
          
        } 
        // if it was released, turn it off
        if (trellis.justReleased(i)) {
          Serial.print("^"); Serial.println(i);
          trellis.clrLED(i);
          trellis.writeDisplay();
          if (i == simonsPattern[simonTracker]) {
            simonTracker++;
            Serial.println("got one!");
            Serial.println(simonTracker);
            Serial.println(currentStage);
          }
          else {
            failed = true;
          }
          noTone(buzzer);
        }

        
      }
    }
  }

  //temp
  //failed == false;
  if (failed == true) {
    //call function to display X, as well as reset/reassign
    Serial.println("failed");
    fail();
    failed = false;
    simonTracker = 0;
    simonSet = true;
    currentStage = 0;
    j = 0;
  }

  //successfully completed simon says
  if (simonTracker + 1 > sizeof(simonsPattern)/sizeof(int)) {
    success();

    // light up all the LEDs in order
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

    failed = false;
    simonTracker = 0;
    simonSet = true;
    currentStage = 0;
    j = 0;

    //getting new pattern
    for(int i = 0; i<sizeof(simonsPattern)/sizeof(int); i++) {
    simonsPattern[i] = random(1, 12);
    
    //what actual code should be if the LED soldering was done properly
    //simonsPattern[i] = random(16);
    }
  }

  //temp
  //simonTracker = 0;
  //if simonTracker has gotten to the size of the array without hittin failed, success!
  if (simonTracker > currentStage) {
    //call function to display O, as well as progress to the next stage
    Serial.println("success");
    success();
    simonTracker = 0;
    simonSet = true;
    j = 0;
    currentStage++;
    delay(1000);
  }

}

void success() {
  trellis.setLED(1);
  trellis.setLED(2);
  trellis.setLED(4);
  trellis.setLED(7);
  trellis.setLED(8);
  trellis.setLED(11);
  trellis.setLED(13);
  trellis.setLED(14);
  trellis.writeDisplay();
  delay(1000);
  clearLEDS();
  trellis.writeDisplay();
  delay(1000);
}

void fail() {
  trellis.setLED(0);
  trellis.setLED(5);
  trellis.setLED(10);
  trellis.setLED(15);
  trellis.setLED(12);
  trellis.setLED(9);
  trellis.setLED(6);
  trellis.setLED(3);
  trellis.writeDisplay();
  delay(1000);
  clearLEDS();
  trellis.writeDisplay();
  delay(1000);
  trellis.setLED(0);
  trellis.setLED(5);
  trellis.setLED(10);
  trellis.setLED(15);
  trellis.setLED(12);
  trellis.setLED(9);
  trellis.setLED(6);
  trellis.setLED(3);
  trellis.writeDisplay();
  delay(1000);
  clearLEDS();
  trellis.writeDisplay();
  delay(1000);
  for(int i = 0; i<sizeof(simonsPattern)/sizeof(int); i++) {
    simonsPattern[i] = random(1, 12);

    //what actual code should be if the LED soldering was done properly
    //simonsPattern[i] = random(16);
  }
}
