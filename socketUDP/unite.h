#ifndef _UNITE_
#define _UNITE_
#include <pthread.h>
typedef struct
{
    char nom[30];
    unsigned char position[2];  /* Position de l'unite */
    unsigned int cout;          /* Cout de l'unite */
    unsigned int vie;           /* Vie de l'unite */
    unsigned int vitesse;       /* Vitesse de l'unite */
    pthread_t thread;           /* Thread qui gere l'unité */
} unite_t;

void *spawn_unite(void *args);
void initialiser_unite(unite_t *unite, int type);
int deplacement_haut(unsigned char carte[15][15], int x, int y);
int deplacement_bas(unsigned char carte[15][15], int x, int y);
int deplacement_gauche(unsigned char carte[15][15], int x, int y);
int deplacement_droite(unsigned char carte[15][15], int x, int y);
int trouver_chemin(unsigned char carte[15][15], unite_t *unite);

#endif