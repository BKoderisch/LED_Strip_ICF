#include <sevenSegmentDisplay.h>  // https://github.com/HackerInside0/Arduino_sevenSegmentDisplay
#include <simtronyx_RGB_LED.h>    // http://blog.simtronyx.de/eine-rgb-led-bibliothek-fuer-den-arduino/
#include "FastLED.h"              // http://fastled.io/
#include <Thread.h>               // https://github.com/ivanseidel/ArduinoThread
#include <ThreadController.h>

// This Software is just a prototyp
// Goal of this programm is to create a animation
// pattern that can be changed with a pushbutton
//
// author: Benjamin Koderisch
//  -b.koderisch@gmail.com-
// version: 1.2
// last update: 27.01.2017
//
// have fun!


////////////////////////////////////////////////////////////////////////////////
//\\\\\\\\\DEFINITIONS\\\\\\\\\following parameter can be changed\\\\\\\\\\\\\//
////////////////////////////////////////////////////////////////////////////////


// LED Strip //
#define DATA_PIN    3
//#define CLK_PIN     4       // for the final led strip
#define NUM_LEDS    60
#define LED_TYPE    WS2812B   // WS2801 for the ICF LED Strip
#define COLOR_ORDER GRB
#define FPS         120       // frames per second
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))


// Inputs //
#define BTN_PIN     2       // input pin of the pushbutton
#define FRIDGE      12      // pin for the door sensor for the fridge


// RGB LED //
#define RED_PIN     11
#define GREEN_PIN   6
#define BLUE_PIN    10
#define LED_S       100     // LED saturation
#define LED_V       45      // LED value


// 7 segment display //
#define A   0
#define B   1
#define C   9
#define D   5
#define E   7
#define F   8
#define G   13





////////////////////////////////////////////////////////////////////////////////
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\Initiations\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
////////////////////////////////////////////////////////////////////////////////


// Led Strip //
CRGBArray<NUM_LEDS> leds;   // initiate the led Strip (leds)


// inputs //
boolean doSwitch = false;   // true, when button was pressed - turn false when switch is over
int fridgeState = 0;        // 1 when fridge is closed - 0 when fridge is open


// RGB LED //
simtronyx_RGB_LED rgbLed(RED_PIN, GREEN_PIN, BLUE_PIN);


// 7 segment display //
sevenSegmentDisplay display(COMMON_CATHODE, A, B, C, D, E, F, G, DP);


// patterns //
uint8_t gCurrentPatternNumber = 0;        // Index Number of which pattern is current
uint8_t gHue = 0;                         // index of current color
typedef void (*SimplePatternList[])();    // List of patterns to cycle through. Each is defines as a seperate function below.



// threads //
ThreadController controll = ThreadController();  // Thread Controller that will controll all threads
Thread* ledStrip = new Thread();                 // Thread to controll the LED Strip
Thread* btnControll = new Thread();              // Thread to controll the button Inputs




//,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,//
//////////////////////////////////Setup/////////////////////////////////////////
//''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''//

void setup() {
  delay(3000);                // 3 second delay for recovery


  // initiate inputs //
  pinMode(BTN_PIN, INPUT);    // set Button as INPUT
  pinMode(FRIDGE, INPUT);     // set fridge as INPUT


  // LED Strip //
  FastLED.addLeds<LED_TYPE, DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);


  // Threads //
  ledStrip->onRun(lightCallback);         // configure ledStrip
  btnControll->onRun(buttonCallback);     // configure btnControll

  // Thread Controller //
  controll.add(ledStrip);                 // add ledStrip to controller
  controll.add(btnControll);              // add btnControll to controller
}




//,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,//
/////////////////////////////////loop///////////////////////////////////////////
//''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''//

void loop() {
  controll.run();
}


// // // // // // // // // fill pattern list // // // // // // // // // // // //

SimplePatternList gPatterns = {animation0, animation1, animation2, animation3,
                              animation4, animation5, animation6, animation7,
                              animation8, animation9};

// // // // // // // // // // // // // // // // // // // // // // // // // // //



////////////////////////////////////////////////////////////////////////////////
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\Thread handling\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
////////////////////////////////////////////////////////////////////////////////

// // // // // This thread handel following tasks  // // // // //
//                                                             //
//  -- interrupt current animation when the frige is open --   //
//               -- update periodicly the hue value --         //
//                    -- show current pattern --               //
//            -- switch pattern when "doSwitch" is true --     //
//                                                             //
// // // // // // // // // // // // // // // // // // // // // //
void lightCallback(){

  // // // // // Fridge inerrupt // // // // //
  while((fridgeState = digitalRead(FRIDGE))==LOW){
    display.set('F');     // 7 segment display shows a 'F'
    animationFridge();    // Fridge animation starts
  }

  // // // // // periodic update // // // // //
  EVERY_N_MILLISECONDS( 20 ) { gHue++; }  // slowly cycle the "base color" through the rainbow

  gPatterns[gCurrentPatternNumber]();     // load current Pattern
  if(doSwitch == true){                   // switch to next pattern when button was pressed
    gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE (gPatterns);
    doSwitch = false;
  }

  FastLED.show();
  FastLED.delay(1000/FPS);
}


// // // // // This thread handel following tasks  // // // // //
//                                                             //
//                    -- controll the RGB LED --               //
//               -- controll the 7 segment display --          //
//                  -- controll button activity --             //
//         -- set "doSwitch" true, when button was pressed --  //
//                                                             //
// // // // // // // // // // // // // // // // // // // // // //

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


// rainbow
void animation0(){
  fill_rainbow( leds, NUM_LEDS, gHue, 3);
}


// confetti
void animation1(){
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}


// sinelon
void animation2(){
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16(13,0,NUM_LEDS);
  leds[pos] += CHSV( gHue, 255, 192);
}


// rainbowWithGlitter
void animation3(){
  // built-in FastLED rainbow, plus some random sparkly glitter
  animation0();
  addGlitter(80);
}


// bpm
void animation4(){
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}


// juggle
void animation5(){
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16(i+7,0,NUM_LEDS)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

void animation6(){

}

void animation7(){

}

void animation8(){

}

void animation9(){

}

void animationFridge(){
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
