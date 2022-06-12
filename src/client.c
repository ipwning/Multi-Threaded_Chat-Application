#include "../include/client.h"
#include <signal.h>
int FD;
_BYTE L;
char *NICKNAME;

void *sendRoutine(void *a){
    char *msg;
    while(L) {
        msg = readBuf(FD);
        sendMsg(FD, msg);
        if(!strcmp(msg, "QUIT\n")) {
            L = 0;
            printf("[%s is disconnected]\n", NICKNAME);
        }
        free(msg);
    }
}

void *recvRoutine(void *a){
    char *msg;
    while(L) {
        msg = recvMsg(FD);
        printf(msg);
        free(msg);
    }
}

void abortHdlr(int sig) {
    puts("Connection faild...");
    close(FD);
    free(NICKNAME);
    exit(-1);
}

void interruptHdlr(int sig) {
    if (FD && L) {
        sendMsg(FD, "QUIT\n");
        close(FD);
    }
    exit(sig);
}

int main(int argc, const char* argv[]) {
    const char* ip;
    int port;
    char *msg;
    pthread_t recvThread;
    pthread_t sendThread;
    int status;
    setup();                            // Call setvbuf for all standard file.
    signal(SIGABRT, abortHdlr);
    signal(SIGINT, interruptHdlr);
    assert(!validateArgv(argc, argv));  // If does not fit argument, occured assert.
    ip = getIP(argv);
    port = getPort(argv);
    FD = connectSocket(ip, port);       // Connect socket by use ip and port.
    NICKNAME = calloc(1, sizeof(char)*strlen(argv[3]) + 0x10);
    strcpy(NICKNAME, argv[3]);
    sendMsg(FD, NICKNAME);               // Send nickname to server
    sendMsg(FD, "\n");
    printf("[%s is connected]\n", NICKNAME);
    L = 1;
    pthread_create(&sendThread, NULL, sendRoutine, NULL);
    pthread_create(&recvThread, NULL, recvRoutine, NULL);
    pthread_join(sendThread, (void**)&status);
    pthread_cancel(recvThread);
    close(FD);
    free(NICKNAME);
    return 0;
}

