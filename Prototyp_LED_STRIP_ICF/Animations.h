#include <Arduino.h>
#include "FastLED.h"

#define fridgeMiddle 100
#define NUM_LEDS    160

int fridgeHead = fridgeMiddle;
int value;

struct data {
  data(int pp, int fc, bool c) {
    color = c;
    pixelpos = pp;
    fillcount = fc;

    
  stripH1 = 80;
  stripT1 = 40;
  stripR1 = true;
  stripH2 = 160;
  stripT2 = 120;
  stripR2 = false;
  }

  int pixelpos;     //pos of a pixel
  int fillcount;    //number of filled pixels
  bool color;

  int stripH1;
  int stripT1;
  bool stripR1;
  int stripH2;
  int stripT2;
  bool stripR2;
};


/////////////////////////////// LED ANIMATIONS /////////////////////////////////

// Solid 
void animation0(CRGBArray<NUM_LEDS> leds, uint8_t gHue, uint8_t gPos, data d) {
  fill_gradient_RGB(leds, 0, CRGB(50,255,0), NUM_LEDS ,CRGB(255,0,0));
}

// two colors are moving from the left to the right
void animation1(CRGBArray<NUM_LEDS> leds, uint8_t gHue, uint8_t gPos, data d) {
  for (int i = 0; i <= NUM_LEDS; i++) {
    leds[i] = CRGB(100, 255, 0); //BRG
  }
  for (int j = 0; j < 80; j++) {
    int pos = (gPos + j) % (NUM_LEDS);
    leds[pos] = CRGB(255, 0 , 0);
  }

}


// fill with pixels
void animation2(CRGBArray<NUM_LEDS> leds, uint8_t gHue, uint8_t gPos, data& d) {
  if (d.color) {
    for (int i = 0; i <= d.fillcount; i++) {
      leds[i] = CRGB(255, 0, 0);
    }
    for (int i = d.fillcount; i <= NUM_LEDS; i ++) {
      leds[i] = CRGB(100, 255, 0);
    }

    for(int i=0; i<= 4; i++){
      leds[d.pixelpos+i] = CRGB(100, 255, 0);     
    }
  } else {
    for (int i = 0; i <= d.fillcount; i++) {
      leds[i] = CRGB(100, 255, 0);
    }
    for (int i = d.fillcount; i <= NUM_LEDS; i ++) {
      leds[i] = CRGB(255, 0, 0);
    }
    for(int i=0; i<= 4; i++){
      leds[d.pixelpos+i] = CRGB(200, 0, 0);     
    }

  }

  d.pixelpos += 2;

  if (d.pixelpos >= d.fillcount) {
    d.pixelpos = 0;
    d.fillcount--;
  }

  if (d.fillcount <= 0) {
    d.fillcount = NUM_LEDS;
    d.color = !d.color;
  }

}

// rainbow
void animation3(CRGBArray<NUM_LEDS> leds, uint8_t gHue, uint8_t gPos, data d) {
  fill_rainbow( leds, NUM_LEDS, gHue, 3);
}

//two colors are moving and changing colors
void animation4(CRGBArray<NUM_LEDS> leds, uint8_t gHue, uint8_t gPos, data d) {
  for (int i = 0; i <= NUM_LEDS; i++) {
    leds[i] = CHSV(gHue, 255, 255); //BRG
  }
  for (int j = 0; j < 50; j++) {
    int pos = (gPos + j) % (NUM_LEDS);
    leds[pos] = CHSV(90, 255, 255);
  }
}


void animation5(CRGBArray<NUM_LEDS> leds, uint8_t gHue, uint8_t gPos, data& d) {
  for(int i = 0 ; i<= NUM_LEDS; i++){
    leds[i] = CRGB(0,0,0);
  }

  for(int i = d.stripT1; i <= d.stripH1; i++){
    leds[i] = CRGB(255, 0, 0);
  }

  for(int i = d.stripT2; i<= d.stripH2; i++){
    leds[i] = CRGB(0, 5, 255);
  }

  if(d.stripH1 >= d.stripT2 && d.stripH1 <= d.stripH2){
    for(int i = d.stripT2; i <= d.stripH1; i++){
      leds[i] = CRGB(0,255,255);
    }
  }

  if(d.stripT1 >= d.stripT2 && d.stripT1 <= d.stripH2){
    for(int i = d.stripT1; i <= d.stripH2; i++){
      leds[i] = CRGB(0,255,255);
    }
  }

  if((d.stripH1 < NUM_LEDS) && (d.stripR1 == true)){
    d.stripT1++;
    d.stripH1++;
  }
  
  if((d.stripT1 >= 0) && (d.stripR1 == false)){
    d.stripT1--;
    d.stripH1--;
  }

  if((d.stripH2 < NUM_LEDS) && (d.stripR2 == true)){
    d.stripT2 += 1;
    d.stripH2 += 1;
  }
  if((d.stripT2 >= 1) && (d.stripR2 == false)){
    d.stripT2 -=1;
    d.stripH2 -=1;
  }

  if((d.stripT1 <= 0) || (d.stripH1 >= NUM_LEDS)){
    d.stripR1 = !d.stripR1;
  }

  if((d.stripT2 <= 0) || (d.stripH2 >= NUM_LEDS)){
    d.stripR2 = !d.stripR2;
  }



}

void animation6(CRGBArray<NUM_LEDS> leds, uint8_t gHue, uint8_t gPos, data d) {
 
}


void animation7(CRGBArray<NUM_LEDS> leds, uint8_t gHue, uint8_t gPos, data d) {

}


void animation8(CRGBArray<NUM_LEDS> leds, uint8_t gHue, uint8_t gPos, data d) {
  
}

void animation9(CRGBArray<NUM_LEDS> leds, uint8_t gHue, uint8_t gPos, data d) {

}





//////////////////////////// FRIDGE ANIMATIONS /////////////////////////////////

void animationFridge(CRGBArray<NUM_LEDS> leds, uint8_t gHue, uint8_t gPos, data d) {

  for (int i = fridgeMiddle; i <= fridgeHead; i++) {
    if(i <= NUM_LEDS){
      leds[i] = CRGB(0, 255, 255);
    }
    
    value = i - fridgeMiddle;
    if(value > 0){
      leds[fridgeMiddle - value] = CRGB(0, 255, 255);
    }
  }

  if (fridgeMiddle - value > 0 ) {
    fridgeHead++;
  }

}

void animationFridgeClose(CRGBArray<NUM_LEDS> leds, uint8_t gHue, uint8_t gPos, data d) {

  for(int i = (fridgeMiddle-value); i <= fridgeMiddle; i++){
    leds[i] = CRGB(0, 255, 255);
  }
  if(value >= 0){
    value -= 3;
  }

  if(fridgeHead <= NUM_LEDS){
    for(int i = fridgeMiddle; i < fridgeHead; i++){
      leds[i] = CRGB(0, 255, 255);
    }
  }else{
     for(int i = fridgeMiddle; i <= NUM_LEDS; i++){
      leds[i] = CRGB(0, 255, 255);
    }
  }
  
  if(fridgeHead >= fridgeMiddle){
      fridgeHead -= 3;
  }
  
}
