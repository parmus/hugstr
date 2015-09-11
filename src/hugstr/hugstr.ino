#include <Adafruit_NeoPixel.h>

//==========================================
//             Configuration

// Cooldown and charging (in seconds)
const float startPower = 0.5;
const float cooldownTime = 10*60;
const float chargeTime = 3;

// Warning blink
const float warnLevel = 0.125;
const unsigned long warnBlinkSpeed = 400;

// Low blink
const unsigned long lowBlinkSpeed = 3000;

// NeoPixel ring ing
const unsigned int ringPin = 0;
const unsigned int ringSize = 16;
const unsigned int ringStartOffset = 2;

// Touch sensor
const unsigned int TouchSensorPin = 1;
const unsigned int TouchSensitivity = 100;  // Higher number -> more sensitive

//==========================================


Adafruit_NeoPixel ring = Adafruit_NeoPixel(ringSize, ringPin, NEO_GRB + NEO_KHZ800);

const uint32_t RED = Adafruit_NeoPixel::Color(255, 0, 0);
const uint32_t GREEN = Adafruit_NeoPixel::Color(0, 255, 0);
const uint32_t BLUE = Adafruit_NeoPixel::Color(0, 0, 255);
const float maxPower = 1000;


void setup() {
  pinMode(TouchSensorPin, INPUT_PULLUP);

  ring.begin();
  ring.show();
}

void loop() {
  static float power = startPower * maxPower;
  static unsigned long lastTick = 0;
  
  unsigned long tick = millis();
  unsigned long timePassed = tick - lastTick;
  
  if (timePassed > 10){
    if (analogRead(TouchSensorPin) < TouchSensitivity){
      power += timePassed / chargeTime;
    } else {
      power -= timePassed / cooldownTime;
    }
    power = constrain(power, 0, maxPower);
    
    if (power == maxPower){
      fullPowerAnimation(GREEN);
    } else if (power == 0){
      // Empty power
      ring.clear();
      ring.setPixelColor(LEDIndex(0), pulse(RED, tick, lowBlinkSpeed));
      ring.show();
    } else if (power < (warnLevel * maxPower)){
      setBar(power, maxPower, blink(BLUE, tick, warnBlinkSpeed));
    } else {
      setBar(power, maxPower, BLUE);
    }
    lastTick = tick;
  }
}


void setBar(unsigned long value, unsigned long maxValue, uint32_t color) {
  word steps = ring.numPixels() * 255;
  
  word stepper = (value * steps) / maxValue;
  for(byte i=0; i < ring.numPixels(); i++){
    ring.setPixelColor(LEDIndex(i), scaleColor(color, uint8_t(min(stepper, 255))));
    stepper = ( stepper>255 ? stepper - 255 : 0);
  }
  ring.show();
}


void fullPowerAnimation(uint32_t color) {
  for (byte repeat=0; repeat<50; repeat++) {
    for (byte offset=0; offset < 3; offset++) {
      ring.clear();
      for (byte i=0; i < ring.numPixels(); i=i+3) {
        ring.setPixelColor(i+offset, color);
      }
      ring.show();
     
      delay(50);
    }
  }
}


/* Calculate a color, which pulses over time.
 * period is the speed of the blink in milliseconds.
 * tick is the time now in milliseconds, usually taken from millis()
 */
unsigned long pulse(uint32_t color, unsigned long tick, unsigned long period){
    unsigned long value = tick % period;
    value = (value < (period / 2) ? value : period - value);
    return scaleColor(color, map(value, 0, (period / 2), 0, 255));
}


/* Calculate a color, which is blinking over time.
 * period is the speed of the blink in milliseconds.
 * tick is the time now in milliseconds, usually taken from millis()
 */
unsigned long blink(uint32_t color, unsigned long tick, unsigned long period){
    return ((tick % period) < (period / 2) ? color : 0);
}


/* Scale (dim) a color in brightness.
 * The brightness parameter must be between 0 (completely off) and 255 (full brightness)
 */
uint32_t scaleColor(uint32_t color, uint8_t brightness){
  uint8_t red = (uint8_t)(color >> 16);
  uint8_t green = (uint8_t)(color >> 8);
  uint8_t blue = (uint8_t)(color);
  return Adafruit_NeoPixel::Color((red * brightness) >> 8, (green * brightness) >> 8, (blue * brightness) >> 8);
}


/* Calculate new LED index by adding an offset. This allows you to decide which LED, should
 * be the first.
 */
byte LEDIndex(byte index){
  return (index + ringStartOffset) % ring.numPixels();
}

