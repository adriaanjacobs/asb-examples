UNIFY_IMPLS = unify_allocator_impl unify_64bit_impl

SELECTED_UNIFY_IMPL = unify_allocator_impl

UNIFICATION_DIR = $(ROOT_DIR)/unification

.PHONY: dyn_alloc_zero $(UNIFY_IMPLS)

dyn_alloc_zero: $(UNIFY_IMPLS)
	mkdir -p bin && \
	ln -sf $(UNIFICATION_DIR)/bin/lib$(SELECTED_UNIFY_IMPL).a $(UNIFICATION_DIR)/bin/libunify.a && \
	$(CC) $(CFLAGS) $@.c -shared -fPIC $(USE_UNIFICATION_LIB) -o bin/lib$@.so

$(UNIFY_IMPLS):
	mkdir -p bin && \
	$(CXX) $(CXXFLAGS) -Iinclude $@.cpp -fPIC -c -o bin/$@.o && \
	ar rcs bin/lib$@.a bin/$@.o && \
	rm bin/$@.o


clean:
	rm -rf bin