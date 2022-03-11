clean:
ifeq ($(shell uname), Darwin)
	find . -perm +111 -type f -exec rm '{}' \;
else
	find .  -maxdepth 1 -type f -executable -exec rm '{}' \;
endif
	rm -f *.o *.S

state_closed:clean
	g++ -g state_closed.cpp -o state_closed
	./state_closed

state_listen:clean
	g++ -g state_listen.cpp -o state_listen
	./state_listen


