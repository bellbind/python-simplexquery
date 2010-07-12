#!/bin/bash

# apt-get install libxqilla-dev libxerces-c2-dev
g++ -Wall -fPIC -c xqilla.cpp -o xqilla.o
gcc -Wall -fPIC `python-config --cflags` -c xquery.c -o xquery.o
g++ -Wall `python-config --ldflags` -lxerces-c -lxqilla -shared \
    -o simplexquery.so xqilla.o xquery.o 
rm xqilla.o xquery.o
