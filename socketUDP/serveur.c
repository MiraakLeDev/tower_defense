/**
 * Ce programme se met en attente d'un message de la part d'un client, via une
 * socket UDP. Ce programme s'arrête dès la réception du message.
 * @author Cyril Rabat
 **/
#include <stdlib.h>      /* Pour exit, EXIT_FAILURE, EXIT_SUCCESS */
#include <stdio.h>       /* Pour printf, fprintf, perror */
#include <signal.h>
#include <sys/socket.h>  /* Pour socket, bind */
#include <arpa/inet.h>   /* Pour sockaddr_in */
#include <string.h>      /* Pour memset */
#include <unistd.h>      /* Pour close */
#include <errno.h>       /* Pour errno */
#include <dirent.h>      /* Pour parcourir les dossiers */

#include "include.h"

#define NB_MAPS 3
#define NB_SCENAR 3

int stop = 0;
void handler(int signum)
{
    if(signum == SIGINT){
        stop = 1;
    }
}

/**** Permet de récupérer le contenu d'un dossier ****/
void recup_data(DIR* d, char* nom_dossier, char contenu[][30]){
    int i = 0;
    int namelen;
    char name[30];
    struct dirent *dir;
    d = opendir(nom_dossier);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            strcpy(name, dir->d_name);
            namelen = strlen(name);
            name[namelen] = '\0';
            if (namelen > 4) {
                /* on filtre les fichiers .bin */
                if(strcmp(name + namelen - 4, ".bin") == 0) {
                    name[namelen - 4] = 0;
                    printf("%s\n", name);
                    strcpy(contenu[i], name);
                    i++;
                }
            }

        }
        closedir(d);
    }
    else {
        printf("Dossier %s manquant\n", nom_dossier);
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {

    int sockfd, i=0;
    int taille_socket = 0;

    struct sockaddr_in adresseServeur, adresseClient;
    requete_udp requete;
    reponse_udp reponse;

    DIR* d = NULL;
    char maps[NB_MAPS][30];         /* les maps du jeux */
    char scenars[NB_SCENAR][30];    /* les scénarios du jeux */

    struct sigaction action_signal;

    /* Récupération des arguments */
    if(argc != 2) {
        fprintf(stderr, "Usage : %s port\n", argv[0]);
        fprintf(stderr, "Où :\n");
        fprintf(stderr, "  port           : port d'écoute du serveur\n");
        exit(EXIT_FAILURE);
    }

    /* Initialisation gestionnaire de signaux SIGINT */
    action_signal.sa_handler = handler;
    sigemptyset(&action_signal.sa_mask);
    action_signal.sa_flags = 0;

    if (sigaction(SIGINT, &action_signal, NULL) == -1)
    {
        perror("Erreur lors du positionnement ");
        exit(EXIT_FAILURE);
    }

    /**** CARTES + SCENARIO ****/
    recup_data(d, "cartes", maps);
    recup_data(d, "scenarios", scenars);

    /* Création de la socket */
    if((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        perror("Erreur lors de la création de la socket ");
        exit(EXIT_FAILURE);
    }

    /* Création de l'adresse du serveur */
    memset(&adresseServeur, 0, sizeof(struct sockaddr_in));
    adresseServeur.sin_family = AF_INET;
    adresseServeur.sin_port = htons(atoi(argv[1]));
    adresseServeur.sin_addr.s_addr = inet_addr("127.0.0.1");
    /*
    adresseServeur.sin_addr.s_addr = htonl(INADDR_ANY);
    */

    /* Préparation de la récupération de l'adresse du client */
    memset(&adresseClient, 0, sizeof(struct sockaddr_in));


    /* Nommage de la socket */
    if(bind(sockfd, (struct sockaddr*)&adresseServeur, sizeof(struct sockaddr_in)) == -1) {
        perror("Erreur lors du nommage de la socket ");
        exit(EXIT_FAILURE);
    }

    taille_socket = sizeof(adresseClient);

    while (stop == 0) {
        /* Attente de la requête du client */
        printf("Serveur en attente du message du client...\n");
        if (recvfrom(sockfd, &requete, sizeof(requete_udp), 0, (struct sockaddr*)&adresseClient, (socklen_t*)&taille_socket) == -1) {
            if(errno != EINTR) {
                perror("Erreur lors de la réception du message ");
                exit(EXIT_FAILURE);
            }
        }
        else{
            printf("Serveur : message recu '%d'.\n", requete.action);
            /**** DEMANDE DE MAPS ****/
            if (requete.action == 1) {
                reponse.taille_liste = NB_MAPS;
                for (i=0;i<NB_MAPS;i++){
                    strcpy(reponse.liste[i],maps[i]);
                    printf("%d. %s\n", i, reponse.liste[i]);
                }
                /* Envoi du message */
                if(sendto(sockfd, &reponse, sizeof(reponse), 0, (struct sockaddr*)&adresseClient, sizeof(struct sockaddr_in)) == -1) {
                    perror("Erreur lors de l'envoi du message ");
                    exit(EXIT_FAILURE);
                }
            }

            /**** DEMANDE DE SCENARIOS ****/
            else if(requete.action == 2){
                reponse.taille_liste = NB_SCENAR;
                for (i=0;i<NB_SCENAR;i++){
                    strcpy(reponse.liste[i],scenars[i]);
                    printf("%d. %s\n", i, reponse.liste[i]);
                }
                /* Envoi du message */
                if(sendto(sockfd, &reponse, sizeof(reponse), 0, (struct sockaddr*)&adresseClient, sizeof(struct sockaddr_in)) == -1) {
                    perror("Erreur lors de l'envoi du message ");
                    exit(EXIT_FAILURE);
                }
            }
        }
    }
    /* Fermeture de la socket */
    if(close(sockfd) == -1) {
    perror("Erreur lors de la fermeture de la socket ");
    exit(EXIT_FAILURE);
    }

    printf("Serveur terminé.\n");

    return EXIT_SUCCESS;
}