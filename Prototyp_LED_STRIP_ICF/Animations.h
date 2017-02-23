#include <Arduino.h>
#include "FastLED.h"


#define NUM_LEDS    160

// rainbow
void animation0(CRGBArray<NUM_LEDS> leds, uint8_t gHue, uint8_t gPos){
  fill_rainbow( leds, NUM_LEDS, gHue, 3);
}


void animation1(CRGBArray<NUM_LEDS> leds, uint8_t gHue, uint8_t gPos){
  for(int i = 0; i <= NUM_LEDS; i++){
    leds[i] = CRGB(100, 255, 0); //BRG
  }
  for(int j = 0; j < 50; j++){
    int pos = (gPos + j) % (NUM_LEDS);
    leds[pos] = CRGB(255, 0 ,0);
  }
}


// sinelon
void animation2(CRGBArray<NUM_LEDS> leds, uint8_t gHue, uint8_t gPos){
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16(13,0,NUM_LEDS);
  leds[pos] += CHSV( gHue, 255, 192);
}


void animation3(CRGBArray<NUM_LEDS> leds, uint8_t gHue, uint8_t gPos){
    for(int i = 0; i <= NUM_LEDS; i++){
    leds[i] = CHSV(gHue, 255, 255); //BRG
  }
  for(int j = 0; j < 50; j++){
    int pos = (gPos + j) % (NUM_LEDS);
    leds[pos] = CHSV(gHue + 100, 255, 255);
  }
}


// bpm
void animation4(CRGBArray<NUM_LEDS> leds, uint8_t gHue, uint8_t gPos){
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}


// juggle
void animation5(CRGBArray<NUM_LEDS> leds, uint8_t gHue, uint8_t gPos){
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16(i+7,0,NUM_LEDS)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

void animation6(CRGBArray<NUM_LEDS> leds, uint8_t gHue, uint8_t gPos){
  for(int i = 0; i <= NUM_LEDS; i++){
    leds[i] = CRGB(255, 110, 0); //BRG
  }
  for(int j = 0; j < 50; j++){
    int pos = (gPos + j) % (NUM_LEDS);
    leds[pos] = CRGB(255, 0 ,0);
  }
}

void animation7(CRGBArray<NUM_LEDS> leds, uint8_t gHue, uint8_t gPos){

}

void animation8(CRGBArray<NUM_LEDS> leds, uint8_t gHue, uint8_t gPos){

}

void animation9(CRGBArray<NUM_LEDS> leds, uint8_t gHue, uint8_t gPos){

}

void animationFridge(CRGBArray<NUM_LEDS> leds, uint8_t gHue, uint8_t gPos){
  for(int i = 0; i < NUM_LEDS/2; i++) {
    // fade everything out
    fadeToBlackBy(leds, NUM_LEDS, 30);

    // let's set an led value
    leds[i] = CHSV(gHue++,255,255);

    // now, let's first 20 leds to the top 20 leds,
    leds(NUM_LEDS/2,NUM_LEDS-1) = leds(NUM_LEDS/2 - 1 ,0);
    FastLED.delay(23);
  }
}

