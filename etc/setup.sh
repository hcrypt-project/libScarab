#!/bin/bash

#
# Download and build dependencies
#

mkdir -p lib
cd lib

# Download libs

wget https://gmplib.org/download/gmp/gmp-6.0.0a.tar.lz
wget http://www.flintlib.org/flint-1.6.tgz
wget http://www.mpir.org/mpir-2.6.0.tar.bz2
wget https://ftp.gnu.org/gnu/mpfr/mpfr-3.1.1.tar.bz2

# Install GMP

lzip -d gmp-6.0.0a.tar.lz
tar xf gmp-6.0.0a.tar

cd gmp-6.0.0
./configure
make
make check
sudo make install
cd ..

# Install mpfr

tar xjf mpfr-3.1.1.tar.bz2
cd mpfr-3.1.1
make
make check
sudo make install
cd ..

# Install mpir

tar xjf mpir-2.6.0.tar.bz2
cd mpir-2.6.0
./configure
make
make check
sudo make install
cd ..

# Install flint (ok, this is perhaps quite a bit strange)

tar xzf flint-1.6.tgz
cd flint-1.6
. flint_env
make library
sudo cp libflint.so /usr/local/lib
sudo cp *.h /usr/local/include
sudo mkdir -p /usr/local/include/zn_poly/src
sudo cp zn_poly/include/*.h /usr/local/include/zn_poly/src/
cd ..

