#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define ATTENTE_MAX 10

void attente_aleatoire(unsigned int delais) {

/* Initialisation du désordre */
srandom(getpid() ^ time(NULL) % delais);

/* Attente */
sleep((unsigned int)(random() % ATTENTE_MAX));

}


int billets_aleatoire(int billets) {
    srandom(getpid() ^ time(NULL));
    return random()% billets+1;
}
