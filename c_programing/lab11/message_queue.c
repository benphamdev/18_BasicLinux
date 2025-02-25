#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#define MSG_KEY 1234
/* Define the message structure */
struct mymsg {
    long mtype;       /* Message type */
    char mtext[1024]; /* Message content */
};
/* Sender process */
void sender() {
    int msgid, ret;
    struct mymsg msg;
    /* Create or open the message queue */
    msgid = msgget(MSG_KEY, IPC_CREAT | 0666);
    if (msgid < 0) {
        perror("msgget");
        exit(1);
    }
    /* Construct the message */
    msg.mtype = 1;
    strncpy(msg.mtext, "Hello, waveshare!", 1024);
    /* Send the message */
    ret = msgsnd(msgid, &msg, sizeof(struct mymsg) - sizeof(long), 0);
    if (ret < 0) {
        perror("msgsnd");
        exit(1);
    }
    printf("Sent message: %s\n", msg.mtext);
}
/* Receiver process */
void receiver() {
    int msgid, ret;
    struct mymsg msg;
    /* Open the message queue */
    msgid = msgget(MSG_KEY, IPC_CREAT | 0666);
    if (msgid < 0) {
        perror("msgget");
        exit(1);
    }
    /* Receive the message */
    ret = msgrcv(msgid, &msg, sizeof(struct mymsg) - sizeof(long), 1, 0);
    if (ret < 0) {
        perror("msgrcv");
        exit(1);
    }
    printf("Received message: %s\n", msg.mtext);
}
int main() {
    pid_t pid;
    /* Create a child process */
    pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        /* Child process acts as the sender */
        sender();
    } else {
        /* Parent process acts as the receiver */
        receiver();
    }
    return 0;
}
