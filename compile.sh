#!/usr/bin/env bash

g++ -O2 -march=native $(pkg-config --libs --cflags opencv) *.cpp -o FunMirror
