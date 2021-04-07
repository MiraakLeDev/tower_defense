#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "tour.h"
#include "jeu.h"
#include "liste_adj.h"

/* Permet de trouver toutes les cases CHEMIN dans le rayon de portée de la tour */
void trouver_case(tour_t* tour, jeu_t* jeu)
{
    int cmp = 0, portee = 0, i = 0, j = 0, x = 0, y = 0;

    portee = 2 * tour->portee + 1; /* nb de cases de la portée de la tourelle */

    /* On scanne toutes les cases potentielles pouvant être de type CHEMIN et se trouvant à la portée de la tourelle */
    for (i = 0; i < portee; i++)
    {
        for (j = 0; j < portee; j++)
        {
            x = i - tour->portee + tour->position[0];
            y = j - tour->portee + tour->position[1];

            if (jeu->carte[x][y] >= 4 && jeu->carte[x][y] <= 229)
            {
                tour->taille_chemin++;
            }
        }
    }

    /* Une fois le scanne terminé on alloue le tableau de chemin de la tour (tableau qui référence les cases où la tour peut tirer) */
    tour->chemin = malloc(sizeof(int *) * tour->taille_chemin);
    for (i = 0; i < portee; i++)
    {
        for (j = 0; j < portee; j++)
        {
            x = i - tour->portee + tour->position[0];
            y = j - tour->portee + tour->position[1];

            /* Si on trouve une case CHEMIN on la référence dans notre tableau */
            if (jeu->carte[x][y] >= 4 && jeu->carte[x][y] <= 229)
            {
                tour->chemin[cmp] = malloc(sizeof(int) * 2);
                tour->chemin[cmp][0] = x;
                tour->chemin[cmp][1] = y;
                cmp++;
            }
        }
    }
}

/* Initialise une tour en fonction de son type et de sa position en X et Y */
void initialiser_tour(tour_t* tour, int type, jeu_t* jeu, int X, int Y)
{
    int i = 0;
    switch (type)
    {
        case 1:
            strncpy(tour->nom, "A", 5);
            tour->cout = TOUR_1_COUT;
            tour->degat_min = TOUR_1_TIR_MIN;
            tour->degat_max = TOUR_1_TIR_MAX;
            tour->portee = TOUR_1_PORTEE;
            tour->vitesse = TOUR_1_VITESSE;
            break;
        case 2:
            strncpy(tour->nom, "B", 5);
            tour->cout = TOUR_2_COUT;
            tour->degat_min = TOUR_2_TIR_MIN;
            tour->degat_max = TOUR_2_TIR_MAX;
            tour->portee = TOUR_2_PORTEE;
            tour->vitesse = TOUR_2_VITESSE;
            break;
        case 3:
            strncpy(tour->nom, "C", 5);
            tour->cout = TOUR_3_COUT;
            tour->degat_min = TOUR_3_TIR_MIN;
            tour->degat_max = TOUR_3_TIR_MAX;
            tour->portee = TOUR_3_PORTEE;
            tour->vitesse = TOUR_3_VITESSE;
            break;
        case 4:
            strncpy(tour->nom, "D", 5);
            tour->cout = TOUR_4_COUT;
            tour->degat_min = TOUR_4_TIR_MIN;
            tour->degat_max = TOUR_4_TIR_MAX;
            tour->portee = TOUR_4_PORTEE;
            tour->vitesse = TOUR_4_VITESSE;
            break;
        case 5:
            strncpy(tour->nom, "E", 5);
            tour->cout = TOUR_5_COUT;
            tour->degat_min = TOUR_5_TIR_MIN;
            tour->degat_max = TOUR_5_TIR_MAX;
            tour->portee = TOUR_5_PORTEE;
            tour->vitesse = TOUR_5_VITESSE;
        break;
    }
    tour->taille_chemin = 0;
    tour->position[0] = X;
    tour->position[1] = Y;
    trouver_case(tour, jeu);
}

/* Fais tirer une tour sur les ennemis se trouvant dans sa portée */
void tour_tire(tour_t *tour,jeu_t * jeu)
{
    int i = 0;
    for (i = 0; i < tour->taille_chemin; i++)
    {
        afficher_liste_adj(&jeu->liste[tour->chemin[i][0]]);
        /*
         if (rechercher_cellule(&liste[tour->chemin[i][0]], tour->chemin[i][1]) != NULL){
            printf("trouvé !");
        }*/
    }
}

/* Fonction utilisée par le thread qui gère la tour */
void *spawn_tour(void *args)
{
    arg_tour *arg = (arg_tour *)args;
    tour_t tour = *arg->tour;

    while (1)
    {
        tour_tire(&tour, arg->jeu);
        usleep(tour.vitesse * 1000);

    }
    detruire_tour(arg->tour);
    return NULL;
}

/* Desallocation mémoire d'une tour */
void detruire_tour(tour_t* tour){
    printf("salut frero");
    /*
    int i = 0;
    for (i = 0; i < tour->taille_chemin; ++i) {
        if (tour->chemin[i] != NULL)
            free(tour->chemin[i]);
    }
     */
}



