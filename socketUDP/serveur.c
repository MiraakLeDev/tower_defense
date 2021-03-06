#include <stdlib.h> /* Pour exit, EXIT_FAILURE, EXIT_SUCCESS */
#include <stdio.h>  /* Pour printf, fprintf, perror */
#include <signal.h>
#include <sys/socket.h> /* Pour socket, bind */
#include <arpa/inet.h>  /* Pour sockaddr_in */
#include <string.h>     /* Pour memset */
#include <unistd.h>     /* Pour close */
#include <fcntl.h>
#include <errno.h>  /* Pour errno */
#include <dirent.h> /* Pour parcourir les dossiers */
#include <sys/wait.h>
#include <time.h>
#include <pthread.h>

#include "include.h"
#include "liste_tcp.h"
#include "jeu.h"

#define NB_MAPS 3
#define NB_SCENAR 3

int stop = 0;
liste_tcp *liste_serveurs;
void handler(int signum)
{
    if (signum == SIGINT)
    {
        stop = 1;
    }
}

/* Permet de récupérer le contenu d'un dossier */
void recup_data(DIR *d, char *nom_dossier, char contenu[][30])
{
    int i = 0;
    int namelen;
    char name[30];
    struct dirent *dir;
    d = opendir(nom_dossier);
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            strcpy(name, dir->d_name);
            namelen = strlen(name);
            name[namelen] = '\0';
            if (namelen > 4)
            {
                /* on filtre les fichiers .bin */
                if (strcmp(name + namelen - 4, ".bin") == 0)
                {
                    name[namelen - 4] = 0;
                    strcpy(contenu[i], name);
                    i++;
                }
            }
        }
        closedir(d);
    }
    else
    {
        printf("Dossier %s manquant\n", nom_dossier);
        exit(EXIT_FAILURE);
    }
}

/**** PARTIE SERVEUR TCP ****/
/* Fonction qui le scénario contenu dans fichier */
void read_scenar(jeu_t *jeu, int fichier, cellule_tcp *cellule)
{
    unsigned int donnees = 0; /* les données de l'évenement */
    unsigned char type;       /* type d'evenement */
    long temps = 0;           /* temps en millisecondes */
    char msg[255];            /* message lors d'évenements */
    int i = 0;
    while (read(fichier, &temps, sizeof(long)) > 0)
    {
        if (read(fichier, &type, sizeof(unsigned char)) < 0)
        {
            perror("ERREUR : Lecture du scénario \n");
            exit(EXIT_FAILURE);
        }
        for (i = 0; i < MAX_JOUEURS; i++)
        {
            if (send(cellule->socketClient[i][0], &type, sizeof(unsigned char), 0) == -1)
            {
                perror("Erreur lors de l'envoi du message ");
                exit(EXIT_FAILURE);
            }
        }

        if ((int)type == 0)
        {
            if (read(fichier, &msg, sizeof(char) * 255) < 0)
            {
                perror("ERREUR : Lecture du scénario \n");
                exit(EXIT_FAILURE);
            }
            for (i = 0; i < MAX_JOUEURS; i++)
            {
                if (send(cellule->socketClient[i][0], &msg, sizeof(char) * 255, 0) == -1)
                {
                    perror("Erreur lors de l'envoi du message ");
                    exit(EXIT_FAILURE);
                }
            }
        }
        else
        {
            if (read(fichier, &donnees, sizeof(unsigned int)) < 0)
            {
                perror("ERREUR : Lecture du scénario \n");
                exit(EXIT_FAILURE);
            }
            for (i = 0; i < MAX_JOUEURS; i++)
            {
                if (send(cellule->socketClient[i][0], &donnees, sizeof(unsigned int), 0) == -1)
                {
                    perror("Erreur lors de l'envoi du message ");
                    exit(EXIT_FAILURE);
                }
            }
        }
        sleep(temps / 1000);
    }

    /* une fois le scénario fini on le signal au client */
    type = 4;
    for (i = 0; i < MAX_JOUEURS; i++)
    {
        if (send(cellule->socketClient[i][0], &type, sizeof(unsigned char), 0) == -1)
        {
            perror("Erreur lors de l'envoi du message ");
            exit(EXIT_FAILURE);
        }
    }
}

/* thread qui gère le relai d'unités entre deux joueurs */
void *thread_send(void *args)
{
    arguments_ennemi *arguments = (arguments_ennemi *)args;
    send_unite unite;
    while (1)
    {
        /* si on reçoit un envoi d'unité on la transfert au joueur visé */
        if (recv(arguments->socket_joueur, &unite, sizeof(send_unite), 0) == -1)
        {
            perror("Erreur lors de la lecture de la taille du message ");
            exit(EXIT_FAILURE);
        }

        if (send(arguments->socket_ennemi[unite.numero_client - 1], &unite, sizeof(send_unite), 0) == -1)
        {
            perror("Erreur lors de l'envoi du message ");
            exit(EXIT_FAILURE);
        }
        sleep(1);
    }
    pthread_exit(NULL);
}

void *thread_partie(void *arg_cellule)
{
    int cmp = MAX_JOUEURS - 1, fichier; /* cmp compte le nombre de joueurs connectés */
    char map_nom[MAX_CHAR + 7] = "cartes/";
    char scenar_nom[MAX_CHAR + 10] = "scenarios/";
    cellule_tcp *cellule = (cellule_tcp *)arg_cellule;
    jeu_t jeu;
    pthread_t thread_j[MAX_JOUEURS]; /* Un thread par joueur est dédié à écouter et relayer des unités entre les joueurs */
    arguments_ennemi ennemi[MAX_JOUEURS];
    int i = 0, j = 0;
    int pret;
    jeu = initialiser_jeu();

    /* CHARGEMENT DE LA CARTE */
    strcat(map_nom, cellule->map);
    strcat(map_nom, ".bin");
    if ((fichier = open(map_nom, O_RDWR)) == -1)
    {
        perror("Erreur lors de l'ouverture du fichier ");
        exit(EXIT_FAILURE);
    }
    set_map(&jeu, fichier);
    close(fichier);

    /* CHARGEMENT DU SCENARIO */
    strcat(scenar_nom, cellule->scenar);
    strcat(scenar_nom, ".bin");
    if ((fichier = open(scenar_nom, O_RDWR)) == -1)
    {
        perror("Erreur lors de l'ouverture du fichier ");
        exit(EXIT_FAILURE);
    }
    set_scenar(&jeu, fichier);

    /* On attend la connexion des quatres joueurs avant de commencer la partie */
    while (cmp >= 0)
    {

        /* Mise en mode passif de la socket */
        if (listen(cellule->socketServeur, 1) == -1)
        {
            perror("Erreur lors de la mise en mode passif ");
            exit(EXIT_FAILURE);
        }

        /* Attente de deux connexions (1ere connexion = socket d'envoi du scénario | 2e connexion = socket permettant de faire communiquer les adversaires)*/
        printf("Serveur port = %d : attente de connexion de %d joueurs...\n", ntohs(cellule->adresseServeur.sin_port), cmp + 1);
        if ((cellule->socketClient[cmp][0] = accept(cellule->socketServeur, NULL, NULL)) == -1)
        {
            perror("Erreur lors de la demande de connexion ");
            exit(EXIT_FAILURE);
        }
        if ((cellule->socketClient[cmp][1] = accept(cellule->socketServeur, NULL, NULL)) == -1)
        {
            perror("Erreur lors de la demande de connexion ");
            exit(EXIT_FAILURE);
        }
        cellule->place_libre--;
        cmp--;
    }
    ennemi->socket_serveur = cellule->socketServeur;

    /*Envoi du jeu aux clients*/
    for (cmp = 0; cmp < MAX_JOUEURS; cmp++)
    {
        ennemi[cmp].socket_joueur = cellule->socketClient[cmp][1];
        j = 0;
        for (i = 0; i < MAX_JOUEURS; i++)
        {
            /* on ajute la socket des joueurs ennemis dans le tableau de sockets */
            if (ennemi[cmp].socket_joueur != cellule->socketClient[i][1])
            {
                ennemi[cmp].socket_ennemi[j] = cellule->socketClient[i][1];
                j++;
            }
        }
        if (send(cellule->socketClient[cmp][0], (void *)&jeu, sizeof(jeu_t), 0) == -1)
        {
            perror("Erreur lors de l'envoi du message ");
            exit(EXIT_FAILURE);
        }
        pthread_create(&thread_j[cmp], NULL, thread_send, (void *)&ennemi[cmp]);
    }

    /*On attend que tous les clients soient prets*/
    for (i = 0; i < MAX_JOUEURS; i++)
    {
        if (recv(cellule->socketClient[i][0], &pret, sizeof(int), 0) == -1)
        {
            perror("Erreur lors de reception du message");
            exit(EXIT_FAILURE);
        }
    }
    pret = 20;
    /*On envoie aux clients que la partie peut commencer*/
    for (i = 0; i < MAX_JOUEURS; i++)
    {
        if (send(cellule->socketClient[i][0], &pret, sizeof(int), 0) == -1)
        {
            perror("Erreur lors de l'envoi du message ");
            exit(EXIT_FAILURE);
        }
    }
    read_scenar(&jeu, fichier, cellule); /*on envoie le scénario*/
    close(fichier);
    supprimer_cellule_tcp(liste_serveurs, cellule);
    printf("Le serveur fils s'éteint\n");
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    int sockfd, i = 0;
    int taille_socket = 0;
    int nb_serveurs = 0;

    struct sockaddr_in adresseServeur, adresseClient;
    requete_udp requete;
    reponse_udp reponse;
    reponse_3_udp reponse_serveur;
    reponse_4_udp reponse_liste_serveurs;

    DIR *d = NULL;
    char maps[NB_MAPS][30];             /* les maps du jeux */
    char scenars[NB_SCENAR][30];        /* les scénarios du jeux */
    char adresse_tcp[16] = "127.0.0.1"; /* adresse des serveurs */
    unsigned int port_tcp;

    struct sigaction action_signal;

    /**** SERVEURS TCP ****/
    pthread_t thread_serveur;
    cellule_tcp *cellule;

    /* Récupération des arguments */
    if (argc != 2)
    {
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
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
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
    if (bind(sockfd, (struct sockaddr *)&adresseServeur, sizeof(struct sockaddr_in)) == -1)
    {
        perror("Erreur lors du nommage de la socket ");
        exit(EXIT_FAILURE);
    }

    taille_socket = sizeof(adresseClient);

    /**** INITIALISATION LISTE DES SERVEURS TCP ****/
    liste_serveurs = initialiser_liste_tcp(NULL);
    srand(time(NULL));

    while (stop == 0)
    {
        /* Attente de la requête du client */
        printf("Serveur en attente du message du client...\n");
        if (recvfrom(sockfd, &requete, sizeof(requete_udp), 0, (struct sockaddr *)&adresseClient, (socklen_t *)&taille_socket) == -1)
        {
            if (errno != EINTR)
            {
                perror("Erreur lors de la réception du message ");
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            printf("Serveur : message recu type = %d.\n", requete.action);
            /**** DEMANDE DE MAPS ****/
            if (requete.action == 1)
            {
                reponse.taille_liste = NB_MAPS;
                for (i = 0; i < NB_MAPS; i++)
                {
                    strcpy(reponse.liste[i], maps[i]);
                    printf("%d. %s\n", i, reponse.liste[i]);
                }
                /* Envoi du message */
                if (sendto(sockfd, &reponse, sizeof(reponse), 0, (struct sockaddr *)&adresseClient, sizeof(struct sockaddr_in)) == -1)
                {
                    perror("Erreur lors de l'envoi du message ");
                    exit(EXIT_FAILURE);
                }
            }

            /**** DEMANDE DE SCENARIOS ****/
            else if (requete.action == 2)
            {
                reponse.taille_liste = NB_SCENAR;
                for (i = 0; i < NB_SCENAR; i++)
                {
                    strcpy(reponse.liste[i], scenars[i]);
                    printf("%d. %s\n", i, reponse.liste[i]);
                }
                /* Envoi du message */
                if (sendto(sockfd, &reponse, sizeof(reponse), 0, (struct sockaddr *)&adresseClient, sizeof(struct sockaddr_in)) == -1)
                {
                    perror("Erreur lors de l'envoi du message ");
                    exit(EXIT_FAILURE);
                }
            }

            /**** DEMANDE NOUVELLE PARTIE ****/
            else if (requete.action == 3)
            {
                /* on choisit un port TCP pour notre serveur */
                port_tcp = rand() % 64511 + 1024;
                /* on initialise le serveur TCP */
                cellule = initialiser_cellule_tcp(adresse_tcp, port_tcp, maps[requete.choix_map], scenars[requete.choix_scenar]);
                /* on l'ajoute à notre ensemble de serveurs déja en ligne */
                ajouter_cellule_tcp(liste_serveurs, cellule);
                printf("Le serveur TCP a démarré sur le port : %d\n", port_tcp);

                /* On prépare la réponse pour le client -> données : adresse du serveur TCP + port */
                reponse_serveur.port = port_tcp;
                strcpy(reponse_serveur.adresse, adresse_tcp);
                if (sendto(sockfd, &reponse_serveur, sizeof(reponse_3_udp), 0, (struct sockaddr *)&adresseClient, sizeof(struct sockaddr_in)) == -1)
                {
                    perror("Erreur lors de l'envoi du message ");
                    exit(EXIT_FAILURE);
                }
                /* On dédie un thread pour notre serveur TCP */
                if (pthread_create(&thread_serveur, NULL, thread_partie, (void *)cellule) != 0)
                {
                    printf("Problème création du thread serveur tcp");
                    exit(EXIT_FAILURE);
                }
            }

            /**** LISTE PARTIES ****/
            else if (requete.action == 4)
            {
                /* on récupère le nombre de serveurs TCP en ligne */
                nb_serveurs = nb_cellules(liste_serveurs);
                reponse_liste_serveurs.nb_serveurs = nb_serveurs;

                /* Si il existe au moins un serveur en ligne alors : */
                if (nb_serveurs > 0)
                {
                    cellule = liste_serveurs->premier;
                    i = 0;
                    /* On parcourt les serveurs en ligne */
                    while (cellule != NULL)
                    {
                        /* tant qu'on ne dépasse pas la taille max d'envoi de paquet on continue  */
                        if (i < TAILLE_CSS_MAX)
                        {
                            reponse_liste_serveurs.port[i] = cellule->port;
                            strcpy(reponse_liste_serveurs.adresse[i], cellule->adresse);
                            strcpy(reponse_liste_serveurs.map[i], cellule->map);
                            strcpy(reponse_liste_serveurs.scenar[i], cellule->scenar);
                            reponse_liste_serveurs.place_libre[i] = cellule->place_libre;
                            cellule = cellule->suivant;
                            i++;
                        }
                        /* Si on dépasse la taille max alors on envoie */
                        else
                        {
                            reponse_liste_serveurs.nb_serveurs = i;
                            reponse_liste_serveurs.end = 0;
                            if (sendto(sockfd, &reponse_liste_serveurs, sizeof(reponse_4_udp), 0, (struct sockaddr *)&adresseClient, sizeof(struct sockaddr_in)) == -1)
                            {
                                perror("Erreur lors de l'envoi du message ");
                                exit(EXIT_FAILURE);
                            }
                            sleep(1);
                            i = 0;
                        }
                    }
                    reponse_liste_serveurs.nb_serveurs = i;
                    /* si end == 1 cela veut dire que c'est le dernier paquet tcp envoyé, la liste de serveur a été envoyé en totalité */
                    reponse_liste_serveurs.end = 1;
                }
                if (sendto(sockfd, &reponse_liste_serveurs, sizeof(reponse_4_udp), 0, (struct sockaddr *)&adresseClient, sizeof(struct sockaddr_in)) == -1)
                {
                    perror("Erreur lors de l'envoi du message ");
                    exit(EXIT_FAILURE);
                }
            }
            printf("\n\n");
        }
    }
    /* Fermeture de la socket */
    if (close(sockfd) == -1)
    {
        perror("Erreur lors de la fermeture de la socket ");
        exit(EXIT_FAILURE);
    }

    printf("Serveur terminé.\n");

    if (d != NULL)
    {
        free(d);
    }
    return EXIT_SUCCESS;
}
