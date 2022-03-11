clean:
ifeq ($(shell uname), Darwin)
	find . -perm +111 -type f -exec rm '{}' \;
else
	find .  -maxdepth 1 -type f -executable -exec rm '{}' \;
endif
	rm -f *.o *.S


wc:clean
	flex wc.l
	#gcc -o calc lex.yy.c -lfl # linux
	gcc -o wc lex.yy.c -ll # mac
	./wc
calc:clean
	bison -d calc.y
	flex calc.l
	#gcc -o calc lex.yy.c -lfl # linux
	gcc -o calc calc.tab.c lex.yy.c -ll # mac
	./calc
