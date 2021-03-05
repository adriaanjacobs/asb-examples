
TARGETS = hatasi-hashing padded-struct print_struct adriaan-hashing uninitialized-value # glib-hashing 

export CFLAGS = -Wall -Wextra

export CC = clang-10 -ftrivial-auto-var-init=zero -enable-trivial-auto-var-init-zero-knowing-it-will-be-removed-from-clang
export CXX = clang++-10 -ftrivial-auto-var-init=zero -enable-trivial-auto-var-init-zero-knowing-it-will-be-removed-from-clang

export ROOT_DIR = $(shell pwd)

export USE_UNIFICATION_LIB = -I$(ROOT_DIR)/unification/include -L$(ROOT_DIR)/unification/bin -lunify -lstdc++

.PHONY: all $(TARGETS) clean codeql codeql-clean unification

all: $(TARGETS)

$(TARGETS): unification
	$(MAKE) -C $@

unification:
	$(MAKE) -C $@

clean: 
	for file in $(TARGETS) unification ; do \
        $(MAKE) -C $$file clean ; \
    done

DB_NAME = asb-db

codeql: codeql-clean clean
	codeql database create --language=cpp --source-root=. --mode=brutal --command="$(MAKE) CFLAGS='-w'" -- $(DB_NAME)

codeql-clean:
	rm -rf $(DB_NAME)


