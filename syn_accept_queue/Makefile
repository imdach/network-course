clean:
ifeq ($(shell uname), Darwin)
	find . -perm +111 -type f -exec rm '{}' \;
else
	find .  -maxdepth 1 -type f -executable -exec rm '{}' \;
endif
	rm -f *.o *.S

syn_queue_test_server:clean
	g++ -g syn_queue_test_server.cpp -o syn_queue_test_server
	./syn_queue_test_server

accept_queue_test_server:clean
	g++ -g accept_queue_test_server.cpp -o accept_queue_test_server
	./accept_queue_test_server

connect_client:
	 go build connect_client.go

view_connection:
	netstat -lnpa | grep :9090  | awk '{print $6}' | sort | uniq -c | sort -rn