
GLIB_HOME = $(PWD)/glib
GLIB_BUILD = $(GLIB_HOME)/_build

.PHONY: glib-hashing glib

glib-hashing: glib
	$(CC) -I$(GLIB_HOME)/glib -I$(GLIB_HOME) -I$(GLIB_BUILD)/glib glib-hashing.c -L$(GLIB_BUILD) -lglib-2.0 -o bin/$@

glib: $(GLIB_HOME)/.git
	cd $(GLIB_HOME); \
	mkdir -p $(GLIB_BUILD); \
	meson --prefix "$(GLIB_BUILD)" -Dselinux=disabled -Ddefault_library=static _build; \
	ninja -C $(GLIB_BUILD)

$(GLIB_HOME)/.git:
	git submodule init
