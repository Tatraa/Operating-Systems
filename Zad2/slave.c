#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    printf("UID: %d\n", getuid());
    printf("GID: %d\n", getgid());
    printf("PID: %d\n", getpid());
    printf("PPID: %d\n", getppid());
    printf("PGID: %d\n\n\n", getpgid(getpid()));
    return 0;
}