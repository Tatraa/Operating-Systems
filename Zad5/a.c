//Kacper Tatrocki 20.04.2022, Krakow#include <stdio.h>#include <unistd.h>#include <stdlib.h>#include <sys/types.h>#include <sys/wait.h>#include <sys/stat.h>#include <fcntl.h>#include <string.h>#include <time.h>#include <stdlib.h>#include <signal.h>char potok[20];void wyjscie(void) {	unlink(potok);												//usuwanie plikow}void sygWyjscie(int signal) {	printf("\tUsuwanie potoku\n");	exit(EXIT_SUCCESS);}int main(int argc, char *argv[]) {		char sciezka[100]="./";	strcpy(potok, argv[5]);		// sprawdzenie poprawności argumentów	if(argc!=6) {		printf("Niepoprawna liczba argumentów!\n");		_exit(EXIT_FAILURE);	}		if(mkfifo(argv[5], 0644)==-1) {									//obluga bledow mkfifo - zrobi specjalny plik FIFO		perror("Blad potoku");		_exit(EXIT_FAILURE);	}		if(atexit(wyjscie)!=0) {										//obsluga bledow atexit - co ma dodatkowego wykonac		perror("Blad atexit'u");		_exit(EXIT_FAILURE);	}		if(signal(SIGINT,sygWyjscie)==SIG_ERR) {						//obsluga bledow signal		perror("Blad signal");		exit(EXIT_FAILURE);	}		srand(time(NULL));		for(int i=0; i<2; i++) {			//o to spytac w sumie - juz w sumie nie				switch(fork()) {			case -1:				perror("fork error");				exit(EXIT_FAILURE);			case 0:													//akcja dla procesu potomnego				if(i==0) {					char temporary[100];					sprintf(temporary, "%s%s", sciezka, argv[1]);					execl(temporary, argv[1], argv[3], argv[5], NULL); 	//uruchamiamy program producenta				} else {					char temporary[100];					sprintf(temporary, "%s%s", sciezka, argv[2]);					execl(temporary, argv[2], argv[4], argv[5], NULL); 	//uruchamiamy program konsumenta				}				break;							default:	break;											//akcja dla procesu macierzystego		}	}		int stat_lock;											//oczekiwanie na procesy potomne	if(wait(&stat_lock) == -1) {				perror ("Blad wait");		_exit (EXIT_FAILURE);	}	if(wait(&stat_lock) == -1) {										//obluga bledow		perror ("Blad wait");		_exit (EXIT_FAILURE);	}	return 0;}