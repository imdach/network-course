#include <stdio.h>
#include <sys/epoll.h>
#include <fcntl.h>

int main() {
    int epfd, nfds, fd;
    struct epoll_event ev, events[2];

    epfd = epoll_create(1);
    if (epfd < 0) {
        perror("create epoll failed");
        return -1;
    }
    fd = open("watch.txt", O_RDONLY);
    if (fd < 0) {
        perror("open file failed");
        return -1;
    }

    ev.events = EPOLLIN;
    int rc = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
    if (rc < 0) {
        perror("epoll ctl add failed");
        return -1;
    }

    for (;;) {
        nfds = epoll_wait(epfd, events, 2, -1);
        printf("file changed!\n");
    }

    return 0;
}