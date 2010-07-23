# Building deb package on ubuntu lucid

## Setup build environment

    apt-get build-dep xqilla
    apt-get install cdbs python-support libxqilla-dev python2.6-dev libxerces-c2-dev

## Make deb package

    VER=1.0.2 # for example
    git clone git://github.com/bellbind/python-simplexquery.git python-simplexquery-$VER
    rm -rf python-simplexquery-$VER/.git/
    tar zcf python-simplexquery_$VER.orig.tar.gz python-simplexquery-$VER/
    cd python-simplexquery-$VER/
    debuild -uc -us
