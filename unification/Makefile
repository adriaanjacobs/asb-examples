

.PHONY: unification

unification: 
	mkdir -p bin
	$(CXX) $(CFLAGS) -std=c++1z -Iinclude unify.cpp -c -o bin/tmp.o
	ar rcs bin/libunify.a bin/tmp.o
	rm bin/tmp.o

clean:
	rm -rf bin