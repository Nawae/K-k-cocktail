
// Code source 5110 tiré de http://devjoe.com/2012/12/26/playing-video-and-other-fun-things-on-the-nokia-3310-lcd-with-arduino-2/
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

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

void setup()   {
  //5110
  display.begin();
  // init done
  display.setContrast(55); //Ajustement du contrast
  display.setTextSize(1); //Ajustement de la taille du texte
  display.setTextWrap(false); // Enlever le retour à la ligne
  display.clearDisplay(); //Nettoyage de l'écran
  text = "Servez-vous !"; //Texte à afficher
  textWidth = text.length()*6+85; // Récupérer la taille du texte pour le for global
}

void loop() {
  for(int i=0; i<textWidth; i+=1) { //Pour la taille du texte
    display.clearDisplay();
    display.setTextSize(1.5);
    display.setCursor(3, 0); //Positionnement du curseur au début
    display.print("KekeCocktails"); // Titre du haut
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
    delay(40); //Permet de ralentir le défilement du texte
  }
}
