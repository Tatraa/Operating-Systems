//Kacper Tatrocki 14.05.2022, Krakow
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include "semafory.h"
#include <time.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>

//program tworzy podaną ilość kopii programu inkrementator który implementuje problem korzystania z zasobu współdzielonego

char nazwaSemafora[20];
bool synchro;

void wyjscie(void) {
	if(synchro) {
		usun_semafor(nazwaSemafora);
	}
}

void sygnal_wyjscie(int signal) {
	if(synchro) {
		printf("Sygnal przerwania, semafor zostaje usuniety\n");
		usun_semafor(nazwaSemafora);
	}
	_exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
	
	if(argc!=7) {							//sprawdzenie poprawnosci argumentow
		printf("Blad: niepoprawna liczba argumentow!\n");
		exit(EXIT_FAILURE);
	}
	
	if(atexit (wyjscie)!=0) {				//obsluga atexit
		perror("atexit error");
		exit(EXIT_FAILURE);
	}
	
	if(signal(SIGINT, sygnal_wyjscie)==SIG_ERR) {			//obsluga sygnału ctrl+c
		perror("Blad signal");
		exit(EXIT_FAILURE);
	}
	
	int stat_lock;
	int iloscInkrementatorow;
	int plik;
	
	char temp[strlen("./")+strlen(argv[1])+1]; 			//sciezka polaczona z nazwa pliku
	
	iloscInkrementatorow=atoi(argv[2]);
	
	strcpy(nazwaSemafora, argv[5]);
	
	if(atoi(argv[6])==0) {
		synchro=false;
	} else {
		synchro=true;
	}
	
	// stwórz plik
	if((plik=open (argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644))==-1) {
		printf("Blad otwarcia pliku %s do zapisu", argv[1]);
		perror("Blad pliku");
		exit(EXIT_FAILURE);
	}
	
	if(write(plik, "0", 2)==-1){
		perror("Blad write do pliku");
		exit(EXIT_FAILURE);
	}
	
	if(close(plik)==-1) {
		perror("Blad zamknięcia pliku");
		exit(EXIT_FAILURE);
	}
	
	if(synchro) {
		sem_t *semafor=stworz_semafor(nazwaSemafora);
		int wartoscSemafora;
		wartosc_semafor(semafor, &wartoscSemafora);
		printf("Utworzono semafor %s (%p) o wartości %d\n", nazwaSemafora, (void *)semafor, wartoscSemafora);
	}
	
	for(int i=0; i<iloscInkrementatorow; i++) {
		
		switch(fork()) {
			case -1:
				perror("fork error");
				exit(EXIT_FAILURE);
			case 0:												//akcja dla procesu potomnego
				sprintf(temp, "%s%s", "./", argv[1]);
				execl(temp, argv[1], argv[3], argv[4], argv[5], argv[6], NULL); //uruchom inkrementator
				perror("execl error");
				exit(EXIT_FAILURE);
				break;
					
			default: break;										//akcja dla procesu macierzystego				
		}
	}
	
	for(int i=0; i<iloscInkrementatorow; i++) {			//oczekiwanie na procesy potomne
		if(wait(&stat_lock)==-1) {
			perror("Blad wait");
			exit(EXIT_FAILURE);
		}
	}
	
	if((plik=open(argv[4], O_RDONLY, 0644))==-1) {
		printf("Blad otwarcia pliku %s do odczytu", argv[4]);
		perror("Blad pliku");
		exit(EXIT_FAILURE);
	}
	
	int odczyt;
	char bufor[10];
	
	odczyt=read(plik, &bufor, 10);
	if(odczyt==-1) {
		printf("Blad odczytu z pliku\n");
		exit(EXIT_FAILURE);
	} else if(odczyt==0) {
		printf("Plik jest pusty!\n");
		exit(EXIT_FAILURE);
	} else {
		bufor [odczyt]='\0';
		if(atoi(bufor)==(atoi(argv[2])*atoi(argv[3]))) {
			printf("\nInkrementacja zakonczona pomyslnie! Liczba w pliku: %s\n", bufor);
		} else {
			printf("\nInkrementacja nie powiodla sie! Liczba w pliku: %s, oczekiwano: %d\n", bufor, (atoi (argv[2]) * atoi (argv[3])));
		}
	}
	if(close(plik)==-1) {
		perror("Blad zamkniecia pliku");
		exit(EXIT_FAILURE);
	}	
	return 0;
}