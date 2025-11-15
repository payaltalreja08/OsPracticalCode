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

    // Access the shared memory created by server
    shmid = shmget(SHM_KEY, sizeof(struct shared_data), 0666);
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

    while (1) {
        sem_wait(&shared->sem_read); // Wait for server to write

        if (strcmp(shared->message, "exit") == 0)
            break;

        printf("Client received: %s\n", shared->message);

        sem_post(&shared->sem_write); // Allow server to write again
    }

    // Detach shared memory
    shmdt(shared);
    printf("Client exiting.\n");
    return 0;
}
