#ifndef _TOUR_
#define _TOUR_
#include <pthread.h>
#include "jeu.h"
#include "liste_adj.h"
typedef struct
{
    unsigned char position[2]; /*Position de la tour*/
    unsigned int cout;         /* Cout de la tour */
    unsigned int degat_min;    /*Degat minimum de la tour*/
    unsigned int degat_max;    /*Degat maximum de la tour*/
    unsigned int portee;       /*Portee de la tour */
    unsigned int vitesse;      /*Vitesse d'attaque' */
    int **chemin;
    int taille_chemin;
    pthread_t thread;
} tour_t;

/*Structure passée au thread permettant de créer la tour*/
typedef struct arg_tour
{
    tour_t* tour;
    jeu_t *jeu;
} arg_tour;


void initialiser_tour(tour_t *tour, int type, jeu_t *jeu, int X, int Y);
void trouver_case(tour_t *tour, jeu_t *jeu);
void tour_tire(tour_t *tour,jeu_t* jeu);
void detruire_tour(tour_t* tour);
void *spawn_tour(void *args);

#endif