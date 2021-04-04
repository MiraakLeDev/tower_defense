#include "liste_adj.h"
#include <stdlib.h>
#include <string.h>
liste_adj initialiser_liste_adj()
{
  liste_adj liste;
  liste.premier = NULL;

  return liste;
}
void ajouter_cellule_unite(liste_adj *liste, cellule_unite *cellule_u)
{

  if (cellule_u == NULL)
  {
    exit(EXIT_FAILURE);
  }
  if (liste->premier == NULL)
  {
    /*initialiser_cellule_unite(cellule_u, unite);*/
    liste->premier = cellule_u;
  }
  else
  {
    cellule_u->suivant = NULL;
    cellule_u->suivant = liste->premier;
    liste->premier = cellule_u;
  }
}
void afficher_liste_adj(liste_adj *liste)
{
  if (liste == NULL)
  {
    exit(EXIT_FAILURE);
  }
  cellule_unite *actuel = liste->premier;
  while (actuel != NULL)
  {
    printf("%s -> ", actuel->unite->nom);
    actuel = actuel->suivant;
  }
  printf("NULL\n");
}
void supprimer_cellule_unite(liste_adj *liste, unite_t *unite)
{
  if (liste == NULL)
  {
    exit(EXIT_FAILURE);
  }
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

cellule_unite *retirer_cellule(liste_adj *liste, unite_t *unite)
{
  if (liste == NULL)
  {
    exit(EXIT_FAILURE);
  }
  cellule_unite *cellule = liste->premier;
  cellule_unite *atrouver;
  if (cellule->unite == unite)
  {
    liste->premier = cellule->suivant;
    return cellule;
  }

  while (cellule->suivant != NULL)
  {
    if (cellule->suivant->unite == unite)
    {
      atrouver = cellule->suivant;
      cellule->suivant = atrouver->suivant;
      return (atrouver);
    }
    else
    {
      cellule = cellule->suivant;
    }
  }

  return NULL;
}
cellule_unite *rechercher_cellule(liste_adj *liste, unite_t *unite)
{
  if (liste == NULL)
  {
    exit(EXIT_FAILURE);
  }
  cellule_unite *cellule = liste->premier;

  while (cellule->suivant != NULL)
  {
    if (cellule->unite == unite)
    {
      return (cellule);
    }
    else
    {
      cellule = cellule->suivant;
    }
  }

  return NULL;
}
void initialiser_unite(unite_t *unite, int type)
{
  switch (type)
  {
  case 1:
    strncpy(unite->nom, "Soldat", 30);
    unite->cout = SOLDAT_COUT;
    unite->vie = SOLDAT_VIE;
    unite->vitesse = SOLDAT_VITESSE;
    break;
  case 2:
    strncpy(unite->nom, "Commando", 30);
    unite->cout = COMMANDO_COUT;
    unite->vie = COMMANDO_VIE;
    unite->vitesse = COMMANDO_VITESSE;
    break;
  case 3:
    strncpy(unite->nom, "Vehicule", 30);
    unite->cout = VEHICULE_COUT;
    unite->vie = VEHICULE_VIE;
    unite->vitesse = VEHICULE_VITESSE;
    break;
  case 4:
    strncpy(unite->nom, "Missile", 30);
    unite->cout = MISSILE_COUT;
    unite->vie = MISSILE_VIE;
    unite->vitesse = MISSILE_VITESSE;
    break;
  case 5:
    strncpy(unite->nom, "Char", 30);
    unite->cout = CHAR_COUT;
    unite->vie = CHAR_VIE;
    unite->vitesse = CHAR_VITESSE;
    break;
  }
}
