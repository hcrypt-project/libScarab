/*
 *  util.c
 *  integer-fhe
 *
 *  Created by Henning Perl on 27.11.10.
 *  Copyright 2010 Henning Perl. All rights reserved.
 *
 */

#include "util.h"

int
min(int a, int b)
{
	return (a<b) ? a : b;
}


void
fmpz_poly_to_F_mpz_mod_poly(F_mpz_mod_poly_t out, fmpz_poly_t in)
{
	mpz_poly_t mpz_poly;
	mpz_poly_init(mpz_poly);
	
	fmpz_poly_to_mpz_poly(mpz_poly, in);
	mpz_poly_to_F_mpz_mod_poly(out, mpz_poly);
	
	mpz_poly_clear(mpz_poly);
}


void
fmpz_poly_rand_coeff_even(fmpz_poly_t poly, int n, ulong length, gmp_randstate_t* state)
{
	mpz_t c, exp_length_half;
	mpz_init(c);
	mpz_init(exp_length_half);
	
	mpz_ui_pow_ui(exp_length_half, 2, length-1);
	
	for (int i = 1; i<n; i++) {
		mpz_urandomb(c, *state, length);
		mpz_sub(c, c, exp_length_half);
		mpz_mul_ui(c, c, 2);
		fmpz_poly_set_coeff_mpz(poly, i, c);
	}
	
	mpz_clear(c);
	mpz_clear(exp_length_half);
}


int
fmpz_probab_prime_p(fmpz_t n, int reps)
{
	int result;
	mpz_t mpz;
	
	mpz_init(mpz);
	
	fmpz_to_mpz(mpz, n);
	result = mpz_probab_prime_p(mpz, reps);
	
	mpz_clear(mpz);
	
	return result;
}


void F_mpz_mod_poly_gcd_euclidean(F_mpz_mod_poly_t res, F_mpz_mod_poly_t poly1, F_mpz_mod_poly_t poly2)
{
	F_mpz_mod_poly_t R, A, B;
	F_mpz_poly_t r;
	int steps = 0;
	
	if (poly1->length == 0) 
	{
		if (poly2->length == 0)
			F_mpz_mod_poly_zero(res);
		else F_mpz_mod_poly_make_monic(res, poly2);
		return;
	}
	
	if (poly2->length == 0) 
	{
		F_mpz_mod_poly_make_monic(res, poly1);
		return;
	}
	
	if ((poly1->length == 1) || (poly2->length == 1))
	{
		_F_mpz_poly_attach_F_mpz_mod_poly(r, res);
		F_mpz_poly_set_coeff_ui(r, 0, 1L);
		_F_mpz_mod_poly_attach_F_mpz_poly(res, r);
		_F_mpz_mod_poly_normalise(res);
		return;
	}
	
	F_mpz_t P;
	F_mpz_init(P);
	F_mpz_set(P, poly1->P);
	
	F_mpz_mod_poly_init(R, P);
	
	if (poly1->length > poly2->length)
	{
		_F_mpz_mod_poly_attach(A, poly1);
		_F_mpz_mod_poly_attach(B, poly2);
	} else
	{
		_F_mpz_mod_poly_attach(A, poly2);
		_F_mpz_mod_poly_attach(B, poly1);
	}
	
	F_mpz_mod_poly_rem(R, A, B);
	
	F_mpz_mod_poly_swap(A, B);
	F_mpz_mod_poly_swap(B, R);
	F_mpz_mod_poly_init(R, P); 
	
	if (B->length > 1)
	{
		F_mpz_mod_poly_rem(R, A, B);
		F_mpz_mod_poly_swap(A, B);
		F_mpz_mod_poly_swap(B, R);
		F_mpz_mod_poly_init(R, P);
		steps = 1;
	}
	
	while (B->length > 1)
	{
		F_mpz_mod_poly_rem(A, A, B);
		F_mpz_mod_poly_swap(A, B); 
	}
	
	if  (B->length == 1) 
	{
		_F_mpz_poly_attach_F_mpz_mod_poly(r, res);
		F_mpz_poly_set_coeff_ui(r, 0, 1L);
		_F_mpz_mod_poly_attach_F_mpz_poly(res, r);
		res->length = 1;
	}
	else F_mpz_mod_poly_make_monic(res, A);
	
	if (steps) 
	{
		F_mpz_mod_poly_clear(A);
	} 
	
	F_mpz_mod_poly_clear(B);
	F_mpz_mod_poly_clear(R);
}


void F_mpz_mod_poly_make_monic(F_mpz_mod_poly_t output, F_mpz_mod_poly_t pol)
{
	if (!pol->length) 
	{
		output->length = 0;
		return;
	}
	
	F_mpz_t *lead_coeff = (F_mpz_t *)&pol->coeffs[pol->length-1];
	
	if (F_mpz_is_one(*lead_coeff))
	{
		F_mpz_mod_poly_set(output, pol);
		return;
	}
	F_mpz_t lead_inv;
	F_mpz_init(lead_inv);
	
	F_mpz_invert(lead_inv, *lead_coeff, pol->P);
	F_mpz_mod_poly_scalar_mul(output, pol, lead_inv);
}