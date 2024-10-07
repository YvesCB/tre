#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "termutil.h"

int nanosleep(const struct timespec *req, struct timespec *rem);

char *buffer;
char *back_buffer;

V2 current_dimension;

V2 get_terminal_dimensions() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    V2 dims = {w.ws_row, w.ws_col};
    return dims;
}

void resize_buffer(char *buffer_to_resize) {
    current_dimension = get_terminal_dimensions();

    malloc(sizeof(char) * current_dimension.x * current_dimension.y);
}

void display_and_swap() {
    // free the display buffer
    free(buffer);
    // swap the buffers
    buffer = back_buffer;
    back_buffer = NULL;

    // write the contents of the buffer to stdout
    fwrite(buffer, sizeof(char), current_dimension.x * current_dimension.y,
           stdout);
}

void render() {
    resize_buffer(back_buffer);

    // Loop through the number of lines the terminal has
    for (int y = 0; y < current_dimension.y; y++) {
        for (int x = 0; x < current_dimension.x; x++) {
            back_buffer[y * x + x] = '@';
        }
    }

    display_and_swap();
}

int main() {
    // Set up signal handler for Ctrl+C
    signal(SIGINT, handle_sigint);
    printf("Hellow world");

    set_raw_mode();

    set_nonblocking_mode();

    buffer = NULL;
    back_buffer = NULL;

    // Loop to read and print key presses
    // char c;
    while (1) {
        clock_t t = clock();
        // c = getchar();
        system("clear");

        /*
        if (c != EOF) {
            printf("Pressed key: %c (ASCII code: %d)\n", c, c);
        }
        */

        render();

        t = clock() - t;
        float ns = (int)(((float)t) / CLOCKS_PER_SEC * 1000000000);
        ns = 330000000 - ns;
        struct timespec waitns = {0, (int)ns};

        nanosleep(&waitns, NULL);

        // c = EOF;
    }

    return 0;
}
