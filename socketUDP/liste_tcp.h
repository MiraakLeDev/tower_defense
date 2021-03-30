#ifndef SOCKETTCP_LISTE_TCP_H
#define SOCKETTCP_LISTE_TCP_H
#include "cellule_tcp.h"
typedef struct Liste
{
    cellule_tcp *premier;
}liste_tcp;

liste_tcp* initialiser_liste_tcp(cellule_tcp* cellule);
void ajouter_cellule_tcp(liste_tcp* liste,cellule_tcp* cellule);
void supprimer_cellule_tcp(liste_tcp* liste, cellule_tcp* aSupprimer);
void afficher_liste_tcp(liste_tcp* liste);
void supprimer_liste_tcp(liste_tcp* liste);
int nb_cellules(liste_tcp* liste);
#endif
