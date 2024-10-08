#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include "termutil.h"
#include "tre.h"

V2 current_dimension;

char *buffer;

void resize_buffer() {
    V2 new_dim = get_term_dimension();

    if (comp_dims(current_dimension, new_dim)) {
        if (buffer) {
            free(buffer);
        }

        current_dimension = new_dim;
        buffer = malloc(current_dimension.y * (current_dimension.x + 1) *
                        sizeof(char));
    }
}

void display() { fputs(buffer, stdout); }

void render() {
    resize_buffer();
    printf("\033[H\033[J");

    int x_quarter = current_dimension.x / 4;
    int y_quarter = current_dimension.y / 4;

    for (int y = 0; y < current_dimension.y; y++) {
        for (int x = 0; x < current_dimension.x; x++) {
            if (x >= x_quarter && x <= 3 * x_quarter && y >= y_quarter &&
                y <= 3 * y_quarter) {
                buffer[(y * current_dimension.x + 1) + x] = 'A';
            } else {
                buffer[(y * current_dimension.x + 1) + x] = ' ';
            }
        }
        // if (y == current_dimension.y - 1)
        //    buffer[y * current_dimension.x] = '\0';
        buffer[y * current_dimension.x] = '\n';
    }

    display();
}

int main() {
    // Set up signal handler for Ctrl+C
    set_raw_mode();

    set_nonblocking_mode();

    signal(SIGINT, handle_sigint);

    current_dimension = get_term_dimension();
    buffer = NULL;

    while (1) {
        clock_t t = clock();
        // c = getchar();

        /*
        if (c != EOF) {
            printf("Pressed key: %c (ASCII code: %d)\n", c, c);
        }
        */

        // TODO: Render flow
        render();

        t = clock() - t;

        float ms = (int)((float)t / CLOCKS_PER_SEC * 1000);
        // printf("Render time: %fms\n", ms);
        ms = 300 - ms;
        if (ms > 0) {
            sleep_ms(ms);
        }
        // c = EOF;
    }

    return 0;
}
