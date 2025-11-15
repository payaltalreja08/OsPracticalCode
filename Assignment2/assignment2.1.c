#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

// ---------- Sorting Functions ---------------
void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}



void insertionSort(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

// ---------- Main Function ----------
int main() {
    int n, choice;

    printf("\n---- MENU ----\n");
    printf("1. Normal Execution (Parent waits for Child)\n");
    printf("2. Demonstrate Zombie Process\n");
    printf("3. Demonstrate Orphan Process\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    printf("Enter number of elements: ");
    scanf("%d", &n);

    int arr[n], childArr[n];
    printf("Enter %d integers: ", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
        childArr[i] = arr[i];
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    }

    else if (pid == 0) {
        // ---------- CHILD PROCESS ----------
        printf("\n[CHILD] PID = %d, PPID = %d\n", getpid(), getppid());
        insertionSort(childArr, n);
        printf("[CHILD] Sorted array using Insertion Sort: ");
        for (int i = 0; i < n; i++)
            printf("%d ", childArr[i]);
        printf("\n");

        if (choice == 3) {
            // Orphan demonstration
            printf("[CHILD] Sleeping for 10 seconds to become orphan...\n");
            sleep(10);
            printf("[CHILD] After becoming orphan, new Parent PID = %d\n", getppid());
        }

        printf("[CHILD] Exiting...\n");
        exit(0);
    }

    else {
        // ---------- PARENT PROCESS ----------
        printf("\n[PARENT] PID = %d, Child PID = %d\n", getpid(), pid);
        bubbleSort(arr, n);
        printf("[PARENT] Sorted array using Bubble Sort: ");
        for (int i = 0; i < n; i++)
            printf("%d ", arr[i]);
        printf("\n");

        if (choice == 1) {
            // Normal execution
            printf("[PARENT] Waiting for child to complete...\n");
            wait(NULL);
            printf("[PARENT] Child process finished. Normal execution completed successfully.\n");
        }

        else if (choice == 2) {
            // Zombie demonstration
            printf("[PARENT] Sleeping for 10 seconds. Child will become zombie...\n");
            
            sleep(10);
            wait(NULL); // Reap the child
            printf("[PARENT] Child reaped. Zombie removed.\n");
        }

        else if (choice == 3) {
            // Orphan demonstration
            printf("[PARENT] Exiting immediately. Child will become orphan.\n");
            
            exit(0);
        }
    }

    return 0;
}
