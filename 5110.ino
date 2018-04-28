
// Code source 5110 tiré de http://devjoe.com/2012/12/26/playing-video-and-other-fun-things-on-the-nokia-3310-lcd-with-arduino-2/
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <Adafruit_NeoPixel.h> // Ring NeoPixe

// CONFIGURATION 5110
// Software SPI (slower updates, more flexible pin options):
// pin 7 - Serial clock out (SCLK)
// pin 6 - Serial data out (DIN)
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin 3 - LCD reset (RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(8, 9, 10, 11, 12);
String text;
int textWidth;
float count = 1.0;
boolean toggle = false;

//CONFIGURATION NEOPIXEL
// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(12, 19, NEO_GRB + NEO_KHZ800);


void setup()   {
  //5110
  //Serial.begin(9600);
  display.begin();
  // init done
  display.setContrast(55); //Ajustement du contrast
  display.setTextSize(1); //Ajustement de la taille du texte
  display.setTextWrap(false); // Enlever le retour à la ligne
  display.clearDisplay(); //Nettoyage de l'écran
  text = "Servez-vous !"; //Texte à afficher
  textWidth = text.length()*6+85; // Récupérer la taille du texte pour le for global

  //NeoPixel
  strip.begin();
  colorWipe(strip.Color(0, 0, 255), 50); // Blue
}


void loop() {
  for(int i=0; i<textWidth; i+=1) { //Pour la taille du texte
    display.clearDisplay();
    display.setTextSize(1.5);
    display.setCursor(0, 0); //Positionnement du curseur au début
    display.print("KekeCocktails!"); // Titre du haut
    display.drawLine(0, 10, display.width(), 10, BLACK); //Ligne séparatrice
    count += 0.2;

    // Boucle sur le texte diflant en forme de vague
    for(int j=0; j<text.length(); j++) {
      display.drawChar((j*6)+85-i, map(sin(count+(float)(j/1.5))*100, -100, 100, 14, 19), text[j], BLACK, WHITE, 1);
    }

    // Dessin tournant
    int sine = sin((float)count/4)*100;
    sine = map(sine, -102, 102, 0, display.width());
    display.drawLine(display.width()-sine, display.height(), sine, 31, BLACK);
    display.drawLine(sine, display.height(), display.width()-sine, 31, BLACK);
    display.drawLine(sine, 31, sine, display.height(), BLACK);
    display.drawLine(display.width()-sine, 31, display.width()-sine, display.height(), BLACK);
    display.display();
    delay(15); //Permet de ralentir le défilement du texte
    
  }
  colorWipe2(strip.Color(0, 0, 255),200); // Blue
  colorWipe(strip.Color(0, 0, 255), 50); // Blue

  // Some example procedures showing how to display to the pixels:
  //colorWipe(strip.Color(255, 0, 0), 50); // Red
  //colorWipe(strip.Color(0, 255, 0), 50); // Green
  //colorWipe(strip.Color(0, 0, 255), 50); // Blue
  //colorWipe2(strip.Color(0, 0, 255), 50); // Blue
  //colorWipe(strip.Color(0, 0, 0, 255), 50); // White RGBW
  // Send a theater pixel chase in...
  //theaterChase(strip.Color(127, 127, 127), 50); // White
  //theaterChase(strip.Color(127, 0, 0), 50); // Red
  //theaterChase(strip.Color(0, 0, 127), 50); // Blue

  //rainbow(20);
  //rainbowCycle(20);
  //theaterChaseRainbow(50);

  
}


//FONCTION NEOPIXEL

// Fill the dots one after the other with a color
void colorWipe2(uint32_t c, int wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, 0);
    strip.show();
    delay(wait);
    delay(wait);
  }
}
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
