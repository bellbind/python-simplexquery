# python-simplexquery

A simple native XQuery processing module using xqilla.

## Usage

    import simplexquery as sxq
    print(sxq.execute("<body>{string(/user)}</body>", "<user>Taro</user>"))

For more details, see 
[examples/example.py](http://github.com/bellbind/python-simplexquery/blob/master/examples/example.py)

## License

[Apache License Version 2.0](http://www.apache.org/licenses/LICENSE-2.0)

(Because all of dependencies use the license)

## Install by easy_install

   easy_install python-simplexquery

You can install on both Python3.x and Python2.x

### Requires for build

- python (2.x, 3.x)
- xqilla (2.0.0 - 2.2.4)
- xerces-c (2.8.x, 3.1.x)


## Build from source

    python setup.py bdist_egg

## For Ubuntu

ppa is now available for ubuntu 10.04 lucid

    sudo apt-add-repository ppa:bellbind/python-simplexquery
    apt-get update
    apt-get install python-simplexquery

## For Windows

You can download stable installer exes from 
[PyPI python-simplexquery](http://pypi.python.org/pypi/python-simplexquery).

More new versions may be avaiable from 
[github download](http://github.com/bellbind/python-simplexquery/downloads)

For building modules yourself, see 
[doc/HOWTO-BUILD-ON-WINDOWS.md](http://github.com/bellbind/python-simplexquery/blob/master/doc/HOWTO-BUILD-ON-WINDOWS.md)

## Concept of the module

This module is designed for using XQuery on python easily, so the module:

- only provides stateless functions which only use python builtins objects
- will not support external functions by python callable 
  because the values handled by XQuery function are based on DOM
- is aimed for a comprehensible example for native modules with: 
  multi python version, 
  closs platform with warningless code, 
  module with external c++ libraries, 
  well managed resources (memory alloc, reference count, exceptions),
  packaging strategy for every platform,
  distribution for source and package

## Resources

- [PyPI python-simplexquery](http://pypi.python.org/pypi/python-simplexquery)
- [XQilla Home](http://xqilla.sourceforge.net/HomePage)
- [W3C XML XQuery](http://www.w3.org/XML/Query/)
