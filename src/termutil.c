#include "termutil.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

// Returns 1 if dims are the same. 0 otherwise
inline int comp_dims(V2 d1, V2 d2) { return (d1.x == d2.x && d1.y == d2.y); }

// Sleeps the system using the nanosleep function
// Takes miliseconds instead to make it easier to use
int sleep_ms(int sleep_ms) {
    int seconds = 0;
    long nanos = 0;
    if (sleep_ms >= 1000) {
        seconds = sleep_ms / 1000;
        int remainder = sleep_ms % 1000;
        nanos = (long)(remainder * 1000000L);
    } else {
        nanos = (long)(sleep_ms * 1000000L);
    }

    const struct timespec duration = {seconds, nanos};

    int response = nanosleep(&duration, NULL);

    return response;
}

V2 get_term_dimension() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    V2 dim = {w.ws_col, w.ws_row};

    return dim;
}

void reset_terminal_mode() {
    static struct termios original_termios;
    tcgetattr(STDIN_FILENO, &original_termios);
    tcsetattr(STDIN_FILENO, TCSANOW, &original_termios);
}

void set_raw_mode() {
    struct termios new_termios;

    // Restoring to original state
    atexit(reset_terminal_mode);

    // Get the terminal attributes
    tcgetattr(STDIN_FILENO, &new_termios);

    // Set the terminal to raw mode
    new_termios.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode and echo
    new_termios.c_cc[VMIN] = 1;  // Minimum number of characters to read
    new_termios.c_cc[VTIME] = 0; // No timeout

    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
}

// Signal handler for Ctrl+C
void handle_sigint(int sig) {
    reset_terminal_mode();
    printf("\nProgram terminated with Ctrl+C\n");
    exit(0);
}

// Set to non-blocking mode
void set_nonblocking_mode() {
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
}
