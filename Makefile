CC=gcc
CFLAGS=-O3 -s
bin/jamo: bin/jamo.o
	mkdir -p bin
	$(CC) $(CFLAGS) $^ -o $@ -l hangul
bin/jamo.o: jamo.c
	mkdir -p bin
	;$(CC) -c $(CFLAGS) $^ -o $@ -I /usr/include/hangul-1.0

clean:
	rm bin/jamo bin/jamo.o
