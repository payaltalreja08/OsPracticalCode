#include <stdio.h>
#include <limits.h>
#include <string.h>

struct Process {
    char name[5];
    int at, bt, remaining_bt, wt, tt, ct;
};

void sortByArrival(struct Process p[], int n) {
    struct Process temp;
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (p[i].at > p[j].at) {
                temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }
        }
    }
}

void FCFS(struct Process p[], int n) {
    sortByArrival(p, n);
    int time = 0;
    float total_wt = 0, total_tt = 0;

    printf("\nGANTT CHART:\n_________________\n|");

    for (int i = 0; i < n; i++) {
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

    printf("\n_________________\n0");
    time = 0;
    for (int i = 0; i < n; i++) {
        if (time < p[i].at)
            time = p[i].at;
        time += p[i].bt;
        printf("   %d", time);
    }

    printf("\n\nAverage Waiting Time (FCFS): %.2f", total_wt / n);
    printf("\nAverage Turnaround Time (FCFS): %.2f\n", total_tt / n);
}

void SJF_Preemptive(struct Process p[], int n) {
    sortByArrival(p, n);

    int completed = 0, time = 0, prev = -1;
    float total_wt = 0, total_tt = 0;
    char gantt[100][5];
    int gantt_time[100];
    int gantt_index = 0;

    for (int i = 0; i < n; i++)
        p[i].remaining_bt = p[i].bt;

    while (completed != n) {
        int idx = -1;
        int min_bt = INT_MAX;

        for (int i = 0; i < n; i++) {
            if (p[i].at <= time && p[i].remaining_bt > 0) {
                if (p[i].remaining_bt < min_bt) {
                    min_bt = p[i].remaining_bt;
                    idx = i;
                }
            }
        }

        if (idx != -1) {
            if (prev != idx) { // record new process in Gantt chart
                strcpy(gantt[gantt_index], p[idx].name);
                gantt_time[gantt_index++] = time;
                prev = idx;
            }

            p[idx].remaining_bt--;
            time++;

            if (p[idx].remaining_bt == 0) {
                p[idx].ct = time;
                p[idx].tt = p[idx].ct - p[idx].at;
                p[idx].wt = p[idx].tt - p[idx].bt;
                total_tt += p[idx].tt;
                total_wt += p[idx].wt;
                completed++;
            }
        } else {
            time++;
        }
    }

    gantt_time[gantt_index] = time;

    // Display Gantt Chart
    printf("\nGANTT CHART:\n_________________\n|");
    for (int i = 0; i < gantt_index; i++)
        printf(" %s |", gantt[i]);
    printf("\n_________________\n");

    for (int i = 0; i <= gantt_index; i++)
        printf("%d   ", gantt_time[i]);
    printf("\n");

    printf("\nAverage Waiting Time (SJF Preemptive): %.2f", total_wt / n);
    printf("\nAverage Turnaround Time (SJF Preemptive): %.2f\n", total_tt / n);
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
        printf("2. SJF (Preemptive) Scheduling\n");
        printf("3. Exit\n");
        printf("---------------------------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1)
            FCFS(p, n);
        else if (choice == 2)
            SJF_Preemptive(p, n);
        else if (choice == 3)
            break;
        else
            printf("Invalid choice!\n");
    }

    return 0;
}
