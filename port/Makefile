clean:
ifeq ($(shell uname), Darwin)
	find . -perm +111 -type f -exec rm '{}' \;
else
	find .  -maxdepth 1 -type f -executable -exec rm '{}' \;
endif
	rm -f *.o *.S


port_test:clean
	gcc socket_bind_test.c -o socket_bind_test
	gcc socket_connect_test.c -o socket_connect_test

connect_time_cost:clean
	gcc connect_test_client.c -o connect_test_client
	gcc connect_test_server.c -o connect_test_server
