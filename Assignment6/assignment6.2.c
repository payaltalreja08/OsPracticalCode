#include <stdio.h>

#define MAX_PAGES 13
#define MAX_FRAMES 10

// Function to check if a page is already in frames
int isPresent(int frames[], int n, int page) {
    for (int i = 0; i < n; i++)
        if (frames[i] == page)
            return 1;
    return 0;
}

// Function to find the LRU page index
int findLRU(int time[], int n) {
    int min = time[0], pos = 0;
    for (int i = 1; i < n; i++) {
        if (time[i] < min) {
            min = time[i];
            pos = i;
        }
    }
    return pos;
}

// Function to find the page to replace in Optimal algorithm
int findOptimal(int pages[], int frames[], int n, int index, int frameCount) {
    int pos = -1, farthest = index;
    for (int i = 0; i < frameCount; i++) {
        int j;
        for (j = index; j < n; j++) {
            if (frames[i] == pages[j]) {
                if (j > farthest) {
                    farthest = j;
                    pos = i;
                }
                break;
            }
        }
        if (j == n) // Page not found in future
            return i;
    }
    return (pos == -1) ? 0 : pos;
}

// LRU Page Replacement Algorithm
int LRU(int pages[], int n, int frameCount) {
    int frames[MAX_FRAMES], time[MAX_FRAMES];
    int pageFaults = 0, counter = 0;

    for (int i = 0; i < frameCount; i++) {
        frames[i] = -1;
        time[i] = 0;
    }

    printf("\n--- LRU Page Replacement (Frame size = %d) ---\n", frameCount);

    for (int i = 0; i < n; i++) {
        int page = pages[i];

        if (!isPresent(frames, frameCount, page)) {
            int pos = -1;
            for (int j = 0; j < frameCount; j++) {
                if (frames[j] == -1) {
                    pos = j;
                    break;
                }
            }
            if (pos == -1)
                pos = findLRU(time, frameCount);

            frames[pos] = page;
            pageFaults++;
        }

        for (int j = 0; j < frameCount; j++) {
            if (frames[j] == page)
                time[j] = ++counter;
        }

        printf("Ref %2d -> ", page);
        for (int j = 0; j < frameCount; j++) {
            if (frames[j] == -1)
                printf("- ");
            else
                printf("%d ", frames[j]);
        }
        printf("\n");
    }

    printf("Total Page Faults (LRU) = %d\n", pageFaults);
    return pageFaults;
}

// Optimal Page Replacement Algorithm
int Optimal(int pages[], int n, int frameCount) {
    int frames[MAX_FRAMES];
    int pageFaults = 0;

    for (int i = 0; i < frameCount; i++)
        frames[i] = -1;

    printf("\n--- Optimal Page Replacement (Frame size = %d) ---\n", frameCount);

    for (int i = 0; i < n; i++) {
        int page = pages[i];

        if (!isPresent(frames, frameCount, page)) {
            int pos = -1;
            for (int j = 0; j < frameCount; j++) {
                if (frames[j] == -1) {
                    pos = j;
                    break;
                }
            }

            if (pos == -1)
                pos = findOptimal(pages, frames, n, i + 1, frameCount);

            frames[pos] = page;
            pageFaults++;
        }

        printf("Ref %2d -> ", page);
        for (int j = 0; j < frameCount; j++) {
            if (frames[j] == -1)
                printf("- ");
            else
                printf("%d ", frames[j]);
        }
        printf("\n");
    }

    printf("Total Page Faults (Optimal) = %d\n", pageFaults);
    return pageFaults;
}

int main() {
    int pages[MAX_PAGES] = {1, 2, 3, 4, 1, 2, 5, 1, 1, 2, 3, 4, 5};
    int n = 13;
    int frameSize, choice;

    printf("Given Page Reference String:\n");
    for (int i = 0; i < n; i++)
        printf("%d ", pages[i]);
    printf("\n-------------------------------------\n");

    // Menu for frame size and algorithm
    printf("Choose Frame Size (3 or 4): ");
    scanf("%d", &frameSize);

    if (frameSize != 3 && frameSize != 4) {
        printf("Invalid frame size! Please enter 3 or 4.\n");
        return 0;
    }

    printf("\nChoose Algorithm:\n");
    printf("1. Optimal Page Replacement\n");
    printf("2. LRU Page Replacement\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    printf("\n-------------------------------------\n");

    switch (choice) {
        case 1:
            Optimal(pages, n, frameSize);
            break;
        case 2:
            LRU(pages, n, frameSize);
            break;
        default:
            printf("Invalid choice!\n");
    }

    printf("\n-------------------------------------\n");
    printf("Program completed.\n");
    return 0;
}
