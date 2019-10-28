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
String christmasEffect(char mode, bool strip){
  static uint8_t hue = 0;
  switch (mode){
    case RUN:
      {
        CRGBPalette16 palette = christmas_palette;
        uint8_t beat = beatsin8( animationSpeed / 3, 64, 255);
        for ( int i = 0; i < NUM_LEDS; i++) {
          leds[strip][i] = ColorFromPalette(palette, hue + (i * 2), beat - hue + (i * 10));
        }
        EVERY_N_MILLISECONDS(10) {
            hue++;
        }
      }
      return "";
      break;
    case INITIALIZE:
      hue = 0;
      return "Ok";
    case GET_NAME:
      return "Christmas";
  }
}

//palette for St Patty
DEFINE_GRADIENT_PALETTE( st_pat_palette ) {
  0,   1, 22,  1,
  130,   1, 168,  2,
  255,   1, 22,  1
};
String stPatEffect(char mode, bool strip){
  static uint8_t gHue = 0;
  switch (mode){
    case RUN:
      {
        CRGBPalette16 palette = st_pat_palette;
        uint8_t beat = beatsin8( animationSpeed / 3, 64, 255);
        for ( int i = 0; i < NUM_LEDS; i++) {
          leds[strip][i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
        }
        EVERY_N_MILLISECONDS(10) {
          gHue++;
        }
      }
      return "";  
      break;
    case INITIALIZE:
      gHue = 0;
      return "Ok";
      break;
    case GET_NAME:
      return "St Patty";
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
String valentineEffect(char mode, bool strip){
  static uint8_t gHue = 0;
  switch (mode){
    case RUN:
      {
        CRGBPalette16 palette = valentine_palette;
        uint8_t beat = beatsin8( animationSpeed / 3, 64, 255);
        for ( int i = 0; i < NUM_LEDS; i++) {
          leds[strip][i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
        }
        EVERY_N_MILLISECONDS(10) {
          gHue++;
        }
      }
      return "";
      break;
    case INITIALIZE:
      gHue = 0;
      return "Ok";
      break;
    case GET_NAME:
      return "Valentine";
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
String turkeyDayEffect(char mode, bool strip){
  static uint8_t gHue = 0;
  switch (mode){
    case RUN:
      {
        CRGBPalette16 palette = turkey_day_palette;
        uint8_t beat = beatsin8( animationSpeed / 3, 64, 255);
        for ( int i = 0; i < NUM_LEDS; i++) {
          leds[strip][i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
        }
        EVERY_N_MILLISECONDS(10) {
          gHue++;
        }
      }
      return "";  
      break;
    case INITIALIZE:
      gHue = 0;
      return "Ok";
      break;
    case GET_NAME:
      return "Turkey Day";
  }
}


String thanksgivingEffect(char mode, bool strip){
  static uint8_t pos = 0;
  static int timer = 0;
  static CRGBPalette16 ThxPalettestriped;
  switch (mode){
    case RUN:
      {
        uint8_t speed =  (256 - animationSpeed)/2;
        if (millis() > timer + speed) {
          pos += 2;
          timer = millis();
        }
        fill_palette( leds[strip], NUM_LEDS, pos, 16, ThxPalettestriped, 255, LINEARBLEND);
        return "";
        break;
      }
    case INITIALIZE:
      pos = 0;
        {CRGB A = CRGB::OrangeRed;
        CRGB AB = CRGB::Olive;
        CRGB B = CRGB::Maroon;
         ThxPalettestriped = CRGBPalette16(A, A, A, A, A, A, A, AB, AB, AB, B, B, B, B, B, B);}
      return "Ok";
      break;
    case GET_NAME:
      return "Thanksgiving";
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
String usaEffect(char mode, bool strip){
  static uint8_t gHue = 0;
  switch (mode){
    case RUN:
      {
        CRGBPalette16 palette = usa_palette;
        uint8_t beat = beatsin8( animationSpeed / 3, 64, 255);
        for ( int i = 0; i < NUM_LEDS; i++) {
          leds[strip][i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
        }
        EVERY_N_MILLISECONDS(10) {
          gHue++;
        }
      }
      return "";
      break;
    case INITIALIZE:
      gHue = 0;
      return "Ok";
      break;
    case GET_NAME:
    return "USA";
  }
}

String independenceEffect(char mode, bool strip){
  static uint8_t pos = 0;
  static int timer = 0;
  static CRGBPalette16 IndPalettestriped;
  switch (mode){
    case RUN:
      {
        uint8_t speed =  (256 - animationSpeed)/2;
        if (millis() > timer + speed) {
          pos += 2;
          timer = millis();
        }
        fill_palette( leds[strip], NUM_LEDS, pos, 16, IndPalettestriped, 255, LINEARBLEND);
      }
      return "";  
    case INITIALIZE:
      {
        CRGB A = CRGB::FireBrick;
        CRGB AB = CRGB::Cornsilk;
        CRGB B = CRGB::MediumBlue;
        IndPalettestriped = CRGBPalette16(A, A, A, A, A, AB, AB, AB, AB, AB, B, B, B, B, B, B);
      }
      pos = 0;
      return "Ok";
      break;
    case GET_NAME:
      return "Independence";
  }
}

//palette for Halloween
DEFINE_GRADIENT_PALETTE( halloween_palette ) {
  0, 208, 50,  1,
  127, 146, 27, 45,
  255,  97, 12, 178
};
String halloweenEffect(char mode, bool strip){
  static uint8_t gHue = 0;
  switch (mode){
    case RUN:
      {  
        CRGBPalette16 palette = halloween_palette;
        uint8_t beat = beatsin8( animationSpeed / 3, 64, 255);
        for ( int i = 0; i < NUM_LEDS; i++) {
          leds[strip][i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
        }
        EVERY_N_MILLISECONDS(10) {
          gHue++;
        }
      }
      return "";
    case INITIALIZE:
      gHue = 0;
      return "Ok";
      break;
    case GET_NAME:
      return "Halloween";
  }
}

//palette for Go Blue
DEFINE_GRADIENT_PALETTE( go_blue_palette ) {
  0,   4, 12, 122,
  127,  55, 58, 50,
  255, 192, 147, 11
};
String goBlueEffect(char mode, bool strip){
  static uint8_t gHue = 0;
  switch (mode){
    case RUN:
      {
        CRGBPalette16 palette = go_blue_palette;
        uint8_t beat = beatsin8( animationSpeed / 3, 64, 255);
        for ( int i = 0; i < NUM_LEDS; i++) { 
          leds[strip][i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
        }
        EVERY_N_MILLISECONDS(10) {
          gHue++;
        }
      }
      return "";
      break;
    case INITIALIZE:
      gHue = 0;
      return "Ok";
      break;
    case GET_NAME:
      return "Go Blue";
  }
}

String hailEffect(char mode, bool strip){
  static uint8_t pos = 0;
  static int timer = 0;
  static CRGBPalette16 hailPalettestriped;
  switch (mode){
    case RUN:
      {
        uint8_t speed =  (256 - animationSpeed)/2;
        if (millis() > timer + speed) {
          pos += 2;
          timer = millis();
        }
        fill_palette( leds[strip], NUM_LEDS, pos, 16, hailPalettestriped, 255, LINEARBLEND);
      }
      return "";  
    case INITIALIZE:
      {
        CRGB A = CRGB::Blue;
        CRGB B = CRGB::Yellow;
        hailPalettestriped = CRGBPalette16(A, A, A, A, A, A, A, A, B, B, B, B, B, B, B, B);
      }
      pos = 0;
      return "Ok";
      break;
    case GET_NAME:
      return "Hail";
  }
}

String touchdownEffect(char mode, bool strip){
  static int idex = 0;
  static int timer = 0;
  switch (mode){
    case RUN:
      {
        uint8_t speed =  (256 - animationSpeed)/2;
        if (millis() > timer + speed) {
          idex++;
          timer = millis();
        }
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
        leds[strip][idexY] = CHSV(64, 255, 255);
        leds[strip][idexB] = CHSV(160, 255, 255);
      }
      return "";
      break;
    case INITIALIZE:
      idex = 0;
      return "Ok";
      break;
    case GET_NAME:
      return "Touchdown";
  }
}

String punkinEffect(char mode, bool strip){
  static uint8_t pos = 0;
  static int timer = 0;
  static CRGBPalette16 punkinPalettestriped;
  switch (mode){
    case RUN:
      {
        uint8_t speed =  (256 - animationSpeed)/2;
        if (millis() > timer + speed) {
          pos += 2;
          timer = millis();
        }
        fill_palette( leds[strip], NUM_LEDS, pos, 16, punkinPalettestriped, 255, LINEARBLEND);
      }
      return "";
    case INITIALIZE:
      {
        CRGB A = CRGB::DarkOrange;
        CRGB B = CRGB::Indigo;
        punkinPalettestriped = CRGBPalette16(A, A, A, A, A, A, A, A, B, B, B, B, B, B, B, B);
      }
      pos = 0;
      return "Ok";
      break;
    case GET_NAME:
      return "Punkin";
  }
}

String loveyDayEffect(char mode, bool strip){
  static int idex = 0;
  static int timer = 0;
  switch (mode){
    case RUN:
      {
        uint8_t speed =  (256 - animationSpeed)/2;
        if (millis() > timer + speed) {
          idex++;
          timer = millis();
        }
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
        leds[strip][idexR] = CHSV(0, 255, 255);
        leds[strip][idexB] = CHSV(244, 255, 255);
      }
      return "";
    case INITIALIZE:
      idex = 0;
      return "Ok";
      break;
    case GET_NAME:
    return "Lovey Day";
  }
}

String hollyJollyEffect(char mode, bool strip){
  static int pos = 0;
  static int timer = 0;
  static CRGBPalette16 HJPalettestriped;
  switch (mode){
    case RUN:
      {
        uint8_t speed =  (256 - animationSpeed)/2;
        if (millis() > timer + speed) {
          pos++;
          timer = millis();
        }
        fill_palette( leds[strip], NUM_LEDS, pos, 16, HJPalettestriped, 255, LINEARBLEND);
      }
      return "";
      break;
    case INITIALIZE:
      {
        CRGB A = CRGB::Red;
        CRGB B = CRGB::Green;
        HJPalettestriped = CRGBPalette16(A, A, A, A, A, A, A, A, B, B, B, B, B, B, B, B);
      }
      pos = 0;
      return "Ok";
      break;
    case GET_NAME:
      return "Holly Jolly";
  }
}

String sinelonColorEffect(char mode, bool strip){
  switch (mode){
    case RUN:
      {
        int fadeSpeed = (animationSpeed + 10) / 2;
        fadeToBlackBy( leds[strip], NUM_LEDS, fadeSpeed);
        int pos = beatsin16((animationSpeed / 15)+1 , 0, NUM_LEDS - 1);
        leds[strip][pos] += CRGB(setRed, setGreen, setBlue);
      }
      return "";
    case INITIALIZE:
      return "Ok";
      break;
    case GET_NAME:
      return "Sinelon Color";
  }
}

String juggleColorEffect(char mode, bool strip){
  switch (mode){
    case RUN:
      {
        int fadeSpeed = (animationSpeed + 10) / 2;
        fadeToBlackBy( leds[strip], NUM_LEDS, fadeSpeed);
        byte dothue = 0;
        for ( int i = 0; i < 8; i++) {
          leds[strip][beatsin16(i +((animationSpeed / 15)+1), 0, NUM_LEDS - 1)] |= CRGB(setRed, setGreen, setBlue);
          dothue += 32;
        }
      }
      return "";
      break;
    case INITIALIZE:
      return "Ok";
      break;
    case GET_NAME:
      return "Juggle Color";
  }
}

String confettiColorEffect(char mode, bool strip){
  static int timer = 0;
  switch (mode){
    case RUN:
      {
        fadeToBlackBy( leds[strip], NUM_LEDS, 20);
        uint8_t speed =  (256 - animationSpeed);
        if (millis() > timer + speed) {
          int pos = random16(NUM_LEDS);
          leds[strip][pos] += CRGB(setRed + random8(64), setGreen, setBlue);
          timer = millis();
        }
      }
      return "";
      break;
    case INITIALIZE:
      return "Ok";
      break;
    case GET_NAME:
      return "Confetti Color";
  }
}

String rainbowEffect(char mode, bool strip){
  static uint8_t thishue = 0;
  static int timer = 0;
  switch (mode){
    case RUN:
      {
        uint8_t speed =  (256 - animationSpeed)/2;
        if (millis() > timer + speed) {
          thishue++;
          timer = millis();
        }
        fill_rainbow(leds[strip], NUM_LEDS, thishue, 10);
      }
      return "";
    case INITIALIZE:
      thishue = 0;
      return "Ok";
      break;
    case GET_NAME:
      return "Rainbow";
  }
}

String glitterRainbowEffect(char mode, bool strip){
  static uint8_t thishue = 0;
  static int timer = 0;
  switch (mode){
    case RUN:
      {
        uint8_t speed =  (256 - animationSpeed)/2;
        if (millis() > timer + speed) {
          thishue++;
          timer = millis();
        }
        fill_rainbow(leds[strip], NUM_LEDS, thishue, 10);
        //add glitter
        if ( random8() < 80) { //chance of glitter = 80
          leds[strip][ random16(NUM_LEDS) ] += CRGB::White;
        }
      }
      return "";
      break;
    case INITIALIZE:
      thishue = 0;
      return "Ok";
      break;
    case GET_NAME:
      return "Glitter Rainbow";
  }
}

String glitterColorEffect(char mode, bool strip){
  switch (mode){
    case RUN:
      {
        fadeToBlackBy( leds[strip], NUM_LEDS, 20);
        if ( random8() < (animationSpeed/2)+10) {
          leds[strip][ random16(NUM_LEDS) ] += CRGB(setRed, setGreen, setBlue);
        }
      }
      return "";
    case INITIALIZE:
      return "Ok";
      break;
    case GET_NAME:
      return "Glitter Color";
  }
}

String bpmEffect(char mode, bool strip){
  static uint8_t gHue = 0;
  switch (mode){
    case RUN:
      {
        CRGBPalette16 palette = PartyColors_p;
        uint8_t beat = beatsin8( animationSpeed / 3, 64, 255);
        for ( int i = 0; i < NUM_LEDS; i++) {
          leds[strip][i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
        }
        EVERY_N_MILLISECONDS(10) {
          gHue++;
        }
      }
      return "";
      break;
    case INITIALIZE:
      gHue = 0;
      return "Ok";
      break;
    case GET_NAME:
      return "BPM";
  }
}

String twinkleEffect(char mode, bool strip){
  static int timer = 0;
  switch (mode){
    case RUN:
      {
        const CRGB lightcolor(8, 7, 1);
        for ( int i = 0; i < NUM_LEDS; i++) {
          if ( !leds[strip][i]) continue; // skip black pixels
          if ( leds[strip][i].r & 1) { // is red odd?
            leds[strip][i] -= lightcolor; // darken if red is odd
          } else {
            leds[strip][i] += lightcolor; // brighten if red is even
          }
        }
        if (millis() > timer + 255 - animationSpeed){
            int j = random16(NUM_LEDS);
            if ( !leds[strip][j] ) leds[strip][j] = lightcolor;
          timer = millis();
        }
      }
      return "";
    case INITIALIZE:
      return "Ok";
      break;
    case GET_NAME:
      return "Twinkle";
  }
}

String lightningEffect(char mode, bool strip){
  static unsigned int dimmer;
  static uint16_t ledstart;
  static uint16_t ledlen;
  static int lightningFlashes = 0;
  static int lightningFlashCounter = 0;
  static int nextLightningFlashTime = 0;
  static int flashOffTime;
  static boolean flashOn = false;
  switch (mode){
    case RUN:
      {
        if (lightningFlashCounter >= lightningFlashes){
          lightningFlashes = random8(3, 8);
          lightningFlashCounter = 0;
          ledstart = random16(NUM_LEDS);           // Determine starting location of flash
          ledlen = random16(NUM_LEDS - ledstart);  // Determine length of flash (not to go beyond NUM_LEDS-1)
          nextLightningFlashTime = millis() + (random8(50) * 100) + (255-animationSpeed);        // delay between strikes
        }  else {
          if (millis() >= nextLightningFlashTime){
            if (lightningFlashCounter == 0) {
              dimmer = 5;
            } else {
              dimmer = random8(1,3);
            }
            if (!flashOn){
              fill_solid(leds[strip] + ledstart, ledlen, CHSV(255, 0, 255 / dimmer));
              flashOffTime = millis() + random(4,10);
              flashOn = true;
            } else {
              if (millis() >= flashOffTime) {
                fill_solid(leds[strip] + ledstart, ledlen, CHSV(255, 0, 0)); // Clear the section of LED's
                flashOn = false;
                if (lightningFlashCounter == 0){
                  nextLightningFlashTime = millis() + 200 + random8(100);   // longer delay until next flash after the leader
                } else {
                  nextLightningFlashTime = millis() + 50 + random8(100);             // shorter delay between strokes
                }
                lightningFlashCounter++;
              }
            }
          }
        }
      }
      return "";
    case INITIALIZE:
      lightningFlashes = 0;
      lightningFlashCounter = 0;
      return "Ok";
      break;
    case GET_NAME:
      return "Lightning";
  }
}


String policeOneEffect(char mode, bool strip){
  static int idex = 0;
  static int timer = 0;
  switch (mode){
    case RUN:
      {
        uint8_t speed =  (256 - animationSpeed)/2;
        if (millis() > timer + speed) {
          idex++;
          timer = millis();
        }
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
            leds[strip][i] = CHSV(0, 255, 255);
          }
          else if (i == idexB) {
            leds[strip][i] = CHSV(160, 255, 255);
          }
          else {
            leds[strip][i] = CHSV(0, 0, 0);
          }
        }
      }
      return "";
      break;
    case INITIALIZE:
      idex = 0;
      return "Ok";
      break;
    case GET_NAME:
      return "Police One";
  }
}

String policeAllEffect(char mode, bool strip){
  static int idex = 0;
  static int timer = 0;
  switch (mode){
    case RUN:
      {
        uint8_t speed =  (256 - animationSpeed)/2;
        if (millis() > timer + speed) {
          idex++;
          timer = millis();
        }
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
        leds[strip][idexR] = CHSV(0, 255, 255);
        leds[strip][idexB] = CHSV(160, 255, 255);
      }
      return "";
    case INITIALIZE:
      idex = 0;
      return "Ok";
      break;
    case GET_NAME:
      return "Police All";
  }
}

String candyCaneEffect(char mode, bool strip){
  static uint8_t pos = 0;
  static int timer = 0;
  static CRGBPalette16 candyCanePalette;
  switch (mode){
    case RUN:
      {
        uint8_t speed =  (256 - animationSpeed)/2;
        if (millis() > timer + speed) {
          pos += 2;
          timer = millis();
        }
        fill_palette( leds[strip], NUM_LEDS, pos, 16, candyCanePalette, 255, LINEARBLEND);
      }
      return "";
      break;
    case INITIALIZE:
      {
        CRGB A = CRGB::Red;
        CRGB B = CRGB::White;
        candyCanePalette = CRGBPalette16(A, A, A, A, A, A, A, A, B, B, B, B, B, B, B, B);
      }
      pos = 0;
      return "Ok";
      break;
    case GET_NAME:
      return "Candy Cane";
  }
}

String cyclonRainbowEffect(char mode, bool strip){
  static uint8_t cyclonHue = 0;
  static int timer = 0;
  static int cyclonPos = 0;
  switch (mode){
    case RUN:
      {
        static bool forwards = false;
        for(int a = 0; a < NUM_LEDS; a++) {
          leds[strip][a].nscale8(250);
        }
        uint8_t speed =  (256 - animationSpeed)/4;
        if (forwards) {
          if (cyclonPos < NUM_LEDS - 1){
            leds[strip][cyclonPos] = CHSV(cyclonHue++, 255,255);
            if (millis() > timer + speed) {
              cyclonPos++;
              timer = millis();
            }
          } else {
            forwards = false;
          }
        } else {
          if (cyclonPos >= 0) {
            leds[strip][cyclonPos] = CHSV(cyclonHue++, 255,255);
            if (millis() > timer + speed) {
              cyclonPos--;
              timer = millis();
            }
          } else {
            forwards = true;
          }
        }
      }
      return "";
      break;
    case INITIALIZE:
      cyclonHue = 0;
      cyclonPos = 0;
      return "Ok";
      break;
    case GET_NAME:
      return "Cyclon Rainbow";
  }
}

String fireEffect(char mode, bool strip){
  static byte heat[NUM_LEDS];
  static int delayStartMillis = 0;
  static int speedMillis = 0;
  switch (mode){
    case RUN:
      {
        if (millis() > delayStartMillis) {
          if (millis() >= speedMillis + 85-(animationSpeed/3)){
            CRGBPalette16 gPal = HeatColors_p; //FastLED built in palette
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
              leds[strip][j] = color;
            }
            speedMillis = millis();
          }
        }
      }
      return "";
      break;
    case INITIALIZE:
      delayStartMillis = millis() + 700;
      for ( int i = 0; i < NUM_LEDS; i++) {
        heat[i] = 0;
      }
      return "Ok";
      break;
    case GET_NAME:
      return "Fire";
  }
}

String easterEffect(char mode, bool strip){
  const uint16_t scale = 30;          // Wouldn't recommend changing this on the fly, or the animation will be really blocky.
  static uint16_t dist;         // A random number for our noise generator.
  static CRGBPalette16 targetPalette(OceanColors_p);
  static CRGBPalette16 currentPalette(CRGB::Black);
  static int timer = 0;
  static int paletteTimer = 0;
  switch (mode){
    case RUN:
      {
        if (millis() >= timer){
          nblendPaletteTowardPalette(currentPalette, targetPalette, 48);  // FOR NOISE ANIMATION
          for (int i = 0; i < NUM_LEDS; i++) {                                     // Just ONE loop to fill up the LED array as all of the pixels change.
            uint8_t index = inoise8(i * scale, dist + i * scale) % 255;            // Get a value from the noise function. I'm using both x and y axis.
            leds[strip][i] = ColorFromPalette(currentPalette, index, 255, LINEARBLEND);   // With that value, look up the 8 bit colour palette value and assign it to the current LED.
          }
          dist += beatsin8(10, 1, 4);
          timer = millis() + ((256-animationSpeed)/5);
        }
        if (millis() >= paletteTimer){
          targetPalette = CRGBPalette16(CHSV(random8(), 255, random8(128, 255)), CHSV(random8(), 255, random8(128, 255)), CHSV(random8(), 192, random8(128, 255)), CHSV(random8(), 255, random8(128, 255)));
          paletteTimer = millis() + (((256-animationSpeed)/15)*1000);
        }
      }
      return "";
      break;
    case INITIALIZE:
      timer = 0;
      paletteTimer = 0;
      return "Ok";
      break;
    case GET_NAME:
      return "Easter";
  }
}


String rippleEffect(char mode, bool strip){
  static uint8_t colour;     // Ripple colour is randomized.
  static int center;         // Center of the current ripple.
  static int step;                  // -1 is the initializing step.
  static uint8_t bgcol;             // Background colour rotates.
  static int timer = 0;
  switch (mode){
    case RUN:
      {
        if (millis() >= timer) {
          for (int i = 0; i < NUM_LEDS; i++) {
            leds[strip][i] = CHSV(bgcol++, 255, 15);  // Rotate background colour.
          }
          switch (step) {
            case -1:                                                          // Initialize ripple variables.
              center = random(NUM_LEDS);
              colour = random8();
              step = 0;
              break;
            case 0:
              leds[strip][center] = CHSV(colour, 255, 255);                        // Display the first pixel of the ripple.
              step ++;
              break;
            case 16:                                                   // At the end of the ripples.
              step = -1;
              break;
            default:                                                             // Middle of the ripples.
              leds[strip][(center + step + NUM_LEDS) % NUM_LEDS] += CHSV(colour, 255, 255 / step * 2);   // Simple wrap from Marc Miller
              leds[strip][(center - step + NUM_LEDS) % NUM_LEDS] += CHSV(colour, 255, 255 / step * 2);
              step ++;                                                         // Next step.
              break;
          }
          timer = millis() + ((256-animationSpeed)/5);
        }
      }
      return "";
    break;
    case INITIALIZE:
      timer = 0;
      step = -1;
      bgcol = 0;
      return "Ok";
      break;
    case GET_NAME:
      return "Ripple";
  }
}

String dotsEffect(char mode, bool strip){
  switch (mode){
    case RUN:
      {
        uint8_t bpm = (animationSpeed / 3) + 1;
        uint8_t fadeval = 224;                 // Trail behind the LED's. Lower => faster fade.
        uint16_t inner = beatsin16(bpm, NUM_LEDS / 4, NUM_LEDS / 4 * 3);
        uint16_t outer = beatsin16(bpm, 0, NUM_LEDS - 1);
        uint16_t middle = beatsin16(bpm, NUM_LEDS / 3, NUM_LEDS / 3 * 2);
        leds[strip][middle] = CRGB::Purple;
        leds[strip][inner] = CRGB::Blue;
        leds[strip][outer] = CRGB::Aqua;
        nscale8(leds[strip], NUM_LEDS, fadeval);
      }
      return "";
      break;
    case INITIALIZE:
      break;
    case GET_NAME:
      return "Dots";
  }
}

// String myNewEffect(char mode, bool strip){
//   // declare any static variables needed here!
//   switch (mode){
//     case RUN:
//       {
//         // do some things here to set the LEDs to display 
//         // one animation frame the led strip is:
//         // leds[strip]
//         // to address an individual led, use:
//         // leds[strip][LED NUMBER]
//         //
//         // speed can be accessed from the variable:
//         // animationSpeed       (0 slowest - 255 fastest)
//         // N.B. Effect speed should be controlled by the code HERE!
//         // but animationSpeed MUST NOT be written to!
//         //
//         // The selected color is stored in:
//         // setRed
//         // setGreen
//         // setBlue
//         // these MUST NOT be written to!
//         //
//         //
//         // DO NOT USE delay
//         // timings can be accomplished using millis()
//       }
//       return "";  // IF the effect code controls brightness return "BR"
//       break;
//     case INITIALIZE:
//         // Initialize any variables to start the effect here.
//       return "Ok";
//       break;
//     case GET_NAME:
//       return "My Awesome Effect";  // return your effect's name here
//   }
// }