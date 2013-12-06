/*
 *  util.h
 *  integer-fhe
 *
 *  Created by Henning Perl on 27.11.10.
 *  Copyright 2010 Henning Perl. All rights reserved.
 *
 */

#pragma once
#ifndef UTIL_H
#define UTIL_H

#include <gmp.h>
#include <fmpz_poly.h>
#include <F_mpz_mod_poly.h>

int min(int a, int b);

void fmpz_poly_to_F_mpz_mod_poly(F_mpz_mod_poly_t out, fmpz_poly_t in);

void fmpz_poly_rand_coeff_even(fmpz_poly_t poly, int n, ulong length, gmp_randstate_t* state);

int fmpz_probab_prime_p(fmpz_t n, int reps);

void F_mpz_mod_poly_gcd_euclidean(F_mpz_mod_poly_t res, F_mpz_mod_poly_t poly1, F_mpz_mod_poly_t poly2);

static inline
void _F_mpz_mod_poly_attach(F_mpz_mod_poly_t out, const F_mpz_mod_poly_t in)
{
	out->coeffs = in->coeffs;
	out->length = in->length;
	out->alloc = in->alloc;
	*(out->P) = *(in->P);
}

void F_mpz_mod_poly_make_monic(F_mpz_mod_poly_t output, F_mpz_mod_poly_t pol);

#endif