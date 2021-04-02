

#include "jeu.h"
#include "ncurses.h"
#include "fenetre.h"
#include "interface.h"
#include <string.h>
#include <unistd.h>
#include<stdio.h>
#include <time.h>
void deplacement_unite(unite_t* unite,jeu_t* jeu){

    while (unite->vie > 0 &&   trouver_chemin(jeu->carte,unite) !=1){
      /*printf("%s:[%d][%d]=%d",unite->nom,unite->position[0],unite->position[1],jeu->carte[unite->position[0]][unite->position[1]]);*/
      usleep(unite->vitesse*1000);
    }




}

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
