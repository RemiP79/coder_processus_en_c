#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>

/******************************************************************************/
/*
 * Fonctions externes
*/
/******************************************************************************/
extern void attente_aleatoire();
extern int billets_aleatoire();
extern int * attacher_segment_memoire();
extern int P();
extern int V();

/******************************************************************************/
/*
 * Fonctions 
*/
/******************************************************************************/

bool entree_personne(int *mem, int semid, int nb_billet) {                                       
    bool client_suivant=false;
    /* On protège l'accès à la shm */
    P(semid);

    /* Reste-t-il des places libres ? */
    if (*mem == 0) {
      /* No more */     
      printf("Caisse fermé. \n");
      V(semid);
      bool client_suivant=false;
      exit(0);  //fermer la caisse
    }
    else if (nb_billet <= *mem){       
        *mem=(*mem - nb_billet);        
        client_suivant = true;
    }

    /* On protège l'accès à la shm */
    V(semid);
    return (client_suivant);
}


/******************************************************************************/
/*
 * Programme principal
*/
/******************************************************************************/
int main(int argc, char *argv[]) {
    
    unsigned int  delais=1;

    int shmid=atoi(argv[1]);
    int semid=atoi(argv[2]);

    int *mem;

    int i = 0;
    int nb_billet;

    /* Attachement du segment de mémoire partagée */
    mem=attacher_segment_memoire(mem, &shmid);
        
    while (1) {        
        attente_aleatoire(delais); //mettre un delais compris entre 1 et 3 ok
        nb_billet = billets_aleatoire(7);
        i++;  
        printf("CAISSE N° : %d ; Une personne %d se présente pour %d billets \n", getpid(), i, nb_billet);
        while (entree_personne(mem, semid, nb_billet) == false) {
            sleep(1); 
        }
        sleep(5);        
    }

return(0);
}
