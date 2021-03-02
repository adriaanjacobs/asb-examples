

.PHONY: adriaan-hashing

adriaan-hashing:
	mkdir -p bin
	$(CC) $(CFLAGS) sources/hashtable.c sources/dyn_array.c sources/main.c -lm -ggdb -o bin/$@

clean:
	rm -rf bin