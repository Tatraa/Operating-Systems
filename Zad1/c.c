/*Kacper Tatrocki 12.03.2022, Krakow*/


#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    printf("Procesy macierzyste: \n");
    printf("UID: %d\n", getuid());
    printf("GID: %d\n", getgid());
    printf("PID: %d\n", getpid());
    printf("PPID: %d\n", getppid());
    printf("PGID: %d\n\n", getpgid(getpid()));

    printf("Procesy potomne:\n");
    for (int i = 0; i < 3; i++) {
        //wywolanie funkcji 3 razy
        switch (fork()) {
        //tworzenie systemu potomnego
            case -1:
                perror("blad forka");
                break;
                //blad

            case 0:
                printf("UID: %d\n", getuid());
                printf("GID: %d\n", getgid());
                printf("PID: %d\n", getpid());
                printf("PPID: %d\n", getppid());
                printf("PGID: %d\n\n", getpgid(getpid()));
                break;
                //akcja procesu potomnego

            default: {
                sleep(1);
                //akcja procesu potomnego
            }
        }
    }
    return 0;
}