#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define ITERATIONS 1000000

static long elapsed_ns(struct timespec start, struct timespec end) {

    return (end.tv_sec - start.tv_sec) * 1000000000L + (end.tv_nsec - start.tv_nsec);
}

int main(void) {

    struct timespec t0, t1;
    long ns;
    volatile pid_t pid;

    clock_gettime(CLOCK_MONOTONIC, &t0);

    for (int i = 0; i < ITERATIONS, i++) {
        pid = getpid();
    }

    clock_gettime(CLOCK_MONOTONIC, &t1);

    ns = elapsed_ns(t0, t1);
    printf("getpid() syscall: %ld ns total, %.1f ns per call\n", ns, (double)ns / ITERATIONS);

    pid_t cachedPid = getpid();

    clock_gettime(CLOCK_MONOTONIC, &t0);
    for (int i = 0; i < ITERATIONS; i++) {
        pid = cachedPid;
    }

    clock_gettime(CLOCK_MONOTONIC, &t1);

    ns = elapsed_ns(t0, t1);
    printf("cached variable: %ld ns total, %.1f ns per call\n", ns, (double)ns / ITERATIONS);

    return 0;
}