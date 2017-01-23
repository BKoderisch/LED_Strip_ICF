#include <Button.h>
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
// 20.01.2017
//
// have fun

//following parameter can be changed
#define DATA_PIN    3
// #define CLK_PIN     4    // for the final led strip
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS    60
CRGB leds[NUM_LEDS];

int btnPin = 2;
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

  pinMode(btnPin, INPUT);
  
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
SimplePatternList gPatterns = {rainbow, confetti};


void loop() {
  controll.run();
}

// callback for ledStrip
void lightCallback(){
  gPatterns[gCurrentPatternNumber]();
  if(doSwitch == true){
    gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE (gPatterns);
    doSwitch = false;
  }

  FastLED.show();
  FastLED.delay(1000/FPS);
}


void buttonCallback(){
  int buttonState = 0;
  if(doSwitch == false){
    buttonState = digitalRead(btnPin);

    if (buttonState == HIGH){
      doSwitch = true;
      delay(200);
    }
  }
}

void rainbow()
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void confetti()
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}
