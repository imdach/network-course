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
    serv_addr.sin_port = htons(8080);

    if (::bind(listen_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1) {
        exit(1);
    }

    // 设置 backlog 为 1
    if (::listen(listen_fd, 50) == -1) {
        exit(1);
    }

    struct sockaddr_in cli_addr;
    socklen_t socklen;
    socklen = sizeof(cli_addr);
    while (true) {
        int client_fd = ::accept(listen_fd, (struct sockaddr *) &cli_addr, &socklen);
        std::cout << "client_fd: " << client_fd << std::endl;
    }
    return 0;
}


