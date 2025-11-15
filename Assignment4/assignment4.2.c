#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_CHAIRS 3       // Number of waiting chairs outside TA's office
#define NUM_STUDENTS 10     // Total students that will come for help

// Synchronization primitives
sem_t students;     // Counts waiting students
sem_t ta;           // Signals when TA is ready to help
pthread_mutex_t mutex; // Protects access to shared variables

int waiting_students = 0;   // Current count of students waiting

// ---------- Function Declarations ----------
void* teaching_assistant(void* arg);
void* student(void* id);

// ---------- TA THREAD FUNCTION ----------
void* teaching_assistant(void* arg) {
    while (1) {
        // Wait until at least one student arrives
        sem_wait(&students);

        // TA wakes up and locks mutex to access waiting_students safely
        pthread_mutex_lock(&mutex);

        waiting_students--; // One student will now be helped
        printf("TA is helping a student. No. of Students waiting: %d\n", waiting_students);

        pthread_mutex_unlock(&mutex);

        // Signal that TA is ready to help this student
        sem_post(&ta);

        // Simulate time spent helping student
        sleep(3);

        printf("TA finished helping the student and is ready for next one.\n");
    }
    return NULL;
}

// ---------- STUDENT THREAD FUNCTION ----------
void* student(void* id) {
    int student_id = *(int*)id;

    while (1) {
        // Student arrives and tries to get help
        sleep(rand() % 5 + 1); // Random arrival time

        pthread_mutex_lock(&mutex);

        if (waiting_students < NUM_CHAIRS) {
            // There is a waiting chair available
            waiting_students++;
            printf("Student %d is waiting. No. of Students waiting: %d\n", student_id, waiting_students);

            // Signal TA that a student is waiting
            sem_post(&students);

            pthread_mutex_unlock(&mutex);

            // Wait for TA to be ready
            sem_wait(&ta);

            // Student is being helped now
            printf("Student %d is getting help from TA.\n", student_id);

        } else {
            // No chairs available — student leaves
            pthread_mutex_unlock(&mutex);
            printf("Student %d found no empty chair and will come back later.\n", student_id);
        }
    }
    return NULL;
}

// ---------- MAIN FUNCTION ----------
int main() {
    //srand(time(NULL));

    pthread_t taThread;
    pthread_t studentThreads[NUM_STUDENTS];
    int student_ids[NUM_STUDENTS];

    // Initialize semaphores and mutex
    sem_init(&students, 0, 0);
    sem_init(&ta, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    // Create TA thread
    pthread_create(&taThread, NULL, teaching_assistant, NULL);

    // Create multiple student threads
    for (int i = 0; i < NUM_STUDENTS; i++) {
        student_ids[i] = i + 1;
        pthread_create(&studentThreads[i], NULL, student, &student_ids[i]);
    }

    // Join threads (infinite loop — not reached, but for completeness)
    pthread_join(taThread, NULL);
    for (int i = 0; i < NUM_STUDENTS; i++) {
        pthread_join(studentThreads[i], NULL);
    }

    // Cleanup
    sem_destroy(&students);
    sem_destroy(&ta);
    pthread_mutex_destroy(&mutex);

    return 0;
}



// gcc sleeping_ta.c -o sleeping_ta -lpthread
// ./sleeping_ta
