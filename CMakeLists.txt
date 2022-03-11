cmake_minimum_required(VERSION 3.21)
project(network_course)

set(CMAKE_CXX_STANDARD 14)

add_executable(network_course main.cpp live/epoll/select_test.c test.c live/epoll/epoll_test.c live/epoll/file_epoll.c live/epoll/my_poll_dev/my_poll_dev.c live/epoll/my_poll_dev_test.c intro/reliable_test_client.c "intro/reliable_test_server.c"
        port/socket_connect_test.c
        port/socket_bind_test.c
        port/connect_test_client.c port/connect_test_server.c port/inet_connection_sock.c
        packetdrill/lex.yy.c
        handshake/syn_retries.c
        terminate/fin_test.cpp
        terminate/fin_test2.cpp
        tcp_state/state_closed.cpp tcp_state/state_listen.cpp
        syn_accept_queue/syn_queue_test_server.cpp
        )
