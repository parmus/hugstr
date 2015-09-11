#include <Adafruit_NeoPixel.h>

//==========================================
//             Configuration

// LEDs
const unsigned int LEDPin = 0;
const unsigned int numLEDs = 16;

// Touch sensor
const int TouchSensorPin = 1;
const unsigned int TouchSensitivity = 100;  // Higher number -> more sensitive

//==========================================


Adafruit_NeoPixel LEDs = Adafruit_NeoPixel(numLEDs, LEDPin, NEO_GRB + NEO_KHZ800);

const uint32_t RED = Adafruit_NeoPixel::Color(255, 0, 0);
const uint32_t GREEN = Adafruit_NeoPixel::Color(0, 255, 0);
const uint32_t BLUE = Adafruit_NeoPixel::Color(0, 0, 255);

const unsigned long maxValue = TouchSensitivity * 2;

unsigned long lastTick;

void setup() {
  pinMode(TouchSensorPin, INPUT_PULLUP);

  LEDs.begin();
  LEDs.show(); // Initialize all pixels to 'off'
}

void loop() {
  unsigned long tick = millis();
  
  if (tick - lastTick > 10){
    unsigned long value = analogRead(TouchSensorPin);
    if (value > TouchSensitivity){
      setBar(value, maxValue, RED);
    } else {
      setBar(value, maxValue, GREEN);
    }
    lastTick = tick;
  }
}

unsigned int LEDIndex(unsigned int index){
  return (index + 2) % 16;
}

uint32_t scaleColor(uint32_t color, uint8_t brightness){
  return scaleColor((uint8_t)(color >> 16), (uint8_t)(color >>  8),(uint8_t) color, brightness);
}

uint32_t scaleColor(uint8_t r, uint8_t g, uint8_t b, uint8_t brightness){
  return Adafruit_NeoPixel::Color((r * brightness) >> 8, (g * brightness) >> 8, (b * brightness) >> 8);
}

void setBar(unsigned long value, unsigned long maxValue, uint32_t color) {
  unsigned long steps = LEDs.numPixels() * 255;
  
  long stepper = (maxValue-min(maxValue, value)) * steps / maxValue;
  for(unsigned int i=0; i < LEDs.numPixels(); i++){
    LEDs.setPixelColor(LEDIndex(i), scaleColor(color, uint8_t(min(stepper, 255))));
    
    stepper = ( stepper>255 ? stepper - 255 : 0);
  }
  LEDs.show();
}
