# Building module for cygwin's python package



## Required cygwin packages

- cygwin >= 1.7.5
- python >= 2.6.5-2
- binutils
- libicu-devel
- curl-devel
- openssl-devel
- libssh2-devel
- zlib-devel
- libstdc++6-devel
- make
- gcc4
- gcc4-g++
- autoconf
- libtool

Notice: xqilla cannot work well with libxerces-c30 3.0.1 in cygwin packages.

## Build xerces-c-3.1.1

download xerces-c-3.1.1.zip and extract by unzip

    unzip -q xerces-c-3.1.1.zip 
    mv xerces-c-3.1.1 xerces-c-src
    cd xerces-c-src

modify configure.ac as

    #AC_LIBTOOL_WIN32_DLL
    #AC_PROG_LIBTOOL
    LT_INIT([win32-dll])
    LT_OUTPUT

then autoreconf and configure, make, make install to "/usr/local"

    autoreconf
    ./configure LDFLAGS="-no-undefined -Wl,--export-all-symbols"
    make
    make install

## Build XQilla-2.2.4

extract XQilla-2.2.4.zip at the same directory of xerces-c-src

    unzip -q XQilla-2.2.4.zip
    mv XQilla-2.2.4 xqilla
    cd xqilla
    ./configure LDFLAGS="-L/usr/local/lib -L/usr/local/bin" --with-xerces-c=`dirname $(pwd)`/xerces-c-src
    make
    make install

## Build python-simplexquery

set LIBRARY_PATH env with "/usr/local/lib".

    LIBRARY_PATH=/usr/local/lib python setup.py build

## Trouble Shooting for cygwin

- reboot from windows if dll error occurs
- reinstall packages if it cannot fix after reboot
- run rebase.exe/rebaseall.exe from ash if it cannot fix by reinstall and the error says "to same address as parent"
