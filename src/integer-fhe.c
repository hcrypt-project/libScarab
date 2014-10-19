/*
 *  keygen.c
 *  integer-fhe
 *
 *  Created by Henning Perl on 01.03.10.
 *
 */

#include "integer-fhe.h"
#undef DEBUG

#ifdef DETERMINISTIC
#define SEED 0
#else
#define SEED time(NULL)
#endif

void
fhe_keygen(fhe_pk_t pk, fhe_sk_t sk)
{
	mpz_t temp;
	mpz_init(temp);
	fmpz_t fmpz_p;
	
	fmpz_poly_t F;
	fmpz_poly_init(F);
	// set f = x^n + 1
	fmpz_poly_set_coeff_ui(F, N, 1);
	fmpz_poly_set_coeff_ui(F, 0, 1);
	
	fmpz_poly_t G;
	fmpz_poly_init(G);
	
	gmp_randstate_t randstate;
	gmp_randinit_default(randstate);
	gmp_randseed_ui(randstate, SEED); // make it deterministic for now
	do {
		fmpz_poly_rand_coeff_even(G, N, LOG_NU, &randstate);
		fmpz_t G_0 = fmpz_poly_get_coeff_ptr(G, 0);
		fmpz_add_ui(G_0, G_0, 1);
		// set p = resultant(G(x), F(x))
		fmpz_p = fmpz_init(fmpz_poly_resultant_bound(G, F)/FLINT_BITS + 2);
		fmpz_poly_resultant(fmpz_p, G, F);
		fmpz_to_mpz(temp, fmpz_p);
		fmpz_clear(fmpz_p);
	} while (!mpz_probab_prime_p(temp, 10));

#ifdef DEBUG
	gmp_printf("Choosing Prime p = %Zd\n", temp);
	printf("Choosing F(x) = ");
	fmpz_poly_print_pretty(F, "x");
	printf("\n");
	printf("Choosing G(x) = ");
	fmpz_poly_print_pretty(G, "x");
	printf("\n");
#endif
	
	F_mpz_t P;
	F_mpz_init(P);
	F_mpz_set_mpz(P, temp);
	
	F_mpz_mod_poly_t F_mod_p, G_mod_p, D_mod_p;
	F_mpz_mod_poly_init(F_mod_p, P);
	F_mpz_mod_poly_init(G_mod_p, P);
	F_mpz_mod_poly_init(D_mod_p, P);
	fmpz_poly_to_F_mpz_mod_poly(F_mod_p, F);
	fmpz_poly_to_F_mpz_mod_poly(G_mod_p, G);
	
	F_mpz_mod_poly_gcd_euclidean(D_mod_p, F_mod_p, G_mod_p);
	
#ifdef DEBUG
	printf("gcd(G(x), F(x) = ");
	F_mpz_mod_poly_print_pretty(D_mod_p, "x");
	printf("\n");
#endif
	
	F_mpz_t *root = (F_mpz_t *)D_mod_p->coeffs;
	F_mpz_neg(*root, *root);	// root of D = -coeff[0]
	F_mpz_mod(*root, *root, P);
	
	fmpz_t r = fmpz_init(fmpz_poly_resultant_bound(G, F)/FLINT_BITS + 2);

	fmpz_poly_t Z, t;
	fmpz_poly_init(Z);
	fmpz_poly_init(t);
	
#ifdef DEBUG
	printf("Executing XGCD with Polynomials G = ");
	fmpz_poly_print_pretty(G, "x");
	printf("\n and F = ");
	fmpz_poly_print_pretty(F, "x");
	printf("\n");
#endif
	fmpz_poly_xgcd(r, Z, t, G, F);
	
#ifdef DEBUG
	printf("r    = ");
	fmpz_print(r);
	printf("\n");
	printf("Z(x) = ");
	fmpz_poly_print_pretty(Z, "x");
	printf("\n");
#endif
	fmpz_p = fmpz_init(mpz_size(temp));
	mpz_to_fmpz(fmpz_p, temp);
	assert(fmpz_equal(fmpz_p, r));
	
	// build pk
	mpz_set(pk->p, temp);
	F_mpz_get_mpz(pk->alpha, *root);
	// build sk
	mpz_set(sk->p, temp);
	fmpz_poly_get_coeff_mpz(sk->B, Z, 0);
	mpz_mul_ui(temp, temp, 2);
	mpz_mod(sk->B, sk->B, temp);
	
	// Build hint
	mpz_t B_i, r_plus, r_minus;
	mpz_init(B_i);
	mpz_init(r_plus);
	mpz_init(r_minus);
	
	mpz_fdiv_q_ui(B_i, sk->B, S2);
	for (int i = 0; i < S2; i++) {
		mpz_set(pk->B[i], B_i);
		fhe_encrypt(pk->c[i], pk, 1);
	}
	mpz_add(pk->B[0], pk->B[0], sk->B);
	mpz_submul_ui(pk->B[0], B_i, S2);
	for (int i = S2; i < S1; i++) {
		mpz_urandomm(B_i, randstate, temp); // p is already 2*pk->p !
		mpz_sub(B_i, B_i, pk->p);
		mpz_set(pk->B[i], B_i);
		fhe_encrypt(pk->c[i], pk, 0);
	}
	
	// add/sub values randomly
	for (int i = 0; i < S2; i++) {
		mpz_urandomm(r_plus, randstate, pk->p);
		mpz_neg(r_minus, r_plus);
		while (1) {
			int j = rand() % S2;
			mpz_add(pk->B[j], pk->B[j], r_plus);
			if (mpz_cmp(pk->B[j], pk->p) > 0) {
				mpz_sub(r_plus, pk->B[j], pk->p);
				mpz_set(pk->B[j], pk->p);
			} else {
				break;
			}
		}
		while (1) {
			int j = rand() % S2;
			mpz_add(pk->B[j], pk->B[j], r_minus);
			mpz_neg(temp, pk->p);
			if (mpz_cmp(pk->B[j], temp) < 0) {
				mpz_add(r_minus, pk->B[j], pk->p);
				mpz_neg(pk->B[j], pk->p);
			} else {
				break;
			}
		}
	}
	// shuffle
	for (int i = 0; i < S1; i++) {
		int j = rand() % S1;
		mpz_swap(pk->B[i], pk->B[j]);
		mpz_swap(pk->c[i], pk->c[j]);
	}
	
#ifndef NDEBUG
	// assert that it really sums up to B
	mpz_set_ui(temp, 0L);
	for (int i = 0; i < S1; i++) {
		if (fhe_decrypt(pk->c[i], sk) == 1) {
			mpz_add(temp, temp, pk->B[i]);
		}
	}
	assert(mpz_cmp(temp, sk->B) == 0);
#endif

	// cleanup
	mpz_clear(B_i);
	mpz_clear(r_plus);
	mpz_clear(r_minus);
	mpz_clear(temp);
	fmpz_clear(fmpz_p);
	fmpz_clear(r);
	fmpz_poly_clear(Z);
	fmpz_poly_clear(t);
	fmpz_poly_clear(F);
	fmpz_poly_clear(G);
	F_mpz_mod_poly_clear(F_mod_p);
	F_mpz_mod_poly_clear(G_mod_p);
	F_mpz_mod_poly_clear(D_mod_p);
	F_mpz_clear(P);
	gmp_randclear(randstate);
}

void
fhe_encrypt(mpz_t c, fhe_pk_t pk, int m)
{
#ifdef DEBUG
	printf("ENCRYPT %i\n", m);
#endif
	if ((m != 0) && (m != 1)) {
		fprintf(stderr, "\nm must be either 0 or 1\n");
		abort();
	}
	
	fmpz_poly_t C;
	fmpz_t C_0, fmpz_c, alpha;
	
	fmpz_poly_init(C);
	fmpz_c = fmpz_init(1000000);
		
	alpha = fmpz_init(mpz_size(pk->alpha));
	mpz_to_fmpz(alpha, pk->alpha);
		
	gmp_randstate_t randstate;
	gmp_randinit_default(randstate);
	gmp_randseed_ui(randstate, SEED);
	
	fmpz_poly_rand_coeff_even(C, N, 2, &randstate);
	
	C_0 = fmpz_poly_get_coeff_ptr(C, 0);
	fmpz_add_ui(C_0, C_0, m);
	
	
#ifdef DEBUG
	printf("C(x) = ");
	fmpz_poly_print_pretty(C, "x");
	printf("\n");
	printf("alpha = ");
	fmpz_print(alpha);
	printf("\n");
	printf("C(alpha) = ");
	fmpz_print(fmpz_c);
	printf("\n");
#endif
	
	fmpz_poly_evaluate(fmpz_c, C, alpha);
	
#ifdef DEBUG
	printf("C(x) = ");
	fmpz_poly_print_pretty(C, "x");
	printf("\n");
	printf("C(alpha) = ");
	fmpz_print(fmpz_c);
	printf("\n");
#endif
	
	fmpz_to_mpz(c, fmpz_c);
	fmpz_clear(fmpz_c);

	mpz_mod(c, c, pk->p);
	
	
#ifdef DEBUG
	gmp_printf("c = %Zd\n", c);
#endif
	
	// cleanup
	fmpz_poly_clear(C);
	fmpz_clear(alpha);
	gmp_randclear(randstate);
}


int
fhe_decrypt(mpz_t c, fhe_sk_t sk)
{
#ifdef DEBUG
	gmp_printf("DECRYPT %Zd\n", c);
#endif
	int m;
	
	mpz_t temp, q, r;
	mpz_init(temp);
	mpz_init(q);
	mpz_init(r);
	
	mpq_t n, d, one_half;
	mpq_init(n);
	mpq_init(d);
	mpq_init(one_half);
	
	mpz_mul(temp, c, sk->B);
	mpz_fdiv_qr(q, r, temp, sk->p);
	
	// round: add 1 to q if r/p > 1/2
	mpq_set_ui(one_half, 1, 2);
	mpq_set_z(n, r);
	mpq_set_z(d, sk->p);
	mpq_div(n, n, d);
	
	if (mpq_cmp(n, one_half) > 0) { // n > 1/2
		mpz_add_ui(q, q, 1);
	}
	
#ifdef DEBUG
	gmp_printf("q: %Zd\n", q);
#endif
	
	mpz_add(temp, c, q);	
	mpz_mod_ui(temp, temp, 2);
	
	m = mpz_get_si(temp);
		
	// cleanup
	mpz_clear(temp);
	mpz_clear(q);
	mpz_clear(r);
	mpq_clear(n);
	mpq_clear(d);
	mpq_clear(one_half);
	
	return m;
}


void
fhe_add(mpz_t res, mpz_t a, mpz_t b, fhe_pk_t pk)
{
	mpz_add(res, a, b);
	mpz_mod(res, res, pk->p);
	fhe_recrypt(res, pk);
}


void
fhe_mul(mpz_t res, mpz_t a, mpz_t b, fhe_pk_t pk)
{
	mpz_mul(res, a, b);
	mpz_mod(res, res, pk->p);
	fhe_recrypt(res, pk);
}


void
fhe_fulladd(mpz_t sum, mpz_t c_out, mpz_t a, mpz_t b, mpz_t c_in, fhe_pk_t pk)
{
	mpz_t temp;
	mpz_init(temp);
	
	mpz_add(temp, a, b);
	mpz_add(temp, temp, c_in);
	mpz_mod(sum, temp, pk->p);
	
	mpz_mul(temp, a, b);
	mpz_addmul(temp, c_in, a);
	mpz_addmul(temp, c_in, b);
	mpz_mod(c_out, temp, pk->p);
	
	mpz_clear(temp);
}


void
fhe_halfadd(mpz_t sum, mpz_t c_out, mpz_t a, mpz_t b, fhe_pk_t pk)
{
	mpz_add(sum, a, b);
	mpz_mod(sum, sum, pk->p);
	
	mpz_mul(c_out, a, b);
	mpz_mod(c_out, c_out, pk->p);
}


#define PRINT_C												\
	printf("(Zeile %i): C-Matrix\n\t", __LINE__);			\
	for (int __i = 0; __i < S1; __i++) {					\
		for (int __j = 0; __j < T; __j++) {					\
			printf("%i ", fhe_decrypt(C[__i][__j], sk));	\
		}													\
		printf("\n\t");										\
	};  printf("\n")

#define PRINT_H												\
	printf("(Zeile %i): H-Matrix\n\t", __LINE__);			\
	for (int __i = 0; __i < T; __i++) {						\
		for (int __j = 0; __j < T; __j++) {					\
			printf("%i ", fhe_decrypt(H[__i][__j], sk));	\
		}													\
		printf("\n\t");										\
	};  printf("\n")

#define PRINT_ham											\
	printf("(Zeile %i): ham-Matrix\n\t", __LINE__);			\
	for (int __i = 0; __i < T; __i++) {						\
		for (int __j = 0; __j < T; __j++) {					\
			printf("%i ", fhe_decrypt(ham(__i,__j), sk));	\
		}													\
		printf("\n\t");										\
	};  printf("\n")

void
fhe_recrypt(mpz_t c, fhe_pk_t pk)
{
#undef DEBUG
	assert(S <= T);
	
	mpz_t C[S1][T], H[T][T], temp, p;
	mpq_t q;
	
	mpz_init(temp);
	mpz_init(p);
	mpq_init(q);
	for (int i = 0; i < S1; i++) {
		for (int j = 0; j < T; j++) {
			mpz_init(C[i][j]);
		}
	}
	for (int i = 0; i < T; i++) {
		for (int j = 0; j < T; j++) {
			mpz_init_set_ui(H[i][j], 0);
		}
	}
	
	// Fill C-matrix
	mpz_mul_ui(p, pk->p, 2);
	for (int i = 0; i < S1; i++) {
		mpz_mul(temp, c, pk->B[i]);
		mpz_mod(temp, temp, p);
		mpq_set_num(q, temp);
		mpq_set_den(q, pk->p);
		mpq_canonicalize(q);
		double d = mpq_get_d(q);
		
#ifdef DEBUG
		printf("\t%i: %1.5f\t\t[", i, d);
#endif
		
		// base convert and encrypt d
		for (int j = 0; j < T; j++) {
#ifdef DEBUG
			printf(" %i", (int)d);
#endif
			fhe_encrypt(C[i][j], pk, (int)d);
			mpz_mul(C[i][j], C[i][j], pk->c[i]);
			mpz_mod(C[i][j], C[i][j], pk->p);
			d -= (int)d;
			d *= 2;
		}
#ifdef DEBUG
		printf(" ]\n");
#endif
	}
#ifdef DEBUG
	PRINT_C;
#endif
	
	// Construct Hammingweight in H-matrix
#define ham(_i,_j) H[_i][_j]
	for (int j = 0; j < T; j++) {
		for (int i = 1; i <= S1; i++) {
			for (int k = (i < (2<<(S-2))) ? i : (2<<(S-2)); k >= 2; k--) {
				mpz_addmul(ham(k-1,j), ham(k-2,j), C[i-1][j]);
				mpz_mod(ham(k-1,j), ham(k-1,j), pk->p);
			}
			mpz_add(ham(0,j), ham(0,j), C[i-1][j]);
			mpz_mod(ham(0,j), ham(0,j), p);
		}
	}
	for (int j = 0; j < T; j++) {
		mpz_set(ham(2,j), ham(3,j));
	}
#ifdef DEBUG
	PRINT_ham;
#endif
#undef ham
	for (int j = 1; j < T; j++) {
		for (int i = min(S, j+1)-1; i >= 0; i--) {
			mpz_swap(H[i][j], H[j][j-i]);
		}
	}
	
#ifdef DEBUG
	PRINT_H;
#endif
	
	// merge rows 0 and 3; 1 and 4
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < S; j++) {
			mpz_set(H[i][i+j+1], H[i+S][i+j+1]);
		}
	}
	
#ifdef DEBUG
	PRINT_H;
#endif
	
	// carry save adder of rows 0,1,2 --> 0,1 (columnwise)
	for (int j = 0; j < T; j++) {
		fhe_fulladd(H[3][j], H[4][j], H[0][j], H[1][j], H[2][j], pk);
	}
	// leftshift the row with the carry bits
	mpz_swap(H[0][T-1], H[3][T-1]);
	fhe_encrypt(H[1][T-1], pk, 0); 
	for (int j = 0; j < T-1; j++) {
		mpz_swap(H[0][j], H[3][j]);
		mpz_swap(H[1][j], H[4][j+1]);
	}	
	
#ifdef DEBUG
	PRINT_H;
#endif

	// ripple-carry-add rows 0 and 1 --> 0 (LSB at T-1)
	// special cases: nothing to do for col T-1, halfadder for T-2
	// note: carry is in temp, result in last row (4)
	fhe_halfadd(H[4][T-2], temp, H[0][T-2], H[1][T-2], pk);
	for (int j = T-3; j >= 0; j--) {
		fhe_fulladd(H[4][j], temp, H[0][j], H[1][j], temp, pk);
	}
	
#ifdef DEBUG
	PRINT_H;
#endif	
	
	// round to nearest integer
	mpz_add(temp, H[4][0], H[4][1]);
	mpz_mod_ui(c, c, 2);
	mpz_add(c, c, temp);
	mpz_mod(c, c, pk->p);
	
	// cleanup
	for (int i = 0; i < S1; i++) {
		for (int j = 0; j < T; j++) {
			mpz_clear(C[i][j]);
		}
	}
	for (int i = 0; i < T; i++) {
		for (int j = 0; j < T; j++) {
			mpz_clear(H[i][j]);
		}
	}
	
	mpz_clear(temp);
	mpz_clear(p);
	mpq_clear(q);
}
