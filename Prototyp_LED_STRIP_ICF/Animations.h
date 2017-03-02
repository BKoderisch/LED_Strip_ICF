#include <Arduino.h>
#include "FastLED.h"

#define fridgeMiddle 100
#define NUM_LEDS    160

#define YP_BLUE CRGB(255, 0, 0)
#define YP_PURPLE CRGB(100, 255, 0)
#define YELLOW CRGB(0,255,255)
#define BLACK CRGB(0,0,0)

float fridgeHead = fridgeMiddle;
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
  fill_gradient_RGB(leds, 0, YP_PURPLE , NUM_LEDS ,YP_BLUE);
}

// two colors are moving from the left to the right
void animation1(CRGBArray<NUM_LEDS> leds, uint8_t gHue, uint8_t gPos, data d) {
  for (int i = 0; i <= NUM_LEDS; i++) {
    leds[i] = YP_PURPLE; //BRG
  }
  for (int j = 0; j < 80; j++) {
    int pos = (gPos + j) % (NUM_LEDS);
    leds[pos] = YP_BLUE;
  }

}


// fill with pixels
void animation2(CRGBArray<NUM_LEDS> leds, uint8_t gHue, uint8_t gPos, data& d) {
  if (d.color) {
    for (int i = 0; i <= d.fillcount; i++) {
      leds[i] = YP_BLUE;
    }
    for (int i = d.fillcount; i <= NUM_LEDS; i ++) {
      leds[i] = YP_PURPLE;
    }

    for(int i=0; i<= 4; i++){
      leds[d.pixelpos+i] = YP_PURPLE;     
    }
  } else {
    for (int i = 0; i <= d.fillcount; i++) {
      leds[i] = YP_PURPLE;
    }
    for (int i = d.fillcount; i <= NUM_LEDS; i ++) {
      leds[i] = YP_BLUE;
    }
    for(int i=0; i<= 4; i++){
      leds[d.pixelpos+i] = YP_BLUE;     
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
    leds[pos] = YP_PURPLE;
  }
}

//two stripes are moving, when colliding color turns yellow
void animation5(CRGBArray<NUM_LEDS> leds, uint8_t gHue, uint8_t gPos, data& d) {
  for(int i = 0 ; i<= NUM_LEDS; i++){
    leds[i] = BLACK;
  }

  for(int i = d.stripT1; i <= d.stripH1; i++){
    leds[i] = YP_BLUE;
  }

  for(int i = d.stripT2; i<= d.stripH2; i++){
    leds[i] = YP_PURPLE;
  }

  if(d.stripH1 >= d.stripT2 && d.stripH1 <= d.stripH2){
    for(int i = d.stripT2; i <= d.stripH1; i++){
      leds[i] = YELLOW;
    }
  }

  if(d.stripT1 >= d.stripT2 && d.stripT1 <= d.stripH2){
    for(int i = d.stripT1; i <= d.stripH2; i++){
      leds[i] = YELLOW;
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
      leds[i] = YELLOW;
    }
    
    value = i - fridgeMiddle;
    if(value > 0){
      leds[fridgeMiddle - value] = YELLOW;
    }
  }

  if (fridgeMiddle - value > 0 ) {
    fridgeHead += 0.5;
  }

}

void animationFridgeClose(CRGBArray<NUM_LEDS> leds, uint8_t gHue, uint8_t gPos, data d) {

  for(int i = (fridgeMiddle-value); i <= fridgeMiddle; i++){
    leds[i] = YELLOW;
  }
  if(value >= 0){
    value -= 3;
  }

  if(fridgeHead <= NUM_LEDS){
    for(int i = fridgeMiddle; i < fridgeHead; i++){
      leds[i] = YELLOW;
    }
  }else{
     for(int i = fridgeMiddle; i <= NUM_LEDS; i++){
      leds[i] = YELLOW;
    }
  }
  
  if(fridgeHead >= fridgeMiddle){
      fridgeHead -= 3;
  }
  
}
