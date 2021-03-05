

.PHONY: uninitialized-value uninitialized-struct all

all: uninitialized-value uninitialized-struct

uninitialized-struct:
	mkdir -p bin
	$(CC) $(CFLAGS) sources/uninitialized-struct.c -o bin/uninitialized-struct

uninitialized-value:
	mkdir -p bin
	$(CC) $(CFLAGS) sources/uninitialized-value.c -o bin/uninitialized-value

clean:
	rm -rf bin