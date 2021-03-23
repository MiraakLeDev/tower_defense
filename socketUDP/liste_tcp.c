#include "liste_tcp.h"
#include "cellule_tcp.h"
#include <stdlib.h>
#include <stdio.h>



liste_tcp* initialiser_liste_tcp(cellule_tcp* cellule){
    liste_tcp* liste = malloc(sizeof(liste_tcp));
    liste->premier = cellule;
    return liste;
}

void ajouter_cellule_tcp(liste_tcp* liste,cellule_tcp* cellule){
    cellule->suivant = liste->premier;

    if(liste->premier != NULL){
        liste->premier->prec=cellule;
    }
    liste->premier = cellule;
    cellule->prec = NULL;
}

void supprimer_cellule_tcp(liste_tcp* liste, cellule_tcp* aSupprimer){
    if (liste == NULL)
    {
        exit(EXIT_FAILURE);
    }

    if(aSupprimer->prec != NULL){
        aSupprimer->prec->suivant =  aSupprimer->suivant;
    }else{
        liste->premier = aSupprimer->suivant;
    }

    if (aSupprimer->suivant != NULL){
        aSupprimer->suivant->prec = aSupprimer->prec;
    }
    detruire_cellule_tcp(aSupprimer);
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