#include <sevenSegmentDisplay.h>
#include <simtronyx_RGB_LED.h>
#include "FastLED.h"
#include <Thread.h>
#include <ThreadController.h>

// This Software is just a prototyp
// Goal of this programm is to create a animation
// pattern that can be changed with a pushbutton
//
// author: Benjamin Koderisch
//  -b.koderisch@gmail.com-
// version: 1.0
// last update: 25.01.2017
//
// have fun

//following parameter can be changed
#define DATA_PIN    3
// #define CLK_PIN     4    // for the final led strip
#define BTN_PIN     2
#define FRIDGE      12
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS    60

// definitions for the rgb LED
#define RED_PIN   11
#define GREEN_PIN 6
#define BLUE_PIN  10
#define LED_S     100
#define LED_V     45

#define A   0
#define B   1
#define C   4
#define D   5
#define E   7
#define F   8 
#define G   13

sevenSegmentDisplay display(COMMON_CATHODE, A, B, C, D, E, F, G, DP);
simtronyx_RGB_LED rgbLed(RED_PIN, GREEN_PIN, BLUE_PIN);

//CRGB leds[NUM_LEDS];
CRGBArray<NUM_LEDS> leds;

int fridgeState = 0;
boolean doSwitch = false;

// this parameter will be replaced by potentiometers
#define FPS         120 // frames per second
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

// Index Number of which pattern is current
uint8_t gCurrentPatternNumber = 0;
// index of current color
uint8_t gHue = 0;

// Thread Controller that will controll all threads
ThreadController controll = ThreadController();

// LED Strip
Thread* ledStrip = new Thread();
// button controll
Thread* btnControll = new Thread();



void setup() {
  delay(3000);  // 3 second delay for recovery

  pinMode(BTN_PIN, INPUT);
  pinMode(FRIDGE, INPUT);
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE, DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);


  // Configure ledStrip Thread
  ledStrip->onRun(lightCallback);
  // Configure btnControll
  btnControll->onRun(buttonCallback);

  // add Threads to controller
  controll.add(ledStrip);
  controll.add(btnControll);
}


// List of patterns to cycle through. Each is defines as a seperate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = {rainbow, confetti, rainbowWithGlitter, sinelon, bpm, juggle};


void loop() {
  controll.run();
}

// callback for ledStrip
void lightCallback(){
  while((fridgeState = digitalRead(FRIDGE))==LOW){
    display.set('F');
    animation1();
  }
  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  gPatterns[gCurrentPatternNumber]();
  if(doSwitch == true){
    gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE (gPatterns);
    doSwitch = false;
  }

  FastLED.show();
  FastLED.delay(1000/FPS);
}


void buttonCallback(){
  rgbLed.setHSV(gHue, LED_S, LED_V);
  display.set(gCurrentPatternNumber);
  int buttonState = 0;
  if(doSwitch == false){
    buttonState = digitalRead(BTN_PIN);

    if (buttonState == HIGH){
      doSwitch = true;
      delay(200);
    }
  }
}

void rainbow()
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 3);
}

void confetti()
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void animation1(){
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

void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}
void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16(13,0,NUM_LEDS);
  leds[pos] += CHSV( gHue, 255, 192);
}
void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16(i+7,0,NUM_LEDS)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

