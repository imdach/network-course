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
    // 连接本地的 8080 端口的服务器
    const struct sockaddr_in remote_addr = {
            .sin_family = AF_INET,
            .sin_port   = htons(8080)
//                    ,
//                                  .sin_addr   = htonl(INADDR_ANY)
            , .sin_addr   = inet_addr("127.0.0.1")
    };
    if (connect(sockfd, (const struct sockaddr *) &remote_addr, sizeof(remote_addr)) < 0) {
        perror("connect error");
    }
    // 获取本地套接字地址
    const struct sockaddr_in local_addr;
    socklen_t local_addr_len = sizeof(local_addr);
    if (getsockname(sockfd, (struct sockaddr *) &local_addr, &local_addr_len) < 0) {
        perror("getsockname error");
    }
    printf("local port: %d\n", ntohs(local_addr.sin_port));
    close(sockfd);
}

int main() {
    int i;
    for (i = 0; i < 10; i++) {
        print_local_port();
    }
    return 0;
}