#include "liste_adj.h"
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

liste_adj initialiser_liste_adj()
{
    liste_adj liste;
    liste.premier = NULL;
    pthread_mutexattr_init(&liste.mutex_attr);
    pthread_mutexattr_settype(&liste.mutex_attr, PTHREAD_MUTEX_NORMAL);
    if (pthread_mutex_init(&liste.mutex_liste, &liste.mutex_attr) != 0)
    {
        printf("\n mutex_list init failed\n");
        exit(EXIT_FAILURE);
    }
    return liste;
}

void ajouter_cellule_unite(liste_adj *liste, cellule_unite *cellule_u)
{

    if (cellule_u == NULL)
    {
        exit(EXIT_FAILURE);
    }

    pthread_mutex_lock(&liste->mutex_liste);
    if (liste->premier == NULL)
    {
        /*initialiser_cellule_unite(cellule_u, unite);*/
        liste->premier = cellule_u;
        cellule_u->suivant = NULL;
    }
    else
    {
        cellule_u->suivant = liste->premier;
        liste->premier = cellule_u;
    }
    pthread_mutex_unlock(&liste->mutex_liste);
}

/* Affichage de la liste adj */
void afficher_liste_adj(liste_adj *liste)
{
    if (liste == NULL)
    {
        exit(EXIT_FAILURE);
    }
    pthread_mutex_lock(&liste->mutex_liste);
    cellule_unite *actuel = liste->premier;
    while (actuel != NULL)
    {
        printf("%s -> ", actuel->unite->nom);
        actuel = actuel->suivant;
    }
    pthread_mutex_unlock(&liste->mutex_liste);
    printf("NULL|  ");
}

/* Supprime une cellule d'une liste correspondant à l'unité passée en param */
void supprimer_cellule_unite(liste_adj *liste, unite_t *unite)
{
    if (liste == NULL)
    {
        exit(EXIT_FAILURE);
    }
    pthread_mutex_lock(&liste->mutex_liste);
    if (liste->premier != NULL)
    {
        cellule_unite *cellule = liste->premier;
        cellule_unite *atrouver;
        if (cellule->unite == unite)
        {
            liste->premier = cellule->suivant;
            free(cellule);
        }

        while (cellule->suivant != NULL)
        {
            if (cellule->suivant->unite == unite)
            {
                atrouver = cellule->suivant;
                cellule->suivant = atrouver->suivant;
                free(atrouver);
            }
            else
            {
                cellule = cellule->suivant;
            }
        }
    }
    pthread_mutex_unlock(&liste->mutex_liste);
}

cellule_unite *retirer_cellule(liste_adj *liste, unite_t *unite)
{
    if (liste == NULL)
    {
        exit(EXIT_FAILURE);
    }
    pthread_mutex_lock(&liste->mutex_liste);
    if (liste->premier != NULL)
    {
        cellule_unite *cellule = liste->premier;
        cellule_unite *atrouver;
        if (cellule->unite == unite)
        {
            liste->premier = cellule->suivant;

            pthread_mutex_unlock(&liste->mutex_liste);
            return cellule;
        }

        while (cellule->suivant != NULL)
        {
            if (cellule->suivant->unite == unite)
            {
                atrouver = cellule->suivant;
                cellule->suivant = atrouver->suivant;

                pthread_mutex_unlock(&liste->mutex_liste);
                return atrouver;
            }
            else
            {
                cellule = cellule->suivant;
            }
        }
    }
    pthread_mutex_unlock(&liste->mutex_liste);
    return NULL;
}

cellule_unite *rechercher_cellule(liste_adj *liste, int position)
{
    if (liste == NULL)
    {
        exit(EXIT_FAILURE);
    }
    pthread_mutex_lock(&liste->mutex_liste);
    if (liste->premier != NULL)
    {
        cellule_unite *cellule = liste->premier;
        if (cellule->unite->position[1] == (unsigned char)position)
        {
            pthread_mutex_unlock(&liste->mutex_liste);
            return cellule;
        }

        while (cellule->suivant != NULL)
        {
            if (cellule->unite->position[1] == (unsigned char)position)
            {
                pthread_mutex_unlock(&liste->mutex_liste);
                return cellule;
            }
            else
            {
                cellule = cellule->suivant;
            }
        }
    }
    pthread_mutex_unlock(&liste->mutex_liste);
    return NULL;
}

void supprimer_liste_adj(liste_adj *liste)
{
    pthread_mutex_lock(&liste->mutex_liste);
    if (liste->premier != NULL)
    {
        cellule_unite *cellule = liste->premier;
        cellule_unite *tmp;
        while (cellule != NULL)
        {
            tmp = cellule;
            cellule = cellule->suivant;
            free(tmp);
        }
    }
    pthread_mutex_unlock(&liste->mutex_liste);
}