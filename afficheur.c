#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include "shm_const.h"
#include <stdio.h>

extern int * attacher_segment_memoire(int * mem, int *shmid);

int *mem;
int shmid;

void terminer_proprement(int sig) {
    // Détacher la mémoire partagée
    if (shmdt(mem) == -1) {
        perror("shmdt");
    }
    // Terminer le processus
    exit(0);
}

int main(int argc, char *argv[]) {
    // Récupérer l'ID du segment de mémoire partagée
    if ((shmid = shmget(CLE_SHM, SHM_SIZE, 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    // Attacher le segment de mémoire partagée
    mem = attacher_segment_memoire(mem, &shmid);

    signal(SIGINT, terminer_proprement);

    while (1) {
        printf("Nombre de places restantes : %d\n", *mem);
        sleep(5);
    }

    // Détacher la mémoire partagée (ne sera jamais atteint dans cette boucle infinie)
    shmdt(mem);

    return 0;
}
