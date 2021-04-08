#ifndef SOCKETTCP_CELLULE_TCP_H
#define SOCKETTCP_CELLULE_TCP_H
#include <arpa/inet.h>

#define MAX_JOUEURS 4

typedef struct cellule
{
    int socketClient[MAX_JOUEURS][2];
    unsigned int place_libre;
    int socketServeur;
    struct sockaddr_in adresseServeur;
    char *map;
    char *scenar;
    int port;
    char adresse[16];
    struct cellule *suivant;
    struct cellule *prec;
} cellule_tcp;

cellule_tcp *initialiser_cellule_tcp(char *adresse, int port, char *map, char *scenar);
void detruire_cellule_tcp(cellule_tcp *cellule);
#endif
