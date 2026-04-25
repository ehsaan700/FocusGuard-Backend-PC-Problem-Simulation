#include "globals.h"

// Helper function to print the current buffer state
void print_buffer() {
    printf("Buffer: [");
    for (int i = 0; i < BUFFER_SIZE; i++) {
        if (strlen(buffer[i]) > 0) {
            printf("%s", buffer[i]);
        } else {
            printf("--"); // empty slot
        }
        if (i < BUFFER_SIZE - 1) printf(", ");
    }
    printf("]\n");
}

// Helper function to get timestamp
double get_time(struct timespec start) {
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return (now.tv_sec - start.tv_sec) + (now.tv_nsec - start.tv_nsec) / 1e9;
}