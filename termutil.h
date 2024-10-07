#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#ifndef DEBUG
#define TERMUTIL

int nanosleep(const struct timespec *req, struct timespec *rem);

typedef struct V2 {
    int x;
    int y;
} V2;

void reset_terminal_mode();

void set_raw_mode();

void handle_sigint(int sig);

void set_nonblocking_mode();

#endif // !DEBUG
