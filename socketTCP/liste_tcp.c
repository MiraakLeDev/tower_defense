#include "liste_tcp.h"
#include "cellule_tcp.h"
#include <stdlib.h>
#include <stdio.h>



liste_tcp initialiser_liste_tcp(cellule_tcp* cellule){
    liste_tcp liste;
    liste.premier = cellule;
    return liste;
}

void ajouter_cellule_tcp(liste_tcp* liste,cellule_tcp* cellule){
    cellule->suivant = liste->premier;
    liste->premier = cellule;
}

void supprimer_cellule_tcp(liste_tcp* liste){
    if (liste == NULL)
    {
        exit(EXIT_FAILURE);
    }
    if (liste->premier != NULL)
    {
        cellule_tcp *aSupprimer = liste->premier;
        liste->premier = liste->premier->suivant;
        free(aSupprimer);
    }
}

void afficher_liste_tcp(liste_tcp* liste){
    cellule_tcp *actuel = NULL;
    int cmp=0;
    if (liste == NULL)
    {
        exit(EXIT_FAILURE);
    }
    actuel = liste->premier;
    while (actuel != NULL)
    {
        printf("%d->",cmp);
        actuel = actuel->suivant;
        cmp++;
    }
    printf("NULL\n");
}