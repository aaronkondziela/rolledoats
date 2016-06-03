.PHONY: all
all: ro

ro: ro.o
	gcc -g -Wall -std=c99 ro.o -o ro

ro.o: ro.c
	gcc -g -Wall -std=c99 -o ro.o -c ro.c

.PHONY: clean
clean:
	rm -f ro *.o
