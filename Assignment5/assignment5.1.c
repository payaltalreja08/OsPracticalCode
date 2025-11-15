#include <stdio.h>

int main() {
    int n, m, i, j, k;
    printf("Enter number of processes: ");
    scanf("%d", &n);
    printf("Enter number of resources: ");
    scanf("%d", &m);

    int alloc[n][m], max[n][m], avail[m], need[n][m];
    int finish[n], safeSeq[n];
    
    printf("\nEnter Allocation Matrix:\n");
    for (i = 0; i < n; i++)
        for (j = 0; j < m; j++)
            scanf("%d", &alloc[i][j]);

    printf("\nEnter Maximum Matrix:\n");
    for (i = 0; i < n; i++)
        for (j = 0; j < m; j++)
            scanf("%d", &max[i][j]);

    printf("\nEnter Available Resources:\n");
    for (i = 0; i < m; i++)
        scanf("%d", &avail[i]);

    // Calculate Need matrix
    for (i = 0; i < n; i++)
        for (j = 0; j < m; j++)
            need[i][j] = max[i][j] - alloc[i][j];

    printf("\nNeed Matrix:\n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++)
            printf("%d ", need[i][j]);
        printf("\n");
    }

    for (i = 0; i < n; i++)
        finish[i] = 0;

    int count = 0;
    while (count < n) {
        int found = 0;
        for (i = 0; i < n; i++) {
            if (finish[i] == 0) {
                int canRun = 1;
                for (j = 0; j < m; j++)
                    if (need[i][j] > avail[j]) {
                        canRun = 0;
                        break;
                    }

                if (canRun) {
                    for (k = 0; k < m; k++)
                        avail[k] += alloc[i][k];

                    safeSeq[count++] = i;
                    finish[i] = 1;
                    found = 1;
                }
            }
        }

        if (found == 0)
            break;
    }

    if (count == n) {
        printf("\nSystem is in a SAFE STATE.\nSafe Sequence: ");
        for (i = 0; i < n; i++)
            printf("P%d ", safeSeq[i]);
        printf("\n");
    } else {
        printf("\nSystem is in an UNSAFE STATE.\n");
        printf("Following processes could not complete due to insufficient available resources:\n");

        for (i = 0; i < n; i++) {
            if (finish[i] == 0) {
                printf("Process P%d cannot complete because it needs (", i);
                for (j = 0; j < m; j++) {
                    printf("%d", need[i][j]);
                    if (j < m - 1) printf(", ");
                }
                printf(") but only (");
                for (j = 0; j < m; j++) {
                    printf("%d", avail[j]);
                    if (j < m - 1) printf(", ");
                }
                printf(") are available.\n");
            }
        }
    }

    return 0;
}
