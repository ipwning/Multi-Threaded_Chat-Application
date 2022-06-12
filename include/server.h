#include "chat.h"

typedef struct client {
    int fd;
    char *name;
    _BYTE isOpend;
} client;

typedef struct sender {
    char *name;
    char *buf;
    int fd;
    _BYTE used;
} sender;

int connectSocket(int port);
int getPort(int argc, const char* argv[]);
