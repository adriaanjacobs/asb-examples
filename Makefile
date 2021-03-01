
TARGETS = glib-hashing hatasi-hashing padded-struct print_struct

.PHONY: all $(TARGETS) clean

all: $(TARGETS)

$(TARGETS): 
	make -C $@

clean: 
	for file in $(TARGETS) ; do \
        make -C $$file clean ; \
    done


	
