#/bin/sh

exec ~/valgrind-3.10.1/inst/bin/valgrind --tool=taintgrind --critical-ins-only=no ./a.out 2>&1 | ~/asb-llvm-plugins/process-taintgrind-output.rb
