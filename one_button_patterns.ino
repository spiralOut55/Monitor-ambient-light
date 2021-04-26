#include <FastLED.h>
#include <OneButton.h>

#define NUM_LEDS  80
#define LED_PIN   6
#define BTN_PIN   9       
#define POT_PIN   A0
 
CRGB leds[NUM_LEDS];

uint8_t patternCounter = 0;

// Push button connected between pin 7 and GND (no resistor required)
OneButton btn = OneButton(BTN_PIN, true, true);





CRGBPalette16 myPal = CRGBPalette16(
     CRGB::DarkViolet,
    CRGB::DarkViolet,
    CRGB::DarkViolet,
    CRGB::DarkViolet,
    
    CRGB::Magenta,
    CRGB::Magenta,
    CRGB::Linen,
    CRGB::Linen,
    
    CRGB::Magenta,
    CRGB::Magenta,
    CRGB::DarkViolet,
    CRGB::DarkViolet,

    CRGB::DarkViolet,
    CRGB::DarkViolet,
    CRGB::Linen,
    CRGB::Linen
    );

uint8_t paletteIndex = 0;

int whiteness = 0;
uint8_t hueIndex = 0;




void setup() {
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(50);
  Serial.begin(57600);

  btn.attachClick(nextPattern);
}

void loop() {

  int potRead = analogRead(POT_PIN);
  int brightness = map(potRead, 0, 1023, 0, 240);
  FastLED.setBrightness(brightness);

  
  switch (patternCounter) {
    case 0:
      
      
      EVERY_N_MILLISECONDS(30){
      if(whiteness < 255){
      whiteness= whiteness + 3;
      }
      }
      
      fill_solid(leds, NUM_LEDS, CRGB(whiteness, whiteness, whiteness));
      break;
    case 1:
      fill_solid(leds, NUM_LEDS, CRGB::DarkSlateBlue);
      break;
    case 2:
      fill_solid(leds, NUM_LEDS, CRGB::Gold);
      break;
    case 3:
      fill_palette(leds, NUM_LEDS, paletteIndex, 255 / NUM_LEDS, myPal, 255, LINEARBLEND);
      EVERY_N_MILLISECONDS(20){
        paletteIndex++;
      }
      break;
    case 4:
      EVERY_N_MILLISECONDS(800){
        leds[random8(0, NUM_LEDS - 1)] = ColorFromPalette(myPal, random8(), 255, LINEARBLEND);
      }

      fadeToBlackBy(leds, NUM_LEDS, 1);
      break;
    case 5:
      fill_solid(leds, NUM_LEDS, CRGB::Red);
      break;
    case 6:
      fill_rainbow2(leds,NUM_LEDS,hueIndex,255/NUM_LEDS);
      EVERY_N_MILLISECONDS(45
      ){
      hueIndex = hueIndex+1;
      }
      break;
     
  }
  
  FastLED.show();
  btn.tick();
}

void nextPattern() {
  patternCounter = (patternCounter + 1) % 7;           // Change the number after the % to the number of patterns you have
}

//------- Put your patterns below -------//

int pixelShift(int wantedPixel){
  int actualPixel;
  if(wantedPixel >= 13 && wantedPixel <= 79){
    actualPixel = wantedPixel - 13;
  }
  if(wantedPixel >= 0 && wantedPixel <= 12){
    actualPixel = wantedPixel + 67;
  }
  return actualPixel;
}

void movingDots() {
  
  uint16_t posBeat  = beatsin16(30, 0, NUM_LEDS - 1, 0, 0);
  uint16_t posBeat2 = beatsin16(60, 0, NUM_LEDS - 1, 0, 0);

  uint16_t posBeat3 = beatsin16(30, 0, NUM_LEDS - 1, 0, 32767);
  uint16_t posBeat4 = beatsin16(60, 0, NUM_LEDS - 1, 0, 32767);

  // Wave for LED color
  uint8_t colBeat  = beatsin8(45, 0, 255, 0, 0);

  leds[(posBeat + posBeat2) / 2]  = CHSV(colBeat, 255, 255);
  leds[(posBeat3 + posBeat4) / 2]  = CHSV(colBeat, 255, 255);

  fadeToBlackBy(leds, NUM_LEDS, 10);
}


void rainbowBeat() {
  
  uint16_t beatA = beatsin16(30, 0, 255);
  uint16_t beatB = beatsin16(20, 0, 255);
  fill_rainbow(leds, NUM_LEDS, (beatA+beatB)/2, 8);
}


void redWhiteBlue() {

  uint16_t sinBeat   = beatsin16(30, 0, NUM_LEDS - 1, 0, 0);
  

  leds[sinBeat]   = CRGB::Red;
 
  
  fadeToBlackBy(leds, NUM_LEDS, 10);
}


void fill_rainbow2( struct CRGB * pFirstLED, int numToFill,
                  uint8_t initialhue,
                  uint8_t deltahue )
{
    CHSV hsv;
    hsv.hue = initialhue;
    hsv.val = 255;
    hsv.sat = 255;
    for( int i = 0; i < numToFill; ++i) {
        pFirstLED[i] = hsv;
        hsv.hue += deltahue;
    }
}
