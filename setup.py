#!/usr/bin/env python
u"""simple native XQuery processing module using xqilla.

usage:
>>> import simplexquery as sxq
>>> sxq.execute(u"<body>{string(/user)}</body>", u"<user>Taro</user>")
u'<body>Taro</body>'

build requires: build env for xqilla c++ programs.
- setup for ubuntu lucid:
    apt-get install libxqilla-dev libxerces-c2-dev
"""
doclines = __doc__.split("\n")

classifiers = """\
Development Status :: 1 - Planning
Intended Audience :: Developers
License :: OSI Approved :: Apache Software License
Programming Language :: Python
Topic :: Text Processing :: Markup :: XML
Topic :: Software Development :: Libraries :: Python Modules
Operating System :: Unix
"""

from setuptools import setup, Extension

setup(
    name="python-simplexquery",
    version="0.2",
    author="bellbind",
    author_email="bellbind@gmail.com",
    url="http://github.com/bellbind/python-simplexquery",
    packages=[],
    keywords=["xml", "xquery", "xqilla"],
    license="http://www.apache.org/licenses/LICENSE-2.0",
    description=doclines[0],
    long_description="\n".join(doclines[2:]),
    classifiers=filter(None, classifiers.split("\n")),
    
    ext_modules=[
        Extension(
            "simplexquery",
            ["xqilla.cpp", "xquery.c"],
            libraries=["xqilla", "xerces-c"]
            )
        ])
