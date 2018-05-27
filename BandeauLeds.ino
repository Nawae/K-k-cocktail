#include "FastLED.h"

#define NUM_LEDS 12

// Data pin that led data will be written out over
#define DATA_PIN 3
// Clock pin only needed for SPI based chipsets when not using hardware SPI
#define CLOCK_PIN 4

CRGB leds[NUM_LEDS];

void setup() {
	// sanity check delay - allows reprogramming if accidently blowing power w/leds
   	delay(2000);
       FastLED.addLeds<WS2801, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
}

void loop() {
    for(int i = -1; i <(NUM_LEDS/2); i++) {
      //Droite
      leds[i] = CRGB(255,0,0);
      leds[i+1] = CRGB(255,0,0);
      FastLED.show();
      //Gauche
      //leds[NUM_LEDS-(i-1)] = CRGB(255,0,0);
      leds[NUM_LEDS-(i+1)] = CRGB(255,0,0);
      leds[NUM_LEDS-(i+1)-1] = CRGB(255,0,0);
      FastLED.show();
      delay(150);
      leds[i-1] = CRGB(0,0,0);
      leds[i] = CRGB(0,0,0);
      FastLED.show();
      leds[NUM_LEDS-(i+1)+1] = CRGB(0,0,0);
      leds[NUM_LEDS-(i+1)] = CRGB(0,0,0);
      FastLED.show();
    }
}
