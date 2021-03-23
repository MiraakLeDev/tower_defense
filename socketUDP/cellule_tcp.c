#include "cellule_tcp.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>     /* Pour close */



cellule_tcp* initialiser_cellule_tcp(char* adresse, int port, char* map, char* scenar){

    cellule_tcp* cellule = malloc(sizeof(cellule_tcp));
    cellule->map = map;
    cellule->scenar = scenar;

    /* Création de la socket */
    if((cellule->socketServeur = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
        perror("Erreur lors de la création de la socket ");
        exit(EXIT_FAILURE);
    }

    /* Création de l'adresse du serveur */
    memset(&cellule->adresseServeur, 0, sizeof(struct sockaddr_in));
    cellule->adresseServeur.sin_family = AF_INET;
    cellule->adresseServeur.sin_addr.s_addr = inet_addr(adresse);
    cellule->adresseServeur.sin_port = htons(port);

    /* Nommage de la socket */
    if(bind(cellule->socketServeur, (struct sockaddr*)&cellule->adresseServeur, sizeof(struct sockaddr_in)) == -1) {
        perror("Erreur lors du nommage de la socket ");
        exit(EXIT_FAILURE);
    }

    cellule->suivant = NULL;
    cellule->prec = NULL;
    return cellule;
}
void modif_cellule_tcp(cellule_tcp* cellule_tcp, int sockclient, struct sockaddr_in socktab, int id_client){
/*
    cellule_tcp->sockclient[id_client] = sockclient;
    cellule_tcp->socktab[id_client] = socktab;
*/
}

void detruire_cellule_tcp(cellule_tcp* cellule)
{
    int i;
    for (i = 0; i < 4; ++i) {
        /* Fermeture des sockets */
        if(close(cellule->socketClient[i]) == -1) {
            perror("Erreur lors de la fermeture de la socket de communication ");
            exit(EXIT_FAILURE);
        }
    }

    if(close(cellule->socketServeur) == -1) {
        perror("Erreur lors de la fermeture de la socket de connexion ");
        exit(EXIT_FAILURE);
    }
    free(cellule);
}