
TARGETS = hatasi-hashing padded-struct print_struct adriaan-hashing uninitialized-value # glib-hashing 

export CFLAGS = -Wall -Wextra

export CC = clang-10 -ftrivial-auto-var-init=zero -enable-trivial-auto-var-init-zero-knowing-it-will-be-removed-from-clang
export CXX = clang++-10 -ftrivial-auto-var-init=zero -enable-trivial-auto-var-init-zero-knowing-it-will-be-removed-from-clang

.PHONY: all $(TARGETS) clean codeql codeql-clean

all: $(TARGETS)

$(TARGETS): 
	make -C $@

clean: 
	for file in $(TARGETS) ; do \
        make -C $$file clean ; \
    done

DB_NAME = asb-db

codeql: codeql-clean clean
	codeql database create --language=cpp --source-root=. --mode=brutal --command="make CFLAGS='-w'" -- $(DB_NAME)

codeql-clean:
	rm -rf $(DB_NAME)


