#include <stdio.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netdb.h>
#include <errno.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>

#define LOG_FATAL(format, ...) \
    do { \
        char buf[1024] = {0}; \
        snprintf(buf, 1024, format, ##__VA_ARGS__); \
        printf(buf); \
        exit(-1); \
    } while (0)

int init_sock(short port) {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    fcntl(fd, F_SETFL, O_NONBLOCK);

    struct sockaddr_in serv_addr = {0};

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(port);
    int optval = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    if (bind(fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))) {
        LOG_FATAL("bind error");
    }
    if (listen(fd, 4096) < 0) {
        LOG_FATAL("bind error");
    }
    return fd;
}

#define MAX_EVENT 1024

int main() {

    int listen_fd = init_sock(8080);
    if (listen_fd < 0) {
        perror("listen failed");
    }
    setsockopt(listen_fd, IPPROTO_TCP, TCP_QUICKACK, (int[]){0}, sizeof(int));
    setsockopt(listen_fd, IPPROTO_TCP, TCP_DEFER_ACCEPT, (int[]){0}, sizeof(int));

    int epfd = epoll_create(1);
    struct epoll_event ev;
    ev.data.fd = listen_fd;
    ev.events = EPOLLIN;
    int rc;

    rc = epoll_ctl(epfd, EPOLL_CTL_ADD, listen_fd, &ev);
    if (rc < 0)return -1;

    struct epoll_event events[MAX_EVENT];
    int connections = 0;

    int loop_count = 0;
    while (1) {
        rc = epoll_wait(epfd, events, MAX_EVENT, -1);
        if (rc <= 0) {
            printf("wait: %d\n", rc);
            continue;
        }
        int i;
        for (i = 0; i < rc; ++i) {
            if (events[i].data.fd == listen_fd) {
                int rc = accept(listen_fd, NULL, 0);
            }
        }

    }
    done:
    printf("%s\n", "block getchar");
    getchar();
    return 0;
}