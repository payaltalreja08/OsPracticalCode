#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>
#include <string.h>

#define FILENAME "shared.txt"
#define MAX_ITEMS 10

pthread_mutex_t mutex;
sem_t full, empty;

void *producer(void *arg)
{
    FILE *fp;
    int num;

    for (int i = 0; i < MAX_ITEMS; i++)
    {
        num = rand() % 10; // generate random number between 0–9

        sem_wait(&empty); 
        pthread_mutex_lock(&mutex);

        fp = fopen(FILENAME, "a");
        if (fp == NULL)
        {
            perror("Error opening file");
            exit(1);
        }

        fprintf(fp, "%d\n", num);
        fclose(fp);

        printf("Producer: wrote %d to file\n", num);

        pthread_mutex_unlock(&mutex);
        sem_post(&full);

       sleep(2);
    }
    pthread_exit(NULL);
}

void *consumer(void *arg)
{
    FILE *fp, *temp;
    int num;

    for (int i = 0; i < MAX_ITEMS; i++)
    {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);

        fp = fopen(FILENAME, "r");
        if (fp == NULL)
        {
            perror("Error opening file");
            exit(1);
        }

        if (fscanf(fp, "%d", &num) == 1)
        {
            printf("Consumer: read %d from file\n", num);
        }
        else
        {
            printf("Consumer: no data available\n");
            fclose(fp);
            pthread_mutex_unlock(&mutex);
            sem_post(&empty);
            continue;
        }

        temp = fopen("temp.txt", "w");
        int skip = 1;
        rewind(fp);
        while (fscanf(fp, "%d", &num) == 1)
        {
            if (skip)
            {
                skip = 0; // skip first (consumed) number
                continue;
            }
            fprintf(temp, "%d\n", num);
        }
        fclose(fp);
        fclose(temp);
        remove(FILENAME);
        rename("temp.txt", FILENAME);

        printf("Consumer: removed consumed number from file\n");

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);

        // Slow down consumer as well
        sleep(3); // 3 seconds delay
    }
    pthread_exit(NULL);
}

int main()
{
    //srand(time(NULL));

    pthread_t prod, cons;
    pthread_mutex_init(&mutex, NULL);
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, 1);

    // Clear file before start
    FILE *fp = fopen(FILENAME, "w");
    if (fp != NULL)
        fclose(fp);

    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    pthread_mutex_destroy(&mutex);
    sem_destroy(&full);
    sem_destroy(&empty);

    printf("\nProcess completed.\n");
    return 0;
}
