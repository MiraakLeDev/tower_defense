#include "jeu.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


jeu_t initialiser_jeu(){
    jeu_t jeu;
    jeu.vies = 10;
    jeu.argent = 2000;
    jeu.adv[0] = 10;
    jeu.adv[1] = 10;
    jeu.adv[2] = 10;

    return jeu;
}

/* Fonction qui remplie la matrice carte de jeu */
void set_map(jeu_t *jeu, int fichier)
{
    size_t taille_texte;
    unsigned char case_terrain;
    int i = 0, j = 0;

    lseek(fichier, 0, SEEK_SET);
    if (read(fichier, &taille_texte, sizeof(size_t)) < 0)
    {
        perror("ERREUR : Lecture de la map \n");
        exit(EXIT_FAILURE);
    }

    lseek(fichier, taille_texte, SEEK_CUR);
    while (read(fichier, &case_terrain, sizeof(unsigned char)) > 0)
    {
        if (j == 15)
        {
            j = 0;
            i++;
        }
        if (case_terrain >= 1 && case_terrain <= 3)
        {
            jeu->spawn[case_terrain][0] = i;
            jeu->spawn[case_terrain][1] = j;
        }
        else if (case_terrain == 254)
        {
            jeu->spawn[0][0] = i;
            jeu->spawn[0][1] = j;
        }
        jeu->carte[i][j] = case_terrain;
        j++;
    }
}

/* Fonction qui initialise la description du jeu */
void set_scenar(jeu_t *jeu, int fichier)
{
    size_t taille_texte;
    lseek(fichier, 0, SEEK_SET);
    if (read(fichier, &taille_texte, sizeof(size_t)) < 0)
    {
        perror("ERREUR : Lecture du scénario \n");
        exit(EXIT_FAILURE);
    }

    if (read(fichier, jeu->description, taille_texte) < 0)
    {
        perror("ERREUR : Lecture du scénario \n");
        exit(EXIT_FAILURE);
    }
    printf("description scenario : %s \n", jeu->description);
}