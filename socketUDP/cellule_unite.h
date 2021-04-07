#ifndef _CELLULE_UNITE_
#define _CELLULE_UNITE_

#include "unite.h"
#include <stdio.h>
typedef struct cellule_unite
{
    unite_t *unite;
    struct cellule_unite *suivant;
} cellule_unite;

cellule_unite *initialiser_cellule_unite(unite_t *unite);

#endif
