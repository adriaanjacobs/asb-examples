UNIFY_IMPLS = unify_allocator_impl unify_64bit_impl

.PHONY: unification dyn_alloc_zero $(UNIFY_IMPLS)

unification: $(UNIFY_IMPLS)
	ar rcs bin/libunify.a $^ && \
	rm $^

$(UNIFY_IMPLS): dyn_alloc_zero
	mkdir -p bin && \
	$(CXX) $(CXXFLAGS) -Iinclude $@.cpp -c -o $@

dyn_alloc_zero:
	mkdir -p bin && \
	$(CC) $(CFLAGS) $@.c -shared -fPIC $(USE_UNIFICATION_LIB) -o bin/lib$@.so

clean:
	rm -rf bin