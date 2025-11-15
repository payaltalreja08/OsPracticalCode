#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5  

int pizzaCounter[BUFFER_SIZE]; // Shared buffer
int in = 0, out = 0;           // Index pointers for buffer

sem_t empty;  // Counts empty slots on counter
sem_t full;   // Counts pizzas on counter
pthread_mutex_t mutex; // Protects critical section

// ---------- PRODUCER (Mr. Simpson) ----------
void* producer(void* arg) {
    int pizza = 1;
    while (1) {
        // Produce a pizza
        sleep(1); // simulate time to bake
        sem_wait(&empty); // Wait if counter is full
        pthread_mutex_lock(&mutex);

        // Place pizza on counter
        pizzaCounter[in] = pizza;
        printf("Mr. Simpson baked Pizza #%d and placed it on the counter at position %d\n", pizza, in);
        in = (in + 1) % BUFFER_SIZE;
        pizza++;

        pthread_mutex_unlock(&mutex);
        sem_post(&full); // Signal that a pizza is available
    }
    return NULL;
}

// ---------- CONSUMER (Joey Tribbiani) ----------
void* consumer(void* arg) {
    int pizza;
    while (1) {
        sem_wait(&full); // Wait if counter is empty
        pthread_mutex_lock(&mutex);

        // Take pizza from counter
        pizza = pizzaCounter[out];
        printf("Joey Tribbiani consumed Pizza #%d from position %d\n", pizza, out);
        out = (out + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex);
        sem_post(&empty); // Signal that a counter slot is free

        sleep(2); // simulate time to eat pizza
    }
    return NULL;
}

// ---------- MAIN FUNCTION ----------
int main() {
    pthread_t prodThread, consThread;

    // Initialize semaphores and mutex
    sem_init(&empty, 0, BUFFER_SIZE); // buffer initially empty
    sem_init(&full, 0, 0);            // no pizzas initially
    pthread_mutex_init(&mutex, NULL);

    printf("=== PIZZA COUNTER SIMULATION STARTED ===\n");
    printf("Mr. Simpson (Producer) and Joey Tribbiani (Consumer) begin their work!\n\n");

    // Create producer and consumer threads
    pthread_create(&prodThread, NULL, producer, NULL);
    pthread_create(&consThread, NULL, consumer, NULL);

    // Let threads run indefinitely
    pthread_join(prodThread, NULL);
    pthread_join(consThread, NULL);

    // Cleanup (not reached here but good practice)
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    return 0;
}


// gcc pizza_counter.c -o pizza_counter -lpthread
// ./pizza_counter