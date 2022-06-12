#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#include <stdint.h>

#include <pthread.h>

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define CLIENT_MAX 5

typedef uint8_t _BYTE;
typedef uint16_t _WORD;
typedef uint32_t _DWORD;
typedef uint64_t _QWORD;

pthread_mutex_t mutex_lock;

void setup();
char* readBuf();
void sendMsg(int fd, const char* msg);
char* recvMsg(int fd);