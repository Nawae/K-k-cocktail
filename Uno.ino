//Nawae 2018/05/13
#include <LiquidCrystal.h>
//LiquidCrystal nomDeVariable(RS, E, d4, d5, d6, d7);
LiquidCrystal monEcran(14,15,16,17,18,19);

const int bout1 = 9; // Constante du pin bouton 1
const int bout2 = 10; // Constante du pin bouton 2
const int bout3 = 11; // Constante du pin bouton 3
const int boutinitialisation = 12; // Constante du pin bouton pour charger les tubes
const int boutpurge = 13; // Constante du pin bouton pour vider les tubes
int posMenu = 0; // Variable de position dans le menu
int choix = 0; // Variable pour stocker le choix
int nombreDeRecette = 9; // Permet le modula pour revenir à 1 en cycle navigation
String ligne1 = "CHOIX COCKTAIL :"; // Affichage de la première ligne
String ligne2[9] = {"1-Margarita     ", "2-Tequila Sunrise", "3-Daiquiri      ", "4-Ti'Punch      ","5-Zombie       ","6-(SA)Afterglow   ","7-Planteur      ","8-(SA)Fruits cup   ","9-(SA)Florida     "}; // Tableau de recette
const int tequila = 1; // Pin pour la board relais
const int orange = 2; // Pin pour la board relais
const int grenadine = 3; // Pin pour la board relais
const int ananas = 4; // Pin pour la board relais
const int citron = 5; // Pin pour la board relais
const int rhum = 6; // Pin pour la board relais
const int canadou = 7; // Pin pour la board relais
const int tripple = 8; // Pin pour la board relais

void setup() {
  monEcran.begin(16,2); // On initialise la communication LCD avec 16 colonnes et deux lignes
  setup_progressbar(); // On charge les caractères pour la barre de progression
  monEcran.clear(); // On efface l'écran
  monEcran.setCursor(2,0); // On centre
  monEcran.print("DEMARRAGE EN"); 
  monEcran.setCursor(5,1); // On passe à la ligne suivante
  monEcran.print("COURS"); 
  delay(2000);
  monEcran.clear(); // On efface l'écran

  pinMode(bout1, INPUT); // Déclaration du bouton comme entrée
  pinMode(bout2, INPUT); // Déclaration du bouton comme entrée
  pinMode(bout3, INPUT); // Déclaration du bouton comme entrée
  pinMode(boutinitialisation, INPUT); // Déclaration du bouton comme entrée
  pinMode(boutpurge, INPUT); // Déclaration du bouton comme entrée

  pinMode(tequila, OUTPUT); // Déclaration des pins de pompe comme sortie pour la board relais
  pinMode(orange, OUTPUT); // Déclaration des pins de pompe comme sortie pour la board relais
  pinMode(grenadine, OUTPUT); // Déclaration des pins de pompe comme sortie pour la board relais
  pinMode(ananas, OUTPUT); // Déclaration des pins de pompe comme sortie pour la board relais
  pinMode(citron, OUTPUT); // Déclaration des pins de pompe comme sortie pour la board relais
  pinMode(rhum, OUTPUT); // Déclaration des pins de pompe comme sortie pour la board relais
  pinMode(canadou, OUTPUT); // Déclaration des pins de pompe comme sortie pour la board relais
  pinMode(tripple, OUTPUT); // Déclaration des pins de pompe comme sortie pour la board relais
  
}

void loop() {
  navigation(); // Appel de la fonction navigation en boucle pour capter les boutons
  affichage(); // Appel de la fonction affichage en boucle pour rafraichir l'écran en fonction des boutons (posmenu)
}

//FONCTION NAVIGATION 
void navigation() {
  boolean etatBout1 = digitalRead(bout1); // Déclaration d'un booléen pour savoir si le bouton est appuyé
  boolean etatBout2 = digitalRead(bout2); // Déclaration d'un booléen pour savoir si le bouton est appuyé
  boolean etatBout3 = digitalRead(bout3); // Déclaration d'un booléen pour savoir si le bouton est appuyé
  boolean etatBoutInit = digitalRead(boutinitialisation); // Déclaration d'un booléen pour savoir si le bouton est appuyé
  boolean etatBoutPurge = digitalRead(boutpurge); // Déclaration d'un booléen pour savoir si le bouton est appuyé
  //Boucle pour naviguer dans les menus
  if (etatBout1 || etatBout2 || etatBout3) { // Boucle pour les boutons avec delay
  //if (etatBout1 || etatBout2 || etatBout3 || etatBoutInit || etatBoutPurge) { // Boucle pour les boutons avec delay
    if (etatBout1) { // Si le bouton 1 (HAUT) est appuyé
      posMenu = (posMenu + 1) % nombreDeRecette; //On avance la position de lecture, On avance dans le tableau
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
    /*if (etatBoutInit) { // Si le bouton Initialisation est appuyé
      initialisation(); //On lance la fonction qui charge les tuyaux
    }
    if (etatBoutPurge) { // Si le bouton Purge est appuyé
      purge(); //On lance la fonction qui vide les tuyaux
    }*/
  delay(200); //attente pour éviter les répétitions
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

  switch (posMenu) { // En fonction de la position dans le menu lors de la selection, on charge les pompes
     case 0: // Recette Margarita
      draw_progressbar(0); // Affichage de la barre de progression, calculée en pourcentage sur le volume totale pour chaque recette
      pompe(citron,2); // Chargement de la pompte citron pour 2 cl
      draw_progressbar(20);
      pompe(tripple,3);
      draw_progressbar(50);
      pompe(tequila,5);
      draw_progressbar(100);
      break;
     case 1: // Recette Tequila Sunrise
      draw_progressbar(0);
      pompe(grenadine,2);
      draw_progressbar(10);
      pompe(tequila,6);
      draw_progressbar(40);
      pompe(orange,12);
      draw_progressbar(100);
      break;
     case 2: // Recette Daiquiri
      draw_progressbar(0);
      pompe(citron,2);
      draw_progressbar(29);
      pompe(canadou,1);
      draw_progressbar(43);
      pompe(rhum,4);
      draw_progressbar(100);
      break;
     case 3: // Recette Ti'Punch
      draw_progressbar(0);
      pompe(citron,1);
      draw_progressbar(11);
      pompe(canadou,2);
      draw_progressbar(33);
      pompe(rhum,6);
      draw_progressbar(100);
      break;
     case 4: // Recette Zombie
      draw_progressbar(0);
      pompe(grenadine,(0,5));
      draw_progressbar(3);
      pompe(citron,2);
      draw_progressbar(16);
      pompe(canadou,(1,5));
      draw_progressbar(25);
      pompe(rhum,6);
      draw_progressbar(62);
      pompe(ananas,6);
      draw_progressbar(100);
      break;
     case 5: // Recette SA Aflterglow
      draw_progressbar(0);
      pompe(grenadine,2);
      draw_progressbar(11);
      pompe(orange,8);
      draw_progressbar(55);
      pompe(ananas,8);
      draw_progressbar(100);
      break;
     case 6: // Recette Planteur
      draw_progressbar(0);
      pompe(grenadine,1);
      draw_progressbar(8);
      pompe(rhum,6);
      draw_progressbar(54);
      pompe(ananas,3);
      draw_progressbar(77);
      pompe(orange,3);
      draw_progressbar(100);
      break;
     case 7: // Recette SA Fruits cup
      draw_progressbar(0);
      pompe(grenadine,1);
      draw_progressbar(4);
      pompe(citron,6);
      draw_progressbar(28);
      pompe(ananas,6);
      draw_progressbar(52);
      pompe(orange,12);
      draw_progressbar(100);
      break;
     case 8: // Recette SA Florida
      draw_progressbar(0);
      pompe(citron,1);
      draw_progressbar(9);
      pompe(grenadine,3);
      draw_progressbar(36);
      pompe(orange,7);
      draw_progressbar(100);
      break;
     default: // Sinon
      monEcran.setCursor(0, 1); // On postionne le curseur sur la deuxième ligne
      monEcran.print("Erreur!"); // On affiche Erreur
      delay(1000);
      break;
  }
  monEcran.setCursor(0, 0); // On postionne le curseur sur la deuxième ligne
  monEcran.print("A LA VOTRE !"); // On affiche la recette
  delay(2000);
}

//FONCTION ACTIVATION DE POMPES
void pompe(int a, int b) { // a = numéro du PIN activant le relais; b = centilitre convertit en temps
  digitalWrite(a, HIGH);
  delay(b*1000);
  digitalWrite(a, LOW);
  delay(300);
}

//FONCTION INITIALISATION DES POMPES
void initialisation() {
  monEcran.clear(); // On efface l'écran
  monEcran.setCursor(0, 0); // On initialise le curseur en haut
  monEcran.print("INITIALISATION"); // On affiche le titre
  monEcran.setCursor(0, 1); // On postionne le curseur sur la deuxième ligne
  monEcran.print("EN COURS "); // On affiche 
  for(int i = 1; i <= 8; i++) {
    // Pour chaque Pin du relais, on active 2 secondes
    digitalWrite(i, HIGH);
    delay(2000);
    digitalWrite(i,LOW);
    delay(200);
  }
}

//FONCTION PURGE DES POMPES
void purge() {
  monEcran.clear(); // On efface l'écran
  monEcran.setCursor(0, 0); // On initialise le curseur en haut
  monEcran.print("PURGE"); // On affiche le titre
  monEcran.setCursor(0, 1); // On postionne le curseur sur la deuxième ligne
  monEcran.print("EN COURS "); // On affiche
  for(int i = 1; i <= 8; i++) {
    // Pour chaque Pin du relais, on active 5 secondes
    digitalWrite(i, HIGH);
    delay(5000);
    digitalWrite(i,LOW);
    delay(200);
  }
}

//Chargement de caractères pour afficher la barre de progression
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
  delay(500);
}

