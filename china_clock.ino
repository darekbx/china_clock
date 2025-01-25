#include <Arduino.h>
#include <AceSegment.h>

using ace_segment::LedModule;
using ace_segment::DirectModule;
using ace_segment::kActiveLowPattern;

#define LED_DISPLAY_TYPE_DIRECT 4
#define LED_DISPLAY_TYPE LED_DISPLAY_TYPE_DIRECT

const uint8_t NUM_DIGITS = 10;
const uint8_t NUM_SEGMENTS = 8;
const uint8_t DIGIT_PINS[NUM_DIGITS] = {3, 5, 6, 9};
const uint8_t SEGMENT_PINS[NUM_SEGMENTS] = {A0, A1, A2, A3, A4, A5, 12, 13};

// Total fields/second
//    = FRAMES_PER_SECOND * NUM_SUBFIELDS * NUM_DIGITS
//    = 60 * 16 * 4
//    = 3840 fields/sec
//    => 260 micros/field
const uint8_t FRAMES_PER_SECOND = 60;
const uint8_t NUM_SUBFIELDS = 16;

DirectModule<NUM_DIGITS, NUM_SUBFIELDS> ledModule(
    kActiveLowPattern,
    kActiveLowPattern,
    FRAMES_PER_SECOND,
    SEGMENT_PINS,
    DIGIT_PINS);

const uint8_t PATTERNS[NUM_DIGITS] = {
  0b00111111, // 0
  0b00000110, // 1
  0b01011011, // 2
  0b01001111, // 3
  0b01100110, // 4
  0b01101101, // 5
  0b01111101, // 6
  0b00000111, // 7
  0b01111111, // 8
  0b01101111  // 9
};
const uint8_t UPSIDE_DOWN_PATTERNS[NUM_DIGITS] = {
  0b00111111, // 0
  0b00110000, // 1
  0b01011011, // 2
  0b01111001, // 3
  0b01110100, // 4
  0b01101101, // 5
  0b01101111, // 6
  0b00111000, // 7
  0b01111111, // 8
  0b01111101  // 9
};


uint8_t brightness = 4; // 1, 2, 4, 8, 16
uint8_t i = 0;
bool dot = false;

void updateDisplay() {
  static uint16_t prevUpdateMillis;

  uint16_t nowMillis = millis();
  if ((uint16_t) (nowMillis - prevUpdateMillis) >= 1000) {
    prevUpdateMillis = nowMillis;
    
    uint8_t d0 = PATTERNS[i];
    ledModule.setPatternAt(0, d0);

    uint8_t d1 = PATTERNS[i];
    if (dot) {
      d1 |= 0b10000000;
    }  
    ledModule.setPatternAt(1, d1);

    uint8_t d2 = UPSIDE_DOWN_PATTERNS[i];
    if (dot) {
      d2 |= 0b10000000;
    }
    ledModule.setPatternAt(2, d2);

    ledModule.setPatternAt(3, PATTERNS[i]);

    i++;
    if (i >= 10) {
      i = 0;
    }

    dot = !dot;
    ledModule.setBrightness(brightness);
  }
}

void flushModule() {
  ledModule.renderFieldWhenReady();
}

void setup() {
  ledModule.begin();
}

void loop() {
  updateDisplay();
  flushModule();
}