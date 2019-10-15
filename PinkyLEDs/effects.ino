String christmasEffect(boolean initialize){
  static uint8_t hue = 0;
  if (initialize){
    return "Christmas";
  } else {
    uint8_t BeatsPerMinute = 62;
    CRGBPalette16 palette = bhw2_xmas_gp;
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

String stPatEffect(boolean initialize){
  if (initialize){
    return "St Patty";
  } else {
    uint8_t BeatsPerMinute = 62;
    CRGBPalette16 palette = bhw2_greenman_gp;
    uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
    for ( int i = 0; i < NUM_LEDS; i++) { //9948
      leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
    }
    return "";  
  }
}

String valentineEffect(boolean initialize){
  if (initialize){
    return "Valentine";
  } else {
    uint8_t BeatsPerMinute = 62;
    CRGBPalette16 palette = bhw2_redrosey_gp;
    uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
    for ( int i = 0; i < NUM_LEDS; i++) { //9948
      leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
    }
    return "";  
  }
}

String turkeyDayEffect(boolean initialize){
  if (initialize){
    return "Turkey Day";
  } else {
    uint8_t BeatsPerMinute = 62;
    CRGBPalette16 palette = bhw2_thanks_gp;
    uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
    for ( int i = 0; i < NUM_LEDS; i++) { //9948
      leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
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

String usaEffect(boolean initialize){
  if (initialize){
    return "USA";
  } else {
    uint8_t BeatsPerMinute = 62;
    CRGBPalette16 palette = bhw3_41_gp;
    uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
    for ( int i = 0; i < NUM_LEDS; i++) { //9948
      leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
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

String halloweenEffect(boolean initialize){
  if (initialize){
    return "Halloween";
  } else {
    uint8_t BeatsPerMinute = 62;
    CRGBPalette16 palette = Orange_to_Purple_gp;
    uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
    for ( int i = 0; i < NUM_LEDS; i++) { //9948
      leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
    }
    return "";  
  }
}

String goBlueEffect(boolean initialize){
  if (initialize){
    return "Go Blue";
  } else {
    uint8_t BeatsPerMinute = 62;
    CRGBPalette16 palette = Pills_3_gp;
    uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
    for ( int i = 0; i < NUM_LEDS; i++) { //9948
      leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
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
  if (initialize){
    return "Touchdown";
  } else {
    idex++;
    if (idex >= NUM_LEDS) {
      idex = 0;
    }
    int idexY = idex;
    int idexB = antipodal_index(idexY);
    int thathue = (thishuehail + 96) % 255;
    leds[idexY] = CHSV(thishuehail, thissat, 255);
    leds[idexB] = CHSV(thathue, thissat, 255);
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
  if (initialize){
    return "Lovey Day";
  } else {
    idex++;
    if (idex >= NUM_LEDS) {
      idex = 0;
    }
    int idexR = idex;
    int idexB = antipodal_index(idexR);
    int thathue = (thishueLovey + 244) % 255;
    leds[idexR] = CHSV(thishueLovey, thissat, 255);
    leds[idexB] = CHSV(thathue, thissat, 255);
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
  if (initialize){
    return "Rainbow";
  } else {
    // FastLED's built-in rainbow generator
    static uint8_t starthue = 0;    thishue++;
    fill_rainbow(leds, NUM_LEDS, thishue, deltahue);
    return "";  
  }
}

/*void addGlitter( fract8 chanceOfGlitter)
{
  if ( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}*/

String glitterRainbowEffect(boolean initialize){
  if (initialize){
    return "Glitter Rainbow";
  } else {
    static uint8_t starthue = 0;
    thishue++;
    fill_rainbow(leds, NUM_LEDS, thishue, deltahue);
    //add glitter
    if ( random8() < 80) { //chance of glitter = 80
      leds[ random16(NUM_LEDS) ] += CRGB::White;
    }
    //addGlitter(80);
    return "";  
  }
}

String glitterColorEffect(boolean initialize){
  if (initialize){
    return "Glitter Color";
  } else {
    fadeToBlackBy( leds, NUM_LEDS, 20);
    //addGlitterColor(80, Rcolor, Gcolor, Bcolor);
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
    uint8_t BeatsPerMinute = 62;
    CRGBPalette16 palette = PartyColors_p;
    uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
    for ( int i = 0; i < NUM_LEDS; i++) { //9948
      leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
    }
    return "";  
  }
}

String twinkleEffect(boolean initialize){
  if (initialize){
    return "Twinkle";
  } else {
    twinklecounter = twinklecounter + 1;
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
    if ( random8() < DENSITY) {
      int j = random16(NUM_LEDS);
      if ( !leds[j] ) leds[j] = lightcolor;
    }
    return "";  
  }
}

String lightningEffect(boolean initialize){
  if (initialize){
    return "Lightning";
  } else {
    twinklecounter = twinklecounter + 1;
    if (twinklecounter < 2) {                               //Resets strip if previous animation was running
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
      lightningFlashDelay = random8(frequency) * 100;        // delay between strikes
      lightningFlashTime = millis();
    }
    return "";  
  }
}


String policeOneEffect(boolean initialize){
  if (initialize){
    return "Police One";
  } else {
    idex++;
    if (idex >= NUM_LEDS) {
      idex = 0;
    }
    int idexR = idex;
    int idexB = antipodal_index(idexR);
    int thathue = (thishuepolice + 160) % 255;
    for (int i = 0; i < NUM_LEDS; i++ ) {
      if (i == idexR) {
        leds[i] = CHSV(thishuepolice, thissat, 255);
      }
      else if (i == idexB) {
        leds[i] = CHSV(thathue, thissat, 255);
      }
      else {
        leds[i] = CHSV(0, 0, 0);
      }
    }
    return "";  
  }
}

String policeAllEffect(boolean initialize){
  if (initialize){
    return "Police All";
  } else {
    idex++;
    if (idex >= NUM_LEDS) {
      idex = 0;
    }
    int idexR = idex;
    int idexB = antipodal_index(idexR);
    int thathue = (thishuepolice + 160) % 255;
    leds[idexR] = CHSV(thishuepolice, thissat, 255);
    leds[idexB] = CHSV(thathue, thissat, 255);
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
  if (initialize){
    return "Fire";
  } else {
    Fire2012WithPalette();
    return "";  
  }
}
String easterEffect(boolean initialize){
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
  if (initialize){
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
        case maxsteps:                                                   // At the end of the ripples.
          step = -1;
          break;
        default:                                                             // Middle of the ripples.
          leds[(center + step + NUM_LEDS) % NUM_LEDS] += CHSV(colour, 255, myfade / step * 2);   // Simple wrap from Marc Miller
          leds[(center - step + NUM_LEDS) % NUM_LEDS] += CHSV(colour, 255, myfade / step * 2);
          step ++;                                                         // Next step.
          break;
      }
    }
    return "";  
  }
}


String dotsEffect(boolean initialize){
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
