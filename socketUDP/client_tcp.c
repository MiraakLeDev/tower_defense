/**
 * Ce programme permet d'envoyer un message au serveur via une socket TCP. 
 * Le message, passé en argument du programme, est envoyé en deux temps : 
 * sa taille, puis le contenu du message. L'adresse IP et le port du serveur
 * sont passés en arguments du programme.
 * @author Cyril Rabat
 **/
#include <stdlib.h>     /* Pour exit, EXIT_SUCCESS, EXIT_FAILURE */
#include <sys/socket.h> /* Pour socket */
#include <arpa/inet.h>  /* Pour IPPROTO_TCP */
#include <stdio.h>      /* Pour perror */
#include <unistd.h>     /* Pour close */
#include <string.h>     /* Pour memset */

#include <time.h>

int main(int argc, char *argv[]) {
    int fd;
    struct sockaddr_in adresse;

    /* Vérification des arguments */
    if(argc != 2) {
        fprintf(stderr, "Usage : %s adresseServeur portServeur\n", argv[0]);
        fprintf(stderr, "Où :\n");
        fprintf(stderr, "  adresseServeur : adresse IPv4 du serveur\n");
        fprintf(stderr, "  portServeur    : numéro de port du serveur\n");
        exit(EXIT_FAILURE);
    }

    /* Création de la socket */
    if((fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
        perror("Erreur lors de la création de la socket ");
        exit(EXIT_FAILURE);
    }

    printf("addr : %s port : %s",  argv[0],  argv[1]);

    /* Remplissage de la structure */
    memset(&adresse, 0, sizeof(struct sockaddr_in));
    adresse.sin_family = AF_INET;
    adresse.sin_port = htons(atoi(argv[1]));
    if(inet_pton(AF_INET, argv[0], &adresse.sin_addr.s_addr) != 1) {
        perror("Erreur lors de la conversion de l'adresse ");
        exit(EXIT_FAILURE);
    }

    sleep(5);

    /* Connexion au serveur */
    if(connect(fd, (struct sockaddr*)&adresse, sizeof(adresse)) == -1) {
        perror("Erreur lors de la connexion ");
        exit(EXIT_FAILURE);
    }

    printf("Je suis connecté au serveur TCP");
    sleep(5);

    /* Envoi du message au serveur
    taille = strlen(argv[2]) + 1;
    if(write(fd, &taille, sizeof(size_t)) == -1) {
    perror("Erreur lors de l'envoi de la taille du message ");
    exit(EXIT_FAILURE);
    }
    if(write(fd, argv[2], sizeof(char) * taille) == -1) {
    perror("Erreur lors de l'envoi du message ");
    exit(EXIT_FAILURE);
    }
    printf("Client : message envoyé.\n");
    */

    /* Fermeture de la socket */
    if(close(fd) == -1) {
        perror("Erreur lors de la fermeture de la socket ");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
