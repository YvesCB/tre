#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

int nanosleep(const struct timespec *req, struct timespec *rem);

char *buffer;

typedef struct V2 {
  int x;
  int y;
} V2;

V2 current_dimension;

V2 get_terminal_dimensions() {
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

  V2 dims = {w.ws_row, w.ws_col};
  return dims;
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

void set_pixel(int row, int col, char c) {
  printf("\033[%d;%dH%c", row, col, c);
  fflush(stdout);
}

void render() {
  current_dimension = get_terminal_dimensions();
  for (int x = 0; x < current_dimension.x; x++) {
    for (int y = 0; y < current_dimension.y; y++) {
      if (x >= current_dimension.x / 4 && x < current_dimension.x / 4 * 3) {
        if (y >= current_dimension.y / 4 && y < current_dimension.y / 4 * 3) {
          set_pixel(y, x, '@');
        }
      }
    }
  }
}

int main() {
  // Set up signal handler for Ctrl+C
  signal(SIGINT, handle_sigint);

  set_raw_mode();

  set_nonblocking_mode();

  buffer = NULL;

  // Loop to read and print key presses
  char c;
  while (1) {
    clock_t t = clock();
    render();
    c = getchar();
    system("clear");

    if (c != EOF) {
      printf("Pressed key: %c (ASCII code: %d)\n", c, c);
    }

    t = clock() - t;
    float ns = (int)(((float)t) / CLOCKS_PER_SEC * 1000000000);
    ns = 33000000 - ns;
    struct timespec waitns = {0, (int)ns};

    nanosleep(&waitns, NULL);

    c = EOF;
  }

  return 0;
}
