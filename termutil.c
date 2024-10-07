#include "termutil.h"

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
