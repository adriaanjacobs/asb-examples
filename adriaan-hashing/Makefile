

.PHONY: adriaan-hashing $(TESTS)

UNIFICATION_DIR = $(ROOT_DIR)/unification

TESTS = test_hashing test_ptr2int2ptr test_dyn_alloc

adriaan-hashing: $(TESTS)

$(TESTS):
	mkdir -p bin
	$(CC) $(CFLAGS) sources/$@.c sources/dyn_array.c sources/hashtable.c -lm -ldl $(USE_UNIFICATION_LIB) -ggdb -o bin/$@

clean:
	rm -rf bin