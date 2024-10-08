#include <time.h>

#ifndef _TERMUTIL
#define _TERMUTIL

typedef struct V2 {
    int x;
    int y;
} V2;

int comp_dims(V2 d1, V2 d2);

int nanosleep(const struct timespec *req, struct timespec *rem);

int sleep_ms(int sleep_ms);

V2 get_term_dimension();

void reset_terminal_mode();

void set_raw_mode();

void handle_sigint(int sig);

void set_nonblocking_mode();

#endif
