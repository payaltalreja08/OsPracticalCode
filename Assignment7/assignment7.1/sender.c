#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#define MAX_TEXT 100

// Define message structure
struct msg_buffer {
    long msg_type;
    char msg_text[MAX_TEXT];
};

int main() {
    key_t key;
    int msgid;
    struct msg_buffer message;

    // Generate unique key
    key = ftok("msgqueue", 65);

    if (key == -1) {
        perror("ftok");
        exit(1);
    }

    // Create message queue
    msgid = msgget(key, 0666 | IPC_CREAT);
    
    if (msgid == -1) {
        perror("msgget");
        exit(1);
    }

    printf("=== Sender Process Started ===\n");
    printf("Enter messages to send (type 'exit' to quit)\n");

    while (1) {
        printf("\nEnter message type (integer > 0): ");
        scanf("%ld", &message.msg_type);
        getchar(); // clear newline

        printf("Enter message text: ");
        fgets(message.msg_text, MAX_TEXT, stdin);
        message.msg_text[strcspn(message.msg_text, "\n")] = '\0';

        // Exit condition
        if (strcmp(message.msg_text, "exit") == 0) {
            msgsnd(msgid, &message, sizeof(message.msg_text), 0);
            printf("Exiting Sender...\n");
            break;
        }

        // Send message
        if (msgsnd(msgid, &message, sizeof(message.msg_text), 0) == -1) {
            perror("msgsnd");
            exit(1);
        }

        printf("Sent: [Type %ld] \"%s\"\n", message.msg_type, message.msg_text);
    }

    return 0;
}