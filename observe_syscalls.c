#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>

int main(void) {

    /* step 1 */ 
    pid_t pid = getpid();
    printf("PID: %d\n", pid);

    /* step 2 */ 
    int fileDescriptor = open("/etc/hostname", O_RDONLY);
    if (fileDescriptor < 0) {
        perror("open");
        return 1;
    }

    /* step 3 */ 
    char buffer[256];
    ssize_t n = read(fileDescriptor, buffer, sizeof(buffer) -1);
    if (n < 0) {

        perror("read");
        close(fileDescriptor);
        return 1;
    }

    buffer[n] = '\0';

    /* step 4 */ 
    close(fileDescriptor);

    /* step 5 */ 
    write(1, "Hostname: ", 10);
    write(1, buffer, n);

    return 0;
}