#!/bin/bash

if uname | grep "MINGW32" ; then
  EXT=pyd
elif uname | grep "CYGWIN" ; then
  EXT=dll
elif uname | grep "Darwin" ; then
  EXT=dylib
else
  EXT=so
fi

PYTHON_CONFIG=python-config
# apt-get install libxqilla-dev libxerces-c2-dev
g++ -W -Wall -Wno-unused-parameter -fPIC \
    -c xqilla.cpp -o xqilla.o
gcc -std=c89 -W -Wall -Wno-unused-parameter -fPIC `$PYTHON_CONFIG --cflags` \
    -c xquery.c -o xquery.o
g++ -W -Wall `$PYTHON_CONFIG --ldflags` -lxerces-c -lxqilla -shared \
    -o simplexquery.$EXT xqilla.o xquery.o 
rm xqilla.o xquery.o
