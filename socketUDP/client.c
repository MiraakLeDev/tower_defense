/**
 * Ce programme permet d'envoyer un message au serveur via une socket UDP. 
 * L'adresse IP et le port du serveur sont passés en argument du programme.
 * @author Cyril Rabat
 **/
#include <stdlib.h>      /* Pour exit, EXIT_FAILURE, EXIT_SUCCESS */
#include <stdio.h>       /* Pour printf, fprintf, perror */
#include <sys/socket.h>  /* Pour socket */
#include <arpa/inet.h>   /* Pour sockaddr_in, inet_pton */
#include <string.h>      /* Pour memset */
#include <unistd.h>      /* Pour close */
#include <errno.h>       /* Pour errno */

#include "include.h"

int main(int argc, char *argv[]) {
    int sockfd, i=0;
    struct sockaddr_in adresseServeur;
    requete_udp requete;
    reponse_udp reponse;

    /* Vérification des arguments */
    if(argc != 4) {
        fprintf(stderr, "Usage : %s adresseServeur portServeur message\n", argv[0]);
        fprintf(stderr, "Où :\n");
        fprintf(stderr, "  adresseServeur : adresse IPv4 du serveur\n");
        fprintf(stderr, "  portServeur    : numéro de port du serveur\n");
        fprintf(stderr, "  message        : le message à envoyer\n");
        exit(EXIT_FAILURE);
    }

    /* Création de la socket */
    if((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        perror("Erreur lors de la création de la socket ");
        exit(EXIT_FAILURE);
    }

    /* Création de l'adresse du serveur */
    memset(&adresseServeur, 0, sizeof(struct sockaddr_in));
    adresseServeur.sin_family = AF_INET;
    adresseServeur.sin_port = htons(atoi(argv[2]));
    if(inet_pton(AF_INET, argv[1], &adresseServeur.sin_addr) != 1) {
        perror("Erreur lors de la conversion de l'adresse ");
        exit(EXIT_FAILURE);
    }

    requete.action = atoi(argv[3]);

    /* Envoi du message */
    if(sendto(sockfd, &requete, sizeof(requete_udp), 0, (struct sockaddr*)&adresseServeur, sizeof(struct sockaddr_in)) == -1) {
        perror("Erreur lors de l'envoi du message ");
        exit(EXIT_FAILURE);
    }
    printf("Client : message envoyé.\n");

    if(recv(sockfd, &reponse, 248, 0) < 0)
    {
        perror("recv()");
        exit(errno);
    } else {
        for (i=0;i<reponse.taille_liste;i++) {
            printf("%d. %s\n", i, reponse.liste[i]);
        }
    }

    /* Fermeture de la socket */
    if(close(sockfd) == -1) {
        perror("Erreur lors de la fermeture de la socket ");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
