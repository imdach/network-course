#include <iostream>
#include <sys/socket.h>

int main() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    std::cout << "socket fd: " << fd << std::endl;
    std::cin.get();
}