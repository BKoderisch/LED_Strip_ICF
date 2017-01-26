#include <Arduino.h>

class Animations{

public:

    // rainbow
    void animation0{
      fill_rainbow( leds, NUM_LEDS, gHue, 3);
    }


    // confetti
    void animation1{
      // random colored speckles that blink in and fade smoothly
      fadeToBlackBy( leds, NUM_LEDS, 10);
      int pos = random16(NUM_LEDS);
      leds[pos] += CHSV( gHue + random8(64), 200, 255);
    }


    // sinelon
    void animation2{
      // a colored dot sweeping back and forth, with fading trails
      fadeToBlackBy( leds, NUM_LEDS, 20);
      int pos = beatsin16(13,0,NUM_LEDS);
      leds[pos] += CHSV( gHue, 255, 192);
    }


    // rainbowWithGlitter
    void animation3{
      // built-in FastLED rainbow, plus some random sparkly glitter
      rainbow();
      addGlitter(80);
    }


    // bpm
    void animation4{
      // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
      uint8_t BeatsPerMinute = 62;
      CRGBPalette16 palette = PartyColors_p;
      uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
      for( int i = 0; i < NUM_LEDS; i++) { //9948
        leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
      }
    }


    // juggle
    void animation5{
      // eight colored dots, weaving in and out of sync with each other
      fadeToBlackBy( leds, NUM_LEDS, 20);
      byte dothue = 0;
      for( int i = 0; i < 8; i++) {
        leds[beatsin16(i+7,0,NUM_LEDS)] |= CHSV(dothue, 200, 255);
        dothue += 32;
      }
    }

    void animation6{

    }

    void animation7{

    }

    void animation8{

    }

    void animation9{

    }

    void animationFridge{
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

}
