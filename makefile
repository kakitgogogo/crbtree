all : test

test : test.c rbtree.c
	gcc -g -Wall -o $@ $^

clean :
	rm test