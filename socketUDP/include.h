#ifndef _INCLUDE_
#define _INCLUDE_

#define TAILLE_CSS_MAX   3 /* Taille Cartes Scenar Serveur MAX */
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
    char liste[TAILLE_CSS_MAX][MAX_CHAR+1];
}reponse_udp;

/* Reponse udp pour transmettre l'adresse et le port d'un serveur tcp */
typedef struct {
    char adresse[16];
    unsigned int port;
}reponse_3_udp;

/* Reponse udp pour transmettre la liste des serveurs, des cartes ou des scénarios */
typedef struct {
    int end;
    unsigned int nb_serveurs;
    char adresse[TAILLE_CSS_MAX][16];
    unsigned int port[TAILLE_CSS_MAX];
    char map[TAILLE_CSS_MAX][MAX_CHAR];
    char scenar[TAILLE_CSS_MAX][MAX_CHAR];
    unsigned int place_libre[TAILLE_CSS_MAX];
}reponse_4_udp;

#endif