# Introduction

This is an implementation of a fully homomorphic encryption scheme using large integers. The mathematical foundation is based on work by C. Gentry [1] for the initial concept as well as N. Smart and F. Vercauteren [2] for the integer-based approach used in this implementation.

The library presented here is beta software and should not be used for any mission critical applications. No warranty expressed or implied is given.


# Installation and usage

First, clone this repository on your machine: <code>git clone https://github.com/blindstore/libScarab.git</code>.

## Prerequisites

The library depends on the following libraries:

* GMP https://gmplib.org Version 6.0.0 https://gmplib.org/download/gmp/gmp-6.0.0a.tar.lz
* MPFR http://www.mpfr.org Version 3.1.1 https://ftp.gnu.org/gnu/mpfr/mpfr-3.1.1.tar.bz2
* MPIR http://www.mpir.org Version 2.6.0 http://www.mpir.org/mpir-2.6.0.tar.bz2
* Flint http://www.flintlib.org Version 1.6 http://www.flintlib.org/flint-1.6.tgz

### For Debian-based systems

* <code>sudo apt-get install build-essential m4 lzip unzip</code>
* Use <code>./scripts/install-dependencies.sh</code> from this repository to install the dependencies.

### For Arch Linux

* Install <code>base-devel m4 unzip gmp mpfr</code> from official repositories.
* Install <code>mpir</code> from AUR. Install version 2.6.0 and pay attention to the comment made by Fincer on 2014-06-20 18:32.
* Install Flint manually: Execute the according part at the bottom of <code>./scripts/install-dependencies.sh</code>, but before create <code>/etc/ld.so.conf.d/locallib.conf</code> with just one line of content: <code>/usr/local/lib</code>.

### For Mac OS

* You can install <code>lzip gmp</code> with <code>brew</code>
* Install the other dependencies using <code>./scripts/install-dependencies.sh</code> 

## Build

The compilation is done using make.

    $ make

This builds an executable <code>bin/integer-fhe</code> that runs a test of the library and a shared library file in <code>build/libscarab.so</code>.


## Install

    $ sudo make install

will place <code>./build/libscarab.so</code> in <code>/usr/local/lib</code> and run <code>ldconfig</code>.


## Usage

You can use the library as demonstrated in <code>test.c</code>. The following methods are exposed:

```
fhe_keygen(fhe_pk_t pk, fhe_sk_t sk);                    // Generate a keypair
fhe_encrypt(mpz_t c, fhe_pk_t pk, int m);                // Encrypt a message (0 or 1)
fhe_decrypt(mpz_t c, fhe_sk_t sk);                       // Decrypt a cyphertext
fhe_recrypt(mpz_t c, fhe_pk_t pk, fhe_sk_t sk); 	 // Recrypt a cyphertext (“refreshing” it)
fhe_add(mpz_t res, mpz_t a, mpz_t b, fhe_pk_t pk);       // Add cyphertexts (= XOR)
fhe_mul(mpz_t res, mpz_t a, mpz_t b, fhe_pk_t pk);       // Multiply cyphertexts (= AND)
fhe_fulladd(mpz_t sum, mpz_t c_out, mpz_t a, mpz_t b, mpz_t c_in, fhe_pk_t pk);  // Add with carry in and carry out
fhe_halfadd(mpz_t sum, mpz_t c_out, mpz_t a, mpz_t b, fhe_pk_t pk);  // Add with carry out
```

## Uninstall

    $ sudo make uninstall

will remove the library from <code>/usr/local/lib</code>.

### Uninstall dependencies

If you used <code>./scripts/install-dependencies</code>, you can uninstall them using

    $ ./scripts/uninstall-dependencies

This will basically call <code>sudo make uninstall</code> for each of GMP, MPFR and MPIR and undo the commands executed for the installation of Flint.


# References

[1] C. Gentry, A fully homomorphic encryption scheme

[2] N. Smart and F. Vercauteren, Fully Homomorphic Encryption with Relatively Small Key and Ciphertext Sizes

[3] H. Perl, M. Brenner and M. Smith, POSTER: An Implementation of the Fully Homomorphic Smart-Vercauteren Crypto-System


# License

The source code is published under a MIT license:

Copyright © 2011, 2014

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
