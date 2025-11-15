#include <stdio.h>
#include <string.h>

struct Process {
    char name[5];
    int at, bt, remaining_bt, ct, wt, tt;
};

// Function to sort processes by arrival time
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

// FCFS Scheduling
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

// Round Robin Scheduling (Preemptive)
void RoundRobin(struct Process p[], int n, int tq) {
    sortByArrival(p, n);
    int time = 0, completed = 0;
    float total_wt = 0, total_tt = 0;

    int queue[100], front = 0, rear = 0, visited[100] = {0};
    char gantt[100][5];
    int gantt_time[100];
    int gantt_index = 0;

    for (int i = 0; i < n; i++)
        p[i].remaining_bt = p[i].bt;

    // Initially push first process
    queue[rear++] = 0;
    visited[0] = 1;

    while (completed != n) {
        int idx = queue[front++];
        if (front > 99) front = 0; // wrap around

        // Record Gantt chart
        strcpy(gantt[gantt_index], p[idx].name);
        gantt_time[gantt_index++] = time;

        if (p[idx].remaining_bt <= tq) {
            time += p[idx].remaining_bt;
            p[idx].remaining_bt = 0;
            completed++;
            p[idx].ct = time;
            p[idx].tt = p[idx].ct - p[idx].at;
            p[idx].wt = p[idx].tt - p[idx].bt;
            total_tt += p[idx].tt;
            total_wt += p[idx].wt;
        } else {
            time += tq;
            p[idx].remaining_bt -= tq;
        }

        // Add new processes that have arrived
        for (int i = 0; i < n; i++) {
            if (p[i].at <= time && !visited[i] && p[i].remaining_bt > 0) {
                queue[rear++] = i;
                if (rear > 99) rear = 0;
                visited[i] = 1;
            }
        }

        // If current process not finished, re-add it
        if (p[idx].remaining_bt > 0)
            queue[rear++] = idx;
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

    printf("\nAverage Waiting Time (Round Robin): %.2f", total_wt / n);
    printf("\nAverage Turnaround Time (Round Robin): %.2f\n", total_tt / n);
}

int main() {
    struct Process p[20];
    int n, choice;
    int tq = 2; // Time Quantum

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
        printf("2. Round Robin (Preemptive)\n");
        printf("3. Exit\n");
        printf("---------------------------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1)
            FCFS(p, n);
        else if (choice == 2)
            RoundRobin(p, n, tq);
        else if (choice == 3)
            break;
        else
            printf("Invalid choice!\n");
    }

    return 0;
}
