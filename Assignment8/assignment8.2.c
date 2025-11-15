#include <stdio.h>
#include <stdlib.h>

#define MAX 100

// Function to sort array
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

// Function for SCAN Disk Scheduling
void scan(int requests[], int n, int head, int disk_size) {
    int total = 0, count = 0;
    int temp[MAX];
    for (int i = 0; i < n; i++) temp[i] = requests[i];
    sort(temp, n);

    int index = 0;
    while (index < n && temp[index] < head) index++;

    printf("\n=== SCAN Disk Scheduling ===\n");
    printf("Head starts at: %d\n", head);
    printf("Sequence of cylinder movements:\n%d", head);

    int current = head;

    // Move towards the end (499)
    for (int i = index; i < n; i++) {
        total += abs(temp[i] - current);
        current = temp[i];
        printf(" -> %d", current);
        count++;
    }

    // Reach end (disk boundary)
    total += abs((disk_size - 1) - current);
    current = disk_size - 1;
    printf(" -> %d", current);

    // Reverse direction
    for (int i = index - 1; i >= 0; i--) {
        total += abs(temp[i] - current);
        current = temp[i];
        printf(" -> %d", current);
        count++;
    }

    printf("\nTotal Head Movement (SCAN): %d cylinders", total);
    printf("\nAverage Seek Distance: %.2f\n", (float) total / count);
}

// Function for LOOK Disk Scheduling
void look(int requests[], int n, int head) {
    int total = 0, count = 0;
    int temp[MAX];
    for (int i = 0; i < n; i++) temp[i] = requests[i];
    sort(temp, n);

    int index = 0;
    while (index < n && temp[index] < head) index++;

    printf("\n=== LOOK Disk Scheduling ===\n");
    printf("Head starts at: %d\n", head);
    printf("Sequence of cylinder movements:\n%d", head);

    int current = head;

    // Move towards the end (highest request)
    for (int i = index; i < n; i++) {
        total += abs(temp[i] - current);
        current = temp[i];
        printf(" -> %d", current);
        count++;
    }

    // Reverse direction (go back only to lowest request)
    for (int i = index - 1; i >= 0; i--) {
        total += abs(temp[i] - current);
        current = temp[i];
        printf(" -> %d", current);
        count++;
    }

    printf("\nTotal Head Movement (LOOK): %d cylinders", total);
    printf("\nAverage Seek Distance: %.2f\n", (float) total / count);
}

int main() {
    int disk_size = 500;
    int head = 185;

    int requests[] = {20, 229, 39, 450, 18, 145, 120, 380, 20, 250};
    int n = sizeof(requests) / sizeof(requests[0]);

    printf("System Setup:\n");
    printf("Disk size: 0 - %d\n", disk_size - 1);
    printf("Current Head Position: %d\n", head);
    printf("Pending Requests: ");
    for (int i = 0; i < n; i++) printf("%d ", requests[i]);
    printf("\nInitial direction: Towards 499\n");

    // Run both algorithms
    scan(requests, n, head, disk_size);
    look(requests, n, head);

    
    return 0;
}
