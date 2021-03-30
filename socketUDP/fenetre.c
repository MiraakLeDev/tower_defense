#include <stdlib.h>

#include "fenetre.h"

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
fenetre_t *fenetre_creer(int posX, int posY, int largeur, int hauteur, char *titre, bool scroll) {
    fenetre_t *retour;   

    /* Allocation de la structure */
    if((retour = malloc(sizeof(fenetre_t))) == NULL) {
        fprintf(stderr, "Erreur lors de l'allocation de la structure de fenêtre.\n");
        exit(EXIT_FAILURE);
    }

    /* Mémorisation des informations générales */
    retour->posx = posX;
    retour->posy = posY;
    retour->hauteur = hauteur;
    retour->largeur = largeur;

    /* Création de la fenêtre principale avec le cadre */
    retour->cadre = newwin(hauteur, largeur, posY, posX);
    box(retour->cadre, 0, 0);
    wprintw(retour->cadre, titre);
    wrefresh(retour->cadre);

    /* Création de la fenêtre intérieure */
    retour->interieur = subwin(retour->cadre, hauteur - 2, largeur - 2, posY + 1, posX + 1);
    scrollok(retour->interieur, scroll);
    wrefresh(retour->interieur);

    return retour;
}

/**
 * Supprime une fenêtre.
 * @param[out] fenetre un pointeur sur la fenêtre à supprimer
 */
void fenetre_supprimer(fenetre_t **fenetre) {
  delwin((*fenetre)->interieur);
  delwin((*fenetre)->cadre);
  free(*fenetre);
  *fenetre = NULL;
}

/**
 * Vérifie si des coordonnées globales se trouvent dans une fenêtre.
 * @param fenetre la fenêtre
 * @param x coordonnée en x
 * @param y coordonnée en y
 * @return TRUE si oui, FALSE sinon
 */
bool fenetre_estdans(fenetre_t *fenetre, int x, int y) {
  bool resultat = FALSE;

  if(((x >= fenetre->posx + 1) && (x < fenetre->largeur + fenetre->posx - 1)) &&
     ((y >= fenetre->posy + 1) && (y < fenetre->hauteur + fenetre->posy - 1)))
    resultat = TRUE;

  return resultat;
}

/**
 * Recupère depuis des coordonnées globales, les coordonnées au sein d'une fenêtre.
 * @param fenetre la fenêtre
 * @param posx coordonnée globale x
 * @param poxy coordonnée globale y
 * @param[out] x coordonnée calculée en x dans la fenêtre
 * @param[out] y coordonnée calculée en y dans la fenêtre
 * @return TRUE si les coordonnees sont dans la fenetre, FALSE sinon
 */
bool fenetre_getcoordonnees(fenetre_t *fenetre, int posx, int posy, int *x, int *y) {
  bool resultat = FALSE;

  if(((posx >= fenetre->posx + 1) && (posx < fenetre->largeur + fenetre->posx - 1)) &&
     ((posy >= fenetre->posy + 1) && (posy < fenetre->hauteur + fenetre->posy - 1))) {
    resultat = TRUE;
    *x = posx - fenetre->posx - 1;
    *y = posy - fenetre->posy - 1;
  }

  return resultat;
}
