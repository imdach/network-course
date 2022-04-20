#include <iostream>

#include <cstdio>
#include <sys/socket.h>
#include <cstdlib>
#include <cstring>
#include <arpa/inet.h>

int main() {
    std::cout << "server start" << std::endl;

    int listen_fd;
    if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        exit(1);
    }
    struct sockaddr_in serv_addr{};
    bzero(&serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(9090);

    if (::bind(listen_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1) {
        exit(1);
    }

    // 设置 backlog 为 50
    if (::listen(listen_fd, 50) == -1) {
        exit(1);
    }

    // 下面没有 accept 调用
    std::cin.get();
    return 0;
}


