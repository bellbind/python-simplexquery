# python-xquery

simple native XQuery processing module using xqilla.

## Usage

    import simplexquery as sxq
    print(sxq.execute(u"<body>{string(/user)}</body>", u"<user>Taro</user>"))

## Requires for build

for ubuntu lucid

    apt-get install libxqilla-dev libxerces-c2-dev


## Build

    python setup.py bdist_egg

## Resources

- XQilla: http://xqilla.sourceforge.net/HomePage
