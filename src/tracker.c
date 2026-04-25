#include "globals.h"

// PRODUCER (Distraction Tracker)
void *producer(void *param) {
    int id = *((int *)param);
    
    while(1) {
        // Wait for manual step if in manual mode
        if (manual_mode) sem_wait(&manual_step);
        else sleep(rand() % 2 + 1); // Auto mode sleep
    
        // Generate sample acctivity
        int rand_index = rand() % 5;
        char new_event[50];
        strcpy(new_event, activities[rand_index]);
        
        //Wait for empty slot
        printf("[Time: %.2fs] [Tracker %d] Waiting for Empty Slot...\n", get_time(start_time), id);
        sem_wait(&empty_slots);
        
        // Lock buffer
        pthread_mutex_lock(&mutex);
        
        // Critical Section: Insert data
        strcpy(buffer[in], new_event);
        printf("\n[Time: %.2fs] [Tracker %d] Wait(Empty) Acquired. Wait(Mutex) Acquired.\n", get_time(start_time), id);
        in = (in + 1) % BUFFER_SIZE;
        count++;
        printf("[Time: %.2fs] [Tracker %d] Inserted Event '%s'. ", get_time(start_time), id, new_event);
        print_buffer();
        printf("[Time: %.2fs] [Tracker %d] Signal(Mutex). Signal(Full).\n\n", get_time(start_time), id);
        
        // Unlock buffer & signal full slot
        pthread_mutex_unlock(&mutex);
        sem_post(&full_slots);
    }
    return NULL;
}