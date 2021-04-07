#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "unite.h"
#include "jeu.h"

int deplacement_haut(unsigned char carte[15][15], int x, int y)
{
    if (carte[x][y] < carte[x + 1][y] || carte[x][y] == 254)
    {
        if (carte[x + 1][y] >= 4 && carte[x + 1][y] <= 229)
        {
            return 1;
        }
    }
    return 0;
}
int deplacement_bas(unsigned char carte[15][15], int x, int y)
{
    if (carte[x][y] < carte[x - 1][y] || carte[x][y] == 254)
    {
        if (carte[x - 1][y] >= 4 && carte[x - 1][y] <= 229)
        {
            return 1;
        }
    }
    return 0;
}

int deplacement_gauche(unsigned char carte[15][15], int x, int y)
{
    if (carte[x][y] < carte[x][y - 1] || carte[x][y] == 254)
    {
        if (carte[x][y - 1] >= 4 && carte[x][y - 1] <= 229)
        {
            return 1;
        }
    }
    return 0;
}

int deplacement_droite(unsigned char carte[15][15], int x, int y)
{
    if (carte[x][y] < carte[x][y + 1] || carte[x][y] == 254)
    {
        if (carte[x][y + 1] >= 4 && carte[x][y + 1] <= 229)
        {
            return 1;
        }
    }
    return 0;
}

int trouver_chemin(unsigned char carte[15][15], unite_t *unite)
{
    int x = (int)unite->position[0];
    int y = (int)unite->position[1];
    if (x >= 0 && x <= 14 && y >= 0 && y <= 14)
    {
        if (deplacement_bas(carte, x, y) == 1)
        {
            unite->position[0]--;
        }
        else if (deplacement_haut(carte, x, y) == 1)
        {
            unite->position[0]++;
        }
        else if (deplacement_gauche(carte, x, y) == 1)
        {
            unite->position[1]--;
        }
        else if (deplacement_droite(carte, x, y) == 1)
        {
            unite->position[1]++;
        }
        else
        {
            return 1;
        }
    }
    return 0;
}

void initialiser_unite(unite_t *unite, int type)
{
  switch (type)
  {
  case 1:
    strncpy(unite->nom, "Soldat", 30);
    unite->cout = SOLDAT_COUT;
    unite->vie = SOLDAT_VIE;
    unite->vitesse = SOLDAT_VITESSE;
    break;
  case 2:
    strncpy(unite->nom, "Commando", 30);
    unite->cout = COMMANDO_COUT;
    unite->vie = COMMANDO_VIE;
    unite->vitesse = COMMANDO_VITESSE;
    break;
  case 3:
    strncpy(unite->nom, "Vehicule", 30);
    unite->cout = VEHICULE_COUT;
    unite->vie = VEHICULE_VIE;
    unite->vitesse = VEHICULE_VITESSE;
    break;
  case 4:
    strncpy(unite->nom, "Missile", 30);
    unite->cout = MISSILE_COUT;
    unite->vie = MISSILE_VIE;
    unite->vitesse = MISSILE_VITESSE;
    break;
  case 5:
    strncpy(unite->nom, "Char", 30);
    unite->cout = CHAR_COUT;
    unite->vie = CHAR_VIE;
    unite->vitesse = CHAR_VITESSE;
    break;
  }
}