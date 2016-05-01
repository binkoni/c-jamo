CC=gcc
CFLAGS=-O3 -s
han: han.o
	$(CC) $(CFLAGS) $^ -o $@ -l hangul
han.o: han.c
	$(CC) -c $(CFLAGS) $^ -o $@ -I /usr/include/hangul-1.0

clean:
	rm han han.o
