clean:
ifeq ($(shell uname), Darwin)
	find . -perm +111 -type f -exec rm '{}' \;
else
	find .  -maxdepth 1 -type f -executable -exec rm '{}' \;
endif
	rm -f *.o *.S

fin_test:clean
	g++ -g fin_test.cpp -o fin_test
	./fin_test

fin_test2:clean
	g++ -g fin_test2.cpp -o fin_test2
	./fin_test2
