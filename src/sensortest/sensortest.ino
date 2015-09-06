#include <Adafruit_NeoPixel.h>

//==========================================
//             Configuration

// LEDs
const unsigned int LEDPin = 0;
const unsigned int numLEDs = 16;

// Touch sensor
const int TouchSensorPin = 1;
const unsigned int TouchSensitivity = 75;  // Higher number -> more sensitive

//==========================================


Adafruit_NeoPixel LEDs = Adafruit_NeoPixel(numLEDs, LEDPin, NEO_GRB + NEO_KHZ800);

unsigned long lastTick;

void setup() {
  pinMode(TouchSensorPin, INPUT_PULLUP);

  LEDs.begin();
  LEDs.show(); // Initialize all pixels to 'off'
}

void loop() {
  unsigned long tick = millis();
  
  if (tick - lastTick > 10){
    setBar(analogRead(TouchSensorPin));
    lastTick = tick;
  }
}

unsigned int LEDIndex(unsigned int index){
  return (index + 2) % 16;
}

void setBar(unsigned long value) {  // Should be 0-1000
  unsigned long steps = LEDs.numPixels() * 255;
  unsigned int c;
  
  long stepper = value * steps / (TouchSensitivity*2);
  for(unsigned int i=0; i < LEDs.numPixels(); i++){
    c = min(stepper, 255);
    if (value > TouchSensitivity){
      LEDs.setPixelColor(LEDIndex(i), LEDs.Color(c, 0, 0));
    } else {
      LEDs.setPixelColor(LEDIndex(i), LEDs.Color(0, c, 0));
    }
    
    stepper = ( stepper>255 ? stepper - 255 : 0);
  }
  LEDs.show();
}
