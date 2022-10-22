/*Kacper Tatrocki  				Krakow, 21.03.2022r. */ 

#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void sighandler(int sig)
{
	printf("\nOtrzymany sygnal: %d\n", sig);
}

int main(int argc, char **argv)
{
	if(argc!=3)    								//zbadanie prawidlowej ilosci argumentow
	{
		fprintf(stderr, "Nieprawidlowa liczba argumentow\n");
		exit(EXIT_FAILURE);
	}

	printf("%c\n", argv[1][0]);							//wywolanie na 1 z 3 sposobow
	int sinll=atoi(argv[2]);           						//pobranie numeru sygnalu

	switch (argv[1][0])								//wybor zadanej opcji
	{
		case 'd':
				if (signal(sinll,SIG_DFL)==SIG_ERR)
				{	
	        		perror("Funkcja signal ma problem z SIGQUIT");
	        		exit(EXIT_FAILURE);					
	        	}
				pause();						//funkcja domyslna

		case 'i':
				if(signal(sinll,SIG_IGN)==SIG_ERR)
				{
					perror("Funkcja signal ma problem z SIGTERM");
					exit(EXIT_FAILURE);
				}
				printf("signal ignored");
				pause();						//funkcja ignorowania

		case 'c':
				if(signal(sinll,sighandler)==SIG_ERR)
				{
					perror("Funkcja signal ma problem z SIGINT");
					exit(EXIT_FAILURE);
				}
				pause();						//funckja przechwycenia 
	}
	return 0;
}   
