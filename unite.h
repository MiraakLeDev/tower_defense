#ifndef TOWER_UNITE_H
#define TOWER_UNITE_H
#include <unistd.h>
typedef struct unite
{
    int cout;
    float vitesse;
    float duree_vie;
    float vie_max;
    float vie_actuelle;
} unite_t;

unite_t initialiser_unite();
#endif
