#include <cstdio>
#include <sys/socket.h>
#include <cstdlib>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <iostream>

using namespace std;

int main() {
    short port = 9090;
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serv_addr = {0};

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(port);

    int optval = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    if (::bind(fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))) {
        exit(1);
    }
    if (listen(fd, 4096) < 0) {
        exit(1);
    }
    cout << "fd: " << fd << endl;
    std::cin.get();
}