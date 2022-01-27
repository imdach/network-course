#include <stdlib.h>
#include <sys/select.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define  MAXLINE     1024

#define err_exit(msg) do { perror(msg); exit(EXIT_FAILURE);} while (0)
#define  SERV_PORT 9090

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

int main(int argc, char **argv) {
    if (argc != 2) {
        err_exit("usage: select_test remote_ip");
    }
    int socket_fd = tcp_connect(argv[1], SERV_PORT);
    char recv_line[MAXLINE], send_line[MAXLINE];
    int n;

    fd_set read_mask;
    fd_set all_reads;
    FD_ZERO(&all_reads);
    // 注册 STDIN
    FD_SET(STDIN_FILENO, &all_reads);
    // 注册 socket fd
    FD_SET(socket_fd, &all_reads);

    for (;;) {
        read_mask = all_reads;
        int rc = select(socket_fd + 1, &read_mask, NULL, NULL, NULL);

        if (rc <= 0) {
            err_exit("select failed");
        }

        if (FD_ISSET(socket_fd, &read_mask)) {
            n = read(socket_fd, recv_line, MAXLINE);
            if (n < 0) {
                err_exit("read error");
            } else if (n == 0) {
                err_exit("server terminated \n");
            }
            recv_line[n] = 0;
            fputs(recv_line, stdout);
            fputs("\n", stdout);
        }

        if (FD_ISSET(STDIN_FILENO, &read_mask)) {
            if (fgets(send_line, MAXLINE, stdin) != NULL) {
                int i = strlen(send_line);
                if (send_line[i - 1] == '\n') {
                    send_line[i - 1] = 0;
                }

                printf("now sending %s\n", send_line);
                size_t rt = write(socket_fd, send_line, strlen(send_line));
                if (rt < 0) {
                    err_exit("write failed ");
                }
                printf("send bytes: %zu \n", rt);
            }
        }
    }
}