all: bloom_filter_test;

bloom_filter_test: bloom_filter.c bloom_filter_test.c
	gcc -Wall -Wextra -g -rdynamic -o $@ $^ -lm

clean:
	-rm -f bloom_filter_test
