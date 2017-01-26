#include <sevenSegmentDisplay.h>  // https://github.com/HackerInside0/Arduino_sevenSegmentDisplay
#include <simtronyx_RGB_LED.h>    // http://blog.simtronyx.de/eine-rgb-led-bibliothek-fuer-den-arduino/
#include "FastLED.h"              // http://fastled.io/
#include <Thread.h>               // https://github.com/ivanseidel/ArduinoThread
#include <ThreadController.h>
#include "Animations.h"           // Load all animations

// This Software is just a prototyp
// Goal of this programm is to create a animation
// pattern that can be changed with a pushbutton
//
// author: Benjamin Koderisch
//  -b.koderisch@gmail.com-
// version: 1.1
// last update: 25.01.2017
//
// have fun!


////////////////////////////////////////////////////////////////////////////////
//\\\\\\\\\DEFINITIONS\\\\\\\\\following parameter can be changed\\\\\\\\\\\\\//
////////////////////////////////////////////////////////////////////////////////


// LED Strip //
#define DATA_PIN    3
//#define CLK_PIN     4       // for the final led strip
#define NUM_LEDS    60
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define FPS         120 // frames per second
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))


// Inputs //
#define BTN_PIN     2
#define FRIDGE      12


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
SimplePatternList gPatterns = {Animations.animation1, Animations.animation2,
                              Animations.animation3, Animations.animation4,
                              Animations.animation5, Animations.animation6,
                              Animations.animation7, Animations.animation8,
                              Animations.animation9};



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
    fridgeAnimation();    // Fridge animation starts
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
