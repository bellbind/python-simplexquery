# Building deb package on ubuntu lucid

## Setup build environment

    apt-get build-dep xqilla
    apt-get install cdbs python-support libxqilla-dev python2.6-dev libxerces-c2-dev

## Make deb package

    git clone git://github.com/bellbind/python-simplexquery.git python-simplexquery-1.0
    rm -rf python-simplexquery-1.0/.git/
    tar zcf python-simplexquery_1.0.orig.tar.gz python-simplexquery-1.0/
    cd python-simplexquery-1.0/
    debuild -uc -us
