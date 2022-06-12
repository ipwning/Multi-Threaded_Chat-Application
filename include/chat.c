#include "chat.h"

void setup() {
    setvbuf(stdin, (char *)NULL, _IONBF, 0);        // flush the stdandard input, output, error buffer.
    setvbuf(stdout, (char *)NULL, _IONBF, 0);
    setvbuf(stderr, (char *)NULL, _IONBF, 0);
    pthread_mutex_init(&mutex_lock, NULL);
}

char* readBuf() {
    char *buf;
    char ch;
    int idx = 0;
    assert( (buf = calloc(1, sizeof(char) * 0x210) ) != NULL ); // Allocate empty buffer that sized 0x200.
    do {
        assert(read(0, &ch, 1) != -1);                          // Get input string for each byte.
        buf[idx++] = ch;
        if(idx % 0x200 == 0) { 
            assert( (buf = realloc(buf, sizeof(char) * (idx + 0x210))) != NULL );   // As the input size grows, the buffer is reallocated to a larger size.
        }
    } while(ch != '\n' && ch != '\0');                          // Until newline or NULL byte.
    if(buf[idx - 1] != '\n')  {                                 // if last byte is not newline, change the byte to newline and put in NULL byte in next. 
        buf[idx] = '\0';
        buf[idx - 1] = '\n';
    }
        
    return buf;
}

void sendMsg(int fd, const char* msg) {
    assert(send(fd, msg, strlen(msg), 0) != -1); 
}

char* recvMsg(int fd) {
    int err;
    int idx = 0;
    int ret = 0;
    char ch;
    char *msg;

    assert( (msg = calloc(1, sizeof(char) * 0x210) ) != NULL ); // receive msg routine is liked readBuf. 
    do {
        assert( (err = recv(fd, (char*)&ch, 1, 0) ) != -1 );     // if error while receving, occured assert.
        msg[idx++] = ch;
        if(idx % 0x200 == 0) { 
            assert( (msg = realloc(msg, sizeof(char) * (idx + 0x210))) != NULL );
        }
    } while(ch != '\0' && ch != '\n');
    if(msg[idx - 1] == '\n') msg[idx] = '\0';
    return msg;
}