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
    int sockfd, i=0, action = -1, choix_map = -1, choix_scenar = -1;
    struct sockaddr_in adresseServeur;
    requete_udp requete;
    reponse_udp reponse;
    reponse_2_udp reponse_serveur;
    char port_tcp[6];

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

    action = atoi(argv[3]);
    switch (action) {
            /* DEMANDE DE CARTES */
        case 1:
            /* DEMANDES DE SCENARIOS */
        case 2:
            /* DEMANDE DE LA LISTE DES PARTIES */
        case 4:
            requete.action = action;
            /* Envoi du message */
            printf("Client : envoi de votre demande...\n");
            if (sendto(sockfd, &requete, sizeof(requete_udp), 0, (struct sockaddr *) &adresseServeur,
                       sizeof(struct sockaddr_in)) == -1) {
                perror("Erreur lors de l'envoi du message ");
                exit(EXIT_FAILURE);
            }

            break;
            /* CREATION D'UNE PARTIE */
        case 3:
            requete.action = 1;
            /* Demande des cartes */
            printf("Client : demande des cartes disponibles...\n");
            if (sendto(sockfd, &requete, sizeof(requete_udp), 0, (struct sockaddr *) &adresseServeur,
                       sizeof(struct sockaddr_in)) == -1) {
                perror("Erreur lors de l'envoi du message ");
                exit(EXIT_FAILURE);
            }
            if (recv(sockfd, &reponse, TAILLE_CS_MAX * (MAX_CHAR + 1) + sizeof(unsigned int), 0) < 0) {
                perror("recv()");
                exit(errno);
            }
            printf("Veuillez choisir parmi les cartes ci-dessous : \n");
            for (i = 0; i < reponse.taille_liste; i++) {
                printf("%d. %s\n", i, reponse.liste[i]);
            }

            do {
                if (scanf("%d[^\n]", &choix_map) == -1) {
                    perror("ERREUR dans le choix de la carte ");
                    exit(EXIT_FAILURE);
                }
            } while (choix_map > reponse.taille_liste - 1 || choix_map < 0);
            requete.choix_map = choix_map;
            printf("Choix enregistré \n\n");

            /* Demande des scénarios */
            requete.action = 2;
            /* Envoi du message */
            printf("Client : demande des scénarios disponibles...\n");
            if (sendto(sockfd, &requete, sizeof(requete_udp), 0, (struct sockaddr *) &adresseServeur,
                       sizeof(struct sockaddr_in)) == -1) {
                perror("Erreur lors de l'envoi du message ");
                exit(EXIT_FAILURE);
            }
            if (recv(sockfd, &reponse, sizeof(reponse_udp), 0) < 0) {
                perror("recv()");
                exit(errno);
            }
            printf("Veuillez choisir parmi les scénarios ci-dessous : \n");
            for (i = 0; i < reponse.taille_liste; i++) {
                printf("%d. %s\n", i, reponse.liste[i]);
            }

            do {
                if (scanf("%d[^\n]", &choix_scenar) == -1) {
                    perror("ERREUR dans le choix du scénario ");
                    exit(EXIT_FAILURE);
                }
            } while (choix_scenar > reponse.taille_liste - 1 || choix_scenar < 0);
            requete.choix_scenar = choix_scenar;
            printf("Choix enregistré \n\n");

            requete.action = 3;
            printf("Client : demande de la création d'une partie...\n");
            if (sendto(sockfd, &requete, sizeof(requete_udp), 0, (struct sockaddr *) &adresseServeur,sizeof(struct sockaddr_in)) == -1) {
                perror("Erreur lors de l'envoi du message ");
                exit(EXIT_FAILURE);
            }

            if (recv(sockfd, &reponse_serveur, sizeof(reponse_2_udp), 0) < 0) {
                perror("recv()");
                exit(errno);
            }
            /* Fermeture de la socket UDP */
            if (close(sockfd) == -1) {
                perror("Erreur lors de la fermeture de la socket ");
                exit(EXIT_FAILURE);
            }
            sprintf(port_tcp, "%d" ,reponse_serveur.port); /* conversion port int en string */
            printf("execl = %d", execl("./client_tcp", reponse_serveur.adresse, port_tcp, NULL));
            break;
    }
    return EXIT_SUCCESS;
}
