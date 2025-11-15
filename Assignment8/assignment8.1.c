#include <stdio.h>
#include <stdlib.h>

#define MAX 100

// Function to sort array in ascending order
void sort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// Function for C-SCAN disk scheduling
void cscan(int requests[], int n, int head, int disk_size) {
    int total = 0, count = 0;
    int temp[MAX];
    for (int i = 0; i < n; i++) temp[i] = requests[i];
    sort(temp, n);

    // Find index where head lies
    int index = 0;
    while (index < n && temp[index] < head) index++;

    printf("\n=== C-SCAN Disk Scheduling ===\n");
    printf("Head starts at: %d\n", head);
    printf("Sequence of cylinder movements:\n%d", head);

    int current = head;

    // Move toward the end
    for (int i = index; i < n; i++) {
        total += abs(temp[i] - current);
        current = temp[i];
        printf(" -> %d", current);
        count++;
    }

    // Move to the end of the disk (499)
    total += abs((disk_size - 1) - current);
    current = 0; // jump to start (not counted as seek)
    printf(" -> 499 -> 0");

    // Service the remaining requests
    for (int i = 0; i < index; i++) {
        total += abs(temp[i] - current);
        current = temp[i];
        printf(" -> %d", current);
        count++;
    }

    printf("\nTotal Head Movement (C-SCAN): %d cylinders", total);
    printf("\nAverage Seek Distance: %.2f\n", (float) total / count);
}

// Function for C-LOOK disk scheduling
void clook(int requests[], int n, int head) {
    int total = 0, count = 0;
    int temp[MAX];
    for (int i = 0; i < n; i++) temp[i] = requests[i];
    sort(temp, n);

    // Find index where head lies
    int index = 0;
    while (index < n && temp[index] < head) index++;

    printf("\n=== C-LOOK Disk Scheduling ===\n");
    printf("Head starts at: %d\n", head);
    printf("Sequence of cylinder movements:\n%d", head);

    int current = head;

    // Move toward the end
    for (int i = index; i < n; i++) {
        total += abs(temp[i] - current);
        current = temp[i];
        printf(" -> %d", current);
        count++;
    }

    // Jump to the lowest request (not counted)
    if (index > 0) {
        current = temp[0];
        printf(" -> %d", current);
    }

    // Service remaining requests
    for (int i = 0; i < index; i++) {
        total += abs(temp[i] - current);
        current = temp[i];
        printf(" -> %d", current);
        count++;
    }

    printf("\nTotal Head Movement (C-LOOK): %d cylinders", total);
    printf("\nAverage Seek Distance: %.2f\n", (float) total / count);
}

int main() {
    int disk_size = 500;
    int head = 85;

    int requests[] = {10, 229, 39, 400, 18, 145, 120, 480, 20, 250};
    int n = sizeof(requests) / sizeof(requests[0]);

    printf("System Setup:\n");
    printf("Disk size: 0 - %d\n", disk_size - 1);
    printf("Current Head Position: %d\n", head);
    printf("Pending Requests: ");
    for (int i = 0; i < n; i++) printf("%d ", requests[i]);
    printf("\nInitial direction: Towards 499\n");

    // Run both algorithms
    cscan(requests, n, head, disk_size);
    clook(requests, n, head);

    

    return 0;
}
