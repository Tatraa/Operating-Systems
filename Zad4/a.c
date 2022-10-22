// Kacper Tatrocki 				05.04.2022r, Krakow


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

int main(int argc, char *argv[]) {
	
	char buff[1024];
	char wiadomosc[1037];									//1037 dlatego ze sprintf jest miedzy 14 a 1037 bitow
	int plik;
	int filedes[2];									//filedes[0] sluzy do zapisu danych a filedes[1] do odczytu
	int odczyt;
	const size_t rozmiarProd=10; 
	const size_t rozmiarKons=10; 
	
	if(pipe(filedes)==-1) {								//obsluga bledow rury
		perror("Blad potoku");
		exit(EXIT_FAILURE);
	}
	
	srand(time(NULL));									//czekanie randomowej chwili czasu
	int pid=fork();
	switch(pid) {
		case -1:
			perror("fork error");							//klasyczny error widelca
			exit(EXIT_FAILURE);
			
		case 0:									//akcja dla procesu konsumenta
			if(close(filedes[1])==-1) { 						//zamykamy nieużywany zapis	
				perror("Blad zamkniecia potoku");
				exit(EXIT_FAILURE);
			}
			
			if((plik=open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644))==-1) {	//owtworz plik do pisania(tylko do pisania), jezeli go nie ma to go stworz, jesli istnieje zmniejsz jego dlugsc do zera, prawo dostepu do pliku w systemie osemkowym. | binarne OR
				printf("Blad otwarcia pliku %s do zapisu", argv[2]);
				perror("Blad pliku!");
				exit(EXIT_FAILURE);
			}
			while(1) {								//zapisywanie skosumowanych produktów do pliku
				sleep((double)(rand()%(int)(2*80))/80);			//czekanie randomowo wybranej ilosci czasu
				
				odczyt=read(filedes[0], &buff, rozmiarKons);
				if(odczyt==-1) {
					printf("Blad odczytu z potoku\n");
					exit(EXIT_FAILURE);
				} else if(odczyt == 0) {
					break;
				} else {
					write(plik, &buff, odczyt);
					buff[odczyt]='\0';
					sprintf(wiadomosc, "%s%s%s", "Konsumuje - ", buff, "\n");
					write(STDOUT_FILENO, wiadomosc, strlen(wiadomosc));	//standadowy strumien wyjscia
				}
			}
			
			if(close(plik)==-1) {							//obsluga bledow
				perror("Blad zamkniecia pliku wyjscia");
				exit(EXIT_FAILURE);
			}
			
			if(close(filedes[0])==-1) {						//obsluga bledow
				perror("Blad zamkniecia potoku");
				exit(EXIT_FAILURE);
			}
			
			break;
			
		default:									//akcja dla procesu producenta
			
			if(close(filedes[0])==-1) { 						//nie używany odczyt jest zamykany
				perror("Blad zamknięcia potoku");
				exit(EXIT_FAILURE);
			}
			if((plik=open(argv[1], O_RDONLY, 0644))==-1) {			//otworz plik do czytania, prawo dostepu do pliku
				printf("Blad otwarcia pliku %s do odczytu", argv[1]);
				perror("Blad pliku");
				exit(EXIT_FAILURE);
			}
			
			while(1) {								//zapisywanie wyprodukowanych produktów do potoku
				sleep((double)(rand()%(int)(2*80))/80);			//czekanie randomowo wybranej ilosci czasu
				
				odczyt=read(plik, &buff, rozmiarProd);
				if(odczyt==-1) {
					printf("Blad odczytu z pliku\n");
					exit(EXIT_FAILURE);
				} else if(odczyt==0) {
					break;
				} else {
					write(filedes[1], &buff, odczyt);
					buff[odczyt]='\0';
					sprintf(wiadomosc, "%s%s%s", "Produkuje - ", buff, "\n");
					write(STDOUT_FILENO, wiadomosc, strlen(wiadomosc));	//standadowy strumien wyjscia
				}
			}
			if(close(plik)==-1) {							//obsluga bledow
				perror("Blad zamkniecia pliku wyjscia");
				exit(EXIT_FAILURE);
			}
			if(close(filedes[1])==-1) {						//obsluga bledow
				perror("Blad zamkniecia potoku");
				exit(EXIT_FAILURE);
			}
			int *stat_loc = 0;							//funkcja wait i jej obsluga bledow
            		if (wait(stat_loc)==-1) {
                		perror("Blad errora");
                		exit(EXIT_FAILURE);
			}		
			break;
	}
	return 0;
}
