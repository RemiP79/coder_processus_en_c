# Project: cinema

OBJ  = sem_op.o shm_op.o aleatoire.o
LINKOBJ  = $(OBJ)
BIN  = cinema entree
CFLAGS = -g #-Wall

all: cinema entree afficheur

cinema: $(LINKOBJ) shm_const.h cinema.c
	$(CC) cinema.c $(LINKOBJ) -o cinema $(CFLAGS)

# sortie: $(LINKOBJ) shm_const.h sortie.c
#	$(CC) sortie.c $(LINKOBJ) -o sortie $(CFLAGS)

entree: $(LINKOBJ) shm_const.h entree.c
	$(CC) entree.c $(LINKOBJ) -o entree $(CFLAGS)

afficheur: $(LINKOBJ) shm_const.h afficheur.c
	$(CC) afficheur.c $(LINKOBJ) -o afficheur $(CFLAGS)

shm_op.o: shm_op.c shm_const.h
	$(CC) -c shm_op.c $(CFLAGS)

sem_op.o: sem_op.c shm_const.h
	$(CC) -c sem_op.c $(CFLAGS)

aleatoire.o: aleatoire.c
	$(CC) -c aleatoire.c $(CFLAGS)

clean: 
	rm -f $(OBJ) $(BIN)


