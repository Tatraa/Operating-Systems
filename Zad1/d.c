/*Kacper Tatrocki 12.03.2022, Krakow*/

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    printf("Procesy macierzyste: \n");
    printf("PID: %d\n", getpid());
    printf("PPID: %d\n", getppid());
    printf("PGID: %d\n\n", getpgid(getpid()));

    printf("Procesy potomne: \n");
    for (int i = 0; i<3; i++) {
        sleep(1);
        switch (fork()) {
            case -1:
                perror("fork error");
                break;

            case 0:
                printf("PID: %d\n", getpid());
                printf("PPID: %d\n", getppid());
                printf("PGID: %d\n\n\n", getpgid(getpid()));
                break;

            default: {
                sleep(4-i);   //(5-i) dlatego ze taki dziala 
            }
        }
    }
    return 0;
}
