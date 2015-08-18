#/bin/sh

GLIB_HOME=$HOME/src/glib2.0-2.40.2/install

gcc glib-hashing.c -I$GLIB_HOME/include/glib-2.0 -I$GLIB_HOME/lib/glib-2.0/include/ -lglib-2.0
