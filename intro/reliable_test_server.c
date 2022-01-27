#include <stdio.h>
#include <stdio.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netdb.h>
#include <errno.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#define  MAXLINE 1024

#define LOG_ERROR(format, ...) \
    do { \
        char buf[1024] = {0}; \
        snprintf(buf, 1024, format, ##__VA_ARGS__); \
        printf(buf); \
    } while (0)

#define LOG_FATAL(format, ...) \
    do { \
        char buf[1024] = {0}; \
        snprintf(buf, 1024, format, ##__VA_ARGS__); \
        printf(buf); \
        exit(-1); \
    } while (0)

int init_sock(short port) {
    int fd = socket(AF_INET, SOCK_STREAM, 0);

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
static int      read_cnt;
static char     *read_ptr;
static char     read_buf[MAXLINE];
ssize_t my_read(int fd, char *ptr) {
    if (read_cnt <= 0) {
        again:
        if ((read_cnt = read(fd, read_buf, sizeof(read_buf))) < 0) {
            if (errno == EINTR)
                goto again;
            return (-1);
        } else if (read_cnt == 0)
            return (0);
        read_ptr = read_buf;
    }

    read_cnt--;
    *ptr = *read_ptr++;
    return (1);
}

ssize_t
readline(int fd, void *vptr, size_t maxlen) {
    ssize_t n, rc;
    char c, *ptr;

    ptr = vptr;
    for (n = 1; n < maxlen; n++) {
        if ((rc = my_read(fd, &c)) == 1) {
            *ptr++ = c;
            if (c == '\n')
                break;    /* newline is stored, like fgets() */
        } else if (rc == 0) {
            *ptr = 0;
            return (n - 1);    /* EOF, n - 1 bytes were read */
        } else
            return (-1);        /* error, errno set by read() */
    }

    *ptr = 0;    /* null terminate like fgets() */
    return (n);
}

int main() {
    int server_sock_fd = init_sock(9090);
    printf("server socket: %d\n", server_sock_fd);
    ssize_t rc;
    struct sockaddr_in cli_addr;
    socklen_t socklen;
    socklen = sizeof(cli_addr);

    int client_fd = accept(server_sock_fd, (struct sockaddr *) &cli_addr, &socklen);
    printf("client_fd: %d\n", client_fd);
    char buf[20];
    int msg_len;
    int count;
    while ((readline(client_fd, buf, sizeof(buf))) > 0) {
        sleep(5);
        printf(">>>>%s\n", buf);
        msg_len = sprintf(buf, "receive msg %d\n", count++);
        printf(">>>>%s\n", buf);

        rc = send(client_fd, buf, msg_len, 0);
        if (rc < 0) {
            LOG_FATAL("send failed");
        }
    }

    return 0;
}