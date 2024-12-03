#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "shm_const.h"

extern int creer_initialiser_semaphore();
extern int * attacher_segment_memoire();
extern int P();
extern int V();

extern int creer_segment_memoire();

pid_t afficheur_pid;

void terminer_afficheur() {
    if (afficheur_pid > 0) {
        kill(afficheur_pid, SIGTERM); // Envoyer SIGTERM à l'afficheur
    }
}

int main(int argc, char *argv[]) {
    pid_t pid_entree; /* no du processus du processus entree  */
    pid_t pid_pere;

    int code_retour_fin_entree;

    int nombre_places_cinema; /* Pour écriture dans la shm */
    char * nombre_places_cinema_str; /* Pour conversion du semid (int) en chaine */

    int nombre_caisses_cinema; /* Pour écriture dans la shm */
    char * nombre_caisses_cinema_str; /* Pour conversion du semid (int) en chaine */

    char * titre_film_cinema;

    int shmid; /* Id du segment de mémoire partagé */
    int semid; /* Id du sémaphore */

    char shmid_str[20]; /* Pour conversion du shmid (int) en chaine */
    char semid_str[20]; /* Pour conversion du semid (int) en chaine */

    int * mem; /* Adresse du segment de mémoire partagée */

    char param_gnome_terminal[80];

    if (argc != 4) {
        fprintf(stderr, "Usage : %s nombre_caisses titre_film nombre_places\n", argv[0]);
        return(3);
    }

    nombre_places_cinema_str = argv[3];
    nombre_places_cinema = atoi(nombre_places_cinema_str);

    nombre_caisses_cinema_str = argv[1];
    nombre_caisses_cinema = atoi(nombre_caisses_cinema_str);

    titre_film_cinema = argv[2];

    /* Avant de créer les fils :
     * - on crée le semaphore
     * - on crée le segment de mémoire partagé
     * - on s'attache le segment de mémoire partagé
     * - on initialise le noombre de places de parking libres dans la shm
     */

    /* Création sémaphore */
    creer_initialiser_semaphore(CLE_SEM, &semid);

    /* Création segment de mémoire partagé */
    creer_segment_memoire(CLE_SHM, &shmid);

    /* Attachement du segment de mémoire partagée */
    mem = attacher_segment_memoire(mem, &shmid);

    /* Pas besoin de sémaphore on est seul :-) */
    *mem = nombre_places_cinema;

    /* Conversion des shmid et semid  (int) en chaine pour appel programme externe */
    sprintf(shmid_str, "%d", shmid);
    sprintf(semid_str, "%d", semid);


 // Création du processus afficheur
    int status_afficheur;
    afficheur_pid = fork();
    if (afficheur_pid == 0) {
        printf("Afficheur");
        execl("./afficheur", "./afficheur", NULL);
        perror("Erreur execl");
        exit(1);
    } 
  
    pid_t pid;
    pid_t pere_pid;
    int status_fils;

    pere_pid = getpid();

    if (pere_pid < 0) {
        perror("pb fork");
        exit(1);
    }
    for (int i = 0; i < nombre_caisses_cinema; i++) {     
        pid = getpid();
        if (pid == pere_pid) {
            pid = fork();
            if (pid == 0) {
                sleep(1);
                printf("Caisse %d (PID : %d)\n", i + 1, getpid());
                fflush(stdout);
                execl("./entree", "./entree", shmid_str, semid_str, NULL);
            } else {
                waitpid(pid, &status_fils, WNOHANG);
            }
        }
    }

    // Attendre que tous les processus entree se terminent
    for (int i = 0; i < nombre_caisses_cinema; i++) {
        wait(NULL);
    }

     if (afficheur_pid > 0) {
        // Processus père
        printf("Plus de place pour %s .\n",titre_film_cinema);
        waitpid(pid, &status_afficheur, 0); // Attendre que l'enfant se termine
        terminer_afficheur();
    }

        // Détacher et supprimer la mémoire partagée
        shmdt(mem);
        shmctl(shmid, IPC_RMID, NULL);

        //printf("Tous les processus sont terminés.\n");
        return 0;
    }

