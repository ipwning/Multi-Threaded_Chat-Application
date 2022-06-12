#include "../include/server.h"
#include <signal.h>

client CLIENTS[CLIENT_MAX] = {0, };
sender SENDER;
extern int SOCKFD;

void *sendRoutine(void *);
void *recvRoutine(void *);

void exitHdlr(int sig) {
    pthread_mutex_lock(&mutex_lock);
    for(int i = 0; i < CLIENT_MAX; ++i) {
        if(CLIENTS[i].isOpend) {
            close(CLIENTS[i].fd);
            free(CLIENTS[i].name);
        }
    }
    pthread_mutex_unlock(&mutex_lock);
    puts("Thank you for using :)");
    exit(sig);
}

int main(int argc, const char *argv[]) {
    pthread_t recvThread[5] = {0, };
    pthread_t sendThread;
    _DWORD port, fd;
    _DWORD err;
    
    setup();                        // Call setvbuf for all standard file.
    signal(SIGABRT, exitHdlr);
    signal(SIGINT, exitHdlr);
    port = getPort(argc, argv);     // Get port number.
    err = pthread_create(&sendThread, NULL, sendRoutine, NULL);
    if(err) {
        puts("Error creating thread");
        return 0;
    }
    while(1) {
        for(int i = 0; i < CLIENT_MAX; ++i) {
            if(!CLIENTS[i].isOpend) {
                fd = connectSocket(port);       // Make socket and accpet client connection.
                pthread_mutex_lock(&mutex_lock);
                CLIENTS[i].isOpend = 1;
                CLIENTS[i].fd = fd;
                memset(&recvThread[i], 0, sizeof(pthread_t));
                pthread_mutex_unlock(&mutex_lock);
                err = pthread_create(&recvThread[i], NULL, recvRoutine, (void*)i);
                if(err) {
                    puts("Error creating thread");
                    return 0;
                }
            }
        }
    }
}

void *sendRoutine(void *a) {
    while(1) {
        if(SENDER.used) {
            _BYTE isDisconnect = !strcmp(SENDER.buf, DISCONNECT_STR);
            for(int i = 0; i < CLIENT_MAX; ++i) {
                pthread_mutex_lock(&mutex_lock);
                if(CLIENTS[i].isOpend && CLIENTS[i].fd != SENDER.fd) {
                    if(isDisconnect) {
                        sendMsg(CLIENTS[i].fd, "[");
                        sendMsg(CLIENTS[i].fd, SENDER.name);
                        sendMsg(CLIENTS[i].fd, SENDER.buf);
                    } else {
                        sendMsg(CLIENTS[i].fd, SENDER.name);
                        sendMsg(CLIENTS[i].fd, ": ");
                        sendMsg(CLIENTS[i].fd, SENDER.buf);
                    }
                }
                pthread_mutex_unlock(&mutex_lock);
            }
            printf(isDisconnect ? "[%s%s":"%s: %s", SENDER.name, SENDER.buf);
            free(SENDER.name);
            free(SENDER.buf);
            SENDER.name = NULL;
            SENDER.buf = NULL;
            SENDER.used = 0;
        }
    }
}

void *recvRoutine(void *arg) {
    _QWORD idx = (_QWORD)arg;
    char *msg;
    int l;
    while(1) {
        msg = recvMsg(CLIENTS[idx].fd);
        if(!CLIENTS[idx].name) {
            l = strlen(msg);
            if(msg[l - 1] == '\n') msg[l - 1] = '\0';
            CLIENTS[idx].name = strdup(msg);
            printf("[%s is connected!]\n", msg);
            for(int i = 0; i < CLIENT_MAX; ++i) {
                pthread_mutex_lock(&mutex_lock);
                if(i != idx && CLIENTS[i].isOpend) {
                    sendMsg(CLIENTS[i].fd, "[");
                    sendMsg(CLIENTS[i].fd, msg);
                    sendMsg(CLIENTS[i].fd, " is connected!]\n");
                }
                SENDER.fd = CLIENTS[i].fd;
                pthread_mutex_unlock(&mutex_lock);
            }
        } else {
            pthread_mutex_lock(&mutex_lock);
            SENDER.name = strdup(CLIENTS[idx].name);
            if(!strcmp(msg, "QUIT\n")) {  
                SENDER.buf = strdup(DISCONNECT_STR);
                CLIENTS[idx].isOpend = 0;
                free(CLIENTS[idx].name);
                CLIENTS[idx].name = NULL;
                close(CLIENTS[idx].fd);
                SENDER.fd = -1;
            } else {
                SENDER.buf = strdup(msg);
                SENDER.fd = CLIENTS[idx].fd;
            }
            SENDER.used = 1;
            pthread_mutex_unlock(&mutex_lock);
        }
        free(msg);
        if(SENDER.fd == -1) break;
    }
}