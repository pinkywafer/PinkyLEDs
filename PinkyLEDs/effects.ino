//palette for Christmas
DEFINE_GRADIENT_PALETTE( christmas_palette ) {
  0,   0, 12,  0,
  40,   0, 55,  0,
  66,   1, 117,  2,
  77,   1, 84,  1,
  81,   0, 55,  0,
  119,   0, 12,  0,
  153,  42,  0,  0,
  181, 121,  0,  0,
  204, 255, 12,  8,
  224, 121,  0,  0,
  244,  42,  0,  0,
  255,  42,  0,  0
};
String christmasEffect(boolean initialize){
  static uint8_t hue = 0;
  if (initialize){
    return "Christmas";
  } else {
    uint8_t BeatsPerMinute = 62;
    CRGBPalette16 palette = christmas_palette;
    uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
    for ( int i = 0; i < NUM_LEDS; i++) { //9948
      leds[i] = ColorFromPalette(palette, hue + (i * 2), beat - hue + (i * 10));
    }
    EVERY_N_MILLISECONDS(10) {
        hue++;
    }
    return "";
  }
}

//palette for St Patty
DEFINE_GRADIENT_PALETTE( st_pat_palette ) {
  0,   1, 22,  1,
  130,   1, 168,  2,
  255,   1, 22,  1
};
String stPatEffect(boolean initialize){
  if (initialize){
    return "St Patty";
  } else {
    static uint8_t gHue = 0;
    uint8_t BeatsPerMinute = 62;
    CRGBPalette16 palette = st_pat_palette;
    uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
    for ( int i = 0; i < NUM_LEDS; i++) { //9948
      leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
    }
    EVERY_N_MILLISECONDS(10) {
      gHue++;
    }
    return "";  
  }
}

//palette for Valentine
DEFINE_GRADIENT_PALETTE( valentine_palette ) {
  0, 103,  1, 10,
  33, 109,  1, 12,
  76, 159,  5, 48,
  119, 175, 55, 103,
  127, 175, 55, 103,
  178, 159,  5, 48,
  221, 109,  1, 12,
  255, 103,  1, 10
};
String valentineEffect(boolean initialize){
  if (initialize){
    return "Valentine";
  } else {
    static uint8_t gHue = 0;
    uint8_t BeatsPerMinute = 62;
    CRGBPalette16 palette = valentine_palette;
    uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
    for ( int i = 0; i < NUM_LEDS; i++) { //9948
      leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
    }
    EVERY_N_MILLISECONDS(10) {
      gHue++;
    }
    return "";  
  }
}

DEFINE_GRADIENT_PALETTE( turkey_day_palette ) {
  0,   9,  5,  1,
  48,  25,  9,  1,
  76, 137, 27,  1,
  96,  98, 42,  1,
  124, 144, 79,  1,
  153,  98, 42,  1,
  178, 137, 27,  1,
  211,  23,  9,  1,
  255,   9,  5,  1
};
String turkeyDayEffect(boolean initialize){
  if (initialize){
    return "Turkey Day";
  } else {
    static uint8_t gHue = 0;
    uint8_t BeatsPerMinute = 62;
    CRGBPalette16 palette = turkey_day_palette;
    uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
    for ( int i = 0; i < NUM_LEDS; i++) { //9948
      leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
    }
    EVERY_N_MILLISECONDS(10) {
      gHue++;
    }
    return "";  
  }
}


String thanksgivingEffect(boolean initialize){
  static CRGBPalette16 ThxPalettestriped;
  if (initialize){
    CRGB A = CRGB::OrangeRed;
    CRGB AB = CRGB::Olive;
    CRGB B = CRGB::Maroon;
    ThxPalettestriped = CRGBPalette16(
                        A, A, A, A, A, A, A, AB, AB, AB, B, B, B, B, B, B
                      );
    return "Thanksgiving";
  } else {
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; /* higher = faster motion */

    fill_palette( leds, NUM_LEDS,
                  startIndex, 16, /* higher = narrower stripes */
                  ThxPalettestriped, 255, LINEARBLEND);
    return "";  
  }
}

//palette for USA
DEFINE_GRADIENT_PALETTE( usa_palette ) {
  0,   0,  0, 45,
  71,   7, 12, 255,
  76,  75, 91, 255,
  76, 255, 255, 255,
  81, 255, 255, 255,
  178, 255, 255, 255,
  179, 255, 55, 45,
  196, 255,  0,  0,
  255,  42,  0,  0
};
String usaEffect(boolean initialize){
  if (initialize){
    return "USA";
  } else {
    static uint8_t gHue = 0;
    uint8_t BeatsPerMinute = 62;
    CRGBPalette16 palette = usa_palette;
    uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
    for ( int i = 0; i < NUM_LEDS; i++) { //9948
      leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
    }
    EVERY_N_MILLISECONDS(10) {
      gHue++;
    }
    return "";  
  }
}

String independenceEffect(boolean initialize){
  static CRGBPalette16 IndPalettestriped;
  if (initialize){
    CRGB A = CRGB::FireBrick;
    CRGB AB = CRGB::Cornsilk;
    CRGB B = CRGB::MediumBlue;
    IndPalettestriped = CRGBPalette16(
                        A, A, A, A, A, AB, AB, AB, AB, AB, B, B, B, B, B, B
                      );
    return "Independence";    
  } else {
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; /* higher = faster motion */

    fill_palette( leds, NUM_LEDS,
                  startIndex, 16, /* higher = narrower stripes */
                  IndPalettestriped, 255, LINEARBLEND);
    return "";  
  }
}

//palette for Halloween
DEFINE_GRADIENT_PALETTE( halloween_palette ) {
  0, 208, 50,  1,
  127, 146, 27, 45,
  255,  97, 12, 178
};
String halloweenEffect(boolean initialize){
  if (initialize){
    return "Halloween";
  } else {
    static uint8_t gHue = 0;
    uint8_t BeatsPerMinute = 62;
    CRGBPalette16 palette = halloween_palette;
    uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
    for ( int i = 0; i < NUM_LEDS; i++) { //9948
      leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
    }
    EVERY_N_MILLISECONDS(10) {
      gHue++;
    }
    return "";  
  }
}

//palette for Go Blue
DEFINE_GRADIENT_PALETTE( go_blue_palette ) {
  0,   4, 12, 122,
  127,  55, 58, 50,
  255, 192, 147, 11
};
String goBlueEffect(boolean initialize){
  if (initialize){
    return "Go Blue";
  } else {
    static uint8_t gHue = 0;
    uint8_t BeatsPerMinute = 62;
    CRGBPalette16 palette = go_blue_palette;
    uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
    for ( int i = 0; i < NUM_LEDS; i++) { //9948
      leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
    }
    EVERY_N_MILLISECONDS(10) {
      gHue++;
    }
    return "";  
  }
}

String hailEffect(boolean initialize){
  static CRGBPalette16 hailPalettestriped;
  if (initialize){
    CRGB A = CRGB::Blue;
    CRGB B = CRGB::Yellow;
    hailPalettestriped = CRGBPalette16(
                         A, A, A, A, A, A, A, A, B, B, B, B, B, B, B, B
                       );
    return "Hail";
  } else {
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; /* higher = faster motion */
    fill_palette( leds, NUM_LEDS,
                  startIndex, 16, /* higher = narrower stripes */
                  hailPalettestriped, 255, LINEARBLEND);  
    return "";  
  }
}

String touchdownEffect(boolean initialize){
  static int idex = 0;
  if (initialize){
    return "Touchdown";
  } else {
    idex++;
    if (idex >= NUM_LEDS) {
      idex = 0;
    }
    int idexY = idex;
    int idexB;
    if (idexY >= (NUM_LEDS / 2)) {
      idexB = ( idexY + NUM_LEDS / 2 ) % NUM_LEDS;
    } else {
      idexB = idexY + (NUM_LEDS / 2);
    }
    leds[idexY] = CHSV(64, 255, 255);
    leds[idexB] = CHSV(160, 255, 255);
    return "";  
  }
}

String punkinEffect(boolean initialize){
  static CRGBPalette16 punkinPalettestriped;
  if (initialize){
    CRGB A = CRGB::DarkOrange;
    CRGB B = CRGB::Indigo;
    punkinPalettestriped = CRGBPalette16(
                              A, A, A, A, A, A, A, A, B, B, B, B, B, B, B, B
                            );
    return "Punkin";
  } else {
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; /* higher = faster motion */
    fill_palette( leds, NUM_LEDS,
                  startIndex, 16, /* higher = narrower stripes */
                  punkinPalettestriped, 255, LINEARBLEND);
    return "";  
  }
}
String loveyDayEffect(boolean initialize){
  static int idex = 0;
  if (initialize){
    return "Lovey Day";
  } else {
    idex++;
    if (idex >= NUM_LEDS) {
      idex = 0;
    }
    int idexR = idex;
    int idexB;
    if (idexR >= (NUM_LEDS / 2)) {
      idexB = ( idexR + NUM_LEDS / 2 ) % NUM_LEDS;
    } else {
      idexB = idexR + (NUM_LEDS / 2);
    }
    leds[idexR] = CHSV(0, 255, 255);
    leds[idexB] = CHSV(244, 255, 255);
    return "";  
  }
}

String hollyJollyEffect(boolean initialize){
  static CRGBPalette16 HJPalettestriped;
  if (initialize){
    CRGB A = CRGB::Red;
    CRGB B = CRGB::Green;
    HJPalettestriped = CRGBPalette16(
                       A, A, A, A, A, A, A, A, B, B, B, B, B, B, B, B
                     );
    return "Holly Jolly";
  } else {
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; /* higher = faster motion */
    fill_palette( leds, NUM_LEDS,
                  startIndex, 16, /* higher = narrower stripes */
                  HJPalettestriped, 255, LINEARBLEND);
    return "";  
  }
}
String sinelonColorEffect(boolean initialize){
  if (initialize){
    return "Sinelon Color";
  } else {
    int Rcolor = setRed;
    int Gcolor = setGreen;
    int Bcolor = setBlue; 
    fadeToBlackBy( leds, NUM_LEDS, 20);
    int pos = beatsin16(13, 0, NUM_LEDS - 1);
    leds[pos] += CRGB(Rcolor, Gcolor, Bcolor);
    return "";  
  }
}

String juggleColorEffect(boolean initialize){
  if (initialize){
    return "Juggle Color";
  } else {
    int Rcolor = setRed;
    int Gcolor = setGreen;
    int Bcolor = setBlue; 
    fadeToBlackBy( leds, NUM_LEDS, 20);
    byte dothue = 0;
    for ( int i = 0; i < 8; i++) {
      leds[beatsin16(i + 7, 0, NUM_LEDS - 1)] |= CRGB(Rcolor, Gcolor, Bcolor);
      dothue += 32;
    }
    return "";  
  }
}

String confettiColorEffect(boolean initialize){
  if (initialize){
    return "Confetti Color";
  } else {
    int Rcolor = setRed;
    int Gcolor = setGreen;
    int Bcolor = setBlue; 
    fadeToBlackBy( leds, NUM_LEDS, 10);
    int pos = random16(NUM_LEDS);
    leds[pos] += CRGB(Rcolor + random8(64), Gcolor, Bcolor);
    return "";  
  }
}

String rainbowEffect(boolean initialize){
  static uint8_t thishue;
  if (initialize){
    thishue = 0;
    return "Rainbow";
  } else {
    // FastLED's built-in rainbow generator
    static uint8_t starthue = 0;    thishue++;
    fill_rainbow(leds, NUM_LEDS, thishue, 10);
    return "";  
  }
}

String glitterRainbowEffect(boolean initialize){
  static uint8_t thishue;
  if (initialize){
    thishue = 0;
    return "Glitter Rainbow";
  } else {
    static uint8_t starthue = 0;
    thishue++;
    fill_rainbow(leds, NUM_LEDS, thishue, 10);
    //add glitter
    if ( random8() < 80) { //chance of glitter = 80
      leds[ random16(NUM_LEDS) ] += CRGB::White;
    }
    return "";  
  }
}

String glitterColorEffect(boolean initialize){
  if (initialize){
    return "Glitter Color";
  } else {
    fadeToBlackBy( leds, NUM_LEDS, 20);
    int Rcolor = setRed;
    int Gcolor = setGreen;
    int Bcolor = setBlue; 
    if ( random8() < 80) {
      leds[ random16(NUM_LEDS) ] += CRGB(Rcolor, Gcolor, Bcolor);
    }
    return "";  
  }
}

String bpmEffect(boolean initialize){
  if (initialize){
    return "BPM";
  } else {
    static uint8_t gHue = 0;
    uint8_t BeatsPerMinute = 62;
    CRGBPalette16 palette = PartyColors_p;
    uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
    for ( int i = 0; i < NUM_LEDS; i++) { //9948
      leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
    }
    EVERY_N_MILLISECONDS(10) {
      gHue++;
    }
    return "";  
  }
}

String twinkleEffect(boolean initialize){
  static int twinklecounter;
  if (initialize){
    twinklecounter = 0;
    return "Twinkle";
  } else {
    twinklecounter++;
    if (twinklecounter < 2) {                               //Resets strip if previous animation was running
      FastLED.clear();
      FastLED.show();
    }
    const CRGB lightcolor(8, 7, 1);
    for ( int i = 0; i < NUM_LEDS; i++) {
      if ( !leds[i]) continue; // skip black pixels
      if ( leds[i].r & 1) { // is red odd?
        leds[i] -= lightcolor; // darken if red is odd
      } else {
        leds[i] += lightcolor; // brighten if red is even
      }
    }
    if ( random8() < 80) { //twinkle density
      int j = random16(NUM_LEDS);
      if ( !leds[j] ) leds[j] = lightcolor;
    }
    return "";  
  }
}

String lightningEffect(boolean initialize){
  const uint8_t flashes = 8;               //the upper limit of flashes per strike
  static unsigned int dimmer;
  uint16_t ledstart;
  uint16_t ledlen;
  static int twinklecounter;
  if (initialize){
    twinklecounter = 0;
    return "Lightning";
  } else {
    twinklecounter++;
    if (twinklecounter < 2) {       //Resets strip if previous animation was running
      FastLED.clear();
      FastLED.show();
    }
    ledstart = random16(NUM_LEDS);           // Determine starting location of flash
    ledlen = random16(NUM_LEDS - ledstart);  // Determine length of flash (not to go beyond NUM_LEDS-1)
    static unsigned int lightningFlashTime = 0;
    static unsigned int lightningFlashDelay = 0;
    if (millis() - lightningFlashTime >= lightningFlashDelay){
      for (int lightningFlashCounter = 0; lightningFlashCounter < random8(3, flashes); lightningFlashCounter++) {
        if (lightningFlashCounter == 0) dimmer = 5;    // the brightness of the leader is scaled down by a factor of 5
        else dimmer = random8(1, 3);          // return strokes are brighter than the leader
        fill_solid(leds + ledstart, ledlen, CHSV(255, 0, 255 / dimmer));
        FastLED.show();                       // Show a section of LED's
        delay(random8(4, 10));                // each flash only lasts 4-10 milliseconds
        fill_solid(leds + ledstart, ledlen, CHSV(255, 0, 0)); // Clear the section of LED's
        FastLED.show();
        if (lightningFlashCounter == 0) delay (150);   // longer delay until next flash after the leader
        delay(50 + random8(100));             // shorter delay between strokes
      }
      lightningFlashDelay = random8(50) * 100;        // delay between strikes
      lightningFlashTime = millis();
    }
    return "";  
  }
}


String policeOneEffect(boolean initialize){
  static int idex = 0;
  if (initialize){
    return "Police One";
  } else {
    idex++;
    if (idex >= NUM_LEDS) {
      idex = 0;
    }
    int idexR = idex;
    int idexB;
    if (idexR >= (NUM_LEDS / 2)) {
      idexB = ( idexR + NUM_LEDS / 2 ) % NUM_LEDS;
    } else {
      idexB = idexR + (NUM_LEDS / 2);
    }
    for (int i = 0; i < NUM_LEDS; i++ ) {
      if (i == idexR) {
        leds[i] = CHSV(0, 255, 255);
      }
      else if (i == idexB) {
        leds[i] = CHSV(160, 255, 255);
      }
      else {
        leds[i] = CHSV(0, 0, 0);
      }
    }
    return "";  
  }
}

String policeAllEffect(boolean initialize){
  static int idex = 0;
  if (initialize){
    return "Police All";
  } else {
    idex++;
    if (idex >= NUM_LEDS) {
      idex = 0;
    }
    int idexR = idex;
    int idexB;
    if (idexR >= (NUM_LEDS / 2)) {
      idexB = ( idexR + NUM_LEDS / 2 ) % NUM_LEDS;
    } else {
      idexB = idexR + (NUM_LEDS / 2);
    }
    leds[idexR] = CHSV(0, 255, 255);
    leds[idexB] = CHSV(160, 255, 255);
    return "";  
  }
}

String candyCaneEffect(boolean initialize){
  static CRGBPalette16 currentPalettestriped;
  if (initialize){
    CRGB A = CRGB::Red;
    CRGB B = CRGB::White;
    currentPalettestriped = CRGBPalette16(
                            A, A, A, A, A, A, A, A, B, B, B, B, B, B, B, B
                          );
    return "Candy Cane";
  } else {
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; /* higher = faster motion */

    fill_palette( leds, NUM_LEDS,
                  startIndex, 16, /* higher = narrower stripes */
                  currentPalettestriped, 255, LINEARBLEND);
    return "";  
  }
}

String cyclonRainbowEffect(boolean initialize){
  if (initialize){
    return "Cyclon Rainbow";
  } else {
    static uint8_t cyclonHue = 0;
    static bool forwards = false;
    for(int a = 0; a < NUM_LEDS; a++) {
      leds[a].nscale8(250);
    }
    static int cyclonPos = 0;
    if (forwards) {
      if (cyclonPos < NUM_LEDS - 1){
        leds[cyclonPos] = CHSV(cyclonHue++, 255,255);
        cyclonPos++;
      } else {
        forwards = false;
      }
    } else {
      if (cyclonPos >= 0) {
        leds[cyclonPos] = CHSV(cyclonHue++, 255,255);
        cyclonPos--;
      } else {
        forwards = true;
      }
    }
    return "";  
  }
}

String fireEffect(boolean initialize){
  static CRGBPalette16 gPal;
  if (initialize){
      gPal = HeatColors_p; //FastLED built in palette
    return "Fire";
  } else {
    // Array of temperature readings at each simulation cell
    static byte heat[NUM_LEDS];
    // Step 1.  Cool down every cell a little
    for ( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((55 * 10) / NUM_LEDS) + 2));
    }
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for ( int k = NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if ( random8() < 120 ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160, 255) );
    }
    // Step 4.  Map from heat cells to LED colors
    for ( int j = 0; j < NUM_LEDS; j++) {
      // Scale the heat value from 0-255 down to 0-240
      // for best results with color palettes.
      byte colorindex = scale8( heat[j], 240);
      CRGB color = ColorFromPalette( gPal, colorindex);
      leds[j] = color;
    }
    return "";  
  }
}

String easterEffect(boolean initialize){
  const uint16_t scale = 30;          // Wouldn't recommend changing this on the fly, or the animation will be really blocky.
  const uint8_t maxChanges = 48;      // Value for blending between palettes.
  static uint16_t dist;         // A random number for our noise generator.
  static CRGBPalette16 targetPalette(OceanColors_p);
  static CRGBPalette16 currentPalette(CRGB::Black);
  if (initialize){
    return "Easter";
  } else {
    EVERY_N_MILLISECONDS(10) {
      nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);  // FOR NOISE ANIMATION
    
      for (int i = 0; i < NUM_LEDS; i++) {                                     // Just ONE loop to fill up the LED array as all of the pixels change.
        uint8_t index = inoise8(i * scale, dist + i * scale) % 255;            // Get a value from the noise function. I'm using both x and y axis.
        leds[i] = ColorFromPalette(currentPalette, index, 255, LINEARBLEND);   // With that value, look up the 8 bit colour palette value and assign it to the current LED.
      }
      dist += beatsin8(10, 1, 4);
    }
    EVERY_N_SECONDS(5) {
      targetPalette = CRGBPalette16(CHSV(random8(), 255, random8(128, 255)), CHSV(random8(), 255, random8(128, 255)), CHSV(random8(), 192, random8(128, 255)), CHSV(random8(), 255, random8(128, 255)));
    }
    return "";  
  }
}


String rippleEffect(boolean initialize){
  static uint8_t colour;     // Ripple colour is randomized.
  static int center;         // Center of the current ripple.
  static int step;                  // -1 is the initializing step.
  static uint8_t bgcol;             // Background colour rotates.
  if (initialize){
    step = -1;
    bgcol = 0;
    return "Ripple";
  } else {
    EVERY_N_MILLISECONDS(10) {
      for (int i = 0; i < NUM_LEDS; i++) leds[i] = CHSV(bgcol++, 255, 15);  // Rotate background colour.
      switch (step) {
        case -1:                                                          // Initialize ripple variables.
          center = random(NUM_LEDS);
          colour = random8();
          step = 0;
          break;
        case 0:
          leds[center] = CHSV(colour, 255, 255);                        // Display the first pixel of the ripple.
          step ++;
          break;
        case 16:                                                   // At the end of the ripples.
          step = -1;
          break;
        default:                                                             // Middle of the ripples.
          leds[(center + step + NUM_LEDS) % NUM_LEDS] += CHSV(colour, 255, 255 / step * 2);   // Simple wrap from Marc Miller
          leds[(center - step + NUM_LEDS) % NUM_LEDS] += CHSV(colour, 255, 255 / step * 2);
          step ++;                                                         // Next step.
          break;
      }
    }
    return "";  
  }
}

String dotsEffect(boolean initialize){
  uint8_t bpm = 30;
  uint8_t fadeval = 224;                                        // Trail behind the LED's. Lower => faster fade.
  if (initialize){
    return "Dots";
  } else {
    uint16_t inner = beatsin16(bpm, NUM_LEDS / 4, NUM_LEDS / 4 * 3);
    uint16_t outer = beatsin16(bpm, 0, NUM_LEDS - 1);
    uint16_t middle = beatsin16(bpm, NUM_LEDS / 3, NUM_LEDS / 3 * 2);
    leds[middle] = CRGB::Purple;
    leds[inner] = CRGB::Blue;
    leds[outer] = CRGB::Aqua;
    nscale8(leds, NUM_LEDS, fadeval);
    return "";  
  }
}

/*
String Effect(boolean initialize){
  if (initialize){
    return "";
  } else {
    
    return "";  
  }
}*/
