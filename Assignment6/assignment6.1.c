#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_PAGES 20  // length of reference string
#define MAX_PAGE_NO 9 // page numbers from 0–9
#define MAX_FRAMES 7  // max frames allowed

// Function to generate random page reference string
void generateReferenceString(int refStr[], int n) {
    srand(time(0));
    for (int i = 0; i < n; i++) {
        refStr[i] = rand() % (MAX_PAGE_NO + 1);
    }
}

// Function to check if a page is already in frames
int isPresent(int frames[], int n, int page) {
    for (int i = 0; i < n; i++) {
        if (frames[i] == page)
            return 1;
    }
    return 0;
}

// FIFO Page Replacement
int fifoPageReplacement(int refStr[], int n, int framesCount) {
    int frames[MAX_FRAMES];
    for (int i = 0; i < framesCount; i++)
        frames[i] = -1;

    int pageFaults = 0;
    int index = 0;

    printf("\nFIFO Page Replacement:\n");

    for (int i = 0; i < n; i++) {
        int page = refStr[i];
        if (!isPresent(frames, framesCount, page)) {
            frames[index] = page;
            index = (index + 1) % framesCount;
            pageFaults++;
        }

        // Display frames after each step
        printf("Ref %2d -> ", page);
        for (int j = 0; j < framesCount; j++) {
            if (frames[j] == -1)
                printf("- ");
            else
                printf("%d ", frames[j]);
        }
        printf("\n");
    }

    return pageFaults;
}

// LRU Page Replacement
int lruPageReplacement(int refStr[], int n, int framesCount) {

    int frames[MAX_FRAMES], counter[MAX_FRAMES];
   
    for (int i = 0; i < framesCount; i++) {
        frames[i] = -1;
        counter[i] = 0;
    }

    int pageFaults = 0, time = 0;

    printf("\nLRU Page Replacement:\n");

    for (int i = 0; i < n; i++) {
        int page = refStr[i];
        int found = 0;

        for (int j = 0; j < framesCount; j++) {
            if (frames[j] == page) {
                found = 1;
                counter[j] = ++time;
                break;
            }
        }

        if (!found) {
            int minIndex = 0;
            for (int j = 0; j < framesCount; j++) {
                if (frames[j] == -1) {
                    minIndex = j;
                    break;
                } else if (counter[j] < counter[minIndex]) {
                    minIndex = j;
                }
            }
            frames[minIndex] = page;
            counter[minIndex] = ++time;
            pageFaults++;
        }

        // Display frames after each step
        printf("Ref %2d -> ", page);
        for (int j = 0; j < framesCount; j++) {
            if (frames[j] == -1)
                printf("- ");
            else
                printf("%d ", frames[j]);
        }
        printf("\n");
    }

    return pageFaults;
}

// Main function
int main() {
    int refStr[MAX_PAGES];
    int n = MAX_PAGES, framesCount;

    printf("Enter number of page frames (1-7): ");
    scanf("%d", &framesCount);

    if (framesCount < 1 || framesCount > MAX_FRAMES) {
        printf("Invalid number of frames! Must be between 1 and 7.\n");
        return 0;
    }

    generateReferenceString(refStr, n);

    printf("\nGenerated Page Reference String:\n");
    for (int i = 0; i < n; i++)
        printf("%d ", refStr[i]);
    printf("\n");

    int fifoFaults = fifoPageReplacement(refStr, n, framesCount);
    int lruFaults = lruPageReplacement(refStr, n, framesCount);

    printf("\nSummary:\n");
    printf("FIFO Page Faults: %d\n", fifoFaults);
    printf("LRU  Page Faults: %d\n", lruFaults);

    if (fifoFaults < lruFaults)
        printf("\nFIFO performs better in this case.\n");
    else if (lruFaults < fifoFaults)
        printf("\nLRU performs better in this case.\n");
    else
        printf("\nBoth algorithms performed equally.\n");

    return 0;
}
