/*Kacper Tatrocki               Krakow, 25.03.2022r. */ 

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    int pid = fork();
    int signyl=atoi(argv[2]);
    //int pgid = getpgid(pid); 		//o to spytac
    switch (pid) {
        case 0:
            setpgid(0, 0);										//sprawienie aby proces potomny stal sie liderem
            if (execlp("./zgrupowanie.x", "zgrupowanie.x", argv[1], argv[2], NULL) == -1) {		//uruchomienie programu zgrupowanie_grupowe poprzez funkcje exec
                perror("execl error");
                exit(EXIT_FAILURE);
            }
            break;

        case -1:
            perror("fork error");									//klasyczny error widelca
            exit(EXIT_FAILURE);
            break;

        default: {
            sleep(1);									
            if(kill(-pid, 0) == -1)									//wykonywanie testu bledow
            {
                if(errno == ESRCH)
                {
                    perror("proces nie istnieje");
                    exit(EXIT_FAILURE);
                }
            }
            else{
                kill(-pid,signyl);
            }

            int *stat_loc = 0;									//funkcja wait i jej obsluga bledow
            if (wait(stat_loc) == -1) {
                perror("wait error");
                exit(EXIT_FAILURE);
            }
            break;
        }
    }
    return 0;
}
