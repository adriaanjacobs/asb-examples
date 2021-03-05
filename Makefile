
TARGETS = hatasi-hashing padded-struct print_struct adriaan-hashing # glib-hashing 

CFLAGS = -Wall -Wextra
export CFLAGS

ROOT_DIR = $(shell pwd)
export ROOT_DIR

USE_UNIFICATION_LIB = -I$(ROOT_DIR)/unification/include -L$(ROOT_DIR)/unification/bin -lunify -lstdc++
export USE_UNIFICATION_LIB

.PHONY: all $(TARGETS) clean codeql codeql-clean unification

all: $(TARGETS)

$(TARGETS): unification
	make -C $@

unification:
	make -C $@

clean: 
	for file in $(TARGETS) unification ; do \
        make -C $$file clean ; \
    done

DB_NAME = asb-db

codeql: codeql-clean clean
	codeql database create --language=cpp --source-root=. --mode=brutal --command="make CFLAGS='-w'" -- $(DB_NAME)

codeql-clean:
	rm -rf $(DB_NAME)


