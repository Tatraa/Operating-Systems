#define _REENTRANT 		//kod wielokrotnego uzytku	
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "watki.h"

#define max_liczba_watkow 10

int liczba;
unsigned int liczba_watkow;
unsigned int liczba_sekcji;
pthread_t watki[max_liczba_watkow];
pthread_mutex_t mutex;
const double min_dlugosc_czekania = 1.0;
const double max_dlugosc_czekania = 4.0 - min_dlugosc_czekania;

void ustaw_kursor(unsigned x, unsigned y) {										//skopiowalem z pliku od profesora xD
	printf("\033[%d;%dH\033[2K", y, x);											//a pozniej bawilem sie wartosciamu
}

void *watekStart(void *argument) {
	
	int licznik=0;
	int numer_watku= *(int *)argument;
	srand(time(NULL));
	
	printf("Watek nr: %d, ID: %ld \n", numer_watku+1, watki[numer_watku]);
	
	sleep(3);
	
	for(int i=0; i<liczba_sekcji; i++) {
		
		ustaw_kursor(0, 4+liczba_watkow+numer_watku);
		printf("\033[32mWatek nr: %d, SP: %d, liczba: %d \033[0m\n", numer_watku+1, i+1, liczba);				//[033 i inne tego typu zmieniaja kolorki
		
		sleep((double)(rand()%(int)(max_dlugosc_czekania*100))/100+min_dlugosc_czekania);		//czekaj rnd czasu

		zamknij_mutex(&mutex);																			//sekcja krytyczna
		
		ustaw_kursor(50, 4+liczba_watkow+numer_watku);
		printf("\033[32mWatek nr: %d, SK: %d, liczba: %d \033[0m\n", numer_watku+1, i+1, liczba);
		
		licznik=liczba;																				//dodaj jeden do liczby
		licznik++;
		
		sleep((double)(rand()%(int)(max_dlugosc_czekania*100))/100+min_dlugosc_czekania);		//czekaj rnd czasu
		
		liczba=licznik;
		
		otworz_mutex(&mutex);																		//koniec sekcji krytycznej	
		
		ustaw_kursor(0, 4+liczba_watkow+numer_watku);
		printf("\033[32mWatek nr: %d, SP: %d, liczba: %d \033[0m\n", numer_watku+1, i+1, liczba);
		
		sleep((double)(rand()%(int)(max_dlugosc_czekania*100))/100+min_dlugosc_czekania);		//czekaj rnd czasu
	}
	return 0;
}

int main(int argc, char *argv[]) {
	
	if(argc!=3) {
		printf("Blad: niepoprawna liczba argumentow!\n");									//ilosc watkow, ilosc sekcji
		exit(EXIT_FAILURE);
	}
	
	liczba_watkow=atoi(argv[1]);
	if(liczba_watkow>max_liczba_watkow) {
		printf("Ilosc watkow przekracza dozwolona ilosc!\n");
		printf("Maksymalna dozwolona ilosc watkow: %d\n", max_liczba_watkow);
		exit(EXIT_FAILURE);
	}
	liczba_sekcji=atoi(argv[2]);
	
	inicjalizuj_mutex(&mutex);
	printf("Zainicjalizowano mutex o adresie %p\n", (void *)&mutex);
	
	liczba=0;
	
	for(int i=0; i<liczba_watkow; i++) {									//utworz podana liczbe watkow
		
		int *pamiec=malloc(sizeof(int));									//alokuje blok pamięci o rozmiarze size.
		*pamiec=i;
		
		sleep((double)i/liczba_watkow);												//tworzenie watku i przekazanie jego numeru jako argument
		watki[i]=utworz_watek(watekStart, (void *)pamiec);
	}
	
	for(int i=0; i<liczba_watkow; i++) {										//czekanie na zakonczenie watku
		polacz_watek(watki[i]);
	}
	
	ustaw_kursor(0, 4+liczba_watkow*2);
	
	if(liczba!=liczba_watkow*liczba_sekcji) {														//czy sekcja sie powiodla
		printf("Synchronizacja nie powiodla sie!\nOczekiwano: %d\nOtrzymano: %d\n", (liczba_watkow * liczba_sekcji), liczba);
	} else {
		printf("Synchronizacja powiodla sie!\nOczekiwano: %d\nOtrzymano: %d\n", (liczba_watkow * liczba_sekcji), liczba);
	}
	
	usun_mutex(&mutex);
	
	return 0;
}