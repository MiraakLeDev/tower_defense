#include "jeu.h"
#include "string.h"

int deplacement_haut(unsigned char carte[15][15],int x, int y){
  if (carte[x][y] < carte[x+1][y] || carte[x][y]==254) {
    if (carte[x+1][y] >=4 && carte[x+1][y] <=229) {
      return 1;
    }
  }
      return 0;
}
int deplacement_bas(unsigned char carte[15][15],int x, int y){
  if (carte[x][y] < carte[x-1][y] || carte[x][y]==254) {
    if (carte[x-1][y] >=4 && carte[x-1][y] <=229) {
      return 1;
    }
  }
      return 0;
}

int deplacement_gauche(unsigned char carte[15][15],int x, int y){
  if (carte[x][y] < carte[x][y-1] || carte[x][y]==254) {
    if (carte[x][y-1] >=4 && carte[x][y-1] <=229) {
      return 1;
    }
  }
      return 0;
}

int deplacement_droite(unsigned char carte[15][15],int x, int y){
    if (carte[x][y] < carte[x][y+1] || carte[x][y]==254) {
      if (carte[x][y+1] >=4 && carte[x][y+1] <=229) {
        return 1;
      }
    }
        return 0;
}


int trouver_chemin(unsigned char carte[15][15],unite_t* unite){
    int x = (int)unite->position[0];
    int y = (int)unite->position[1];
    if (x>=0 && x<=14 && y>=0 && y<=14 ) {
       if (deplacement_bas(carte,x,y)==1) {

         unite->position[0]--;

       }else if(deplacement_haut(carte,x,y)==1){
         unite->position[0]++;
       }else if(deplacement_gauche(carte,x,y)==1){
         unite->position[1]--;
       }else if (deplacement_droite(carte,x,y)==1) {
         unite->position[1]++;
       }else{
         return 1;
       }
    }
    return 0;
}



void initialiser_tour(tour_t* tour,int type){
  switch (type) {
    case 1:
      strncpy(tour->nom,"A",5);
      tour->cout = TOUR_1_COUT;
      tour->degat_min = TOUR_1_TIR_MIN;
      tour->degat_max = TOUR_1_TIR_MAX;
      tour->portee = TOUR_1_PORTEE;
      tour->vitesse = TOUR_1_VITESSE;
      break;
    case 2:
      strncpy(tour->nom,"B",5);
      tour->cout = TOUR_2_COUT;
      tour->degat_min = TOUR_2_TIR_MIN;
      tour->degat_max = TOUR_2_TIR_MAX;
      tour->portee = TOUR_2_PORTEE;
      tour->vitesse = TOUR_2_VITESSE;
      break;
    case 3:
      strncpy(tour->nom,"C",5);
      tour->cout = TOUR_3_COUT;
      tour->degat_min = TOUR_3_TIR_MIN;
      tour->degat_max = TOUR_3_TIR_MAX;
      tour->portee = TOUR_3_PORTEE;
      tour->vitesse = TOUR_3_VITESSE;
      break;
    case 4:
      strncpy(tour->nom,"D",5);
      tour->cout = TOUR_4_COUT;
      tour->degat_min = TOUR_4_TIR_MIN;
      tour->degat_max = TOUR_4_TIR_MAX;
      tour->portee = TOUR_4_PORTEE;
      tour->vitesse = TOUR_4_VITESSE;
      break;
    case 5:
      strncpy(tour->nom,"E",5);
      tour->cout = TOUR_5_COUT;
      tour->degat_min = TOUR_5_TIR_MIN;
      tour->degat_max = TOUR_5_TIR_MAX;
      tour->portee = TOUR_5_PORTEE;
      tour->vitesse = TOUR_5_VITESSE;
      break;
  }
}


#define TOUR_1_TIR_MIN      1
#define TOUR_1_TIR_MAX     10
#define TOUR_1_PORTEE       1
#define TOUR_1_VITESSE    500
