#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>
#include "padzielona.h"
#include "semafory.h"

#define NELE 20 // Rozmiar elementu bufora (jednostki towaru) w bajtach
#define NBUF 5 // Liczba elementow bufora

#define SEMAFOR_PRODUCENT "/sem_producent"
#define SEMAFOR_KONSUMENT "/sem_konsument"
#define NAZWA_PD "/prod_kons_pd"

// Segment pamieci dzielonej
typedef struct {
	char bufor[NBUF][NELE]; // Wspolny bufor danych
	int zapis;
	int odczyt;
} SegmentPD;		

void wyjscie(void) {
	usun_PD(NAZWA_PD);
	usun_semafor(SEMAFOR_PRODUCENT);
	usun_semafor(SEMAFOR_KONSUMENT);
}

void sygnalWyjscie(int signal) {
	printf("\nSygnal przerwania, usuwanie semaforow i pamieci dzielonej\n");
	exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
	
	int PDPodstawa;
	int stat_lock;
	sem_t *semafor;
	
	// sprawdzenie poprawności argumentów
	if(argc!=3) {
		printf("Wywołaj program wpisujac: %s program_producent program_konsument\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	
	semafor=stworz_semafor(SEMAFOR_PRODUCENT, NBUF);
	printf("Utworzono semafor PRODUCENTA %s (%p) o wartosci poczatkowej %d\n", SEMAFOR_PRODUCENT, (void *)semafor, wartosc_semafor(semafor));

	semafor=stworz_semafor(SEMAFOR_KONSUMENT, 0);
	printf("Utworzono semafor KONSUMENTA %s (%p) o wartosci poczatkowej %d\n", SEMAFOR_KONSUMENT, (void *)semafor, wartosc_semafor(semafor));
	
	PDPodstawa=stworz_PD(NAZWA_PD);
	ustaw_rozmiar_PD(PDPodstawa, sizeof(SegmentPD));
	printf("Utworzono pamiec dzielona %s o deskryptorze %d i rozmiarze %ld\n\n\n", NAZWA_PD, PDPodstawa, sizeof(SegmentPD));
	
	if(atexit(wyjscie)!=0) {
		perror("atexit error");
		_exit(EXIT_FAILURE);
	}
	
	if(signal(SIGINT,sygnalWyjscie)==SIG_ERR) {
		perror("Blad signal");
		exit(EXIT_FAILURE);
	}
	
	srand(time(NULL));
	
	for(int i=0; i<2; i++) {
		
		switch(fork ()) {
			case -1:
				perror("fork error");
				exit(EXIT_FAILURE);
			case 0:
														//akcja dla procesu potomnego
				if(i==0) {
					char temp[20];
					sprintf(temp, "%s%s", "./", argv[1]);
					execl(temp, argv[1], SEMAFOR_PRODUCENT, SEMAFOR_KONSUMENT, NAZWA_PD, "surowce", NULL); // urucham program producenta
					perror("blad execl");
					exit(EXIT_FAILURE);
				} else {
					char temp[20];
					sprintf(temp, "%s%s", "./", argv[2]);
					execl(temp, argv[2], SEMAFOR_PRODUCENT, SEMAFOR_KONSUMENT, NAZWA_PD, "skonsumowane", NULL); // urucham program konsumenta
					perror("blad execl");
					exit(EXIT_FAILURE);
				}
				break;
				
			default:		break;						// akcja dla procesu macierzystego
		}
	}

	if(wait(&stat_lock)==-1) {							// oczekiwanie na procesy potomne
		perror("Blad wait");	
		_exit(EXIT_FAILURE);
	}
	
	if(wait(&stat_lock)==-1) {
		perror("Blad wait");
		_exit(EXIT_FAILURE);
	}
	
	zamknij_PD(PDPodstawa);
	printf("\n\nKoniec procesu!\n");
	
	return 0;
}