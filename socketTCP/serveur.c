/**
 * Ce programme se met en attente de connexion de la part d'un client. Une fois
 * la connexion établie, il se met en attente de la réception d'un message qui
 * est envoyé en deux temps : d'abord la taille, puis le contenu du message. 
 * Ce programme s'arrête dès la réception du message.
 * @author Cyril Rabat
 **/
#include <stdlib.h>     /* Pour exit, EXIT_SUCCESS, EXIT_FAILURE */
#include <sys/socket.h> /* Pour socket */
#include <arpa/inet.h>  /* Pour sockaddr_in, IPPROTO_TCP */
#include <stdio.h>      /* Pour perror */
#include <unistd.h>     /* Pour close */
#include <string.h>     /* Pour memset */
#include <pthread.h>

#include "liste_tcp.h"

#define MAX_PARTIES 2

liste_tcp* liste_serveurs;

int fork_partie(cellule_tcp* cellule){
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
    supprimer_cellule_tcp(liste_serveurs, cellule);
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
    cellule_tcp* cellule;
    int statut = 0, i = 0;
    pthread_t thread[MAX_PARTIES];

    liste_serveurs = initialiser_liste_tcp(NULL);

    while(i<MAX_PARTIES){
        cellule = initialiser_cellule_tcp("127.0.0.1",(49155+i),"map","scenar");

        ajouter_cellule_tcp(liste_serveurs, cellule);
        i++;
    }

    for (i = 0; i < MAX_PARTIES; ++i) {
        pthread_join(thread[i], NULL);
        printf("THREAD %d TERMINE\n",i);
    }

    printf("Serveur terminé.\n");

  return EXIT_SUCCESS;
}
