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
    for (int i = 0; i < 3; i++) {
        //wywolanie funkcji 3 razy
        switch (fork()) {
        //tworzenie systemu potomnego
            case 0:
                if (setpgid(0, 0))
                    //ustawienie pgid biezaceg procesu na jego samego
                    perror("blad setpgida");

                printf("PID: %d\n", getpid());
                printf("PPID: %d\n", getppid());
                printf("PGID: %d\n\n", getpgid(getpid()));
                break;

            case -1:
                perror("fork error");
                break;

            default: {
                wait(NULL);
            }
        }
    }
    return 0;
}



/*Ile procesow powstanie przy n-krotnym wywolaniu funkcji fork i dlaczego?
  dla takiej komendy :for (i = 0; i < n; i++) fork();
  wywola ja 2^n razy */