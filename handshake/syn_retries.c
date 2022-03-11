#include <stdio.h>
#include <netdb.h>
#include <errno.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <sys/select.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define LOG_FATAL(format, ...) \
    do { \
        char buf[1024] = {0}; \
        snprintf(buf, 1024, format, ##__VA_ARGS__); \
        printf(buf); \
        exit(-1); \
    } while (0)



int main() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    printf("fd: %d\n", fd);
    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(8080);
    int ret = connect(fd, (struct sockaddr *) &server_addr, sizeof(server_addr));
    printf("ret: %d\n", ret);
    getchar();
}

