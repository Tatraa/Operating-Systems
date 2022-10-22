//Kacper Tatrocki 14.05.2022, Krakow
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include "semafory.h"
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>

//program odczytuje plik numer.txt, inkrementuje zawartą w nim liczbę i zapisuje ją z powrotem do pliku


int main(int argc, char *argv[]) {
	
	int odczyt;
	char wiadomosc[250];
	char buf[100];
	int liczba;
	int iloscSekcjiKrytycznych;
	sem_t *semafor;
	int wartoscSemafora;
	int plik;
	bool synchro;
	
	if(argc!=5) {
		printf("Blad: niepoprawna liczba argumentow!\n");
		_exit(EXIT_FAILURE);
	}
	
	iloscSekcjiKrytycznych=atoi(argv[1]);					//pobieranie danych z argumentow
	
	if(atoi(argv[4])==0) {
		synchro=false;
	} else {
		synchro=true;
	}
	
	if(synchro) {
		semafor=otworz_semafor(argv[3]);
	}
	
	for(int i=0; i<iloscSekcjiKrytycznych; i++) {
		
		srand(time(NULL));								//wlasne sprawy
		
		sleep((double)(rand()%(int)(2.5*100))/100);			//czekaj losowa ilosc czasu
		
		if(synchro) {									//opusc semafor
			wartosc_semafor(semafor, &wartoscSemafora);
			printf("PID %d przed SekcjaKrytyczna semafor: %d\n", getpid(), wartoscSemafora);
			opusc_semafor(semafor);
		}
		
		if((plik=open(argv[2], O_RDONLY, 0644)) == -1) {			//sekcja krytyczna
			printf("Blad otwarcia pliku %s do odczytu", argv[2]);
			perror("Blad pliku");
			_exit(EXIT_FAILURE);
		}
		
		odczyt=read(plik, &buf, 10);
		if(odczyt==-1) {
			printf("Blad odczytu z pliku\n");
			_exit(EXIT_FAILURE);
		} else if (odczyt==0) {
			break;
		} else {
			buf[odczyt]='\0';
			wartosc_semafor(semafor, &wartoscSemafora);
			sprintf(wiadomosc, "\tPID %d semafor %d odczytany numer: %s SekcjaKrytyczna: %d", getpid(), wartoscSemafora, buf, i);
			printf("%s\n", wiadomosc);
		}
		
		if(close(plik)==-1) {
			perror("Blad zamknięcia pliku");
			exit(EXIT_FAILURE);
		}
		
		liczba=atoi(buf);
		liczba++;
		sprintf(buf, "%d", liczba);
		printf("liczba %d", liczba);
		
		//czekaj losową ilość czasu
		sleep((double)(rand()%(int)(2.5*100))/100);
		
		if((plik=open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644))==-1) {
			printf("Blad otwarcia pliku %s do zapisu", argv[1]);
			perror("Blad pliku");
			_exit(EXIT_FAILURE);
		}
		
		if(write(plik, &buf, strlen(buf))==-1){
			perror("Blad write do pliku");
			_exit(EXIT_FAILURE);
		}
		
		if(close(plik)==-1) {
			perror("Blad zamknięcia pliku");
			exit(EXIT_FAILURE);
		}
		
		//podnieś semafor
		if(synchro) {
			podnies_semafor(semafor);
			wartosc_semafor(semafor, &wartoscSemafora);
			printf("PID %d po SekcjiKrytycznej semafor: %d\n", getpid(), wartoscSemafora);
		}
	}
	
	if(synchro) {
		zamknij_semafor(semafor);
	}	
	return 0;
}