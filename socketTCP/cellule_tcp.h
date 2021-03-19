#ifndef SOCKETTCP_CELLULE_TCP_H
#define SOCKETTCP_CELLULE_TCP_H
#include <arpa/inet.h>
typedef struct cellule
{
    int sockclient[4];
    struct sockaddr_in socktab[4];
    char* map;
    char* scenar;
    struct cellule* suivant;
}cellule_tcp;

cellule_tcp initialiser_cellule_tcp(char* map, char* scenar);
void modif_cellule_tcp(cellule_tcp* cellule_tcp, int sockclient, struct sockaddr_in socktab, int id_client);

#endif
