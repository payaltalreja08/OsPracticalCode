// #include <stdio.h>
// #include <stdlib.h>
// #include <pthread.h>
// #include <semaphore.h>
// #include <unistd.h>
// #include <string.h>

// sem_t rw_mutex;         // Controls access to file (only one writer or multiple readers)
// pthread_mutex_t mutex;  // Protects read_count
// int read_count = 0;

// const char *filename = "flights.txt";

// // Function for writer (updates file)
// void write_flight(int writer_id) {
//     FILE *fp = fopen(filename, "r+");
//     if (fp == NULL) {
//         printf("[Writer %d] File not found. Creating new flight file...\n", writer_id);
//         fp = fopen(filename, "w");
//         fprintf(fp, "FlightID\tDate\t\tDestination\tSeats\n");
//         fprintf(fp, "AI101\t2025-11-05\tDelhi\t50\n");
//         fprintf(fp, "AI202\t2025-11-07\tMumbai\t60\n");
//         fprintf(fp, "AI303\t2025-11-10\tChennai\t40\n");
//         fclose(fp);
//         return;
//     }

//     printf("[Writer %d] Writing (booking) flight update...\n", writer_id);

//     // Read current data
//     char lines[5][256];
//     int count = 0;
//     while (fgets(lines[count], sizeof(lines[count]), fp) && count < 5)
//         count++;

//     int flight_to_update = rand() % 3 + 1; // random flight to update
//     int seats;
//     char id[10], date[20], dest[20];
//     sscanf(lines[flight_to_update], "%s %s %s %d", id, date, dest, &seats);

//     if (seats > 0)
//         seats--;
//     sprintf(lines[flight_to_update], "%s\t%s\t%s\t%d\n", id, date, dest, seats);

//     freopen(filename, "w", fp);
//     for (int i = 0; i < count; i++)
//         fputs(lines[i], fp);

//     fclose(fp);
//     printf("[Writer %d] Updated flight %s. Remaining seats: %d\n\n", writer_id, id, seats);
// }

// // Reader thread – only prints that it’s reading
// void *reader(void *arg) {
//     int id = *(int *)arg;
//     while (1) {
//         pthread_mutex_lock(&mutex);
//         read_count++;
//         if (read_count == 1)
//             sem_wait(&rw_mutex);
//         pthread_mutex_unlock(&mutex);

//         printf("[Reader %d] Reading flight data...\n", id);
//         sleep(1);
//         printf("[Reader %d] Done reading.\n\n", id);

//         pthread_mutex_lock(&mutex);
//         read_count--;
//         if (read_count == 0)
//             sem_post(&rw_mutex);
//         pthread_mutex_unlock(&mutex);

//         sleep(rand() % 4 + 1);
//     }
//     return NULL;
// }

// // Writer thread
// void *writer(void *arg) {
//     int id = *(int *)arg;
//     while (1) {
//         sem_wait(&rw_mutex);
//         write_flight(id);
//         sem_post(&rw_mutex);

//         sleep(rand() % 5 + 2);
//     }
//     return NULL;
// }

// int main() {
//     //srand(time(NULL));

//     sem_init(&rw_mutex, 0, 1);
//     pthread_mutex_init(&mutex, NULL);

//     pthread_t readers[3], writers[2];
//     int r_id[3], w_id[2];

//     for (int i = 0; i < 3; i++) {
//         r_id[i] = i + 1;
//         pthread_create(&readers[i], NULL, reader, &r_id[i]);
//     }

//     for (int i = 0; i < 2; i++) {
//         w_id[i] = i + 1;
//         pthread_create(&writers[i], NULL, writer, &w_id[i]);
//     }

//     for (int i = 0; i < 3; i++)
//         pthread_join(readers[i], NULL);
//     for (int i = 0; i < 2; i++)
//         pthread_join(writers[i], NULL);

//     sem_destroy(&rw_mutex);
//     pthread_mutex_destroy(&mutex);
//     return 0;
// }






#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_READERS 5
#define NUM_WRITERS 2


int flightSeats = 100;

// Mutex and semaphore
pthread_mutex_t readerCountLock;
sem_t resourceAccess;

int readerCount = 0;

// Reader thread
void* reader(void* arg) {

    int id = *(int*)arg;

    while(1) {
        // Entry section for reader
        pthread_mutex_lock(&readerCountLock);
        readerCount++;
        if(readerCount == 1)
            sem_wait(&resourceAccess); // First reader locks resource
        pthread_mutex_unlock(&readerCountLock);

        // Critical section: reading flight info
        printf("Reader %d is reading flight info: Seats available = %d\n", id, flightSeats);
        sleep(1);

        // Exit section for reader
        pthread_mutex_lock(&readerCountLock);
        readerCount--;
        if(readerCount == 0)
            sem_post(&resourceAccess); // Last reader unlocks resource
        pthread_mutex_unlock(&readerCountLock);

        sleep(rand() % 3); // Random delay before next read
    }
    
}

// Writer thread
void* writer(void* arg) {
    int id = *(int*)arg;
    while(1) {
        sem_wait(&resourceAccess); // Wait for exclusive access

        // Critical section: updating flight info
        int seatsToBook = rand() % 5 + 1; // Book 1–5 seats
        if(flightSeats >= seatsToBook) {
            flightSeats -= seatsToBook;
            printf("Writer %d booked %d seats | Seats left = %d\n", id, seatsToBook, flightSeats);
        } else {
            printf("Writer %d wanted to book %d seats, but only %d left\n", id, seatsToBook, flightSeats);
        }
        sleep(2);

        sem_post(&resourceAccess); // Release resource
        sleep(rand() % 3);         // Random delay before next booking
    }
}

int main() {
    //srand(time(NULL));

    pthread_t readers[NUM_READERS], writers[NUM_WRITERS];
    int r_id[NUM_READERS], w_id[NUM_WRITERS];

    pthread_mutex_init(&readerCountLock, NULL);
    sem_init(&resourceAccess, 0, 1);

    // Create reader threads
    for(int i = 0; i < NUM_READERS; i++) {
        r_id[i] = i + 1;
        pthread_create(&readers[i], NULL, reader, &r_id[i]);
    }

    // Create writer threads
    for(int i = 0; i < NUM_WRITERS; i++) {
        w_id[i] = i + 1;
        pthread_create(&writers[i], NULL, writer, &w_id[i]);
    }

    // Join threads (infinite loops, so program runs continuously)
    for(int i = 0; i < NUM_READERS; i++)
        pthread_join(readers[i], NULL);
    for(int i = 0; i < NUM_WRITERS; i++)
        pthread_join(writers[i], NULL);

    return 0;
}
