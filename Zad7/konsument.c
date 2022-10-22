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
#include "semafory.h"
#include "padzielona.h"

#define NELE 10 // Rozmiar elementu bufora (jednostki towaru) w bajtach
#define NBUF 5 // Liczba elementow bufora

// Segment pamieci dzielonej
typedef struct {
	char bufor[NBUF][NELE]; // Wspolny bufor danych
	int zapis;
	int odczyt;				// Pozycje wstawiania i wyjmowania z bufora
} SegmentPD;

int OB(SegmentPD *PD) {					// sygnal konca danych , po odczytaniu \0 konsument zakonczy prace (ostatni bloks)
	int i;
	for(i=0; i<NELE; i++) {
		if(PD->bufor[PD->odczyt][i]=='\0') {			// w strukturze ten konkretny argument
			return 1;
		}
	}
	return 0;
}

int main(int argc, char *argv[]) {
	
	int plik;
	int PDPodstawa;
	SegmentPD *PD;
	sem_t *semafor_producent;
	sem_t *semafor_konsument;
	
	// sprawdzenie poprawności argumentów
	if(argc!=5) {
		printf("Wywolaj program wpisujac: %s semafor_producent semafor_konsument nazwa_pd plik_wy\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	
	srand(time(NULL));
	
	if((plik=open (argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1) {
		printf("Blad otwarcia pliku %s do zapisu", argv[1]);
		perror("Blad pliku");
		exit(EXIT_FAILURE);
	}
	
	semafor_producent=otworz_semafor(argv[1]);
	printf("Konsument: otwarto semafor producenta %s deskryptor %p\n", argv[1], (void *)semafor_producent);

	semafor_konsument=otworz_semafor(argv[2]);
	printf("Konsument: otwarto semafor konsumenta %s deskryptor %p\n", argv[2], (void *)semafor_konsument);
	
	PDPodstawa=otworz_PD(argv[3]);
	printf("Konsument: pamiec dzielona %s deskryptor %d\n", argv[3], PDPodstawa);
	
	PD=(SegmentPD *)odwzoruj_PD(PDPodstawa, sizeof(SegmentPD));
	
	PD->odczyt=0;
	
	while(1) {												// zapisywanie skosumowanych produktow do pliku
		sleep((double)(rand()%(int)(2.0*100))/100);					// czekaj losowa ilosc czasu
		
		// opuść semafor
		printf("Konsument: przed SekcjaKrytyczna semafor: %d\n", wartosc_semafor(semafor_konsument));
		opusc_semafor(semafor_konsument);
		
		if(!OB(PD)) {												// skopiuj caly blok
			if(write (plik, PD->bufor[PD->odczyt], NELE) == -1) {
				perror("Blad write do potoku");
				exit(EXIT_FAILURE);
			}
			if(write(STDOUT_FILENO, "\tKonsumuje --> (10) ", 19) == -1) {
				perror("Blad write na ekran");
				exit(EXIT_FAILURE);
			}
			if(write(STDOUT_FILENO, PD->bufor[PD->zapis], NELE) == -1) {
				perror("Blad write na ekran");
				exit(EXIT_FAILURE);
			}
			printf("\n\tIndeks bufora odczyt: %d\n", PD->odczyt);
		} else {

			if(write (plik, PD->bufor[PD->odczyt], strlen(PD->bufor[PD->odczyt])) == -1) {						// skopiuj ostatni fragment
				perror("Blad write do potoku");
				exit(EXIT_FAILURE);
			}
			printf("\tKonsumuje <-- (%ld) %s\n", strlen(PD->bufor[PD->odczyt]), PD->bufor[PD->odczyt]);
			printf("Konsument: Sygnal konca odczytu\n");
			
			podnies_semafor(semafor_producent);										// podnies semafor producenta 
			printf("Konsument: po SekcjiKrytycznej semafor: %d\n", wartosc_semafor (semafor_producent));
			
			break;
		}
		
		PD->odczyt=(PD->odczyt+1)%NBUF;
		
		// podnieś semafor producenta
		podnies_semafor(semafor_producent);
		printf("Konsument: po SekcjiKrytycznej semafor: %d\n", wartosc_semafor (semafor_producent));
	}
	
	if(close(plik)==-1) {
		perror("Blad zamkniecia pliku wyjscia");
		exit(EXIT_FAILURE);
	}
	
	usun_odwzorowanie_PD(PD, sizeof(SegmentPD));
	
	zamknij_PD(PDPodstawa);
	zamknij_semafor(semafor_konsument);
	zamknij_semafor(semafor_producent);
	
	printf("Konsument: koniec procesu\n");
	
	return 0;
}