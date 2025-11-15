#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <math.h>


int isPrime(int num) {
    if (num < 2)
        return 0;
    for (int i = 2; i <= sqrt(num); i++) {
        if (num % i == 0)
            return 0;
    }
    return 1;
}

int main(int argc, char *argv[]) {
    
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <positive integer>\n", argv[0]);
        exit(1);
    }
    
    int n = atoi(argv[1]);
    
    if (n <= 0) {
        fprintf(stderr, "Error: Please enter a positive integer.\n");
        exit(1);
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    }

    else if (pid == 0) {
        
        printf("\nChild Process (PID: %d)\n", getpid());
        printf("First %d prime numbers:\n", n);

        int count = 0, num = 2;
        while (count < n) {
            if (isPrime(num)) {
                printf("%d ", num);
                count++;
            }
            num++;
        }
        printf("\nChild: Prime sequence generation complete.\n");
        exit(0);
    }

    else {
        
        printf("\nParent Process (PID: %d) waiting for child process...\n", getpid());
        int status;
        wait(&status);
        printf("Parent: Child process finished execution. Exiting.\n");
    }

    return 0;
}


//gcc filename -lm