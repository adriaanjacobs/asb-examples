

.PHONY: unitialized-value uninitialized-struct all

all: unitialized-value uninitialized-struct

unitialized-value:
	mkdir -p bin
	$(CC) $(CFLAGS) sources/uninitialized-struct.c -o bin/uninitialized-struct

unitialized-struct:
	mkdir -p bin
	$(CC) $(CFLAGS) sources/uninitialized-value.c -o bin/uninitialized-value

clean:
	rm -rf bin