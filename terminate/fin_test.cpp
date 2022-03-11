#include <iostream>
#include <cstring>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(9090);
    int ret = connect(fd, (struct sockaddr *) &server_addr, sizeof(server_addr));
    std::cout << "connect done, ret: " << ret << std::endl;
    std::cin.get();
    std::cout << "closing socket" << std::endl;
    close(fd); // 关闭 socket，触发发送 FIN
    std::cin.get();
}





