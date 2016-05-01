CC=gcc
CFLAGS=-O3 -s
bin/han: bin/han.o
	mkdir -p bin
	$(CC) $(CFLAGS) $^ -o $@ -l hangul
bin/han.o: han.c
	mkdir -p bin
	$(CC) -c $(CFLAGS) $^ -o $@ -I /usr/include/hangul-1.0

clean:
	rm bin/han bin/han.o
