clean:
ifeq ($(shell uname), Darwin)
	find . -perm +111 -type f -exec rm '{}' \;
else
	find .  -maxdepth 1 -type f -executable -exec rm '{}' \;
endif
	rm -f *.o *.S


select_test:clean
	gcc select_test.c -o select_test
epoll_test:clean
	gcc -std=c99 epoll_test.c -o epoll_test

file_epoll:clean
	gcc -std=c99 file_epoll.c -o file_epoll
my_poll_dev_test:clean
	gcc -std=c99 my_poll_dev_test.c -o my_poll_dev_test
