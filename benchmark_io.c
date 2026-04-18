#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define TOTAL_BYTES  (1 * 1024 * 1024)  /* 1 MB of data */
#define CHUNK_SIZE   32                  /* each logical write is 32 bytes */
#define NUM_WRITES   (TOTAL_BYTES / CHUNK_SIZE)
#define BUF_CAPACITY 4096                /* manual buffer size */
#define DIV_ROUND_UP(n, d) (((n) + (d) - 1) / (d))


static long elapsed_ns(struct timespec start, struct timespec end)
{
    return (end.tv_sec - start.tv_sec) * 1000000000L
         + (end.tv_nsec - start.tv_nsec);
}

static void benchmark_unbuffered(int fileDescriptor) {
    char chunk[CHUNK_SIZE];
    memset(chunk, 'A', CHUNK_SIZE);
    for(int i = 0; i < NUM_WRITES; i++) {
        write(fileDescriptor, chunk, CHUNK_SIZE);
    }
}

static void benchmark_manual_buffer(int fileDescriptor) {

    char buffer[BUF_CAPACITY];
    size_t off = 0;
    char chunk[CHUNK_SIZE];
    memset(chunk, 'A', CHUNK_SIZE);

    for (int i = 0; i < NUM_WRITES; i++) {

        memcpy(buffer + off, chunk, CHUNK_SIZE);
        off += CHUNK_SIZE;
        if (off == BUF_CAPACITY) {

            write(fileDescriptor, buffer, BUF_CAPACITY);
            off = 0;
        }
    }

    if ( off > 0) {
        write(fileDescriptor, buffer, off);
    }
}

static void benchmark_stdio(int fileDescriptor) {

    FILE *f = fdopen(dup(fileDescriptor), "w");
    if (!f) {
        perror("fdopen");
        return;
    }

    char chunk[CHUNK_SIZE];
    memset(chunk, 'A', CHUNK_SIZE);
    for (int i = 0; i < NUM_WRITES; i++) {
        fwrite(chunk, 1, CHUNK_SIZE, f);
    }

    fclose(f);
}

static void run(const char *label, void (*fn)(int), int fd)
{
    struct timespec t0, t1;
    long ns;
    int approx_writes;

    lseek(fd, 0, SEEK_SET);
    clock_gettime(CLOCK_MONOTONIC, &t0);
    fn(fd);
    clock_gettime(CLOCK_MONOTONIC, &t1);

    ns = elapsed_ns(t0, t1);
    approx_writes = label[0] == 'u' ? NUM_WRITES :
                    label[0] == 'm' ? DIV_ROUND_UP(TOTAL_BYTES, BUF_CAPACITY) :
                    DIV_ROUND_UP(TOTAL_BYTES, BUFSIZ);

    printf("  %-20s %8ld us   (%d syscall-writes approx)\n",
           label, ns / 1000, approx_writes);
}

int main(void)
{
    int fd = open("/dev/null", O_WRONLY);
    if (fd < 0) {
        perror("open /dev/null");
        return 1;
    }

    printf("Writing %d bytes as %d x %d-byte chunks:\n\n",
           TOTAL_BYTES, NUM_WRITES, CHUNK_SIZE);

    run("unbuffered",    benchmark_unbuffered,    fd);
    run("manual_buffer", benchmark_manual_buffer, fd);
    run("stdio",         benchmark_stdio,         fd);

    close(fd);
    return 0;
}