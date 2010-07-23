#!/bin/bash

COMPILER_FLAG="-W -Wall -Wno-unused-parameter -Werror -fPIC"
if uname | grep "MINGW32" ; then
  EXT=pyd
elif uname | grep "CYGWIN" ; then
  export LIBRARY_PATH=/usr/local/lib
  COMPILER_FLAG="-Wall -Wno-unused-parameter -Werror"
  EXT=dll
elif uname | grep "Darwin" ; then
  EXT=dylib
else
  EXT=so
fi

PYTHON_CONFIG=python-config
# apt-get install libxqilla-dev libxerces-c2-dev
g++  $COMPILER_FLAG \
    -c xqilla.cpp -o xqilla.o
gcc -std=c89 $COMPILER_FLAG \
    `$PYTHON_CONFIG --cflags` \
    -c xquery.c -o xquery.o
g++ -shared -W -Wall -Werror \
    xqilla.o xquery.o -lxerces-c -lxqilla \
    `$PYTHON_CONFIG --ldflags` \
    -o simplexquery.$EXT 
rm xqilla.o xquery.o
