#include <Adafruit_NeoPixel.h>

//==========================================
//             Configuration

//#define DEBUG

// Cooldown and charging (in seconds)
const float start_charge = 0.5;
const float cooldown = 10*60;
const float charge = 3;

// Low blink
const unsigned long low_blink_speed = 3000;

// Warning blink
const float warn_level = 0.125;
const unsigned long warn_blink_speed = 400;

// LED pin
const unsigned int led_pin = 6;
const unsigned int num_leds = 16;

// Sensor pin
const int analogInPin = A9;
const unsigned int edge = 270;

//==========================================


Adafruit_NeoPixel strip = Adafruit_NeoPixel(num_leds, led_pin, NEO_GRB + NEO_KHZ800);

const float max_power = 1000;
const unsigned long steps = num_leds * 255;

unsigned long last_tick, tick;
float power = start_charge * max_power;

void setup() {
#ifdef DEBUG
  Serial.begin(9600);
#endif

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  tick = millis();
  
  if (tick - last_tick > 10){
    float diff = float(tick - last_tick);
    if (read_sensor()){
      power += diff / charge;
    } else {
      power -= diff / cooldown;
    }
    power = constrain(power, 0, max_power);
    
    if (power == max_power){
      full_power_animation();
    } else if (power == 0){
      empty_power();
    } else if (power < (warn_level * max_power) && (tick / warn_blink_speed) % 2){
      set_off();
    } else {
      set_bar();
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

const long low_blink_speed_half = low_blink_speed / 2;
void empty_power(){
  long frame = tick % low_blink_speed;
  long value = frame % low_blink_speed_half;
  if (frame / low_blink_speed_half){
    value = low_blink_speed_half - value;
  }
  strip.setPixelColor(0, strip.Color(map(value, 0, low_blink_speed_half, 0, 90), 0, 0));
  for(unsigned int i=1; i < strip.numPixels(); i++){
    strip.setPixelColor(i, 0);
  }
  strip.show();
}

void set_off(){
  for(unsigned int i=0; i < strip.numPixels(); i++){
    strip.setPixelColor(i, 0);
  }
  strip.show();
}
  
void set_bar() {
  unsigned int c;
  unsigned long value;
  
  value = int(power) * steps / max_power;
  for(unsigned int i=0; i < strip.numPixels(); i++){
    c = min(value, 255);
    strip.setPixelColor(i, strip.Color(0, 0, c));
    value = ( value>255 ? value - 255 : 0);
  }
  strip.show();
}

void full_power_animation() {
  uint32_t c = strip.Color(0, 255, 0);
  for (int j=0; j<50; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();
     
      delay(50);
     
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

