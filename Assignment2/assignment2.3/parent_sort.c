#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void bubbleSort(int arr[], int n) {
    int i, j, temp;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int main() {
    int n, i;
    printf("Enter number of elements: ");
    scanf("%d", &n);

    int arr[n];
    printf("Enter %d integers: ", n);
    for (i = 0; i < n; i++)
        scanf("%d", &arr[i]);

    // Sort before creating the child
    bubbleSort(arr, n);

    printf("\nParent process (PID = %d) sorted array:\n", getpid());
    for (i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");

    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    }
    else if (pid == 0) {
        // Child Process
        printf("\nChild process created (PID = %d)\n", getpid());
        printf("Executing binary_search program using execve()...\n");

        // Convert sorted array to strings for execve
        char *args[n + 2];
        args[0] = "./child_binary";

        for (i = 0; i < n; i++) {
            char *num = malloc(10);
            sprintf(num, "%d", arr[i]);
            args[i + 1] = num;
        }

        args[n + 1] = NULL;

        execve("./child_binary", args, NULL);
        perror("execve failed");
        exit(1);
    }

    else {
        // Parent Process waits for child
        int status;
        printf("\nParent: Waiting for child to complete binary search...\n");
        wait(&status);
        printf("Parent: Child process finished execution.\n");
    }

    return 0;
}
