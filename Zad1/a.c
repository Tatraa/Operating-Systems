/*Kacper Tatrocki 12.03.2022, Krakow*/

#include <stdio.h>
#include <unistd.h>

int main() {
    printf("UID: %d\n", getuid());
    printf("GID: %d\n", getgid());
    printf("PID: %d\n", getpid());
    printf("PPID: %d\n", getppid());
    printf("PGID: %d\n", getpgid(getpid()));
    //wypisanie identyfikatorow zwiazane z procesami
    //oraz funkcje sluzace do ich uzyskiwania
    return 0;
}
