#!/bin/sh

ASB_DETECTION_ROOT=../../asb-detection

"$ASB_DETECTION_ROOT/objectize.sh" -O0 src/hatasi.cpp
"$ASB_DETECTION_ROOT/objectize.sh" -O0 src/main.cpp

"$ASB_DETECTION_ROOT/link.sh" clang++ main.o hatasi.o

"$ASB_DETECTION_ROOT/dynalize.sh" ./a.out
