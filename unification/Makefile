UNIFY_IMPLS = unify_allocator_impl unify_64bit_impl unify_page_granular

INTERPOSE_LIBS = dyn_alloc_zero mmap_interpose dyn_alloc_wrappers

SELECTED_INTERPOSE_LIB = dyn_alloc_wrappers

SELECTED_UNIFY_IMPL = unify_64bit_impl

UNIFICATION_DIR = $(ROOT_DIR)/unification

.PHONY: $(INTERPOSE_LIBS) $(UNIFY_IMPLS)

default: $(SELECTED_INTERPOSE_LIB)

#ln -sf $(UNIFICATION_DIR)/bin/lib$(SELECTED_UNIFY_IMPL).a $(UNIFICATION_DIR)/bin/libunify.a

$(INTERPOSE_LIBS): 
	mkdir -p bin && \
	$(CXX) $(CXXFLAGS) $@.cpp $(SELECTED_UNIFY_IMPL).cpp temporary_allocator.cpp -shared -fPIC -Iinclude -o bin/lib$@.so

$(UNIFY_IMPLS):
	mkdir -p bin && \
	$(CXX) $(CXXFLAGS) -Iinclude $@.cpp -fPIC -c -o bin/$@.o && \
	ar rcs bin/lib$@.a bin/$@.o && \
	rm bin/$@.o


clean:
	rm -rf bin