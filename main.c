#include <stdio.h>
#include "unite.h"

int main()
{
    unite_t soldat;
    soldat = initialiser_unite();
    printf("hp : %f\n", soldat.vie_max);
    return 0;
}
