//NAWAE 2018/05/13
//Dispositif de création de Cocktails
//Premier programme Arduino / Premier Projet

//CHARGEMENT DES LIBRAIRIES
#include <LiquidCrystal.h> // Chargement de la librairie pour l'écran LCD
#include <pitches.h> // Chargement de la librairie pour la musique
#include <Adafruit_NeoPixel.h> // Chargement de la librairie pour Ring NeoPixe

//CONFIGURATION LCD
//LiquidCrystal nomDeVariable(RS, E, d4, d5, d6, d7);
LiquidCrystal monEcran(54,55,56,57,58,59); // Configuration de l'écran

//CONFIGURATION MUSIQUE
const int SPEAKER = 24;
#define SILENCE  0
#define EIGHTH   1
#define QUARTER  2
#define HALF 3

//CONFIGURATION DE L'ANNEAU DE LEDS
// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(12, 22, NEO_GRB + NEO_KHZ800);

//CONFIGURATION COCKTAILS & BOUTONS
const int bout1 = 9; // Constante du pin bouton HAUT
const int bout2 = 10; // Constante du pin bouton BAS
const int bout3 = 11; // Constante du pin bouton VALIDATION
const int boutinitialisation = 13; // Constante du pin bouton pour charger les tubes
const int boutpurge = 12; // Constante du pin bouton pour vider les tubes
int posMenu = 0; // Variable de position dans le menu
int choix = 0; // Variable pour stocker le choix
int nombreDeRecette = 12; // Permet le modula pour revenir à 1 en cycle navigation
String ligne1 = "CHOIX COCKTAIL :"; // Affichage de la première ligne
String ligne2[12] = {"1-Ti Rozenn     ", "2-Gin Tonic'    ", "3-Planteur      ", "4-Le Rio (SA)      ","5-Gimlet       ","6-Le Bali    ","7-L'inverse      ","8-Le Nathael      ","9-Flora Dora     ","10-Gin Fizz       ","11-Bacardi Cocktail      ","12-Le Tonic' (SA)          "}; // Tableau de recette
const int orange = 1; // Pin rattaché à la board relais, auquel est relié l'alimentation du moteur contrôlant la pompe de jus d'Orange.
const int schwepps = 2; // Pin pour la board relais
const int grenadine = 3; // Pin pour la board relais
const int ananas = 4; // Pin pour la board relais
const int citron = 5; // Pin pour la board relais
const int rhum = 6; // Pin pour la board relais
const int gin = 7; // Pin pour la board relais
const int canadou = 8; // Pin pour la board relais
float temps; // Variable qui stocke la mesure du temps
float avancement; // Variable pour la gestion de la barre de progression dynamique

void setup() {

  // Configuration pour la board relais
  pinMode(1, OUTPUT); // Déclaration des pins de pompe comme sortie pour la board relais
  pinMode(2, OUTPUT); // Déclaration des pins de pompe comme sortie pour la board relais
  pinMode(3, OUTPUT); // Déclaration des pins de pompe comme sortie pour la board relais
  pinMode(4, OUTPUT); // Déclaration des pins de pompe comme sortie pour la board relais
  pinMode(5, OUTPUT); // Déclaration des pins de pompe comme sortie pour la board relais
  pinMode(6, OUTPUT); // Déclaration des pins de pompe comme sortie pour la board relais
  pinMode(7, OUTPUT); // Déclaration des pins de pompe comme sortie pour la board relais
  pinMode(8, OUTPUT); // Déclaration des pins de pompe comme sortie pour la board relais

  for(int i = 1; i <= 8; i++) {
    // Pour chaque Pin du relais, on les désactive tout de suite. Sécurité
    digitalWrite(i,HIGH);
  }

  // Configuration de l'écran
  monEcran.begin(16,2); // On initialise la communication LCD avec 16 colonnes et deux lignes
  setup_progressbar(); // On charge les caractères pour la barre de progression
  monEcran.clear(); // On efface l'écran
  monEcran.setCursor(2,0); // On centre
  monEcran.print("DEMARRAGE EN"); 
  monEcran.setCursor(5,1); // On passe à la ligne suivante
  monEcran.print("COURS"); 
  //delay(2000); // Permet de voir le rechargement du programme
  //monEcran.clear(); // On efface l'écran

  // Configuration des boutons
  pinMode(bout1, INPUT); // Déclaration du bouton comme entrée
  pinMode(bout2, INPUT); // Déclaration du bouton comme entrée
  pinMode(bout3, INPUT); // Déclaration du bouton comme entrée
  pinMode(boutinitialisation, INPUT); // Déclaration du bouton comme entrée
  pinMode(boutpurge, INPUT); // Déclaration du bouton comme entrée

  //Configuration du haut-parleur
  pinMode(SPEAKER, OUTPUT); // Configuration du pin speaker comme sortie.

  // Configuration de l'anneau de leds
  pinMode(22, OUTPUT); // Configuration du pin 22 comme sortie pour les leds ring.
  strip.begin();
  colorLED(strip.Color(0, 250, 0), 200); // Simulation de Chargement
  colorLED(strip.Color(0, 250, 0), 200); // Simulation de Chargement
  colorLED(strip.Color(0, 0, 0), 1); // Simulation de Chargement
  //colorWipe(strip.Color(0, 250, 0), 100); // Vert 
  //colorWipe(strip.Color(0, 0, 0), 100); // Eteint
  //colorWipe(strip.Color(250, 0, 0), 100); // Rouge

    

}

void loop() {
  int i;
  int j;
  j=0; // Initialisation de la variable permettant de gérer 255 couleurs. 
  do // Boucle do permettant de requeter les fonctions de navigations à chaque boucle de couleurs. Permettant un refresh fréquent.
  {
  
  navigation(); // Appel de la fonction navigation en boucle pour capter les boutons
  affichage(); // Appel de la fonction affichage en boucle pour rafraichir l'écran en fonction des boutons (posmenu)

    for(i=0; i<strip.numPixels(); i++) { // Pour chaque led du Ring, 
      strip.setPixelColor(i, Wheel((i+j) & 255)); // On attribue une couleur
    }
    strip.show(); // On lance la mise à jour des couleurs
    delay(50); // On attend 50 millisec avant de rechanger

  j++; // On incrémente le compteur, et on passe à une autre couleur.
  } while (j<256);
}

//FONCTION NAVIGATION 
void navigation() {
  boolean etatBout1 = digitalRead(bout1); // Déclaration d'un booléen pour savoir si le bouton est appuyé
  boolean etatBout2 = digitalRead(bout2); // Déclaration d'un booléen pour savoir si le bouton est appuyé
  boolean etatBout3 = digitalRead(bout3); // Déclaration d'un booléen pour savoir si le bouton est appuyé
  boolean etatBoutInit = digitalRead(boutinitialisation); // Déclaration d'un booléen pour savoir si le bouton est appuyé
  boolean etatBoutPurge = digitalRead(boutpurge); // Déclaration d'un booléen pour savoir si le bouton est appuyé
  //Boucle pour naviguer dans les menus
  //if (etatBout1 || etatBout2 || etatBout3) { // Boucle pour les boutons avec delay
  if (etatBout1 || etatBout2 || etatBout3 || etatBoutInit || etatBoutPurge) { // Boucle pour les boutons avec delay
    if (etatBout1) { // Si le bouton 1 (HAUT) est appuyé
      posMenu = (posMenu + 1) % nombreDeRecette; // On avance la position de lecture, On avance dans le tableau
    }
    if (etatBout2) { // Si le bouton 2 (BAS) est appuyé
      if (posMenu == 0) { // Si on était sur la position 0, et pour éviter la position -1 (bug), On fixe à la dernière valeur
        posMenu = (nombreDeRecette-1) % nombreDeRecette; // On évite le chiffre négatif en retournant à la fin du tableau.
      }
      else {
        posMenu = (posMenu - 1) % nombreDeRecette; // On recule dans le tableau
      }
    }
    if (etatBout3) { // Si on valide la sélection, lancement de la distribution
      choix = posMenu +1; // Récupération du numéro de la recette (uniquement pour une question d'affichage)
      distribution(); // Lancement de la distribution
      monEcran.clear(); // On efface l'écran
      posMenu = 0; // On réinitialise l'affichage
    }
    if (etatBoutInit) { // Si le bouton Initialisation est appuyé
      initialisation(); //On lance la fonction qui charge les tuyaux
    }
    if (etatBoutPurge) { // Si le bouton Purge est appuyé
      purge(); //On lance la fonction qui vide les tuyaux
    }
  delay(500); //attente pour éviter les répétitions
  } 
}
//FONCTION AFFICHAGE
void affichage() {
  monEcran.setCursor(0, 0); // On initialise le curseur en haut
  monEcran.print(ligne1); // On affiche le titre
  monEcran.setCursor(0, 1); // On postionne le curseur sur la deuxième ligne
  monEcran.print(ligne2[posMenu]); // On affiche la recette
}

//FONCTION DISTRIBUTION DU COCKTAIL
void distribution() {
  monEcran.clear(); // On efface l'écran
  monEcran.setCursor(0, 0); // On initialise le curseur en haut
  monEcran.print("DISTRIBUTION"); // On affiche le titre
  monEcran.setCursor(0, 1); // On postionne le curseur sur la deuxième ligne
  monEcran.print("RECETTE "); // On affiche la recette
  monEcran.setCursor(8, 1); // On postionne le curseur sur la deuxième ligne au 2ème pixel
  monEcran.print(choix); // On affiche la recette
  delay (1000);
  monEcran.setCursor(0, 0); // On initialise le curseur en haut
  monEcran.print("DISTRIBUTION ..."); // On affiche le titre
  //colorWipe(strip.Color(253, 108, 158), 0); // On met en rose le ring
  temps = millis(); // On sauvegarde l'heure actuel
  int i; // Variable pour le rainbow maison
  int j; // Variable pour le rainbow maison
  j=0; // Initialisation de la variable pour les 255 couleurs
      
  switch (posMenu) { // En fonction de la position dans le menu lors de la selection, on charge les pompes
     case 0: // Ti Punch
      //On allume toutes les pomptes du cocktails
      digitalWrite(rhum, LOW);
      digitalWrite(citron, LOW);
      digitalWrite(canadou, LOW);
      do // On lance une boucle qui ne s'arrêtera qu'à la fin du liquide le plus présent.
      {
       avancement = ((millis() - temps)/conv(3)*100);
       draw_progressbar(avancement);
       pompeV2(canadou,1); //  Si la Pompe est activée, alors on regarde si elle a pu délivrer x Cl ( x CL = X * 6000 millisecondes). Si c'est le cas, alors on coupe la pompe, sinon on laisse allumer.
       pompeV2(citron,2);
       pompeV2(rhum,3);
      // Début du rainbow maison
      for(i=0; i<strip.numPixels(); i++) { // Pour chaque led du Ring, 
      strip.setPixelColor(i, Wheel((i+j) & 255)); // On attribue une couleur
      }
      strip.show(); // On lance la mise à jour des couleurs
      j++;
      if (j==255){
        j=0;
      }
      // Fin du rainbow maison   
      } while ((millis() - temps) < conv(3)); // Tant qu'on ne dépasse pas le temps nécessaire au plus grand liquide (ici rhum)
      break;
     case 1: // Gin Tonic'
      //On allume toutes les pomptes du cocktails
      digitalWrite(schwepps, LOW);
      digitalWrite(gin, LOW);
      do // On lance une boucle qui ne s'arrêtera qu'à la fin du liquide le plus présent.
      {
       avancement = ((millis() - temps)/conv(15)*100);
       draw_progressbar(avancement);
       pompeV2(schwepps,15); //  Si la Pompe est activée, alors on regarde si elle a pu délivrer x Cl ( x CL = X * 6000 millisecondes). Si c'est le cas, alors on coupe la pompe, sinon on laisse allumer.
       pompeV2(gin,3);
      // Début du rainbow maison
      for(i=0; i<strip.numPixels(); i++) { // Pour chaque led du Ring, 
      strip.setPixelColor(i, Wheel((i+j) & 255)); // On attribue une couleur
      }
      strip.show(); // On lance la mise à jour des couleurs
      j++;
      if (j==255){
        j=0;
      }
      // Fin du rainbow maison   
      } while ((millis() - temps) < conv(15)); // Tant qu'on ne dépasse pas le temps nécessaire au plus grand liquide
      break;
     case 2: // Planteur
      //On allume toutes les pomptes du cocktails
      digitalWrite(orange, LOW);
      digitalWrite(grenadine, LOW);
      digitalWrite(ananas, LOW);
      digitalWrite(citron, LOW);
      digitalWrite(rhum, LOW);
      digitalWrite(canadou, LOW);
      do // On lance une boucle qui ne s'arrêtera qu'à la fin du liquide le plus présent.
      {
       avancement = ((millis() - temps)/conv(6)*100);
       draw_progressbar(avancement);
       pompeV2(orange,6); //  Si la Pompe est activée, alors on regarde si elle a pu délivrer x Cl ( x CL = X * 6000 millisecondes). Si c'est le cas, alors on coupe la pompe, sinon on laisse allumer.
       pompeV2(grenadine,1);
       pompeV2(ananas,6);
       pompeV2(citron,2);
       pompeV2(rhum,6);
       pompeV2(canadou,1);
      // Début du rainbow maison
      for(i=0; i<strip.numPixels(); i++) { // Pour chaque led du Ring, 
      strip.setPixelColor(i, Wheel((i+j) & 255)); // On attribue une couleur
      }
      strip.show(); // On lance la mise à jour des couleurs
      j++;
      if (j==255){
        j=0;
      }
      // Fin du rainbow maison   
      } while ((millis() - temps) < conv(6)); // Tant qu'on ne dépasse pas le temps nécessaire au plus grand liquide
      break;
     case 3: // Loeiza SA
      //On allume toutes les pomptes du cocktails
      digitalWrite(orange, LOW);
      digitalWrite(grenadine, LOW);
      digitalWrite(ananas, LOW);
      digitalWrite(citron, LOW);
      do // On lance une boucle qui ne s'arrêtera qu'à la fin du liquide le plus présent.
      {
       avancement = ((millis() - temps)/conv(8)*100);
       draw_progressbar(avancement);
       pompeV2(orange,8); //  Si la Pompe est activée, alors on regarde si elle a pu délivrer x Cl ( x CL = X * 6000 millisecondes). Si c'est le cas, alors on coupe la pompe, sinon on laisse allumer.
       pompeV2(grenadine,3);
       pompeV2(ananas,6);
       pompeV2(citron,2);
      // Début du rainbow maison
      for(i=0; i<strip.numPixels(); i++) { // Pour chaque led du Ring, 
      strip.setPixelColor(i, Wheel((i+j) & 255)); // On attribue une couleur
      }
      strip.show(); // On lance la mise à jour des couleurs
      j++;
      if (j==255){
        j=0;
      }
      // Fin du rainbow maison   
      } while ((millis() - temps) < conv(8)); // Tant qu'on ne dépasse pas le temps nécessaire au plus grand liquide
      break;
     case 4: // Gimlet
      //On allume toutes les pomptes du cocktails
      digitalWrite(citron, LOW);
      digitalWrite(gin, LOW);
      digitalWrite(canadou, LOW);
      do // On lance une boucle qui ne s'arrêtera qu'à la fin du liquide le plus présent.
      {
       avancement = ((millis() - temps)/conv(4)*100);
       draw_progressbar(avancement);
       pompeV2(citron,2); //  Si la Pompe est activée, alors on regarde si elle a pu délivrer x Cl ( x CL = X * 6000 millisecondes). Si c'est le cas, alors on coupe la pompe, sinon on laisse allumer.
       pompeV2(gin,4);
       pompeV2(canadou,2);
      // Début du rainbow maison
      for(i=0; i<strip.numPixels(); i++) { // Pour chaque led du Ring, 
      strip.setPixelColor(i, Wheel((i+j) & 255)); // On attribue une couleur
      }
      strip.show(); // On lance la mise à jour des couleurs
      j++;
      if (j==255){
        j=0;
      }
      // Fin du rainbow maison   
      } while ((millis() - temps) < conv(4)); // Tant qu'on ne dépasse pas le temps nécessaire au plus grand liquide
      break;
     case 5: // Le Bali
       //On allume toutes les pomptes du cocktails
      digitalWrite(orange, LOW);
      digitalWrite(schwepps, LOW);
      digitalWrite(ananas, LOW);
      digitalWrite(citron, LOW);
      digitalWrite(rhum, LOW);
      digitalWrite(canadou, LOW);
      do // On lance une boucle qui ne s'arrêtera qu'à la fin du liquide le plus présent.
      {
       avancement = ((millis() - temps)/conv(8)*100);
       draw_progressbar(avancement);
       pompeV2(orange,4); //  Si la Pompe est activée, alors on regarde si elle a pu délivrer x Cl ( x CL = X * 6000 millisecondes). Si c'est le cas, alors on coupe la pompe, sinon on laisse allumer.
       pompeV2(schwepps,8);
       pompeV2(ananas,4);
       pompeV2(citron,4);
       pompeV2(rhum,4);
       pompeV2(canadou,1);
      // Début du rainbow maison
      for(i=0; i<strip.numPixels(); i++) { // Pour chaque led du Ring, 
      strip.setPixelColor(i, Wheel((i+j) & 255)); // On attribue une couleur
      }
      strip.show(); // On lance la mise à jour des couleurs
      j++;
      if (j==255){
        j=0;
      }
      // Fin du rainbow maison   
      } while ((millis() - temps) < conv(8)); // Tant qu'on ne dépasse pas le temps nécessaire au plus grand liquide
      break;
     case 6: // L'inversé
      //On allume toutes les pomptes du cocktails
      digitalWrite(orange, LOW);
      digitalWrite(grenadine, LOW);
      digitalWrite(ananas, LOW);
      digitalWrite(citron, LOW);
      digitalWrite(rhum, LOW);
      digitalWrite(canadou, LOW);
      do // On lance une boucle qui ne s'arrêtera qu'à la fin du liquide le plus présent.
      {
       avancement = ((millis() - temps)/conv(6)*100);
       draw_progressbar(avancement);
       pompeV2(orange,1); //  Si la Pompe est activée, alors on regarde si elle a pu délivrer x Cl ( x CL = X * 6000 millisecondes). Si c'est le cas, alors on coupe la pompe, sinon on laisse allumer.
       pompeV2(grenadine,6);
       pompeV2(ananas,6);
       pompeV2(citron,2);
       pompeV2(rhum,6);
       pompeV2(canadou,1);
      // Début du rainbow maison
      for(i=0; i<strip.numPixels(); i++) { // Pour chaque led du Ring, 
      strip.setPixelColor(i, Wheel((i+j) & 255)); // On attribue une couleur
      }
      strip.show(); // On lance la mise à jour des couleurs
      j++;
      if (j==255){
        j=0;
      }
      // Fin du rainbow maison   
      } while ((millis() - temps) < conv(6)); // Tant qu'on ne dépasse pas le temps nécessaire au plus grand liquide
      break;
     case 7: // Le Léo
      //On allume toutes les pomptes du cocktails
      digitalWrite(grenadine, LOW);
      digitalWrite(ananas, LOW);
      digitalWrite(citron, LOW);
      digitalWrite(gin, LOW);
      do // On lance une boucle qui ne s'arrêtera qu'à la fin du liquide le plus présent.
      {
       avancement = ((millis() - temps)/conv(5)*100);
       draw_progressbar(avancement);
       pompeV2(grenadine,1); //  Si la Pompe est activée, alors on regarde si elle a pu délivrer x Cl ( x CL = X * 6000 millisecondes). Si c'est le cas, alors on coupe la pompe, sinon on laisse allumer.
       pompeV2(ananas,4);
       pompeV2(citron,2);
       pompeV2(gin,5);
      // Début du rainbow maison
      for(i=0; i<strip.numPixels(); i++) { // Pour chaque led du Ring, 
      strip.setPixelColor(i, Wheel((i+j) & 255)); // On attribue une couleur
      }
      strip.show(); // On lance la mise à jour des couleurs
      j++;
      if (j==255){
        j=0;
      }
      // Fin du rainbow maison   
      } while ((millis() - temps) < conv(5)); // Tant qu'on ne dépasse pas le temps nécessaire au plus grand liquide
      break;
     case 8: // Flora Dora
      //On allume toutes les pomptes du cocktails
      digitalWrite(schwepps, LOW);
      digitalWrite(grenadine, LOW);
      digitalWrite(citron, LOW);
      digitalWrite(gin, LOW);
      do // On lance une boucle qui ne s'arrêtera qu'à la fin du liquide le plus présent.
      {
       avancement = ((millis() - temps)/conv(15)*100);
       draw_progressbar(avancement);
       pompeV2(schwepps,15); //  Si la Pompe est activée, alors on regarde si elle a pu délivrer x Cl ( x CL = X * 6000 millisecondes). Si c'est le cas, alors on coupe la pompe, sinon on laisse allumer.
       pompeV2(grenadine,1);
       pompeV2(citron,3);
       pompeV2(gin,4);
      // Début du rainbow maison
      for(i=0; i<strip.numPixels(); i++) { // Pour chaque led du Ring, 
      strip.setPixelColor(i, Wheel((i+j) & 255)); // On attribue une couleur
      }
      strip.show(); // On lance la mise à jour des couleurs
      j++;
      if (j==255){
        j=0;
      }
      // Fin du rainbow maison   
      } while ((millis() - temps) < conv(15)); // Tant qu'on ne dépasse pas le temps nécessaire au plus grand liquide
      break;
     case 9: // Gin Fizz
      //On allume toutes les pomptes du cocktails
      digitalWrite(schwepps, LOW);
      digitalWrite(citron, LOW);
      digitalWrite(gin, LOW);
      digitalWrite(canadou, LOW);
      do // On lance une boucle qui ne s'arrêtera qu'à la fin du liquide le plus présent.
      {
       avancement = ((millis() - temps)/conv(15)*100);
       draw_progressbar(avancement);
       pompeV2(schwepps,15); //  Si la Pompe est activée, alors on regarde si elle a pu délivrer x Cl ( x CL = X * 6000 millisecondes). Si c'est le cas, alors on coupe la pompe, sinon on laisse allumer.
       pompeV2(citron,3);
       pompeV2(gin,6);
       pompeV2(canadou,2);
      // Début du rainbow maison
      for(i=0; i<strip.numPixels(); i++) { // Pour chaque led du Ring, 
      strip.setPixelColor(i, Wheel((i+j) & 255)); // On attribue une couleur
      }
      strip.show(); // On lance la mise à jour des couleurs
      j++;
      if (j==255){
        j=0;
      }
      // Fin du rainbow maison   
      } while ((millis() - temps) < conv(15)); // Tant qu'on ne dépasse pas le temps nécessaire au plus grand liquide
      break;
     case 10: // Bacardi Cocktail
      //On allume toutes les pomptes du cocktails
      digitalWrite(schwepps, LOW);
      digitalWrite(grenadine, LOW);
      digitalWrite(citron, LOW);
      digitalWrite(rhum, LOW);
      do // On lance une boucle qui ne s'arrêtera qu'à la fin du liquide le plus présent.
      {
       avancement = ((millis() - temps)/conv(6)*100);
       draw_progressbar(avancement);
       pompeV2(schwepps,6); //  Si la Pompe est activée, alors on regarde si elle a pu délivrer x Cl ( x CL = X * 6000 millisecondes). Si c'est le cas, alors on coupe la pompe, sinon on laisse allumer.
       pompeV2(grenadine,3);
       pompeV2(citron,3);
       pompeV2(rhum,4);
      // Début du rainbow maison
      for(i=0; i<strip.numPixels(); i++) { // Pour chaque led du Ring, 
      strip.setPixelColor(i, Wheel((i+j) & 255)); // On attribue une couleur
      }
      strip.show(); // On lance la mise à jour des couleurs
      j++;
      if (j==255){
        j=0;
      }
      // Fin du rainbow maison   
      } while ((millis() - temps) < conv(6)); // Tant qu'on ne dépasse pas le temps nécessaire au plus grand liquide
      break;
     case 11: // Cendrillon
      //On allume toutes les pomptes du cocktails
      digitalWrite(orange, LOW);
      digitalWrite(schwepps, LOW);
      digitalWrite(ananas, LOW);
      digitalWrite(citron, LOW);
      digitalWrite(canadou, LOW);
      do // On lance une boucle qui ne s'arrêtera qu'à la fin du liquide le plus présent.
      {
       avancement = ((millis() - temps)/conv(8)*100);
       draw_progressbar(avancement);
       pompeV2(orange,3); //  Si la Pompe est activée, alors on regarde si elle a pu délivrer x Cl ( x CL = X * 6000 millisecondes). Si c'est le cas, alors on coupe la pompe, sinon on laisse allumer.
       pompeV2(schwepps,8);
       pompeV2(ananas,3);
       pompeV2(citron,2);
       pompeV2(canadou,2);
      // Début du rainbow maison
      for(i=0; i<strip.numPixels(); i++) { // Pour chaque led du Ring, 
      strip.setPixelColor(i, Wheel((i+j) & 255)); // On attribue une couleur
      }
      strip.show(); // On lance la mise à jour des couleurs
      j++;
      if (j==255){
        j=0;
      }
      // Fin du rainbow maison   
      } while ((millis() - temps) < conv(8)); // Tant qu'on ne dépasse pas le temps nécessaire au plus grand liquide
      break;
     default: // Sinon
      monEcran.setCursor(0, 1); // On postionne le curseur sur la deuxième ligne
      monEcran.print("Erreur!"); // On affiche Erreur
      delay(1000);
      break;
  }
  for(int i = 1; i <= 8; i++) {
    // Pour chaque Pin du relais, on les désactive tout de suite. Sécurité
    digitalWrite(i,HIGH);
  }
  draw_progressbar(100);
  Zelda();
  monEcran.setCursor(0, 0); // On postionne le curseur sur la première ligne
  monEcran.print("A LA VOTRE !"); // On affiche un message de dégustation

  colorWipe(strip.Color(0, 250, 0), 0); // On clignote
  delay(250);
  colorWipe(strip.Color(0, 0, 0), 0); // On clignote
  delay(250);
  colorWipe(strip.Color(0, 250, 0), 0); // On clignote
  delay(250);
  colorWipe(strip.Color(0, 0, 0), 0); // On clignote
  delay(250);
  colorWipe(strip.Color(0, 250, 0), 0); // On clignote
  delay(250);
  colorWipe(strip.Color(0, 0, 0), 0); // On clignote
  delay(250);
  colorWipe(strip.Color(0, 250, 0), 0); // On clignote
  delay(250);
  colorWipe(strip.Color(0, 0, 0), 0); // On clignote
  delay(250);
  colorWipe(strip.Color(0, 250, 0), 0); // On clignote
  delay(250);
  colorWipe(strip.Color(0, 0, 0), 0); // On clignote
  delay(250);
  colorWipe(strip.Color(0, 250, 0), 0); // On clignote
  delay(250);
  colorWipe(strip.Color(0, 0, 0), 0); // On clignote
}

//FONCTION ACTIVATION DE POMPES A SUPPRIMER
void pompe(int a, int b) { // a = numéro du PIN activant le relais; b = centilitre convertit en temps
  digitalWrite(a, LOW);
  delay(b*3000);
  delay(b*3000);
  digitalWrite(a, HIGH);
  delay(100);
}

//FONCTION CONVERSATION CL EN MILLISECONDES
//Permet de changer de moteur sans changer tout le code.
long conv(long a) { // a = nombre de CL
  return a*6000;
}

//FONCTION POMPES CL  fonction permettant de rentrer l'alcool, la dose, et de faire les vérifications nécessaires
void pompeV2(int a, int b) { // a = numéro du PIN activant le relais; b = centilitre convertit en temps;
  if(digitalRead(a)==0) // Si la pompe est encore activée, alors on compare, sinon, on l'a déjà éteinte.
  {
    if((millis() - temps) > conv(b)) // Si la différence entre le temps sauvegardé et le temps actuel, soit le temps passé dans la boucle, est plus grand que le nombre de cl * 6 sec, alors on peux éteindre la pompe, la dose a déjà été versée
    {
      digitalWrite(a, HIGH); // On ferme le courant.
    }
  }
}

//FONCTION INITIALISATION DES POMPES
void initialisation() {
  monEcran.clear(); // On efface l'écran
  monEcran.setCursor(0, 0); // On initialise le curseur en haut
  monEcran.print("INITIALISATION"); // On affiche le titre
  monEcran.setCursor(0, 1); // On postionne le curseur sur la deuxième ligne
  monEcran.print("EN COURS "); // On affiche 
     
  for(int i = 1; i <= 8; i++) {
     // Pour chaque Pin du relais, on active
    digitalWrite(i, LOW);
  }
  temps = millis(); // On sauvegarde l'heure actuel
  do {
      // Chargement des tuyaux, on patiente
      // On allume les lumières de chargement en attendant
      colorLED(strip.Color(0, 250, 0), 50); // Simulation de Chargement
     } while ((millis() - temps) < 7500); // Tant qu'on n'a pas rempli les tuyaux en 7,5 sec, on laisse allumé.

    for(int i = 1; i <= 8; i++) {
    // Pour chaque Pin du relais, on éteint
    digitalWrite(i, HIGH);
  }
  Simpson();
}

//FONCTION PURGE DES POMPES
void purge() {
  monEcran.clear(); // On efface l'écran
  monEcran.setCursor(0, 0); // On initialise le curseur en haut
  monEcran.print("PURGE"); // On affiche le titre
  monEcran.setCursor(0, 1); // On postionne le curseur sur la deuxième ligne
  monEcran.print("EN COURS "); // On affiche
  for(int i = 1; i <= 8; i++) {
    // Pour chaque Pin du relais, on active
    digitalWrite(i, LOW);
  }
  temps = millis(); // On sauvegarde l'heure actuel
  do {
      // DeChargement des tuyaux, on patiente
      // On allume les lumières de DeChargement en attendant
      colorLED(strip.Color(250, 0, 0), 50); // Simulation de Purge
     } while ((millis() - temps) < 10000); // Tant qu'on n'a pas rempli les tuyaux d'eau en 10 sec, on laisse allumé.

    for(int i = 1; i <= 8; i++) {
    // Pour chaque Pin du relais, on éteint
    digitalWrite(i, HIGH);
  }
  Simpson();
}

//FONCTION MUSIQUE ZELDA
//NOTE_G5, NOTE_FS5, NOTE_DS5, NOTE_G4, NOTE_FS4, NOTE_E5, NOTE_GS5, NOTE_C6
void Zelda() {
    tone(SPEAKER, NOTE_G5, 125);
    delay(162);
    tone(SPEAKER, NOTE_FS5, 125);
    delay(162);
    tone(SPEAKER, NOTE_DS5, 125);
    delay(162);
    tone(SPEAKER, NOTE_G4, 125);
    delay(162);
    tone(SPEAKER, NOTE_FS4, 125);
    delay(162);
    tone(SPEAKER, NOTE_E5, 125);
    delay(162);
    tone(SPEAKER, NOTE_GS5, 125);
    delay(162);
    tone(SPEAKER, NOTE_C6, 500);
    delay(650);
  }

//FONCTION MUSIQUE SIMPSON

void Simpson() {
  int bpm = 100;
  int bps = bpm / 60;
  int fullNoteMs = 1000 / bps;
  int halfNoteMs = fullNoteMs / 2;
  int quarterNoteMs = halfNoteMs / 2;
  int eighthNoteMs = quarterNoteMs / 2;
  int note = 0;
  int duration = 0;
  int simpsons[] = {HALF, NOTE_C4, QUARTER, NOTE_E4, NOTE_FS4, EIGHTH, NOTE_A4, HALF, NOTE_G4, QUARTER, NOTE_E4, NOTE_C4, NOTE_G3, EIGHTH, NOTE_FS3, NOTE_FS3, NOTE_FS3, QUARTER, NOTE_G3};
  int numNotes = sizeof(simpsons)/sizeof(int);

    do {
     if (simpsons[note] == HALF) {
     duration = halfNoteMs;
    } else {
        if (simpsons[note] == QUARTER) {
        duration = quarterNoteMs;
        } else {
          if (simpsons[note] == EIGHTH) {
          duration = eighthNoteMs;
          } else {
            if (simpsons[note] == SILENCE) {
              noTone(SPEAKER);
              delay(duration);
            } else {
              tone(SPEAKER, simpsons[note]);
              delay(duration);
              noTone(SPEAKER); 
              }
            }
          }   
        }
    note++;
  } while (note <= numNotes);

}

//FONCTION NEOPIXEL
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}
void colorLED(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<=(strip.numPixels()-1); i++) {
    strip.setPixelColor(i, c);
    strip.setPixelColor(i-1, 0);
    strip.show();
    delay(wait);
  }
    strip.setPixelColor(strip.numPixels()-1, 0);
    strip.show();
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

// Chargement de caractères pour afficher la barre de progression
/* Caractères personnalisés */
byte DIV_0_OF_5[8] = {
  B00000, 
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
}; // 0 / 5

byte DIV_1_OF_5[8] = {
  B10000, 
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000
}; // 1 / 5

byte DIV_2_OF_5[8] = {
  B11000, 
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000
}; // 2 / 5 

byte DIV_3_OF_5[8] = {
  B11100, 
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100
}; // 3 / 5

byte DIV_4_OF_5[8] = {
  B11110, 
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110
}; // 4 / 5

byte DIV_5_OF_5[8] = {
  B11111, 
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
}; // 5 / 5

//Issue de https://www.carnetdumaker.net/articles/faire-une-barre-de-progression-avec-arduino-et-liquidcrystal/
/**
 * Fonction de configuration de l'écran LCD pour la barre de progression.
 * Utilise les caractères personnalisés de 0 à 5 (6 et 7 restent disponibles).
 */
void setup_progressbar() {

  /* Enregistre les caractères personnalisés dans la mémoire de l'écran LCD */
  monEcran.createChar(0, DIV_0_OF_5);
  monEcran.createChar(1, DIV_1_OF_5);
  monEcran.createChar(2, DIV_2_OF_5);
  monEcran.createChar(3, DIV_3_OF_5);
  monEcran.createChar(4, DIV_4_OF_5);
  monEcran.createChar(5, DIV_5_OF_5);
}

void draw_progressbar(byte percent) {
  monEcran.setCursor(0, 1);
  /* Map la plage (0 ~ 100) vers la plage (0 ~ LCD_NB_COLUMNS * 5) */
  byte nb_columns = map(percent, 0, 100, 0, 16 * 5);
  /* Dessine chaque caractère de la ligne */
  for (byte i = 0; i < 16; ++i) {
    /* En fonction du nombre de colonnes restant à afficher */
    if (nb_columns == 0) { // Case vide
      monEcran.write((byte) 0);
    } else if (nb_columns >= 5) { // Case pleine
      monEcran.write(5);
      nb_columns -= 5;
    } else { // Derniére case non vide
      monEcran.write(nb_columns);
      nb_columns = 0;
    }
  }
  delay(10);
}
