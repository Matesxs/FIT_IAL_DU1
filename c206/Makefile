
PRJ=c206
#
PROGS=$(PRJ)-test
CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -pedantic -lm

all: $(PROGS)

.PHONY: test
test: $(PROGS)
	./$(PROGS) > test.out
	@diff $(PRJ)-test.output test.out && echo "Tests passed!" || echo "Tests didn't pass :("

$(PRJ)-test: $(PRJ).c $(PRJ)-test.c
	$(CC) $(CFLAGS) -o $@ $(PRJ).c $(PRJ)-test.c

clean:
	rm -f *.o *.output $(PROGS)
#