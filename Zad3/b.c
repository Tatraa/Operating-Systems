/*Kacper Tatrocki               Krakow, 25.03.2022r*/


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
    
    int pid = fork();
    int signyl=atoi(argv[2]);

    switch (pid) {
        case 0:
            if (execlp("./prog_a.x", "prog_a.x", argv[1], argv[2], NULL) == -1) {		//uruchomienie powyzszego programu poprzez funkcje exec
                perror("execl error");							//w procesie potomnym innego procesu
                exit(EXIT_FAILURE);
            }
            break;

        case -1:
            perror("fork error");								//klasyczny error widelca
            exit(EXIT_FAILURE);
            break;
            
        default: {
            sleep(1);										//jezeli sig=0 to funkcja kill nie wysyla sygnalu, ale wykonuje test bledow
            if(kill(pid, 0) == ESRCH)								//np. sprawdza czy funkcja istnieje- jezeli nie, to errno = ESRCH
            {
                perror("proces nie istnieje");
                exit(EXIT_FAILURE);
            }
            else										//w przeciwnym wypadku zabija proces
            {
                kill(pid,signyl);
            }

            int *stat_loc = 0;								//funkcja wait i jej obsluga bledow
            if (wait(stat_loc) == -1) {
                perror("wait error");
                exit(EXIT_FAILURE);
            }
            break;
        }
    }
    return 0;
} 
