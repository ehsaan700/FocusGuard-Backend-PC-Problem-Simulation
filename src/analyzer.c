#include "globals.h"

// CONSUMER (Distraction Analyzer)
void *consumer(void *param) {
    int id = *((int *)param);
    
    while(1) {
        // Wait for manual step if in manual mode
        if (manual_mode) sem_wait(&manual_step);
        else sleep(rand() % 3 + 1); // Auto mode sleep
        
        // Wait for full slot
        printf("[Time: %.2fs] [Analyzer %d] Waiting for Data...\n", get_time(start_time), id);
        sem_wait(&full_slots);
        
        // Lock buffer
        pthread_mutex_lock(&mutex);
        
        // Critical Section: Remove data
        char consumed_event[50];
        strcpy(consumed_event, buffer[out]);
        strcpy(buffer[out], ""); // clear slot for display
        printf("\n[Time: %.2fs] [Analyzer %d] Wait(Full) Acquired. Wait(Mutex) Acquired.\n", get_time(start_time), id);
        out = (out + 1) % BUFFER_SIZE;
        count--;
        printf("[Time: %.2fs] [Analyzer %d] Removed Event '%s'. ", get_time(start_time), id, consumed_event);
        print_buffer();
        printf("[Time: %.2fs] [Analyzer %d] Signal(Mutex). Signal(Empty).\n\n", get_time(start_time), id);
        
        //Unlock buffer & signal empty slot
        pthread_mutex_unlock(&mutex);
        sem_post(&empty_slots);
    }
    return NULL;
}