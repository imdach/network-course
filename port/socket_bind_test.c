#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void print_local_port() {
    int sockfd;
    if (sockfd = socket(AF_INET, SOCK_STREAM, 0), -1 == sockfd) {
        perror("socket create error");
    }
    const struct sockaddr_in serv_addr = {
            .sin_family = AF_INET,
            .sin_port   = htons(0), // bind(0)
            .sin_addr   = htonl(INADDR_ANY)
    };

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("bind error");
    }

    // 获取本地套接字地址
    const struct sockaddr_in local_addr;
    socklen_t local_addr_len = sizeof(local_addr);
    if (getsockname(sockfd, (struct sockaddr *) &local_addr, &local_addr_len) < 0) {
        perror("getsockname error");
    }

    printf("bind local port: %d\n", ntohs(local_addr.sin_port));
    close(sockfd);
}

int main() {
    int i;
    for (i = 0; i < 10; i++) {
        print_local_port();
    }
    return 0;
}