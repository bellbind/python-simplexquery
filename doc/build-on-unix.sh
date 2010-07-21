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

PYTHON_CONFIG=python3-config
# apt-get install libxqilla-dev libxerces-c2-dev
g++ -W -Wall -Wno-unused-parameter -Werror -fPIC \
    -c xqilla.cpp -o xqilla.o
gcc -std=c89 -W -Wall -Wno-unused-parameter -Werror -fPIC \
    `$PYTHON_CONFIG --cflags` \
    -c xquery.c -o xquery.o
g++ -shared -W -Wall -Werror `$PYTHON_CONFIG --ldflags` \
    -lxerces-c -lxqilla \
    -o simplexquery.$EXT xqilla.o xquery.o 
rm xqilla.o xquery.o
