#ifndef _LISTE_ADJ_
#define _LISTE_ADJ_
#include "cellule_unite.h"
#include <pthread.h>
typedef struct Liste_adj
{
    pthread_mutex_t mutex_liste;
    pthread_mutexattr_t mutex_attr;
    cellule_unite *premier;
} liste_adj;

liste_adj initialiser_liste_adj();
void ajouter_cellule_unite(liste_adj *liste, cellule_unite *cellule_u);
void supprimer_cellule_unite(liste_adj *liste, unite_t *aSupprimer);
void afficher_liste_adj(liste_adj *liste);
void supprimer_liste_adj(liste_adj *liste);
cellule_unite *rechercher_cellule(liste_adj *liste, int position);
cellule_unite *retirer_cellule(liste_adj *liste, unite_t *unite);

#endif
