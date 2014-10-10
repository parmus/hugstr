#include <Adafruit_NeoPixel.h>

//==========================================
//             Configuration

//#define DEBUG

// Cooldown and charging (in seconds)
const float start_charge = 0.6;
const float cooldown = 2*60;
const float charge = 5;

// Warning blink
const float warnlevel = 0.125;
const unsigned long blink_speed = 400;

// LED pin
const unsigned int led_pin = 6;
const unsigned int num_leds = 16;

// Sensor pin
const int analogInPin = A9;
const unsigned int edge = 270;

//==========================================


Adafruit_NeoPixel strip = Adafruit_NeoPixel(num_leds, led_pin, NEO_GRB + NEO_KHZ800);

unsigned long last_tick;
const float max_power = 1000;
float power = start_charge * max_power;

void setup() {
#ifdef DEBUG
  Serial.begin(9600);
#endif

  pinMode(9, INPUT_PULLUP);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  unsigned long tick = millis();
  
  if (tick - last_tick > 10){
    float diff = float(tick - last_tick);
    if (read_sensor()){
      power += diff / charge;
    } else {
      power -= diff / cooldown;
    }
    power = constrain(power, 0, max_power);
    
    if (power < (warnlevel * max_power)){
      set_bar((tick / blink_speed) % 2 == 0 ? int(power) : 0);
    } else {
      set_bar(int(power));
    }
    last_tick = tick;
  }
}

boolean read_sensor(){
  int sensorValue = analogRead(analogInPin);
#ifdef DEBUG
  Serial.println(sensorValue);
#endif
  return (sensorValue < edge);
}

void set_bar(unsigned long value) {  // Should be 0-1000
  unsigned long steps = strip.numPixels() * 255;
  unsigned int c;
  
  value = value * steps / max_power;
  for(unsigned int i=0; i < strip.numPixels(); i++){
    c = min(value, 255);
    strip.setPixelColor(i, strip.Color(0, 0, c));
    value = ( value>255 ? value - 255 : 0);
  }
  strip.show();
}
