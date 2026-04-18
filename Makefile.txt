CC = gcc
CFLAGS = -std=c99 -D_POSIX_C_SOURCE=200809L -O2 -Wall -Wextra

all: fork_loop observe_syscalls benchmark_syscall benchmark_io

fork_loop: fork_loop.c
	$(CC) $(CFLAGS) -o fork_loop fork_loop.c

observe_syscalls: observe_syscalls.c
	$(CC) $(CFLAGS) -o observe_syscalls observe_syscalls.c

benchmark_syscall: benchmark_syscall.c
	$(CC) $(CFLAGS) -o benchmark_syscall benchmark_syscall.c

benchmark_io: benchmark_io.c
	$(CC) $(CFLAGS) -o benchmark_io benchmark_io.c

clean:
	rm -f fork_loop observe_syscalls benchmark_syscall benchmark_io