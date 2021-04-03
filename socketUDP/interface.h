#ifndef _INTERFACE_
#define _INTERFACE_
/* Propriétés de la fenêtre d'informations */
#define INFOS_POSX        0 /* Position en X */
#define INFOS_POSY        0 /* Position en Y */
#define INFOS_LARGEUR    60 /* Largeur avec le cadre */
#define INFOS_HAUTEUR    10 /* Hauteur avec le cadre */

/* Propriétés de la fenêtre de la carte */
#define CARTE_POSX        5 /* Position en X */
#define CARTE_POSY       10 /* Position en Y */
#define CARTE_LARGEUR    17 /* Largeur avec le cadre */
#define CARTE_HAUTEUR    17 /* Hauteur avec le cadre */

/* Propriétés de la fenêtre des outils */
#define OUTILS_POSX      23 /* Position en X */
#define OUTILS_POSY      10 /* Position en Y */
#define OUTILS_LARGEUR   15 /* Largeur avec le cadre */
#define OUTILS_HAUTEUR    9 /* Hauteur avec le cadre */

/* Propriétés de la fenêtre d'état */
#define ETAT_POSX        39 /* Position en X */
#define ETAT_POSY        10 /* Position en Y */
#define ETAT_LARGEUR     15 /* Largeur avec le cadre */
#define ETAT_HAUTEUR      9 /* Hauteur avec le cadre */

/* Propriétés de la fenêtre des attaques */
#define ATTAQUES_POSX    23 /* Position en X */
#define ATTAQUES_POSY    19 /* Position en Y */
#define ATTAQUES_LARGEUR 31 /* Largeur avec le cadre */
#define ATTAQUES_HAUTEUR  8 /* Hauteur avec le cadre */

/* Dimensions de l'interface (pour vérification avec celles du terminal) */
#define LARGEUR INFOS_LARGEUR
#define HAUTEUR ATTAQUES_POSY + ATTAQUES_HAUTEUR

/* Types de case */
#define CASE_VIDE         0
#define CASE_MIN_JOUEUR   1
#define CASE_MAX_JOUEUR   3
#define CASE_MIN_CHEMIN   4
#define CASE_MAX_CHEMIN 229
#define CASE_ORDI       254
#define CASE_FORT       255

/* Couleurs des cases */
#define COULEUR_VIDE      2
#define COULEUR_FORT      7
#define COULEUR_CHEMIN    3
#define COULEUR_JOUEUR    6
#define COULEUR_ORDI      6

/* Outils */
#define OUTIL_NONE       -1
#define OUTIL_TOUR_1      1
#define OUTIL_TOUR_2      2
#define OUTIL_TOUR_3      3
#define OUTIL_TOUR_4      4
#define OUTIL_TOUR_5      5
#define OUTIL_UNFREEZE    6

/* Structure représentant l'interface de l'application */
typedef struct {
    fenetre_t *infos;    /* Fenêtre d'informations */
    fenetre_t *outils;   /* Fenêtre des outils */
    fenetre_t *carte;    /* Fenêtre de la carte */
    fenetre_t *etat;     /* Fenêtre d'état */
    fenetre_t *attaques; /* Fenêtre des attaques */
    unsigned int outilsel; /* L'outil sélectionné */
} interface_t;

/**
 * Définition de la palette.
 */
void palette();

/*void deplacement_unite(unite_t* unite,jeu_t* jeu, interface_t* interface);*/
/**
 * Création d'une interface.
 * @param jeu l'état du jeu
 * @return l'interface créée
 */
interface_t interface_creer(jeu_t *jeu);

/**
 * Suppression d'une interface.
 * @param interface l'interface à supprimer
 */
void interface_supprimer(interface_t *interface);

/**
 * Met à jour la fenêtre 'Attaques'.
 * @param interface l'interface
 * @param jeu l'état du jeu
 */
void interface_MAJAttaques(interface_t *interface, jeu_t *jeu);

/**
 * Met à jour la fenêtre 'Outils'.
 * @param interface l'interface
 * @param jeu l'état du jeu
 */
void interface_MAJOutils(interface_t *interface, jeu_t *jeu);

/**
 * Met à jour la fenêtre 'Etat'.
 * @param interface l'interface
 * @param jeu l'état du jeu
 */
void interface_MAJEtat(interface_t *interface, jeu_t *jeu);

/**
 * Gestion des actions dans la fenêtre des outils.
 * @param interface l'interface
 * @param jeu l'état du jeu
 * @param posX la position X du clic dans la fenêtre
 * @param posY la position Y du clic dans la fenêtre
 */
void interface_outils(interface_t *interface, jeu_t *jeu, int posX, int posY);

/**
 * Gestion des actions dans la fenêtre des attaques.
 * @param interface l'interface
 * @param jeu l'état du jeu
 * @param posX la position X du clic dans la fenêtre
 * @param posY la position Y du clic dans la fenêtre
 */
void interface_attaques(interface_t *interface, jeu_t *jeu, int posX, int posY);

/**
 * Gestion des actions dans la fenêtre de la carte.
 * @param interface l'interface
 * @param jeu l'état du jeu
 * @param posX la position X du clic dans la fenêtre
 * @param posY la position Y du clic dans la fenêtre
 */
void interface_carte(interface_t *interface, jeu_t *jeu, int posX, int posY,int* cmp);

/**
 * Gestion des actions de l'utilisateur dans l'interface.
 * @param interface l'interface
 * @param jeu les paramètres de la partie
 * @param c la touche pressée
 */
void interface_main(interface_t *interface, jeu_t *jeu, int c,int* cmp);

#endif
