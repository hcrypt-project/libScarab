#!/bin/bash

set -e

#
# Download and build dependencies
#

cd lib

# Uninstall GMP

cd gmp-6.0.0
sudo make uninstall
cd ..

# Uninstall mpfr

cd mpfr-3.1.1
sudo make uninstall
cd ..

# Uninstall mpir

cd mpir-2.6.0
sudo make uninstall
cd ..

# Uninstall flint

cd flint-1.6
sudo rm /usr/local/lib/libflint.so
dir=$(pwd)
cd /usr/local/include
sudo rm d_mat.h flint.h F_mpn_mul-tuning.h fmpz.h F_mpz.h F_mpz_LLL.h F_mpz_mat.h F_mpzmod_mat.h F_mpz_mod_poly.h fmpz_montgomery.h F_mpz_mpoly.h fmpz_poly.h F_mpz_poly.h F_zmod_mat.h long_extras.h longlong.h longlong_wrapper.h memory-manager.h mpfr_mat.h mpn_extras.h mpq_mat.h mpz_extras.h mpz_mat.h mpz_poly.h mpz_poly-tuning.h NTL-interface.h packed_vec.h profiler.h profiler-main.h test-support.h theta.h ZmodF.h ZmodF_mul.h ZmodF_mul-tuning.h ZmodF_poly.h zmod_mat.h zmod_poly.h
cd $dir
sudo rm -rf /usr/local/include/zn_poly/src
cd ..

sudo ldconfig
