#ifndef _INCLUDE_
#define _INCLUDE_

#define TAILLE_CS_MAX   10 /* Taille Cartes Scenar MAX */
#define MAX_CHAR        30 /* nombre max de caractères par titre */

typedef struct {
    unsigned int action; /* 1 : demande des maps | 2 : demande de scénario | 3 : démarrage partie | 4 : demande des parties démarrées */
    /* Uniquement pour choix 3 */
    unsigned int choix_map;
    unsigned int choix_scenar;
}requete_udp;

typedef struct {
    unsigned int taille_liste;
    char liste[TAILLE_CS_MAX][MAX_CHAR+1];
}reponse_udp;


#endif