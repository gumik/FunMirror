#!/usr/bin/env bash

g++ $(pkg-config --libs --cflags opencv) *.cpp -o FunMirror
