
TARGETS = glib-hashing hatasi-hashing padded-struct print_struct

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
	codeql database create --language=cpp --source-root=. --threads=$(shell nproc) --mode=brutal --command="make" -- $(DB_NAME)

codeql-clean:
	rm -rf $(DB_NAME)




	
