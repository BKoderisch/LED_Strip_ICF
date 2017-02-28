#include <sevenSegmentDisplay.h>  // https://github.com/HackerInside0/Arduino_sevenSegmentDisplay
#include <simtronyx_RGB_LED.h>    // http://blog.simtronyx.de/eine-rgb-led-bibliothek-fuer-den-arduino/
#include "FastLED.h"              // http://fastled.io/
#include <Thread.h>               // https://github.com/ivanseidel/ArduinoThread
#include <ThreadController.h>
#include "Animations.h"

// This Software is just a prototyp
// Goal of this programm is to create a animation
// pattern that can be changed with a pushbutton
//
// author: Benjamin Koderisch
//  -b.koderisch@gmail.com-
// version: 1.3
// last update: 23.02.2017
//
// have fun!


////////////////////////////////////////////////////////////////////////////////
//\\\\\\\\\DEFINITIONS\\\\\\\\\following parameter can be changed\\\\\\\\\\\\\//
////////////////////////////////////////////////////////////////////////////////


// LED Strip //
#define DATA_PIN   11
#define CLK_PIN    12       // for the final led strip
#define LED_TYPE    WS2801
#define COLOR_ORDER GRB
#define FPS         20       // frames per second

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))


// Inputs //
#define BTN_PIN     3       // input pin of the pushbutton
#define FRIDGE      2       // pin for the door sensor for the fridge


// 7 segment display //
#define A   4
#define B   5
#define C   6
#define D   7
#define E   8
#define F   9
#define G   10





////////////////////////////////////////////////////////////////////////////////
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\Initiations\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
////////////////////////////////////////////////////////////////////////////////


// Led Strip //
CRGBArray<NUM_LEDS> leds;   // initiate the led Strip (leds)


// inputs //
boolean doSwitch = false;   // true, when button was pressed - turn false when switch is over
int fridgeState = 0;        // 1 when fridge is closed - 0 when fridge is open



// 7 segment display //
sevenSegmentDisplay display(COMMON_CATHODE, A, B, C, D, E, F, G, DP);


// patterns //
uint8_t gCurrentPatternNumber = 0;        // Index Number of which pattern is current
uint8_t gHue = 0;                         // index of current color
uint8_t gPos = 0;                         // index of current position
typedef void (*SimplePatternList[])(CRGBArray<NUM_LEDS> leds, uint8_t gHue, uint8_t gPos);    // List of patterns to cycle through. Each is defines as a seperate function below.



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
  FastLED.addLeds<LED_TYPE, DATA_PIN, CLK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);


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



  // // // // // periodic update // // // // //
  EVERY_N_MILLISECONDS( 20 ) { gHue++; }  // slowly cycle the "base color" through the rainbow
  if(gPos < NUM_LEDS){
    gPos++;
  } else{
    gPos = 0;
  }

  gPatterns[gCurrentPatternNumber](leds, gHue, gPos);     // load current Pattern
  if(doSwitch == true){                   // switch to next pattern when button was pressed
    gCurrentPatternNumber = (gCurrentPatternNumber + 1) % (ARRAY_SIZE (gPatterns));
    doSwitch = false;
  }

      // // // // // Fridge inerrupt // // // // //
  if(digitalRead(FRIDGE)==LOW){
    display.set('F');     // 7 segment display shows a 'F'
    animationFridge(leds, gHue, gPos);    // Fridge animation starts
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

