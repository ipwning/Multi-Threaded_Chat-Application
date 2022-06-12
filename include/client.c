#include "client.h"

int validateArgv(int argc, const char* argv[]) {
    int ret = 0;
    int len;
    
    if(argc != 4) {                                     // If does not fit the argument count, return -1.
        ret = 1;
    } else if (strlen(argv[1]) <= 0) {                  // If ip string is empty, return -1.
        ret = 1;
    } else if( (len = strlen(argv[2])) <= 0) {          // If port string is empty, return -1.
        ret = 1;
    } else {
        for(int i = 0; i < len; i++) {
            if('9' < argv[2][i] || argv[2][i] < '0')    // If port is not decimal number, return -1.
                ret = 1;
        }
    }
    return ret;
}

const char* getIP(const char* argv[]) {
    return argv[1];                                     // Return ip string.
}

int getPort(const char* argv[]) {
    return strtoull(argv[2], NULL, 10);                 // Return port number.
}

int connectSocket(const char* ip, int port) {
    struct sockaddr_in addr;
    int fd;

    addr.sin_family = AF_INET;                          // Set Address family (AF_INET)
    inet_pton(AF_INET, ip, &(addr.sin_addr));           // Put in ip value in addr.sin_addr variable.
    addr.sin_port = htons(port);                        // Put in port value in addr.sin_port variable.
    assert( ( fd = socket(AF_INET, SOCK_STREAM, 0) ) != -1 );           // If fail socket function, occured assert.
    assert(connect(fd, (struct sockaddr*)&addr, sizeof(addr)) != -1 );  // If fail connect function, occured assert.
    return fd;
}