#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <mqueue.h>
#include "kolejki.h"

mqd_t serwer;

void wyjscie(void) {
	kolejka_zamknij(serwer);
}

void sygnalWyjscie(int signal) {
	printf("\nSygnał przerwania, zamykanie kolejki\n");
	exit(EXIT_SUCCESS);
}

int main() {
	
	char kolejka_odpowiedz_nazwa[10];
	mqd_t kolejka_odpowiedz;
	char odczyt[DLUGOSC_KOMUNIKATU];
	char string[DLUGOSC_KOMUNIKATU+10];
	
	sprintf(kolejka_odpowiedz_nazwa, "/%d", getpid());
	
	sleep(3);																			//pocz chwile
	
	serwer=kolejka_otworz(KOLEJKA_NAZWA, O_WRONLY);				
	
	{
		struct mq_attr atrybuty=kolejka_pobierz_atrybuty(serwer);
		printf("Otworzono kolejkę \"%s\" o deskryptorze %d\n", KOLEJKA_NAZWA, serwer);
		printf("mq_flags: = %ld\n", atrybuty.mq_flags);									//opcje 0 lub O_NONBLOCK - owtworzenie kolejki w trybie nieblokujacym
		printf("mq_maxmsg: = %ld\n", atrybuty.mq_maxmsg);								//maksymalna liczba komunikatow w kolejce
		printf("mq_msgsize: = %ld\n", atrybuty.mq_msgsize);								//maksymalny rozmiar komunikatu
		printf("mq_curmsgs: = %ld\n\n", atrybuty.mq_curmsgs);							//liczba komunikatow aktualnie w kolejce
	}
	
	if(atexit(wyjscie)!=0) {
		perror("atexit error");
		_exit(EXIT_FAILURE);
	}
	
	if(signal(SIGINT,sygnalWyjscie)==SIG_ERR) {
		perror("Błąd signal");
		exit(EXIT_FAILURE);
	}
	
	srand(time(NULL));
	
	sleep((double)(rand()%(int)(2.5*100))/100);											//czekaj rnd czasu
	
	while(1) {
		
		printf("Wpisz dzialanie\n");
		if(scanf("%s", odczyt)==EOF) {													//end of file
			printf("Koniec odczytu, mozesz zamknac konsole\n");
			exit(EXIT_SUCCESS);
		}
		sprintf(string, "%d %s", getpid(), odczyt);										//dopisz pid do komunikatu
		
		kolejka_wyslij(serwer, string, 1);												//wysalanie zapytania
		printf("\nWysłano zapytanie do serwera: %s\n", string);
		
		kolejka_odpowiedz=kolejka_utworz(kolejka_odpowiedz_nazwa, O_RDONLY);			//kolejka do odbierania wiadomosci
		
		printf("Oczekiwanie na odpowiedz z serwera\n");									//odpowiedz serwera
		kolejka_odbierz(kolejka_odpowiedz, string, NULL);
		printf("Odpowiedz z serwera: %s\n\n", string);
		
		kolejka_zamknij(kolejka_odpowiedz);												//zamknij i usun kolejke
		kolejka_usun(kolejka_odpowiedz_nazwa);
		
		sleep((double)(rand()%(int)(2.5*100))/100);										//czekaj rnd czasu
	}
	kolejka_zamknij(serwer);															//zamknij kolejke serwera
		
	printf("\n\nKlient: koniec procesu\n");
	
	return 0;
}