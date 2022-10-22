/*Kacper Tatrocki 12.03.2022, Krakow*/


/*Kacper Tatrocki 12.03.2022, Krakow */

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    for (int i = 0; i < 3; i++) {
        switch (fork()) {
            case 0:
                execlp("./slave", "slave", NULL); {
                perror("execlp error");
                _exit(2);
                }
                break;
                //proces potomny nie tworzy procesow potomnych
                //powstaje n nowych procesow w ktorych zaden sie nie dzieli

            case -1:
                perror("fork error");
                _exit(1);
                break;

            default: {
                wait(NULL);
                break;
            }
        }
    }
    return 0;
}