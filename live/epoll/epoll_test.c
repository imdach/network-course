

#include <sys/socket.h>
#include <memory.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <errno.h>

#define err_exit(msg) do { perror(msg); exit(EXIT_FAILURE);} while (0)

const int EPOLL_SIZE = 50;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        err_exit("args not 2");
    }
    int server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr, client_addr;
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(atoi(argv[1]));

    if (bind(server_socket_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
        printf("bind error\n");
        exit(1);
    }

    if (listen(server_socket_fd, 5) == -1) {
        printf("listen error\n");
        exit(1);
    }

    int epoll_fd = epoll_create(EPOLL_SIZE);

    int socklen;

    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = server_socket_fd;

    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_socket_fd, &event);

    struct epoll_event ep_events[EPOLL_SIZE];
    socklen = sizeof(client_addr);

    while (1) {
        int event_count = epoll_wait(epoll_fd, ep_events, EPOLL_SIZE, -1);
        if (event_count < 0) {
            puts("epoll_wait error");
            break;
        }

        for (int i = 0; i < event_count; ++i) {
            if (ep_events[i].data.fd == server_socket_fd) {
                int client_socket_fd = accept(server_socket_fd,
                                              (struct sockaddr *) &client_addr, &socklen);
                event.events = EPOLLIN;
                event.data.fd = client_socket_fd;
                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_socket_fd, &event);
                printf("connected client %d\n", client_socket_fd);
            } else {
                int socket_fd = ep_events[i].data.fd;
                char buf[BUFSIZ];
                int n = read(socket_fd, buf, BUFSIZ);
                if (n < 0) {
                    if (errno != EAGAIN) {
                        perror("read error");
                        close(socket_fd);
                    }
                    break;
                } else if (n == 0) {
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, socket_fd, NULL);
                    close(socket_fd);
                } else {
                    write(STDIN_FILENO, buf, n);
                }
            }

        }

    }
    close(server_socket_fd);
    close(epoll_fd);
    return 0;
}
