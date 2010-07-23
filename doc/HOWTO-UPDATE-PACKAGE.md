# Howto check, version up, and release when the source updated 


## Check list when to packaging

### Build checkx

ok when no warnings in compiler output message

- gcc compile on ubuntu
- cl compile on windows

### Test check

ok when python test pass allx

- build with python2 
- build with python3 

### Packaging check

check contents in package

- setup.py sdist
- setup.py bdist_egg on ubuntu
- setup.py bdist_wininst on windows
- deb on ubuntu: with no warning of lintian message 

Format of wininst exe is self extract zip.
So you can list by "unzip -t python-simplexquery-*-win32-py*.exe"

You can list contents in deb file by  "dpkg-deb -c python-simplexquery*.deb"

## When to version up

A version string consists release.major.minor 

- incr release: when update with backward incompatibity
- incr major: when update with append functions
- incr minor: when update require new packaging

## Check list when version up

- xquery.c
- setup.py
- debian/changelog

These files contain version string

## When to release

minor update 

- sdist upload
- bugfix bdist_wininst upload to github

major update

- sdist upload
- bdist_wininst upload

## Resources

- [Dive Into Python3: Packaging Python Libraries](http://diveintopython3.org/packaging.html)
- [Ubuntu: PackagingGuide/Complete](https://wiki.ubuntu.com/PackagingGuide/Complete)
