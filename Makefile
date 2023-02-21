CFLAGS = -I. -Os -g

all: csv.o coders main

clean:
	rm -f csv.o coders main

csv.o: csv.c csv.h
	cc $(CFLAGS) -c csv.c

coders: test/coders.c csv.h csv.o
	cc $(CFLAGS) -o coders test/coders.c csv.o

main: test/main.c csv.h csv.o
	cc $(CFLAGS) -o main test/main.c csv.o

check: coders main
	./coders
	./main 0 test/test.csv
	./main 1 test/test.csv
