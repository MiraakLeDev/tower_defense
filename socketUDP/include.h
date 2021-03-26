#ifndef _INCLUDE_
#define _INCLUDE_

#define TAILLE_CS_MAX   10 /* Taille Cartes Scenar MAX */
#define MAX_CHAR        30 /* nombre max de caractères par titre */

/* Requete UDP pour demander une info au serveur */
typedef struct {
    unsigned int action; /* 1 : demande des maps | 2 : demande de scénario | 3 : démarrage partie | 4 : demande des parties démarrées */
    /* Uniquement pour choix 3 */
    unsigned int choix_map;
    unsigned int choix_scenar;
}requete_udp;

/* Reponse udp pour transmettre la liste des serveurs, des cartes ou des scénarios */
typedef struct {
    unsigned int taille_liste;
    char liste[TAILLE_CS_MAX][MAX_CHAR+1];
}reponse_udp;

/* Reponse udp pour transmettre l'adresse et le port d'un serveur tcp */
typedef struct {
    char adresse[16];
    unsigned int port;
}reponse_2_udp;


#endif