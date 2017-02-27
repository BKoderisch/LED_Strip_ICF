#include <Arduino.h>
#include "FastLED.h"

#define fridgeMiddle 100
#define NUM_LEDS    160

int fridgeHead = fridgeMiddle;
int fridgeTail = fridgeMiddle;


/////////////////////////////// LED ANIMATIONS /////////////////////////////////

// rainbow
void animation0(CRGBArray<NUM_LEDS> leds, uint8_t gHue, uint8_t gPos){
  fill_rainbow( leds, NUM_LEDS, gHue, 3);
}

// two colors are moving from the left to the right
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



//////////////////////////// FRIDGE ANIMATIONS /////////////////////////////////

void animationFridge(CRGBArray<NUM_LEDS> leds, uint8_t gHue, uint8_t gPos){
  for( i = fridgeMiddle; i <= ; i++){
    leds[i] = CHSV(0, 255, 255);
    int value = fridgeHead - fridgeMiddle;
    leds[fridgeMiddle - value] = CHSV(0,255,255);
  }

  if(fridgeHead < (0,75*NUM_LEDS)){
    fridgeHead++;
  }

}

void animationFridgeClose(CRGBArray<NUM_LEDS> leds, uint8_t gHue, uint8_t gPos){
  for(int i = fridgeTail;  i <= fridgeHead; i++){
    leds[i] = CHSV(0, 255, 255);
    int value = fridge
  }
}
