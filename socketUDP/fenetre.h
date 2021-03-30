#ifndef _FENETRE_
#define _FENETRE_

#include <ncurses.h>

/* Structure représentant une fenêtre avec un cadre et un intérieur */
typedef struct {
  int posx, posy;        /* La position de la fenêtre sur l'écran */
  int largeur, hauteur;  /* La hauteur et la largeur de la fenêtre */
    WINDOW *cadre;       /* Le cadre de la fenêtre */
    WINDOW *interieur;   /* L'intérieur de la fenêtre */
} fenetre_t;

/**
 * Crée une nouvelle fenêtre.
 * @param posX la position en X de la fenêtre
 * @param posY la position en Y de la fenêtre
 * @param largeur la largeur de la fenêtre
 * @param hauteur la heuteur de la fenêtre
 * @param titre le titre de la fenêtre
 * @param scroll si 'TRUE', active le défilement dans la fenêtre
 * @return la fenêtre créée
 */
fenetre_t *fenetre_creer(int posX, int posY, int largeur, int hauteur, char *titre, bool scroll);

/**
 * Supprime une fenêtre.
 * @param[out] fenetre un pointeur sur la fenêtre à supprimer
 */
void fenetre_supprimer(fenetre_t **fenetre);

/**
 * Vérifie si des coordonnées globales se trouvent dans une fenêtre.
 * @param fenetre la fenêtre
 * @param x coordonnée en x
 * @param y coordonnée en y
 * @return TRUE si oui, FALSE sinon
 */
bool fenetre_estdans(fenetre_t *fenetre, int x, int y);

/**
 * Recupère depuis des coordonnées globales, les coordonnées au sein d'une fenêtre.
 * @param fenetre la fenêtre
 * @param posx coordonnée globale x
 * @param poxy coordonnée globale y
 * @param[out] x coordonnée calculée en x dans la fenêtre
 * @param[out] y coordonnée calculée en y dans la fenêtre
 * @return TRUE si les coordonnees sont dans la fenetre, FALSE sinon
 */
bool fenetre_getcoordonnees(fenetre_t *fenetre, int posx, int posy, int *x, int *y);

#endif