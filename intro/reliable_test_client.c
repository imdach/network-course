//
// Created by ya on 2022/1/22.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdarg.h>

#define err_exit(msg) do { perror(msg); exit(EXIT_FAILURE);} while (0)
#define  MAXLINE 1024
static int read_cnt;
static char *read_ptr;
static char read_buf[MAXLINE];

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

int tcp_connect(char *address, int port) {
    int socket_fd;
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, address, &server_addr.sin_addr);

    socklen_t server_len = sizeof(server_addr);
    int connect_rt = connect(socket_fd, (struct sockaddr *) &server_addr, server_len);
    if (connect_rt < 0) {
        err_exit("connect failed ");
    }

    return socket_fd;
}

#define set_errno(e)    errno = ( e )
#define SERV_PORT 9090

int readline(int fd, char *bufptr, size_t len) {
    char *bufx = bufptr;
    static char *bp;
    static int cnt = 0;
    static char b[1500];
    char c;

    while (--len > 0) {
        if (--cnt <= 0) {
            cnt = recv(fd, b, sizeof(b), 0);
            if (cnt < 0) {
                if (errno == EINTR) {
                    len++;        /* the while will decrement */
                    continue;
                }
                return -1;
            }
            if (cnt == 0)
                return 0;
            bp = b;
        }
        c = *bp++;
        *bufptr++ = c;
        if (c == '\n') {
            *bufptr = '\0';
            return bufptr - bufx;
        }
    }
    set_errno(EMSGSIZE);
    return -1;
}

ssize_t
readline2(int fd, void *vptr, size_t maxlen) {
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

int main2(int argc, char **argv) {
    int socket_fd = tcp_connect(argv[1], SERV_PORT);
    char buf[20];
    int len;
    int rc;
    while ((fgets(buf, sizeof(buf), stdin)) != NULL) {
        printf(">>>>%s\n", buf);
        len = strlen(buf);
        rc = send(socket_fd, buf, len, 0);
        printf("send: %d\n", rc);
        if (rc < 0) {
            err_exit("send failed");
        }
        rc = readline(socket_fd, buf, sizeof(buf));
        if (rc < 0) {
            err_exit("readline failed");
        } else if (rc == 0) {
            perror("readline failed.");
            err_exit("server terminated\n");
        } else {
            fputs(buf, stdout);
        }

    }

    return 0;
}

char *program_name;

void error(int status, int err, char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    fprintf(stderr, "%s: ", program_name);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    if (err)
        fprintf(stderr, ": %s (%d)\n", strerror(err), err);
    if (status)
        exit(status);
}

int main(int argc, char **argv) {
    char buf[120];
    int rc;
    int len;
    int socket_fd = tcp_connect(argv[1], SERV_PORT);
    while (fgets(buf, sizeof(buf), stdin) != NULL) {
        len = strlen(buf);
        rc = send(socket_fd, buf, len, 0);
        if (rc < 0)
            error(1, errno, "send failed");
        rc = readline(socket_fd, buf, sizeof(buf));
        if (rc < 0)
            error(1, errno, "readline failed");
        else if (rc == 0)
            error(1, 0, "server terminated\n");
        else
            fputs(buf, stdout);
    }
    return 0;
}
