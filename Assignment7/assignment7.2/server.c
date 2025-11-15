#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>

#define SHM_KEY 0x1234

struct shared_data {
    sem_t sem_write;
    sem_t sem_read;
    char message[100];
};

int main() {
    int shmid;
    struct shared_data *shared;

    // Create shared memory segment
    shmid = shmget(SHM_KEY, sizeof(struct shared_data), 0666 | IPC_CREAT);
    if (shmid < 0) {
        perror("shmget");
        exit(1);
    }

    // Attach shared memory
    shared = (struct shared_data *)shmat(shmid, NULL, 0);
    if (shared == (void *)-1) {
        perror("shmat");
        exit(1);
    }

    // Initialize semaphores (only first time)
    sem_init(&shared->sem_write, 1, 1); // Server starts with permission to write
    sem_init(&shared->sem_read, 1, 0);  // Client must wait for message

    while (1) {
        sem_wait(&shared->sem_write); // Wait until writing allowed

        printf("\nEnter message to send (or type 'exit' to quit): ");
        fgets(shared->message, sizeof(shared->message), stdin);
        shared->message[strcspn(shared->message, "\n")] = '\0';

        sem_post(&shared->sem_read); // Signal client to read

        if (strcmp(shared->message, "exit") == 0)
            break;
    }

    // Detach and delete shared memory
    shmdt(shared);
    shmctl(shmid, IPC_RMID, NULL);

    printf("\nServer exiting. Shared memory deleted.\n");
    return 0;
}
