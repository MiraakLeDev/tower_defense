#include "cellule_unite.h"
typedef struct Liste
{
    cellule_unite *premier;
} liste_adj;

liste_adj initialiser_liste_adj();
void ajouter_cellule_unite(liste_adj *liste, cellule_unite *cellule_u);
void supprimer_cellule_unite(liste_adj *liste, unite_t *aSupprimer);
void afficher_liste_adj(liste_adj *liste);
void supprimer_liste_adj(liste_adj *liste);
cellule_unite *rechercher_cellule(liste_adj *liste, unite_t *unite);
int nb_cellules(liste_adj *liste);
cellule_unite *retirer_cellule(liste_adj *liste, unite_t *unite);
void initialiser_unite(unite_t *unite, int type);
