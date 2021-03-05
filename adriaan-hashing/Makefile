

.PHONY: adriaan-hashing test_hashing test_ptr2int2ptr

UNIFICATION_DIR = $(ROOT_DIR)/unification

TESTS = test_hashing test_ptr2int2ptr

adriaan-hashing: test_hashing test_ptr2int2ptr

$(TESTS):
	mkdir -p bin
	$(CC) $(CFLAGS) sources/$@.c sources/dyn_array.c sources/hashtable.c -lm $(USE_UNIFICATION_LIB) -ggdb -o bin/$@

clean:
	rm -rf bin