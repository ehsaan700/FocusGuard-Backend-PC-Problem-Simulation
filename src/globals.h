#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define BUFFER_SIZE 5

// extern tells the compiler these variables exist, but are defined in main.c
extern char buffer[BUFFER_SIZE][50];
extern int in;  // next insert index
extern int out; // next remove index
extern int count; // Track number of items 

extern sem_t empty_slots; // no of empty spaces
extern sem_t full_slots;  // no of filled spaces
extern pthread_mutex_t mutex;

extern int manual_mode;
extern sem_t manual_step;

extern const char* activities[];
extern struct timespec start_time;

// Function Prototypes
void print_buffer();
double get_time(struct timespec start);
void *producer(void *param);
void *consumer(void *param);

#endif