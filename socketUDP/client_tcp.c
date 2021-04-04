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
pthread_mutex_t mutex;
jeu_t jeu;
typedef struct arguments{
  int num; /*Utilisé pour le thread scenario pour la socket + thread unite pour le type de l'unite*/
  interface_t *interface;
  int id_joueur;

}arguments_t;
void deplacement_unite(unite_t* unite,jeu_t* jeu,interface_t* interface){

    while (unite->vie > 0 && trouver_chemin(jeu->carte,unite) !=1){
        pthread_mutex_lock(&mutex);
        if (strcmp(unite->nom,"Soldat") == 0) {
          mvwprintw(interface->carte->interieur,unite->position[0],unite->position[1],"x");
        }else if (strcmp(unite->nom,"Commando") == 0) {
          mvwprintw(interface->carte->interieur,unite->position[0],unite->position[1],"y");
        }else if (strcmp(unite->nom,"Vehicule") == 0) {
          mvwprintw(interface->carte->interieur,unite->position[0],unite->position[1],"u");
        }else if (strcmp(unite->nom,"Missile") == 0) {
          mvwprintw(interface->carte->interieur,unite->position[0],unite->position[1],"w");
        }else if (strcmp(unite->nom,"Char") == 0) {
          mvwprintw(interface->carte->interieur,unite->position[0],unite->position[1],"s");
        }else{
          mvwprintw(interface->carte->interieur,unite->position[0],unite->position[1],"?");
        }
        wrefresh(interface->carte->interieur);

        pthread_mutex_unlock(&mutex);
        usleep(unite->vitesse*1000);
        pthread_mutex_lock(&mutex);
        mvwaddch(interface->carte->interieur,unite->position[0],unite->position[1],' ' | COLOR_PAIR(COULEUR_CHEMIN));
        wrefresh(interface->carte->interieur);
        pthread_mutex_unlock(&mutex);



    }
}
void* spawn(void* args){
    unite_t unite;
    arguments_t *unite_arg = (arguments_t*)args;

    initialiser_unite(&unite,unite_arg->num);
    unite.position[0]= jeu.spawn[unite_arg->id_joueur][0];
    unite.position[1]= jeu.spawn[unite_arg->id_joueur][1];
    deplacement_unite(&unite,&jeu,unite_arg->interface);
    /*wprintw(unite_arg->interface->infos->interieur,"\nUnite detruite");*/
    /*wrefresh(unite_arg->interface->infos->interieur);*/
    pthread_exit(NULL);

}
void* scenario(void* args){
    unsigned int donnees=0;
    unsigned char type=0;
    char msg[50];
    arguments_t *arguments = (arguments_t*)args;
    arguments_t unite;
    pthread_t thread[100];
    int cmp=0;
    while ((int)type != 4) {
        if (recv(arguments->num, &type, sizeof(unsigned char), 0) == -1) {
            perror("Erreur lors de la lecture de la taille du message ");
            exit(EXIT_FAILURE);
        }
        if ((int)type == 0) {
            if (recv(arguments->num, &msg, sizeof(char)*50, 0) == -1) {
                perror("Erreur lors de la lecture de la taille du message ");
                exit(EXIT_FAILURE);
            }
            pthread_mutex_lock(&mutex);
            wprintw(arguments->interface->infos->interieur,"\nMessage : %s",msg);
            wrefresh(arguments->interface->infos->interieur);
            pthread_mutex_unlock(&mutex);
        }
        else{
            if (recv(arguments->num, &donnees, sizeof(unsigned int), 0) == -1) {
                perror("Erreur lors de la lecture de la taille du message ");
                exit(EXIT_FAILURE);
        }

            /*Spawn unites*/
            unite.num = donnees;
            unite.interface = arguments->interface;
            unite.id_joueur = arguments->id_joueur;
            pthread_create(&thread[cmp],NULL,&spawn,(void*)&unite);

        }

        cmp++;
    }
    free(arguments);
    pthread_exit(NULL);
}


int main(int argc, char *argv[]) {
    int fd;
    struct sockaddr_in adresse;
    int ch;
    interface_t interface;
    bool quitter = FALSE;
    pthread_t thread;
    arguments_t arguments;
    int cmp=0;

    if (pthread_mutex_init(&mutex, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }

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
    /* Création de l'interface */
    interface = interface_creer(&jeu);
    jeu.argent = 2000;
    wrefresh(interface.outils->interieur);
    /* Vérification des dimensions du terminal */
    if((COLS < LARGEUR) || (LINES < HAUTEUR)) {
        ncurses_stopper();
        fprintf(stderr,
                "Les dimensions du terminal sont insufisantes : l=%d,h=%d au lieu de l=%d,h=%d\n",
                COLS, LINES, LARGEUR, HAUTEUR);
        exit(EXIT_FAILURE);
    }
    arguments.num = fd;
    arguments.interface = &interface;
    arguments.id_joueur = 0;
    pthread_create(&thread,NULL,&scenario,(void*)&arguments);

    while(quitter == FALSE) {
      ch = getch();

      if((ch == 'Q') || (ch == 'q'))
        quitter = true;
      else{
        interface_main(&interface, &jeu, ch,&cmp);
        cmp++;
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
    pthread_mutex_destroy(&mutex);

    return EXIT_SUCCESS;
}
