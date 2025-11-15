#include <stdio.h>
#include <stdlib.h>

int n, m;
int alloc[10][10], max[10][10], avail[10];
int need[10][10];
int safeSeq[10];

void readData() {
    FILE *fp = fopen("state.txt", "r");
    if (fp == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }

    fscanf(fp, "%d", &n);
    fscanf(fp, "%d", &m);

    printf("\nNumber of Processes: %d", n);
    printf("\nNumber of Resources: %d\n", m);

    printf("\nAllocation Matrix:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            fscanf(fp, "%d", &alloc[i][j]);
            printf("%d ", alloc[i][j]);
        }
        printf("\n");
    }

    printf("\nMax Matrix:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            fscanf(fp, "%d", &max[i][j]);
            printf("%d ", max[i][j]);
        }
        printf("\n");
    }

    printf("\nAvailable Resources:\n");
    for (int i = 0; i < m; i++) {
        fscanf(fp, "%d", &avail[i]);
        printf("%d ", avail[i]);
    }
    printf("\n");

    fclose(fp);
}

void calculateNeed() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            need[i][j] = max[i][j] - alloc[i][j];
        }
    }
}

int isSafe() {
    int work[10], finish[10] = {0}, count = 0;

    for (int i = 0; i < m; i++)
        work[i] = avail[i];

    while (count < n) {
        int found = 0;
        for (int i = 0; i < n; i++) {
            if (!finish[i]) {
                int j;
                for (j = 0; j < m; j++)
                    if (need[i][j] > work[j])
                        break;
                if (j == m) {
                    for (int k = 0; k < m; k++)
                        work[k] += alloc[i][k];
                    safeSeq[count++] = i;
                    finish[i] = 1;
                    found = 1;
                }
            }
        }
        if (!found) {
            printf("\nSystem is in an UNSAFE state.\n");
            printf("Reason: At least one process cannot be completed because its resource needs exceed available resources.\n");
            return 0;
        }
    }

    printf("\nSystem is in a SAFE state.\nSafe sequence: ");
    for (int i = 0; i < n; i++)
        printf("P%d ", safeSeq[i]);
    printf("\n");
    return 1;
}

void requestResources() {
    int req[10], p;
    printf("\nEnter process number making the request (0-%d): ", n - 1);
    scanf("%d", &p);

    printf("Enter the resource request vector (%d values): ", m);
    for (int i = 0; i < m; i++)
        scanf("%d", &req[i]);

    for (int i = 0; i < m; i++) {
        if (req[i] > need[p][i]) {
            printf("\nError: Process has exceeded its maximum claim.\n");
            return;
        }
        if (req[i] > avail[i]) {
            printf("\nResources are not available. Request cannot be granted.\n");
            return;
        }
    }

    // Pretend to allocate resources
    for (int i = 0; i < m; i++) {
        avail[i] -= req[i];
        alloc[p][i] += req[i];
        need[p][i] -= req[i];
    }

    if (isSafe()) {
        printf("Request can be GRANTED.\n");
    } else {
        printf("Request CANNOT be granted. Rolling back...\n");
        for (int i = 0; i < m; i++) {
            avail[i] += req[i];
            alloc[p][i] -= req[i];
            need[p][i] += req[i];
        }
    }
}

int main() {
    readData();
    calculateNeed();
    printf("\nChecking initial system state...");
    isSafe();
    requestResources();
    return 0;
}
