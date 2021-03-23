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
#include <sys/wait.h>
#include <time.h>

#include "include.h"
#include "liste_tcp.h"

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


/**** PARTIE SERVEUR TCP ****/
int fork_partie(liste_tcp* liste_serveurs, cellule_tcp* cellule){
    int cmp = 0;
    char* msg;
    size_t taille;
    while (cmp < 4) {
        /* Mise en mode passif de la socket */
        if (listen(cellule->socketServeur, 1) == -1) {
            perror("Erreur lors de la mise en mode passif ");
            exit(EXIT_FAILURE);
        }

        /* Attente d'une connexion */
        printf("Serveur : attente de connexion...\n");
        if ((cellule->socketClient[cmp] = accept(cellule->socketServeur, NULL, NULL)) == -1) {
            perror("Erreur lors de la demande de connexion ");
            exit(EXIT_FAILURE);
        }

        /* Lecture du message */
        if (read(cellule->socketClient[cmp], &taille, sizeof(size_t)) == -1) {
            perror("Erreur lors de la lecture de la taille du message ");
            exit(EXIT_FAILURE);
        }
        if ((msg = (char *) malloc(sizeof(char) * taille)) == NULL) {
            perror("Erreur lors de l'allocation mémoire pour le message ");
            exit(EXIT_FAILURE);
        }
        if (read(cellule->socketClient[cmp], msg, sizeof(char) * taille) == -1) {
            perror("Erreur lors de la lecture de la taille du message ");
            exit(EXIT_FAILURE);
        }
        printf("Serveur : message recu '%s'.\n", msg);
        cmp++;
    }
    supprimer_cellule_tcp(liste_serveurs ,cellule);
    return 0;
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

    /**** SERVEURS TCP ****/
    pid_t pid_serveur;
    liste_tcp* liste_serveurs;
    cellule_tcp* cellule;

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


    /* Préparation de la récupération de l'adresse du client */
    memset(&adresseClient, 0, sizeof(struct sockaddr_in));

    /* Nommage de la socket */
    if(bind(sockfd, (struct sockaddr*)&adresseServeur, sizeof(struct sockaddr_in)) == -1) {
        perror("Erreur lors du nommage de la socket ");
        exit(EXIT_FAILURE);
    }

    taille_socket = sizeof(adresseClient);

    /**** INITIALISATION LISTE DES SERVEURS TCP ****/
    liste_serveurs = initialiser_liste_tcp(NULL);
    srand (time (NULL));

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

            else if(requete.action == 3){
                if ((pid_serveur = fork()) == 0) {
                    cellule = initialiser_cellule_tcp("127.0.0.1", rand() % 64511 + 1024, "map", "scenar");
                    ajouter_cellule_tcp(liste_serveurs, cellule);
                    fork_partie(liste_serveurs, cellule);
                    exit(EXIT_SUCCESS);
                }
                else if(pid_serveur == -1){
                    perror("ERREUR : Création fils TCP impossible\n");
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
