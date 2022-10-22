#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <mqueue.h>
#include "kolejki.h"

void wyjscie(void) {
	kolejka_usun(KOLEJKA_NAZWA);
}

void sygnalWyjscie(int signal) {
	printf("\nSygnal przerwania, usuwanie kolejki\n");
	exit(EXIT_SUCCESS);
}

int main() {
	
	kolejka_usun(KOLEJKA_NAZWA);			//to na koniec wyjasnic bo beka troche

	int PIDKlienta;
	char string[DLUGOSC_KOMUNIKATU];
	int l1, l2;
	char operator;
	int wynik;
	char wynik_s[DLUGOSC_KOMUNIKATU];
	mqd_t kolejka_biurko;
	mqd_t odpowiedz;
	
	kolejka_biurko=kolejka_utworz(KOLEJKA_NAZWA, O_RDONLY);
	
	{
		struct mq_attr atrybuty=kolejka_pobierz_atrybuty(kolejka_biurko);
		printf("Utworzono kolejke \"%s\" deskryptor %d o atrybutach:\n", KOLEJKA_NAZWA, kolejka_biurko);
		printf("mq_flags: = %ld\n", atrybuty.mq_flags);							//opcje 0 lub O_NONBLOCK
		printf("mq_maxmsg: = %ld\n", atrybuty.mq_maxmsg);						//maksymalna liczba komunikatow w kolejce
		printf("mq_msgsize: = %ld\n", atrybuty.mq_msgsize);						//maksymalny rozmiar komunikatu
		printf("mq_curmsgs: = %ld\n\n", atrybuty.mq_curmsgs);					//liczba komunikatow aktualnie w kolejce
	}
	
	if(atexit(wyjscie)!=0) {
		perror("atexit error");
		exit(EXIT_FAILURE);
	}
	
	if(signal(SIGINT,sygnalWyjscie)==SIG_ERR) {
		perror("bląd signal");
		exit(EXIT_FAILURE);
	}
	
	srand(time(NULL));
	
	while(1) {
		
		kolejka_odbierz(kolejka_biurko, string, NULL);														//odczytaj komunikat z kolejki
		
		sscanf(string, "%d %d%c%d", &PIDKlienta, &l1, &operator, &l2);
		
		printf("Odebrano zapytanie od %d - działanie %d %c %d\n", PIDKlienta, l1, operator, l2);
		
		if(operator == '+') {																				//obliczenia
			wynik=l1 + l2;
			sprintf(wynik_s, "%d", wynik);
		} else if (operator == '-') {
			wynik=l1 - l2;
			sprintf(wynik_s, "%d", wynik);
		} else if (operator == '*') {
			wynik=l1 * l2;
			sprintf(wynik_s, "%d", wynik);
		} else if (operator == '/') {
			wynik=l1 / l2;
			sprintf(wynik_s, "%d", wynik);
		} else {
			sprintf(wynik_s, "Nieznany operator");
		}
		
		sleep((double)(rand()%(int)(2.5*100))/100);															//czekaj rnd czasu
		
		sprintf(string, "/%d", PIDKlienta);																	//otworz kolejke do nadania odpowiedzi
		odpowiedz=kolejka_otworz(string, O_WRONLY);
		
		printf("Wysylanie odpowiedzi %s do procesu %d\n", wynik_s, PIDKlienta);								//odpowiedz
		kolejka_wyslij(odpowiedz, wynik_s, 1);
		kolejka_zamknij(odpowiedz);
	}
	
	kolejka_usun(KOLEJKA_NAZWA);
	
	printf ("\n\nSerwer: koniec procesu\n");
	
	return 0;
}