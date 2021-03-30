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
#include <pthread.h>    /* Pour thread */

#include <time.h>
#include "jeu.h"
#include "ncurses.h"
#include "fenetre.h"
#include "interface.h"
typedef struct arguments{
  int socket;
  jeu_t* jeu;
  interface_t *interface;

}arguments_t;

void* scenario(void* args){
  unsigned int donnees=0;
  unsigned char type=0;
  char msg[255];
  arguments_t *arguments = (arguments_t*)args;

  while (type != 100) {

    if (recv(arguments->socket, &type, sizeof(unsigned char), 0) == -1) {
        perror("Erreur lors de la lecture de la taille du message ");
        exit(EXIT_FAILURE);
    }
    if ((int)type == 0) {
      if (recv(arguments->socket, &msg, sizeof(char)*255, 0) == -1) {
          perror("Erreur lors de la lecture de la taille du message ");
          exit(EXIT_FAILURE);
      }
      printf(" args %d ",type);
      printf("message %s\n",msg );
    }else{
      if (recv(arguments->socket, &donnees, sizeof(unsigned int), 0) == -1) {
          perror("Erreur lors de la lecture de la taille du message ");
          exit(EXIT_FAILURE);
      }
      printf(" args %d %d\n",type,donnees);
    }
  }

 printf("arguments : %d\n",arguments->socket);
 pthread_exit(NULL);
    return NULL;
}


int main(int argc, char *argv[]) {
    int fd;
    jeu_t jeu;
    struct sockaddr_in adresse;
    int ch;
    interface_t interface;
    bool quitter = FALSE;
    pthread_t thread;
    arguments_t arguments;

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

    sleep(1);

    /* Connexion au serveur */
    if(connect(fd, (struct sockaddr*)&adresse, sizeof(adresse)) == -1) {
        perror("Erreur lors de la connexion ");
        exit(EXIT_FAILURE);
    }

    printf("Je suis connecté au serveur TCP\n");
    /*Reception du jeu */
    if (recv(fd, (jeu_t *)&jeu, sizeof(jeu_t), 0) == -1) {
        perror("Erreur lors de la lecture de la taille du message ");
        exit(EXIT_FAILURE);
    }

    printf("Récupération de la map\n");
    printf("description :\n %s\n",jeu.description);
    /* Initialisation de ncurses */
    ncurses_initialiser();
    ncurses_souris();
    ncurses_couleurs();
    palette();
    clear();
    refresh();
    arguments.jeu = &jeu;
    arguments.socket = fd;
    /* Vérification des dimensions du terminal */
    if((COLS < LARGEUR) || (LINES < HAUTEUR)) {
        ncurses_stopper();
        fprintf(stderr,
                "Les dimensions du terminal sont insufisantes : l=%d,h=%d au lieu de l=%d,h=%d\n",
                COLS, LINES, LARGEUR, HAUTEUR);
        exit(EXIT_FAILURE);
    }

    /* Création de l'interface */
    /*interface = interface_creer(&jeu);*/
    pthread_create(&thread,NULL,&scenario,(void*)&arguments);
    pthread_join(thread,NULL);
    /* Boucle principale */
    while(quitter == FALSE) {
      ch = getch();

      if((ch == 'Q') || (ch == 'q'))
        quitter = true;
      else{
        interface_main(&interface, &jeu, ch);
        wprintw(interface.infos->interieur, "\ntest");
        wrefresh(interface.infos->interieur);
      }
    }

    /* Suppression de l'interface */
    interface_supprimer(&interface);

    /* Arrêt de ncurses */
    ncurses_stopper();

    /* Fermeture de la socket */
    if(close(fd) == -1) {
        perror("Erreur lors de la fermeture de la socket ");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
