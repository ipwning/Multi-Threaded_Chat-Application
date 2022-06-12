#include "chat.h"

/*
    function definition that used in src/client.c
*/
int validateArgv(int argc, const char* argv[]);
const char* getIP(const char* argv[]);
int getPort(const char * argv[]);
int connectSocket(const char* ip, int port);    