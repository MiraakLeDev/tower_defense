#include "jeu.h"
#include <string.h>
void initialiser_tour(tour_t* tour,int type){
  switch (type) {
    case 1:
      tour->cout = TOUR_1_COUT;
      tour->degat_min = TOUR_1_TIR_MIN;
      tour->degat_max = TOUR_1_TIR_MAX;
      tour->portee = TOUR_1_PORTEE;
      tour->vitesse = TOUR_1_VITESSE;
      break;
    case 2:
      tour->cout = TOUR_2_COUT;
      tour->degat_min = TOUR_2_TIR_MIN;
      tour->degat_max = TOUR_2_TIR_MAX;
      tour->portee = TOUR_2_PORTEE;
      tour->vitesse = TOUR_2_VITESSE;
      break;
    case 3:
      tour->cout = TOUR_3_COUT;
      tour->degat_min = TOUR_3_TIR_MIN;
      tour->degat_max = TOUR_3_TIR_MAX;
      tour->portee = TOUR_3_PORTEE;
      tour->vitesse = TOUR_3_VITESSE;
      break;
    case 4:
      tour->cout = TOUR_4_COUT;
      tour->degat_min = TOUR_4_TIR_MIN;
      tour->degat_max = TOUR_4_TIR_MAX;
      tour->portee = TOUR_4_PORTEE;
      tour->vitesse = TOUR_4_VITESSE;
      break;
    case 5:
      tour->cout = TOUR_5_COUT;
      tour->degat_min = TOUR_5_TIR_MIN;
      tour->degat_max = TOUR_5_TIR_MAX;
      tour->portee = TOUR_5_PORTEE;
      tour->vitesse = TOUR_5_VITESSE;
      break;
  }
}
void initialiser_unite(unite_t* unite,int type){
  switch (type) {
    case 1:
      strncpy(unite->nom,"Soldat",30);
      unite->cout = SOLDAT_COUT;
      unite->vie = SOLDAT_VIE;
      unite->vitesse = SOLDAT_VITESSE;
      break;
    case 2:
      strncpy(unite->nom,"Commando",30);
      unite->cout = COMMANDO_COUT;
      unite->vie = COMMANDO_VIE;
      unite->vitesse = COMMANDO_VITESSE;
      break;
    case 3:
      strncpy(unite->nom,"Vehicule",30);
      unite->cout = VEHICULE_COUT;
      unite->vie = VEHICULE_VIE;
      unite->vitesse = VEHICULE_VITESSE;
      break;
    case 4:
      strncpy(unite->nom,"Missile",30);
      unite->cout = MISSILE_COUT;
      unite->vie = MISSILE_VIE;
      unite->vitesse = MISSILE_VITESSE;
      break;
    case 5:
      strncpy(unite->nom,"Char",30);
      unite->cout = CHAR_COUT;
      unite->vie = CHAR_VIE;
      unite->vitesse = CHAR_VITESSE;
      break;
  }
}


#define TOUR_1_TIR_MIN      1
#define TOUR_1_TIR_MAX     10
#define TOUR_1_PORTEE       1
#define TOUR_1_VITESSE    500
