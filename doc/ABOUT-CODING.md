# Document about coding

## Coding style

- use no TAB
- each line shoud be less than 80 chars
- .py: standard emacs python-mode with PEP-8
  - see: [PEP-8](http://www.python.org/dev/peps/pep-0008/)
  - see: [Google Python Style Guide](http://google-styleguide.googlecode.com/svn/trunk/pyguide.html)
- .c, .cpp: "python-new" style on cc-mode
  - see: [Python Emacs](http://wiki.python.org/moin/EmacsEditor)

## Design

- Use Python.h with only C code
- C files including Python.h just convert python object to standard C objects.
  Don't write application logic in the files.
- Use python memory allocator only.
  Don't use stdc's malloc/free and default new/delete directly.
- Use anonymous namespace for local C++ classes and functions.


## Tips

- you can add debian/changelog by "dch -v 1.0.1-0mnu1"
- git tag with message by 'git tag v1.0.1 -m "http://pypi.python.org/pypi/python-simplexquery/1.0.1"'
