# Building win32 modules on Windows/VC++2008EE

(Tested on Windows7 x64)

## Setup Visual C++ 2008 Express Edition

download and install(, register) "Visual C++ 2008 Express"

- vcsetup.exe from http://www.microsoft.com/express/downloads/

download and install "Windows SDK 7.1"

- winsdk_web.exe from http://www.microsoft.com/downloads/details.aspx?displaylang=en&FamilyID=6b6c21d2-2006-4afa-9702-529fa782d63b

## Build XQilla-2.2.4

(all command lines ran on cygwin)

### Setup sources

download and extract sources in clean directory (e.g. "c:\vc2008"):

    cd /cygdrive/c/vc2008
    wget http://ftp.kddilabs.jp/infosystems/apache/xerces/c/3/sources/xerces-c-3.1.1.zip
    unzip xerces-c-3.1.1.zip
    mv xerces-c-3.1.1 xerces-c-src
    wget http://downloads.sourceforge.net/project/xqilla/xqilla/2.2.4/XQilla-2.2.4.zip
    unzip XQilla-2.2.4.zip
    mv XQilla-2.2.4 xqilla

download and extract xerces-c lib (vc9 version) in the directory:

    wget http://ftp.kddilabs.jp/infosystems/apache/xerces/c/3/binaries/xerces-c-3.1.1-x86-windows-vc-9.0.zip
    unzip xerces-c-3.1.1-x86-windows-vc-9.0.zip

### Build library

start Visual Studio:

- open "C:\vc2008\xqilla\Win32Projects/VC8/XQilla.sln" file by menu "File -> Open -> Project and Solution"
- convert it for Visual C++ 2008 project

modify project environments:

- open dialog by menu "Tools -> Options"
- select "Projects and Solutions -> VC++ Directories" on left list
- select "Include files" on right dropdown box
- add 3 new lines and set directories:

      C:\Program Files\Microsoft SDKs\Windows\v6.0A\Include
      C:\vc2008\xerces-c-3.1.1-x86-windows-vc-9.0\include
      C:\vc2008\xqilla\include

- select "Library files" on right dropdown box
- add a new line and set directory:

      C:\vc2008\xerces-c-3.1.1-x86-windows-vc-9.0\lib

- push "OK"

build on Debug mode:

- build solution by menu "Build -> Build Solution (F7)"

change Release mode:

- open dialog by menu "Project -> Properties"
- open other dialog by push top-right button "Configuration Manager"
- change mode by select "Release" on left dropdown box 
- close by "close" button
- close by "OK" button

build on Release mode:

- build solution by menu "Build -> Build Solution (F7)"

## build python-simplexquery bdist

(change the 4 vars for your environment)

    BASE_DIR='C:\vc2008'
    CYGBASE_DIR=/cygdrive/c/vc2008
    PYTHON_HOME=/cygdrive/c/Python27
    PYTHON_VER=2.7
    
    git clone git://github.com/bellbind/python-simplexquery.git
    cd python-simplexquery
    export INCLUDE_DIRS=$BASE_DIR'\xerces-c-3.1.1-x86-windows-vc-9.0\include,'$BASE_DIR'\xqilla\include'
    export LIBRARY_DIRS=$BASE_DIR'\xerces-c-3.1.1-x86-windows-vc-9.0\lib,'$BASE_DIR'\xqilla\build\windows\VC8\Win32\Release'
    export LIBRARIES="xerces-c_3,xqilla22"
    export EXTRA_COMPILE_ARGS="/EHsc,/Wall,/WX"
    
    $PYTHON_HOME/python.exe setup.py build
    cp $CYGBASE_DIR/xerces-c-3.1.1-x86-windows-vc-9.0/bin/xerces-c_3_1.dll build/lib.win32-$PYTHON_VER/
    cp $CYGBASE_DIR/xerces-c-3.1.1-x86-windows-vc-9.0/LICENSE build/lib.win32-$PYTHON_VER/LICENSE-xerces-c.txt
    cp $CYGBASE_DIR/xerces-c-3.1.1-x86-windows-vc-9.0/NOTICE build/lib.win32-$PYTHON_VER/NOTICE-xerces-c.txt
    cp $CYGBASE_DIR/xqilla/build/windows/VC8/Win32/Release/xqilla22.dll build/lib.win32-$PYTHON_VER/
    cp $CYGBASE_DIR/xqilla/LICENSE build/lib.win32-$PYTHON_VER/LICENSE-xqilla.txt
    PYTHONPATH=build/lib.win32-$PYTHON_VER $PYTHON_HOME/python.exe -B test
    
    $PYTHON_HOME/python.exe setup.py bdist_wininst
    $PYTHON_HOME/python.exe setup.py bdist_msi

