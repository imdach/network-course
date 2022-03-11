#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>

#define LOG_FATAL(format, ...) \
    do { \
        char buf[1024] = {0}; \
        snprintf(buf, 1024, format, ##__VA_ARGS__); \
        printf(buf); \
        exit(-1); \
    } while (0)


void create_inet_addr(struct sockaddr_in *addr, char *ip, int port) {
    memset(addr, 0, sizeof(struct sockaddr_in));
    addr->sin_family = AF_INET;
    addr->sin_addr.s_addr = inet_addr(ip);
    addr->sin_port = htons(port);
}


int time_elapse(struct timeval *tv_begin) {
    struct timeval tv_cur = {0};
    memcpy(&tv_cur, tv_begin, sizeof(struct timeval));
    gettimeofday(tv_begin, NULL);
    return (tv_begin->tv_sec - tv_cur.tv_sec) * 1000 + (tv_begin->tv_usec - tv_cur.tv_usec) / 1000;
}


int main(int argc, char *argv[]) {

    char remote_ip[16];
    strcpy(remote_ip, "127.0.0.1");

    struct timeval tv_begin;
    gettimeofday(&tv_begin, NULL);

    int fd;
    int remote_port = 8080;
    int success_counter = 0;
    while (success_counter < 50000) {
        fd = socket(AF_INET, SOCK_STREAM, 0);

        if (fd < 0) {
            perror("socket create failed");
            break;
        }

        struct sockaddr_in addr;
        create_inet_addr(&addr, remote_ip, remote_port);

        int rc = connect(fd, (struct sockaddr *) &addr, sizeof(addr));

        if (rc == 0) {
            ++success_counter;
            if (success_counter % 1000 == 0) {
                int time_cost = time_elapse(&tv_begin);
                printf("time cost[%d]: time: %d\n", success_counter, time_cost);
            }
        } else {
            perror("connect failed");
            printf("rc: %d\n", rc);
            close(fd);
            break;
        }
    }
    printf("in the end %d\n", success_counter);
    sleep(1000000);
    getchar();
    return 0;
}