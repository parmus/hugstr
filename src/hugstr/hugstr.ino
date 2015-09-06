#include <Adafruit_NeoPixel.h>

//==========================================
//             Configuration

// Cooldown and charging (in seconds)
const float startPower = 0.5;
const float cooldownTime = 10*60;
const float chargeTime = 3;

// Low blink
const unsigned long lowBlinkSpeed = 3000;

// Warning blink
const float warnLevel = 0.125;
const unsigned long warnBlinkSpeed = 400;

// LEDs
const unsigned int LEDPin = 0;
const unsigned int numLEDs = 16;

// Touch sensor
const int TouchSensorPin = 1;
const unsigned int TouchSensitivity = 75;  // Higher number -> more sensitive

//==========================================


Adafruit_NeoPixel LEDs = Adafruit_NeoPixel(numLEDs, LEDPin, NEO_GRB + NEO_KHZ800);

const float maxPower = 1000;
const unsigned long steps = numLEDs * 255;

unsigned long lastTick, tick;
float power = startPower * maxPower;

void setup() {
  pinMode(TouchSensorPin, INPUT_PULLUP);

  LEDs.begin();
  LEDs.show(); // Initialize all pixels to 'off'
}

void loop() {
  tick = millis();
  
  if (tick - lastTick > 10){
    float diff = float(tick - lastTick);
    if (isHugging()){
      power += diff / chargeTime;
    } else {
      power -= diff / cooldownTime;
    }
    power = constrain(power, 0, maxPower);
    
    if (power == maxPower){
      fullPowerAnimation();
    } else if (power == 0){
      emptyPower();
    } else if (power < (warnLevel * maxPower) && (tick / warnBlinkSpeed) % 2){
      setOff();
    } else {
      setBar();
    }
    lastTick = tick;
  }
}

boolean isHugging(){
  int sensorValue = analogRead(TouchSensorPin);
  return (sensorValue < TouchSensitivity);
}

unsigned int LEDIndex(unsigned int index){
  return (index + 2) % 16;
}

const long low_blink_speed_half = lowBlinkSpeed / 2;
void emptyPower(){
  long frame = tick % lowBlinkSpeed;
  long value = frame % low_blink_speed_half;
  if (frame / low_blink_speed_half){
    value = low_blink_speed_half - value;
  }
  LEDs.setPixelColor(LEDIndex(0), LEDs.Color(map(value, 0, low_blink_speed_half, 0, 90), 0, 0));
  for(unsigned int i=1; i < LEDs.numPixels(); i++){
    LEDs.setPixelColor(LEDIndex(i), 0);
  }
  LEDs.show();
}

void setOff(){
  for(unsigned int i=0; i < LEDs.numPixels(); i++){
    LEDs.setPixelColor(LEDIndex(i), 0);
  }
  LEDs.show();
}
  
void setBar() {
  unsigned int c;
  unsigned long value;
  
  value = int(power) * steps / max_power;
  for(unsigned int i=0; i < LEDs.numPixels(); i++){
    c = min(value, 255);
    LEDs.setPixelColor(LEDIndex(i), LEDs.Color(0, 0, c));
    value = ( value>255 ? value - 255 : 0);
  }
  LEDs.show();
}

void fullPowerAnimation() {
  uint32_t c = LEDs.Color(0, 255, 0);
  for (int j=0; j<50; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < LEDs.numPixels(); i=i+3) {
        LEDs.setPixelColor(i+q, c);    //turn every third pixel on
      }
      LEDs.show();
     
      delay(50);
     
      for (int i=0; i < LEDs.numPixels(); i=i+3) {
        LEDs.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

