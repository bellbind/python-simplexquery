#!/usr/bin/env python
"""A simple native XQuery processing module using xqilla.

Usage:

>>> import simplexquery as sxq
>>> sxq.execute(u"<body>{string(/user)}</body>", u"<user>Taro</user>")
u'<body>Taro</body>'

Build requires: build env for xqilla c++ programs.
- setup for ubuntu lucid:
    apt-get install libxqilla-dev libxerces-c2-dev
"""
doclines = __doc__.split("\n")

classifiers = """\
Development Status :: 1 - Planning
Intended Audience :: Developers
License :: OSI Approved :: Apache Software License
Operating System :: POSIX
Operating System :: Microsoft :: Windows
Operating System :: Unix
Programming Language :: Python
Programming Language :: Python :: 2
Programming Language :: Python :: 3
Topic :: Text Processing :: Markup :: XML
Topic :: Software Development :: Libraries :: Python Modules
"""
try:
    from setuptools import setup, Extension
    pass
except:
    from distutils.core import setup, Extension
    pass

setting = {
    "LIBRARIES": ["xqilla", "xerces-c"],
    "LIBRARY_DIRS": [],
    "INCLUDE_DIRS": [],
    }

try:
    import os
    for key in setting.keys():
        if key in os.environ:
            setting[key] = list(filter(
                None, (s.strip() for s in os.environ[key].split(","))))
            pass
        pass
    pass
except:
    pass

setup(
    name="python-simplexquery",
    version="0.6",
    
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
            libraries=setting["LIBRARIES"],
            library_dirs=setting["LIBRARY_DIRS"],
            include_dirs=setting["INCLUDE_DIRS"],
            )
        ])
