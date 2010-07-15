# python-simplexquery

simple native XQuery processing module using xqilla.

## Usage

    import simplexquery as sxq
    print(sxq.execute("<body>{string(/user)}</body>", "<user>Taro</user>"))

see example.py

## Install by easy_install

   easy_install python-simplexquery

### Requires for build

- xqilla
- xerces-c

for ubuntu lucid

    apt-get build-dep xqilla
    apt-get install libxqilla-dev

## Build from source

    python setup.py bdist_egg

## for Windows

download from [github download](http://github.com/bellbind/python-simplexquery/downloads)

## Resources

- [PyPI page](http://pypi.python.org/pypi/python-simplexquery)
- [XQilla Home](http://xqilla.sourceforge.net/HomePage)
- [W3C XML XQuery](http://www.w3.org/XML/Query/)
