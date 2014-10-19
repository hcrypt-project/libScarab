/*
 *  types.h
 *  integer-fhe
 *
 *  Created by Henning Perl on 25.11.10.
 *  Copyright 2010 Henning Perl. All rights reserved.
 *
 */

#pragma once
#ifndef TYPES_H
#define TYPES_H

#include <stdio.h>
#include <gmp.h>
#include "parameters.h"

/** type defs **/
typedef struct {
	mpz_t p, alpha;
	mpz_t c[S1], B[S1];
} _fhe_pk;
typedef _fhe_pk fhe_pk_t[1];

typedef struct {
	mpz_t p, B;
} _fhe_sk;
typedef _fhe_sk fhe_sk_t[1];

/** memory management **/

void fhe_pk_init(fhe_pk_t pk);

void fhe_pk_clear(fhe_pk_t pk);

void fhe_sk_init(fhe_sk_t sk);

void fhe_sk_clear(fhe_sk_t sk);

/** output **/

void fhe_pk_print(fhe_pk_t pk);

void fhe_sk_print(fhe_sk_t sk);

#endif