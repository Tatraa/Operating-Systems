/*Kacper Tatrocki               Krakow, 25.03.2022r. */ 

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <errno.h>


int main(int argc, char *argv[]) {

    if(argc!=3)										//zbadanie prawidlowej ilosci argumentow
    {
        fprintf(stderr, "Nieprawidlowa liczba argumentow\n");
        exit(EXIT_FAILURE);
    }
    int sinll=atoi(argv[2]);

    for(int i=0; i<5; ++i){ 									//uruchomienie kilku procesow wykonujacych program do obslugi sygnalow
        int pid=fork();
        switch (pid) {
            case 0:
                if (execlp("./prog_a.x", "prog_a.x", argv[1], argv[2], NULL) == -1) {	//uruchomienie programu a poprzez funkcje exec
                    perror("execl error");
                    exit(EXIT_FAILURE);
                }
                break;

            case -1:
                perror("fork error");								//klasyczny error widelca
                exit(EXIT_FAILURE);
                break;
                
            default: {
                if(signal(sinll,SIG_IGN)==SIG_ERR)						//proces bedacy liderem grupy procesow ignoruje sygnaly, a na koncu czeka na zakonczenie wszystkich
                {										//swoich procesow potomnych
                    perror("Funkcja signal ma problem z sinll");
                    exit(EXIT_FAILURE);
                }
            }
        }
    }
}
