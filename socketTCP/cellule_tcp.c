#include "cellule_tcp.h"
#include <string.h>



cellule_tcp initialiser_cellule_tcp(char* map, char* scenar){
    cellule_tcp cellule;
    cellule.map = map;
    cellule.scenar = scenar;
    /*strcpy(cellule.map,map);
    strcpy(cellule.scenar,scenar);*/
    cellule.suivant = NULL;
    return cellule;
}
void modif_cellule_tcp(cellule_tcp* cellule_tcp, int sockclient, struct sockaddr_in socktab, int id_client){

    cellule_tcp->sockclient[id_client] = sockclient;
    cellule_tcp->socktab[id_client] = socktab;
}