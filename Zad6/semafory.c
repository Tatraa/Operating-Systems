//Kacper Tatrocki 10.05.2022, Krakow
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//wlasna biblioteka funkcji do tworzenia, otwierania, uzyskiwania wartosci, operowania, zamykania i usuwania semafora
sem_t *stworz_semafor(const char *name) {
	sem_t *adres_semafora;
	if((adres_semafora=sem_open(name, O_CREAT | O_EXCL, 0644, 1))==SEM_FAILED) {
		perror("Blad tworzenia semafora");
		exit(EXIT_FAILURE);
	} else {
		return adres_semafora;
	}
}

sem_t *otworz_semafor(const char *name) {
	sem_t *adres_semafora;
	if((adres_semafora=sem_open(name, 0))==SEM_FAILED) {
		perror("Blad otworzenia semafora");
		exit(EXIT_FAILURE);
	} else {
		return adres_semafora;
	}
}

void zamknij_semafor(sem_t *sem_t) {
	if(sem_close(sem_t)==-1) {
		perror("Blad zamykania semafora");
		exit(EXIT_FAILURE);
	}
}

void wartosc_semafor(sem_t *sem, int *value) {
	if(sem_getvalue(sem, value)==-1) {
		perror("Blad odczytywania wartosci semafora");
		exit(EXIT_FAILURE);
	}
}

void podnies_semafor(sem_t *sem) {
	if(sem_post(sem)==-1) {
		perror("Blad podnoszenia semafora");
		exit(EXIT_FAILURE);
	}
}

void opusc_semafor(sem_t *sem) {
	if(sem_wait(sem)==-1) {
		perror("Blad opuszczania semafora");
		exit(EXIT_FAILURE);
	}
}

void usun_semafor(const char *name) {
	if(sem_unlink(name)==-1) {
		perror("Blad usuwania semafora");
		exit(EXIT_FAILURE);
	}
}