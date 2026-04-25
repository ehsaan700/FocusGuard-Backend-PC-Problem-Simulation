#include "globals.h"

// Define the global variables
char buffer[BUFFER_SIZE][50];
int in = 0;  
int out = 0; 
int count = 0; // Track number of items for display purposes

sem_t empty_slots;
sem_t full_slots;  
pthread_mutex_t mutex;

int manual_mode = 0;
sem_t manual_step; // Semaphore for manual mode

// Sample data for the Focus & Distraction Analyzer
const char* activities[] = {"Typing", "Mouse Click", "Window Switch", "Idle Timer", "Scrolling"};
struct timespec start_time;

int main(int argc, char *argv[]) {
    srand(time(NULL));
    clock_gettime(CLOCK_MONOTONIC, &start_time);
    
    // by default have 3 producers and 3 consumers
    int num_producers = 3;
    int num_consumers = 3;

    // if number of producers or consumers other than 3 then use the command line args
    if (argc == 3) {
        num_producers = atoi(argv[1]);
        num_consumers = atoi(argv[2]);
    }

    //initialize empty strings in buffer
    for(int i=0; i<BUFFER_SIZE; i++) strcpy(buffer[i], "");

    printf("Focus & Distraction Analyzer Backend\n");
    printf("1. Automatic Mode (Continuous)\n");
    printf("2. Manual Mode\n");
    printf("Select Mode (1 or 2): ");
    
    int choice;
    scanf("%d", &choice);
    getchar();
    
    if (choice == 2) {
        manual_mode = 1;
        sem_init(&manual_step, 0, 0); //initialize manual lock
        printf("\n MANUAL MODE INITIATED. Press ENTER to advance threads.\n\n");
    } else {
        printf("\n AUTOMATIC MODE INITIATED.\n\n");
    }

    //initialize mutex and semaphores
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty_slots, 0, BUFFER_SIZE); // N empty slots
    sem_init(&full_slots, 0, 0);            // 0 full slots

    pthread_t producers[num_producers];
    pthread_t consumers[num_consumers];
    int producer_ids[num_producers];
    int consumer_ids[num_consumers];

    // create Threads
    for(int i = 0; i < num_producers; i++) {
        producer_ids[i] = i + 1;
        pthread_create(&producers[i], NULL, producer, &producer_ids[i]);
    }
    for(int i = 0; i < num_consumers; i++) {
        consumer_ids[i] = i + 1;
        pthread_create(&consumers[i], NULL, consumer, &consumer_ids[i]);
    }

    // manual mode
    if (manual_mode) {
        while(1) {
            getchar(); // Wait for user to press ENTER
            sem_post(&manual_step); // Release one thread to do work
            sem_post(&manual_step); // Release another so producer and consumer pair can interact
        }
    } else {
       
        sleep(30);   // by default auto mode runs for 30 seconds it could be changed tho..
        printf("\nSimulation complete. Shutting down...\n");
        exit(0);
    }

    // cleanup
    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty_slots);
    sem_destroy(&full_slots);
    if(manual_mode) sem_destroy(&manual_step);

    return 0;
}