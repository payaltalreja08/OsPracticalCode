#include <stdio.h>

struct Process {
    char name[5];
    int at, bt, wt, tt, ct; // arrival, burst, waiting, turnaround, completion
};

void findFCFS(struct Process p[], int n) {
    int time = 0;
    float total_wt = 0, total_tt = 0;

    printf("\nGANTT CHART:\n");
    printf("________________________\n|");

    for (int i = 0; i < n ; i++) {
        if (time < p[i].at)
            time = p[i].at;

        time += p[i].bt;
        p[i].ct = time;
        p[i].tt = p[i].ct - p[i].at;
        p[i].wt = p[i].tt - p[i].bt;

        total_tt += p[i].tt;
        total_wt += p[i].wt;

        printf(" %s |", p[i].name);
    }

    printf("\n_________________________\n0");

    time = 0;
    for (int i = 0; i < n; i++) {
        if (time < p[i].at)
            time = p[i].at;
        time += p[i].bt;
        printf("   %d", time);
    }

    printf("\n\nAverage Waiting Time = %.2f", total_wt / n);
    printf("\nAverage Turnaround Time = %.2f\n", total_tt / n);
}

void findSJF(struct Process p[], int n) {
   
    int completed = 0, time = 0;
    int is_completed[20] = {0};
    float total_tt = 0, total_wt = 0;

    printf("\nGANTT CHART:\n");
    printf("_________________________\n|");

    while (completed != n) {
        int idx = -1, min_bt = 9999;
        for (int i = 0; i < n; i++) {
            if (p[i].at <= time && is_completed[i] == 0) {
                if (p[i].bt < min_bt) {
                    min_bt = p[i].bt;
                    idx = i;
                }
                if (p[i].bt == min_bt && p[i].at < p[idx].at)
                    idx = i;
            }
        }

        if (idx != -1) {
            time += p[idx].bt;
            p[idx].ct = time;
            p[idx].tt = p[idx].ct - p[idx].at;
            p[idx].wt = p[idx].tt - p[idx].bt;
            total_tt += p[idx].tt;
            total_wt += p[idx].wt;
            is_completed[idx] = 1;
            completed++;

            printf(" %s |", p[idx].name);
        } else {
            time++;
        }
    }

    printf("\n_________________________\n");

    // Time display for Gantt chart
    time = 0;
    printf("0");
    completed = 0;
    while (completed != n) {
        int idx = -1, min_bt = 9999;
        for (int i = 0; i < n; i++) {
            if (p[i].at <= time && is_completed[i] == 1) {
                if (p[i].ct == time + p[i].bt) {
                    idx = i;
                    break;
                }
            }
        }
        if (idx != -1) {
            time = p[idx].ct;
            printf("   %d", time);
            completed++;
        } else break;
    }

    printf("\n\nAverage Waiting Time = %.2f", total_wt / n);
    printf("\nAverage Turnaround Time = %.2f\n", total_tt / n);
}

int main() {
    struct Process p[20];
    int n, choice;

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        printf("\nProcess name: ");
        scanf("%s", p[i].name);
        printf("Arrival time: ");
        scanf("%d", &p[i].at);
        printf("Burst time: ");
        scanf("%d", &p[i].bt);
    }

    while (1) {
        printf("\n---------------------------------------------\n");
        printf("1. FCFS Scheduling\n");
        printf("2. SJF (Non-Preemptive) Scheduling\n");
        printf("3. Exit\n");
        printf("---------------------------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1)
            findFCFS(p, n);
        else if (choice == 2)
            findSJF(p, n);
        else if (choice == 3)
            break;
        else
            printf("Invalid choice!\n");
    }

    return 0;
}
