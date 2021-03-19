#ifndef _INCLUDE_
#define _INCLUDE_

#define TAILLE_CS_MAX 10 /* Taille Cartes Scenar MAX */

typedef struct {

    unsigned int action; /* 1 : demande des maps | 2 : demande de scénario | 3 : démarrage partie | 4 : demande des parties démarrées */
}requete_udp;

typedef struct {
    unsigned int taille_liste;
    char liste[TAILLE_CS_MAX][30];
}reponse_udp;


#endif