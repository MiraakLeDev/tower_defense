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
#include "liste_adj.h"
#include "tour.h"
#include "ncurses.h"
#include "fenetre.h"
#include "interface.h"

jeu_t jeu;

typedef struct arguments
{
    int num;                /* Utilisé pour le thread scenario pour la socket + thread unite pour le type de l'unite*/
    interface_t *interface;
} arguments_t;

typedef struct arguments_unite
{
    unite_t unite;
    interface_t *interface;
    pthread_mutex_t *mutex;
} arg_unit;


void *spawn_unite(void *args)
{
    unite_t unite;
    arg_unit *arg_unite = (arg_unit *)args;
    cellule_unite *unite_c;

    /* on l'englobe dans une cellule et on l'ajoute à la bonne liste_adj */
    unite = arg_unite->unite;
    unite_c = initialiser_cellule_unite(&unite);

    ajouter_cellule_unite(&jeu.liste[unite.position[0]], unite_c);

    deplacement_unite(&unite, &jeu, arg_unite->interface);

    /* Une fois que l'unité a fini de se deplacer ou qu'elle s'est faite tuer, on la supprime en mémoire */
    supprimer_cellule_unite(&jeu.liste[unite.position[0]], &unite);
    pthread_exit(NULL);
}


void *scenario(void *args)
{
    unsigned int donnees = 0;
    unsigned char type = 0;
    char msg[255];
    arguments_t *arguments = (arguments_t *)args;
    arg_unit arg_unite;
    unite_t unite;
    int i = 0;
    while ((int)type != 4)
    {
        if (recv(arguments->num, &type, sizeof(unsigned char), 0) == -1)
        {
            perror("Erreur lors de la lecture de la taille du message ");
            exit(EXIT_FAILURE);
        }
        if ((int)type == 0)
        {
            if (recv(arguments->num, &msg, sizeof(char) * 255, 0) == -1)
            {
                perror("Erreur lors de la lecture de la taille du message ");
                exit(EXIT_FAILURE);
            }
            interface_message(arguments->interface, msg);
        }
        else
        {
            if (recv(arguments->num, &donnees, sizeof(unsigned int), 0) == -1)
            {
                perror("Erreur lors de la lecture de la taille du message ");
                exit(EXIT_FAILURE);
            }

            /* Initialise unite (comme ce sont des unités créées par le scénario, on les fait spawn au spawn Ordinateur = 0) */
            initialiser_unite(&unite, donnees);
            unite.position[0] = jeu.spawn[0][0];
            unite.position[1] = jeu.spawn[0][1];

            /* prépare les arguments à passer au thread */
            arg_unite.unite = unite;
            arg_unite.interface = arguments->interface;
            pthread_create(&unite.thread, NULL, spawn_unite, (void *)&arg_unite);
        }
    }
    free(arguments);
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    int fd;
    struct sockaddr_in adresse;
    int ch;
    interface_t interface;
    bool quitter = FALSE;
    pthread_t thread;
    arguments_t arguments;
    int i = 0;

    for (i = 0; i < 15; i++)
    {
        jeu.liste[i] = initialiser_liste_adj();
    }

    /* Vérification des arguments */
    if (argc != 2)
    {
        fprintf(stderr, "Usage : %s adresseServeur portServeur\n", argv[0]);
        fprintf(stderr, "Où :\n");
        fprintf(stderr, "  adresseServeur : adresse IPv4 du serveur\n");
        fprintf(stderr, "  portServeur    : numéro de port du serveur\n");
        exit(EXIT_FAILURE);
    }

    /* Création de la socket */
    if ((fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
    {
        perror("Erreur lors de la création de la socket ");
        exit(EXIT_FAILURE);
    }

    printf("addr : %s port : %s", argv[0], argv[1]);

    /* Remplissage de la structure */
    memset(&adresse, 0, sizeof(struct sockaddr_in));
    adresse.sin_family = AF_INET;
    adresse.sin_port = htons(atoi(argv[1]));
    if (inet_pton(AF_INET, argv[0], &adresse.sin_addr.s_addr) != 1)
    {
        perror("Erreur lors de la conversion de l'adresse ");
        exit(EXIT_FAILURE);
    }

    sleep(1);

    /* Connexion au serveur */
    if (connect(fd, (struct sockaddr *)&adresse, sizeof(adresse)) == -1)
    {
        perror("Erreur lors de la connexion ");
        exit(EXIT_FAILURE);
    }

    printf("Je suis connecté au serveur TCP\n");
    /*Reception du jeu */
    if (recv(fd, (jeu_t *)&jeu, sizeof(jeu_t), 0) == -1)
    {
        perror("Erreur lors de la lecture de la taille du message ");
        exit(EXIT_FAILURE);
    }

    printf("Récupération de la map\n");
    printf("description :\n %s\n", jeu.description);

    /* Initialisation de ncurses */
    ncurses_initialiser();
    ncurses_souris();
    ncurses_couleurs();
    palette();
    clear();
    refresh();
    /* Création de l'interface */
    interface = interface_creer(&jeu);
    wrefresh(interface.outils->interieur);
    /* Vérification des dimensions du terminal */
    if ((COLS < LARGEUR) || (LINES < HAUTEUR))
    {
        ncurses_stopper();
        fprintf(stderr,
                "Les dimensions du terminal sont insufisantes : l=%d,h=%d au lieu de l=%d,h=%d\n",
                COLS, LINES, LARGEUR, HAUTEUR);
        exit(EXIT_FAILURE);
    }
    arguments.num = fd;
    arguments.interface = &interface;

    jeu.liste[5] = initialiser_liste_adj();
    pthread_create(&thread, NULL, &scenario, (void *)&arguments);


    while (quitter == FALSE)
    {
        ch = getch();

        if ((ch == 'Q') || (ch == 'q'))
            quitter = true;
        else
        {
            interface_main(&interface, &jeu, ch);
            wrefresh(interface.infos->interieur);
        }
    }

    /* Suppression de l'interface */
    interface_supprimer(&interface);

    /* Arrêt de ncurses */
    ncurses_stopper();

    /* Fermeture de la socket */
    if (close(fd) == -1)
    {
        perror("Erreur lors de la fermeture de la socket ");
        exit(EXIT_FAILURE);
    }

    /*
    for (i = 0; i < 15; ++i) {
        supprimer_liste_adj(&jeu.liste[i]);
    }*/

    return EXIT_SUCCESS;
}
