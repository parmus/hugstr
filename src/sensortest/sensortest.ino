#include <Adafruit_NeoPixel.h>

//==========================================
//             Configuration

//#define DEBUG

// LED pin
const unsigned int led_pin = 0;
const unsigned int num_leds = 16;

// Sensor pin
const int analogInPin = 1;
const unsigned int edge = 75;

//==========================================


Adafruit_NeoPixel strip = Adafruit_NeoPixel(num_leds, led_pin, NEO_GRB + NEO_KHZ800);

unsigned long last_tick;

void setup() {
  pinMode(analogInPin, INPUT_PULLUP);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  unsigned long tick = millis();
  
  if (tick - last_tick > 10){
    set_bar(analogRead(analogInPin));
    last_tick = tick;
  }
}

unsigned int led_index(unsigned int index){
  return (index + 2) % 16;
}

void set_bar(unsigned long value) {  // Should be 0-1000
  unsigned long steps = strip.numPixels() * 255;
  unsigned int c;
  
  long stepper = value * steps / (edge*2);
  for(unsigned int i=0; i < strip.numPixels(); i++){
    c = min(stepper, 255);
    if (value > edge){
      strip.setPixelColor(led_index(i), strip.Color(c, 0, 0));
    } else {
      strip.setPixelColor(led_index(i), strip.Color(0, c, 0));
    }
    
    stepper = ( stepper>255 ? stepper - 255 : 0);
  }
  strip.show();
}
