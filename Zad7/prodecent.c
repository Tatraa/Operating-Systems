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

int main (int argc, char *argv[]) {
	
	int plik;
	int PDPodstawa;
	SegmentPD *PD;
	sem_t *semafor_producent;
	sem_t *semafor_konsument;
	int odczyt;
	
	// sprawdzenie poprawnosci argumentow
	if(argc!=5) {
		printf("Wywolaj program wpisujac: %s semafor_producent semafor_konsument nazwa_pd plik_we\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	
	srand(time(NULL));
	
	if((plik=open(argv[4], O_RDONLY, 0644)) == -1) {
		printf("Blad otwarcia pliku %s do odczytu\n", argv[1]);
		perror("Blad pliku");
		exit(EXIT_FAILURE);
	}
	
	semafor_producent=otworz_semafor(argv[1]);
	printf("Producent: otwarto semafor producenta %s deskryptor %p\n", argv[1], (void *)semafor_producent);
	
	semafor_konsument=otworz_semafor(argv[2]);
	printf("Producent: otwarto semafor konsumenta %s deskryptor %p\n", argv[2], (void *)semafor_konsument);
	
	PDPodstawa=otworz_PD(argv[3]);
	printf("Producent: pamiec dzielona %s deskryptor %d\n", argv[3], PDPodstawa);
	
	PD=(SegmentPD *)odwzoruj_PD(PDPodstawa, sizeof(SegmentPD));
	
	PD->zapis=0;
	
	while(1) {									// zapisywanie wyprodukowanych produktow do pamieci dzielonej
		
		sleep((double)(rand()%(int)(2.0*100))/100);					// czekaj losowa ilosc czasu
		
		printf("Producent: przed SekcjaKrytyczna semafor: %d\n", wartosc_semafor(semafor_producent));			// opusc semafor
		opusc_semafor(semafor_producent);
		
		odczyt=read(plik, PD->bufor[PD->zapis], NELE);
		if(odczyt==-1) {
			printf("Blad odczytu z pliku\n");
			exit(EXIT_FAILURE);
		} if(odczyt != NELE) {
			PD->bufor[PD->zapis][odczyt] = '\0';
			printf("\tProdukuje --> (%ld) %s\n", strlen(PD->bufor[PD->zapis]), PD->bufor[PD->zapis]);	// w strukturze ten konkretny argument
			printf("Producent: koniec pliku\n");
			
			podnies_semafor(semafor_konsument);														// podnies semafor konsumenta ostatni raz
			printf("Producent: po SekcjiKrytycznej semafor: %d\n", wartosc_semafor(semafor_konsument));
			break;
		} else {
			if(write(STDOUT_FILENO, "\tProdukuje --> (10) ", 19) == -1) {
				perror("Blad write na ekran");
				exit(EXIT_FAILURE);
			}
			if(write(STDOUT_FILENO, PD->bufor[PD->zapis], NELE) == -1) {
				perror("Blad write na ekran");
				exit(EXIT_FAILURE);
			}
			printf("\n\tIndeks bufora zapis: %d\n", PD->zapis);
		}
		PD->zapis=(PD->zapis+1)%NBUF;
		
		podnies_semafor(semafor_konsument);												// podnies semafor konsumenta
		printf("Producent: po SekcjiKrytycznej semafor: %d\n", wartosc_semafor(semafor_konsument));
	}
	
	if(close(plik)==-1) {
		perror("Blad zamknięcia pliku wyjścia");
		exit(EXIT_FAILURE);
	}
	
	usun_odwzorowanie_PD(PD, sizeof(SegmentPD));
	
	zamknij_PD(PDPodstawa);
	zamknij_semafor(semafor_producent);
	zamknij_semafor(semafor_konsument);
	
	printf("Producent: koniec procesu\n");
	
	return 0;
}