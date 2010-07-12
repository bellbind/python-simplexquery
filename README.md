# python-simplexquery

simple native XQuery processing module using xqilla.

## Usage

    import simplexquery as sxq
    print(sxq.execute(u"<body>{string(/user)}</body>", u"<user>Taro</user>"))

## Requires for build

- xqilla
- xerces-c

for ubuntu lucid

    apt-get install libxqilla-dev libxerces-c2-dev

## Install by easy_install

   easy_install python-simplexquery

## Build from source

    python setup.py bdist_egg

## Resources

- [PyPI page](http://pypi.python.org/pypi/python-simplexquery)
- [XQilla Home](http://xqilla.sourceforge.net/HomePage)
