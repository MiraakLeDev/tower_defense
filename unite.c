#include "unite.h"
#include "joueur.h"

unite_t initialiser_unite()
{
    unite_t unite;
    unite.cout = 0;
    unite.duree_vie = 0;
    unite.vie_actuelle = unite.vie_max;
    unite.vie_max = 0;
    unite.speed = 0;
    return unite;
    /*creer un thread*/
}
void infliger_degat(unite_t* unite,float degat){
    if(unite->vie_actuelle - degat <= 0){
        mort(&unite);
    }else{
        unite->vie_actuelle -= degat;
    }
}

void mort_unite(unite_t *unite){
    /*joueur->argent += unite->cout
     * free(unite) fait avec les threads
     * */
}

