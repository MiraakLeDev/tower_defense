#ifndef SOCKETTCP_CELLULE_TCP_H
#define SOCKETTCP_CELLULE_TCP_H
#include <arpa/inet.h>
typedef struct cellule
{
    int socketClient[4];
    int socketServeur;
    struct sockaddr_in adresseServeur;
    char* map;
    char* scenar;
    struct cellule* suivant;
    struct cellule* prec;
}cellule_tcp;

cellule_tcp* initialiser_cellule_tcp(char* adresse, int port, char* map, char* scenar);
void modif_cellule_tcp(cellule_tcp* cellule_tcp, int sockclient, struct sockaddr_in socktab, int id_client);
void detruire_cellule_tcp(cellule_tcp* cellule);
#endif
