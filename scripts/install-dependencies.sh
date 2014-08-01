#!/bin/bash

#
# Download and build dependencies
#

mkdir -p lib
cd lib

# Download libs

if [ ! -f "gmp-6.0.0a.tar.lz" ]; then
    wget https://gmplib.org/download/gmp/gmp-6.0.0a.tar.lz
fi

if [ ! -f "flint-1.6.tgz" ]; then
    wget http://www.flintlib.org/flint-1.6.tgz
fi

if [ ! -f "mpir-2.6.0.tar.bz2" ]; then
    wget http://www.mpir.org/mpir-2.6.0.tar.bz2
fi

if [ ! -f "mpfr-3.1.1.tar.bz2"]; then
    wget https://ftp.gnu.org/gnu/mpfr/mpfr-3.1.1.tar.bz2
fi

# Install GMP

if [! -d "gmp-6.0.0"]; then
    lzip -d gmp-6.0.0a.tar.lz
    tar xf gmp-6.0.0a.tar
fi

cd gmp-6.0.0
./configure
make
make check
sudo make install
cd ..

# Install mpfr

if [! -d "mpfr-3.1.1"]; then
    tar xjf mpfr-3.1.1.tar.bz2
fi

cd mpfr-3.1.1
make
make check
sudo make install
cd ..

# Install mpir

if [! -d "mpir-2.6.0"]; then
    tar xjf mpir-2.6.0.tar.bz2
fi

cd mpir-2.6.0
./configure
make
make check
sudo make install
cd ..

# Install flint (ok, this is perhaps quite a bit strange)

if [! -d "flint-1.6"]; then
    tar xzf flint-1.6.tgz
fi

cd flint-1.6
. flint_env
make library
sudo cp libflint.so /usr/local/lib
sudo cp *.h /usr/local/include
sudo mkdir -p /usr/local/include/zn_poly/src
sudo cp zn_poly/include/*.h /usr/local/include/zn_poly/src/
cd ..

