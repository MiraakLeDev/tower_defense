#include "unite.h"
unite_t initialiser_unite()
{
    unite_t unite;
    unite.cout = 0;
    unite.duree_vie = 0;
    unite.vie_actuelle = unite.vie_max;
    unite.vie_max = 0;
    unite.speed = 0;
    return unite;
}
float infliger_degat(unite_t* unite,float degat){
  unite->actuelle -= degat;

}
