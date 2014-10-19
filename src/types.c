/*
 *  types.c
 *  integer-fhe
 *
 *  Created by Henning Perl on 25.11.10.
 *  Copyright 2010 Henning Perl. All rights reserved.
 *
 */

#include "types.h"


/** memory management **/

void fhe_pk_init(fhe_pk_t pk)
{
	mpz_init(pk->p);
	mpz_init(pk->alpha);
	for (int i = 0; i < S1; i++) {
		mpz_init(pk->B[i]);
		mpz_init(pk->c[i]);
	}
}


void fhe_pk_clear(fhe_pk_t pk)
{
	mpz_clear(pk->p);
	mpz_clear(pk->alpha);
	for (int i = 0; i < S1; i++) {
		mpz_clear(pk->B[i]);
		mpz_clear(pk->c[i]);
	}
}


void fhe_sk_init(fhe_sk_t sk)
{
	mpz_init(sk->p);
	mpz_init(sk->B);
}


void fhe_sk_clear(fhe_sk_t sk)
{
	mpz_clear(sk->p);
	mpz_clear(sk->B);
}

/** output **/

void fhe_pk_print(fhe_pk_t pk)
{
	printf("public key:\n");
	gmp_printf("\tp  =\t%Zd\n", pk->p);
	gmp_printf("\tα  =\t%Zd\n", pk->alpha);
	printf("\tc[i]\tB[i]\n");
	for (int i = 0; i < S1; i++) {
		gmp_printf("\t%Zd\n\t\t%Zd\n", pk->c[i], pk->B[i]);
	}
}

void fhe_pk_print_mathematica(fhe_pk_t pk)
{
	printf("public key:\n");
	gmp_printf("{%Zd, %Zd, %i, %i, {{", pk->p, pk->alpha, S1, S2);
	for (int i = 0; i < S1; i++) {
		gmp_printf("%Zd", pk->c[i]);
		if (i < S1-1) {
			printf(", ");
		}
	}
	printf("}, {");
	for (int i = 0; i < S1; i++) {
		gmp_printf("%Zd", pk->B[i]);
		if (i < S1-1) {
			printf(", ");
		}	}
	printf("}}}\n");
}

void fhe_sk_print(fhe_sk_t sk)
{
	printf("secret key:\n");
	gmp_printf("\tp =\t%Zd\n", sk->p);
	gmp_printf("\tB =\t%Zd\n", sk->B);
}

void fhe_sk_print_mathematica(fhe_sk_t sk)
{
	printf("secret key:\n");
	gmp_printf("{%Zd, %Zd}\n", sk->p, sk->B);
}
