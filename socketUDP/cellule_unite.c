#include "cellule_unite.h"
#include <stdlib.h>
cellule_unite *initialiser_cellule_unite(unite_t *unite)
{
    cellule_unite *unite_c = malloc(sizeof(cellule_unite));
    unite_c->unite = unite;
    unite_c->suivant = NULL;
    return unite_c;
}
