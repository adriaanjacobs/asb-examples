UNIFY_IMPLS = unify_allocator_impl unify_64bit_impl

SELECTED_UNIFY_IMPL = unify_allocator_impl

UNIFICATION_DIR = $(ROOT_DIR)/unification

.PHONY: dyn_alloc_zero $(UNIFY_IMPLS)

#ln -sf $(UNIFICATION_DIR)/bin/lib$(SELECTED_UNIFY_IMPL).a $(UNIFICATION_DIR)/bin/libunify.a

dyn_alloc_zero: 
	mkdir -p bin && \
	$(CXX) $(CXXFLAGS) $@.c $(SELECTED_UNIFY_IMPL).cpp -shared -fPIC -Iinclude -o bin/lib$@.so

$(UNIFY_IMPLS):
	mkdir -p bin && \
	$(CXX) $(CXXFLAGS) -Iinclude $@.cpp -fPIC -c -o bin/$@.o && \
	ar rcs bin/lib$@.a bin/$@.o && \
	rm bin/$@.o


clean:
	rm -rf bin