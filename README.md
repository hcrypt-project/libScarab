Introduction

This is an implementation of a fully homomorphic encryption scheme using large integers. The mathematical foundation is based on work by C. Gentry [1] for the initial concept as well as N. Smart and F. Vercauteren [2] for the integer-based approach used in this implementation.

The library presented here is beta software and should not be used for any mission critical applications. No warranty expressed or implied is given.
Installation and usage
Requirements

The following libraries are required for building hcrypt:

    GMP: GNU Multiple Precision Arithmetic Library — http://gmplib.org/
    FLINT: Fast Library for Number Theory version 1.6 — http://flintlib.org/flint-1.6.tgz
        MPIR: Multiple Precision Integers and Rationals — http://mpir.org/ (required by FLINT)
        MPFR — http://www.mpfr.org/ (required by FLINT)

The library expects the flint headers under in your include directory. Also, copy zn_poly/include/*.h to ./zn_poly/src/ relative to your include path.
Building

The compilation is done using make.

$ make

This builds an integer-fhe executeable in the current directory that runs a test of the library.
Usage

You can use the library as demonstrated in test.c. The following methods are exposed:
function prototype 	meaning

fhe_keygen(fhe_pk_t pk, fhe_sk_t sk); 	Generate a keypair
fhe_encrypt(mpz_t c, fhe_pk_t pk, int m); 	Encrypt a message (0 or 1)
fhe_decrypt(mpz_t c, fhe_sk_t sk); 	Decrypt a cyphertext
fhe_recrypt(mpz_t c, fhe_pk_t pk, fhe_sk_t sk); 	Recrypt a cyphertext (“refreshing” it)
fhe_add(mpz_t res, mpz_t a, mpz_t b, fhe_pk_t pk); 	Add cyphertexts (= XOR)
fhe_mul(mpz_t res, mpz_t a, mpz_t b, fhe_pk_t pk); 	Multiply cyphertexts (= AND)
fhe_fulladd(mpz_t sum, mpz_t c_out, mpz_t a, mpz_t b, mpz_t c_in, fhe_pk_t pk); 	Add with carry in and carry out
fhe_halfadd(mpz_t sum, mpz_t c_out, mpz_t a, mpz_t b, fhe_pk_t pk); 	Add with carry out

References

[1] C. Gentry, A fully homomorphic encryption scheme
[2] N. Smart and F. Vercauteren, Fully Homomorphic Encryption with Relatively Small Key and Ciphertext Sizes
[3] H. Perl, M. Brenner and M. Smith, POSTER: An Implementation of the Fully Homomorphic Smart-Vercauteren Crypto-System

License

The source code is published under a MIT license:

Copyright © 2011

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
