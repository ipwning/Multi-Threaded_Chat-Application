#include "server.h"

int SOCKFD;

int validateArgv(int argc, const char* argv[]) {  
    int ret = 0;
    int len;
    /*
        Validation check  routine is liked client's thing.
    */
    if(argc != 2) {
        ret = 1;
    } else if( (len = strlen(argv[1])) <= 0) {
        ret = 1;
    }
    else {
        for(int i = 0; i < len; i++) {
            if('9' < argv[1][i] || argv[1][i] < '0')
                ret = 1;
        }
    }
    return ret;
}

int getPort(int argc, const char* argv[]) {
    assert(!validateArgv(argc, argv));      // If does not pass vlidation routine, occured assert.
    return strtoull(argv[1], NULL, 10);     // Return port number.
}

void printAddrInfo(struct sockaddr_in *addr) {
    char ip[INET_ADDRSTRLEN];   // Allocate stack buffer to max ip address length.
    inet_ntop(AF_INET, &(addr->sin_addr), ip, INET_ADDRSTRLEN);         // Get ip address string.
    printf("[Connection from %s:%hu]\n", ip, ntohs(addr->sin_port));    // Print information
}

int connectSocket(int port) {
    struct sockaddr_in addr;
    struct sockaddr_in client_addr;
    int client_addr_size;
    int sockfd; 
    if(!SOCKFD) {
        addr.sin_family = AF_INET;                                              // Set Address family (AF_INET)
        addr.sin_port = htons(port);                                            // Put in port value in addr.sin_port variable.
        addr.sin_addr.s_addr = htonl(INADDR_ANY);                               // Set ip to IPADDR_ANY for accept any remote client.

        assert( ( sockfd = socket(AF_INET, SOCK_STREAM, 0) ) != -1 );           // If fail socket function, occured assert.
        assert( bind( sockfd, (struct sockaddr*)&addr, sizeof(addr) ) != -1  ); // If fail bind function, occured assert.
        assert( listen( sockfd, 5 ) != -1 );                                        // If fail listen function, occured assert.
        SOCKFD = sockfd;
    }
    client_addr_size = sizeof(client_addr);
    assert( (sockfd = accept( SOCKFD, (struct sockaddr_in*)&client_addr, &client_addr_size ) ) != -1 ); // If fail accept function, occured assert.
    printAddrInfo(&client_addr);    // If success the accept to client's connection, print the client information.
    return sockfd;
}