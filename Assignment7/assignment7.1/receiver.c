#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#define MAX_TEXT 100

// Define message structure
struct msg_buffer {
    long msg_type;
    char msg_text[MAX_TEXT];
};

// Function to convert string to uppercase
void toUpperCase(char *str) {
    for (int i = 0; str[i]; i++)
        str[i] = toupper(str[i]);
}

int main() {
    key_t key;
    int msgid;
    struct msg_buffer message;

    // Generate same key as sender
    key = ftok("msgqueue", 65);
    
    if (key == -1) {
        perror("ftok");
        exit(1);
    }

    // Access message queue
    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget");
        exit(1);
    }

    printf("=== Receiver Process Started ===\n");
    printf("Waiting for messages...\n");

    while (1) {
        // Receive any message type
        if (msgrcv(msgid, &message, sizeof(message.msg_text), 0, 0) == -1) {
            perror("msgrcv");
            exit(1);
        }

        // Exit condition
        if (strcmp(message.msg_text, "exit") == 0) {
            printf("\nReceiver exiting...\n");

            // Remove message queue
            if (msgctl(msgid, IPC_RMID, NULL) == -1) {
                perror("msgctl");
                exit(1);
            }
            printf("Message queue cleaned up.\n");
            break;
        }

        toUpperCase(message.msg_text);
        printf("Received [Type %ld]: %s\n", message.msg_type, message.msg_text);
        fflush(stdout);
    }

    return 0;
}
