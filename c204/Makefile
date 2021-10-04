
PRJ=c204
#
C202PATH=../c202/
PROGS=$(PRJ)-test
CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -pedantic -fcommon -lm -I$(C202PATH) -g

all: $(PROGS)

.PHONY: test
test: $(PROGS)
	./$(PROGS) > test.out
	@diff $(PRJ)-test.output test.out && echo "Tests passed!" || echo "Tests didn't pass :("

$(PRJ)-test: $(PRJ).c $(PRJ)-test.c $(C202PATH)c202.c
	$(CC) $(CFLAGS) -o $@ $(PRJ).c $(PRJ)-test.c $(C202PATH)c202.c

clean:
	rm -f *.o *.output $(PROGS)
#