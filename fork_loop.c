#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
    int i = 0;

    while (fork() && i < 5) {
        wait (NULL);
        printf("Hey!");
        i++;
    }

    printf("%d", i);
    return 0;
}