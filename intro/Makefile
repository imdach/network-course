clean:
ifeq ($(shell uname), Darwin)
	find . -perm +111 -type f -exec rm '{}' \;
else
	find .  -maxdepth 1 -type f -executable -exec rm '{}' \;
endif
	rm -f *.o *.S

reliable_test:clean
	gcc -g reliable_test_client.c -o reliable_test_client
	gcc -g reliable_test_server.c -o reliable_test_server
