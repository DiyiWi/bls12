/*
 * RELIC is an Efficient LIbrary for Cryptography
 * Copyright (C) 2007-2012 RELIC Authors
 *
 * This file is part of RELIC. RELIC is legal property of its developers,
 * whose names are not listed here. Please refer to the COPYRIGHT file
 * for contact information.
 *
 * RELIC is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * RELIC is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with RELIC. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @defgroup eb Elliptic curves over binary fields.
 */

/**
 * @file
 *
 * Interface of the module for arithmetic on binary elliptic curves.
 *
 * @version $Id$
 * @ingroup eb
 */

#ifndef RELIC_EB_H
#define RELIC_EB_H

#include "relic_fb.h"
#include "relic_bn.h"
#include "relic_conf.h"
#include "relic_types.h"

/*============================================================================*/
/* Constant definitions                                                       */
/*============================================================================*/

/**
 * Binary elliptic curve identifiers.
 */
enum {
	/** NIST B-163 binary curve. */
	NIST_B163 = 1,
	/** NIST K-163 Koblitz curve. */
	NIST_K163,
	/** NIST B-133 binary curve. */
	NIST_B233,
	/** NIST K-233 Koblitz curve. */
	NIST_K233,
	/** Curve over 2^{251} used in eBATs. */
	EBACS_B251,
	/** Curve over 2^{257} good for halving. */
	HALVE_B257,
	/** SECG K-239 binary curve. */
	SECG_K239,
	/** NIST B-283 binary curve. */
	NIST_B283,
	/** NIST K-283 Koblitz curve. */
	NIST_K283,
	/** NIST B-409 binary curve. */
	NIST_B409,
	/** NIST K-409 Koblitz curve. */
	NIST_K409,
	/** NIST B-571 binary curve. */
	NIST_B571,
	/** NIST K-571 Koblitz curve. */
	NIST_K571,
	/** MIRACL low-security pairing-friendly curve. */
	ETAT_P271,
	/** MIRACL low-security pairing-friendly curve. */
	ETAT_T271,
	/** Pairing-friendly curve with 80-bit security. */
	ETAT_S353,
	/** MIRACL high-security pairing-friendly curve. */
	ETAT_S1223,
};

/**
 * Size of a precomputation table using the binary method.
 */
#define EB_TABLE_BASIC		(FB_BITS)

/**
 * Size of a precomputation table using Yao's windowing method.
 */
#define EB_TABLE_YAOWI      (FB_BITS / EB_DEPTH + 1)

/**
 * Size of a precomputation table using the NAF windowing method.
 */
#define EB_TABLE_NAFWI      (FB_BITS / EB_DEPTH + 1)

/**
 * Size of a precomputation table using the single-table comb method.
 */
#define EB_TABLE_COMBS      (1 << EB_DEPTH)

/**
 * Size of a precomputation table using the double-table comb method.
 */
#define EB_TABLE_COMBD		(1 << (EB_DEPTH + 1))

/**
 * Size of a precomputation table using the w-(T)NAF method.
 */
#define EB_TABLE_LWNAF		(1 << (EB_DEPTH - 2))

/**
 * Size of a precomputation table using the chosen algorithm.
 */
#if EB_FIX == BASIC
#define EB_TABLE			EB_TABLE_BASIC
#elif EB_FIX == YAOWI
#define EB_TABLE			EB_TABLE_YAOWI
#elif EB_FIX == NAFWI
#define EB_TABLE			EB_TABLE_NAFWI
#elif EB_FIX == COMBS
#define EB_TABLE			EB_TABLE_COMBS
#elif EB_FIX == COMBD
#define EB_TABLE			EB_TABLE_COMBD
#elif EB_FIX == LWNAF
#define EB_TABLE			EB_TABLE_LWNAF
#endif

/**
 * Maximum size of a precomputation table.
 */
#ifdef STRIP
#define EB_TABLE_MAX 		EB_TABLE
#else
#define EB_TABLE_MAX 		MAX(EB_TABLE_BASIC, EB_TABLE_COMBD)
#endif

/*============================================================================*/
/* Type definitions                                                           */
/*============================================================================*/

/**
 * Represents an elliptic curve point over a binary field.
 */
typedef struct {
#if ALLOC == STATIC
	/** The first coordinate. */
	fb_t x;
	/** The second coordinate. */
	fb_t y;
	/** The third coordinate (projective representation). */
	fb_t z;
#elif ALLOC == DYNAMIC || ALLOC == STACK || ALLOC == AUTO
	/** The first coordinate. */
	fb_st x;
	/** The second coordinate. */
	fb_st y;
	/** The third coordinate (projective representation). */
	fb_st z;
#endif
	/** Flag to indicate that this point is normalized. */
	int norm;
} eb_st;

/**
 * Pointer to an elliptic curve point.
 */
#if ALLOC == AUTO
typedef eb_st eb_t[1];
#else
typedef eb_st *eb_t;
#endif

/*============================================================================*/
/* Macro definitions                                                          */
/*============================================================================*/

/**
 * Initializes a point on a binary elliptic curve with a null value.
 *
 * @param[out] A			- the point to initialize.
 */
#if ALLOC == AUTO
#define eb_null(A)		/* empty */
#else
#define eb_null(A)		A = NULL;
#endif

/**
 * Calls a function to allocate a point on a binary elliptic curve.
 *
 * @param[out] A			- the new point.
 * @throw ERR_NO_MEMORY		- if there is no available memory.
 */
#if ALLOC == DYNAMIC
#define eb_new(A)															\
	A = (eb_t)calloc(1, sizeof(eb_st));										\
	if (A == NULL) {														\
		THROW(ERR_NO_MEMORY);												\
	}																		\

#elif ALLOC == STATIC
#define eb_new(A)															\
	A = (eb_t)alloca(sizeof(eb_st));										\
	if (A == NULL) {														\
		THROW(ERR_NO_MEMORY);												\
	}																		\
	fb_null((A)->x);														\
	fb_null((A)->y);														\
	fb_null((A)->z);														\
	fb_new((A)->x);															\
	fb_new((A)->y);															\
	fb_new((A)->z);															\

#elif ALLOC == AUTO
#define eb_new(A)			/* empty */

#elif ALLOC == STACK
#define eb_new(A)															\
	A = (eb_t)alloca(sizeof(eb_st));										\

#endif

/**
 * Calls a function to clean and free a point on a binary elliptic curve.
 *
 * @param[out] A			- the point to clean and free.
 */
#if ALLOC == DYNAMIC
#define eb_free(A)															\
	if (A != NULL) {														\
		free(A);															\
		A = NULL;															\
	}																		\

#elif ALLOC == STATIC
#define eb_free(A)															\
	if (A != NULL) {														\
		fb_free((A)->x);													\
		fb_free((A)->y);													\
		fb_free((A)->z);													\
		A = NULL;															\
	}																		\

#elif ALLOC == AUTO
#define eb_free(A)			/* empty */

#elif ALLOC == STACK
#define eb_free(A)															\
	A = NULL;																\

#endif

/**
 * Negates a binary elliptic curve point. Computes R = -P.
 *
 * @param[out] R			- the result.
 * @param[in] P				- the point to negate.
 */
#if EB_ADD == BASIC
#define eb_neg(R, P)		eb_neg_basic(R, P)
#elif EB_ADD == PROJC
#define eb_neg(R, P)		eb_neg_projc(R, P)
#endif

/**
 * Adds two binary elliptic curve points. Computes R = P + Q.
 *
 * @param[out] R			- the result.
 * @param[in] P				- the first point to add.
 * @param[in] Q				- the second point to add.
 */
#if EB_ADD == BASIC
#define eb_add(R, P, Q)		eb_add_basic(R, P, Q);
#elif EB_ADD == PROJC
#define eb_add(R, P, Q)		eb_add_projc(R, P, Q);
#endif

/**
 * Subtracts a binary elliptic curve point from another. Computes R = P - Q.
 *
 * @param[out] R			- the result.
 * @param[in] P				- the first point.
 * @param[in] Q				- the second point.
 */
#if EB_ADD == BASIC
#define eb_sub(R, P, Q)		eb_sub_basic(R, P, Q)
#elif EB_ADD == PROJC
#define eb_sub(R, P, Q)		eb_sub_projc(R, P, Q)
#endif

/**
 * Doubles a binary elliptic curve point. Computes R = 2P.
 *
 * @param[out] R			- the result.
 * @param[in] P				- the point to double.
 */
#if EB_ADD == BASIC
#define eb_dbl(R, P)		eb_dbl_basic(R, P);
#elif EB_ADD == PROJC
#define eb_dbl(R, P)		eb_dbl_projc(R, P);
#endif

/**
 * Computes the Frobenius map of a binary elliptic curve point on a Koblitz
 * curve. Computes R = t(P).
 *
 * @param[out] R			- the result.
 * @param[in] P				- the point.
 */
#if EB_ADD == BASIC
#define eb_frb(R, P)		eb_frb_basic(R, P)
#elif EB_ADD == PROJC
#define eb_frb(R, P)		eb_frb_projc(R, P)
#endif

/**
 * Multiplies a binary elliptic curve point by an integer. Computes R = kP.
 *
 * @param[out] R			- the result.
 * @param[in] P				- the point to multiply.
 * @param[in] K				- the integer.
 */
#if EB_MUL == BASIC
#define eb_mul(R, P, K)		eb_mul_basic(R, P, K)
#elif EB_MUL == LODAH
#define eb_mul(R, P, K)		eb_mul_lodah(R, P, K)
#elif EB_MUL == HALVE
#define eb_mul(R, P, K)		eb_mul_halve(R, P, K)
#elif EB_MUL == LWNAF
#define eb_mul(R, P, K)		eb_mul_lwnaf(R, P, K)
#elif EB_MUL == RWNAF
#define eb_mul(R, P, K)		eb_mul_rwnaf(R, P, K)
#endif

/**
 * Builds a precomputation table for multiplying a fixed binary elliptic point
 * point.
 *
 * @param[out] T			- the precomputation table.
 * @param[in] P				- the point to multiply.
 */
#if EB_FIX == BASIC
#define eb_mul_pre(T, P)		eb_mul_pre_basic(T, P)
#elif EB_FIX == YAOWI
#define eb_mul_pre(T, P)		eb_mul_pre_yaowi(T, P)
#elif EB_FIX == NAFWI
#define eb_mul_pre(T, P)		eb_mul_pre_nafwi(T, P)
#elif EB_FIX == COMBS
#define eb_mul_pre(T, P)		eb_mul_pre_combs(T, P)
#elif EB_FIX == COMBD
#define eb_mul_pre(T, P)		eb_mul_pre_combd(T, P)
#elif EB_FIX == LWNAF
#define eb_mul_pre(T, P)		eb_mul_pre_lwnaf(T, P)
#endif

/**
 * Multiplies a fixed binary elliptic point using a precomputation table.
 * Computes R = kP.
 *
 * @param[out] R			- the result.
 * @param[in] T				- the precomputation table.
 * @param[in] K				- the integer.
 */
#if EB_FIX == BASIC
#define eb_mul_fix(R, T, K)		eb_mul_fix_basic(R, T, K)
#elif EB_FIX == YAOWI
#define eb_mul_fix(R, T, K)		eb_mul_fix_yaowi(R, T, K)
#elif EB_FIX == NAFWI
#define eb_mul_fix(R, T, K)		eb_mul_fix_nafwi(R, T, K)
#elif EB_FIX == COMBS
#define eb_mul_fix(R, T, K)		eb_mul_fix_combs(R, T, K)
#elif EB_FIX == COMBD
#define eb_mul_fix(R, T, K)		eb_mul_fix_combd(R, T, K)
#elif EB_FIX == LWNAF
#define eb_mul_fix(R, T, K)		eb_mul_fix_lwnaf(R, T, K)
#endif

/**
 * Multiplies and adds two binary elliptic curve points simultaneously. Computes
 * R = kP + lQ.
 *
 * @param[out] R			- the result.
 * @param[in] P				- the first point to multiply.
 * @param[in] K				- the first integer.
 * @param[in] Q				- the second point to multiply.
 * @param[in] L				- the second integer,
 */
#if EB_SIM == BASIC
#define eb_mul_sim(R, P, K, Q, L)	eb_mul_sim_basic(R, P, K, Q, L)
#elif EB_SIM == TRICK
#define eb_mul_sim(R, P, K, Q, L)	eb_mul_sim_trick(R, P, K, Q, L)
#elif EB_SIM == INTER
#define eb_mul_sim(R, P, K, Q, L)	eb_mul_sim_inter(R, P, K, Q, L)
#elif EB_SIM == JOINT
#define eb_mul_sim(R, P, K, Q, L)	eb_mul_sim_joint(R, P, K, Q, L)
#endif

/*============================================================================*/
/* Function prototypes                                                        */
/*============================================================================*/

/**
 * Initializes the binary elliptic curve arithmetic module.
 */
void eb_curve_init(void);

/**
 * Finalizes the binary elliptic curve arithmetic module.
 */
void eb_curve_clean(void);

/**
 * Returns the a coefficient of the currently configured binary elliptic curve.
 *
 * @return the a coefficient of the elliptic curve.
 */
dig_t *eb_curve_get_a(void);

/**
 * Returns the b coefficient of the currently configured binary elliptic curve.
 *
 * @return the b coefficient of the elliptic curve.
 */
dig_t *eb_curve_get_b(void);

/**
 * Returns the c coefficient of the currently configured supersingular binary
 * elliptic curve.
 *
 * @return the c coefficient of the supersingular elliptic curve.
 */
dig_t *eb_curve_get_c(void);

/**
 * Returns a optimization identifier based on the coefficient a of the curve.
 *
 * @return the optimization identifier.
 */
int eb_curve_opt_a(void);

/**
 * Returns a optimization identifier based on the coefficient b of the curve.
 *
 * @return the optimization identifier.
 */
int eb_curve_opt_b(void);

/**
 * Returns a optimization identifier based on the coefficient c of the curve.
 *
 * @return the optimization identifier.
 */
int eb_curve_opt_c(void);

/**
 * Tests if the configured binary elliptic curve is a Koblitz curve.
 *
 * @return 1 if the binary elliptic curve is a Koblitz curve, 0 otherwise.
 */
int eb_curve_is_kbltz(void);

/**
 * Tests if the configured binary elliptic curve is supersingular.
 *
 * @return 1 if the binary elliptic curve is supersingular, 0 otherwise.
 */
int eb_curve_is_super(void);

/**
 * Returns the generator of the group of points in the binary elliptic curve.
 *
 * @param[out] g			- the returned generator.
 */
void eb_curve_get_gen(eb_t g);

/**
 * Returns the precomputation table for the generator.
 *
 * @return the table.
 */
eb_t *eb_curve_get_tab(void);

/**
 * Returns the order of the group of points in the binary elliptic curve.
 *
 * @param[out] n			- the returned order.
 */
void eb_curve_get_ord(bn_t n);

/**
 * Returns the cofactor of the binary elliptic curve.
 *
 * @param[out] n			- the returned cofactor.
 */
void eb_curve_get_cof(bn_t h);

/**
 * Returns the parameter Vm of a Koblitz curve.
 *
 * @param[out] vm			- the returned parameter.
 */
void eb_curve_get_vm(bn_t vm);

/**
 * Returns the parameter S0 of a Koblitz curve.
 *
 * @param[out] s0			- the returned parameter.
 */
void eb_curve_get_s0(bn_t s0);

/**
 * Returns the parameter S1 of a Koblitz curve.
 *
 * @param[out] s1			- the returned parameter.
 */
void eb_curve_get_s1(bn_t s1);

/**
 * Configures a new ordinary binary elliptic curve by its coefficients and
 * generator.
 *
 * @param[in] a				- the coefficient a of the curve.
 * @param[in] b				- the coefficient b of the curve.
 * @param[in] g				- the generator.
 * @param[in] n				- the order of the generator.
 * @param[in] h				- the cofactor.
 */
void eb_curve_set_ordin(fb_t a, fb_t b, eb_t g, bn_t n, bn_t h);

/**
 * Configures a new Koblitz binary elliptic curve by its coefficients and
 * generator.
 *
 * @param[in] a				- the coefficient a of the curve.
 * @param[in] g				- the generator.
 * @param[in] n				- the order of the generator.
 * @param[in] h				- the cofactor.
 */
void eb_curve_set_kbltz(fb_t a, eb_t g, bn_t n, bn_t h);

/**
 * Configures a new supersingular binary elliptic curve by its coefficients and
 * generator.
 *
 * @param[in] a				- the coefficient a of the curve.
 * @param[in] b				- the coefficient b of the curve.
 * @param[in] c				- the coefficient c of the curve.
 * @param[in] g				- the generator.
 * @param[in] n				- the order of the generator.
 * @param[in] h				- the cofactor.
 */
void eb_curve_set_super(fb_t a, fb_t b, fb_t c, eb_t g, bn_t n, bn_t h);

/**
 * Configures a new binary elliptic curve by its parameter identifier.
 *
 * @param[in] param			- the parameters identifier.
 */
void eb_param_set(int param);

/**
 * Configures some set of curve parameters for the current security level.
 */
int eb_param_set_any(void);

/**
 * Configures some set of ordinary curve parameters for the current security
 * level.
 *
 * @return STS_OK if there is a curve at this security level, STS_ERR otherwise.
 */
int eb_param_set_any_ordin(void);

/**
 * Configures some set of Koblitz curve parameters for the current security
 * level.
 *
 * @return STS_OK if there is a curve at this security level, STS_ERR otherwise.
 */
int eb_param_set_any_kbltz(void);

/**
 * Configures some set of supersingular curve parameters for the current
 * security level.
 *
 * @return STS_OK if there is a curve at this security level, STS_ERR otherwise.
 */
int eb_param_set_any_super(void);

/**
 * Returns the parameter identifier of the currently configured binary elliptic
 * curve.
 *
 * @return the parameter identifier.
 */
int eb_param_get(void);

/**
 * Prints the current configured binary elliptic curve.
 */
void eb_param_print(void);

/**
 * Returns the current security level.
 */
int eb_param_level(void);

/**
 * Tests if a point on a binary elliptic curve is at the infinity.
 *
 * @param[in] p				- the point to test.
 * @return 1 if the point is at infinity, 0 otherise.
 */
int eb_is_infty(eb_t p);

/**
 * Assigns a binary elliptic curve point to a point at the infinity.
 *
 * @param[out] p			- the point to assign.
 */
void eb_set_infty(eb_t p);

/**
 * Copies the second argument to the first argument.
 *
 * @param[out] r			- the result.
 * @param[in] p				- the binary elliptic curve point to copy.
 */
void eb_copy(eb_t r, eb_t p);

/**
 * Compares two binary elliptic curve points.
 *
 * @param[in] p				- the first binary elliptic curve point.
 * @param[in] q				- the second binary elliptic curve point.
 * @return CMP_EQ if p == q and CMP_NE if p != q.
 */
int eb_cmp(eb_t p, eb_t q);

/**
 * Assigns a random value to a binary elliptic curve point.
 *
 * @param[out] p			- the binary elliptic curve point to assign.
 */
void eb_rand(eb_t p);

/**
 * Computes the right-hand side of the elliptic curve equation at a certain
 * elliptic curve point.
 *
 * @param[out] rhs			- the result.
 * @param[in] p				- the point.
 */
void eb_rhs(fb_t rhs, eb_t p);

/** Tests if a point is in the curve.
 *
 * @param[in] p				- the point to test.
 */
int eb_is_valid(eb_t p);

/**
 * Builds a precomputation table for multiplying a random binary elliptic point.
 *
 * @param[out] t			- the precomputation table.
 * @param[in] p				- the point to multiply.
 * @param[in] w				- the window width.
 */
void eb_tab(eb_t *t, eb_t p, int w);

/**
 * Prints a binary elliptic curve point.
 *
 * @param[in] p				- the binary elliptic curve point to print.
 */
void eb_print(eb_t p);

/**
 * Negates a binary elliptic curve point represented by affine coordinates.
 *
 * @param[out] r			- the result.
 * @param[in] p				- the point to negate.
 */
void eb_neg_basic(eb_t r, eb_t p);

/**
 * Negates a binary elliptic curve point represented by projective coordinates.
 *
 * @param[out] r			- the result.
 * @param[in] p				- the point to negate.
 */
void eb_neg_projc(eb_t r, eb_t p);

/**
 * Adds two binary elliptic curve points represented in affine coordinates.
 *
 * @param[out] r			- the result.
 * @param[in] p				- the first point to add.
 * @param[in] q				- the second point to add.
 */
void eb_add_basic(eb_t r, eb_t p, eb_t q);

/**
 * Adds two binary elliptic curve points represented in projective coordinates.
 * Computes R = P + Q.
 *
 * @param[out] r			- the result.
 * @param[in] p				- the first point to add.
 * @param[in] q				- the second point to add.
 */
void eb_add_projc(eb_t r, eb_t p, eb_t q);

/**
 * Subtracts a binary elliptic curve point from another, both points represented
 * by affine coordinates.
 *
 * @param[out] r			- the result.
 * @param[in] p				- the first point.
 * @param[in] q				- the second point.
 */
void eb_sub_basic(eb_t r, eb_t p, eb_t q);

/**
 * Subtracts a binary elliptic curve point from another, both points represented
 * by projective coordinates.
 *
 * @param[out] r			- the result.
 * @param[in] p				- the first point.
 * @param[in] q				- the second point.
 */
void eb_sub_projc(eb_t r, eb_t p, eb_t q);

/**
 * Doubles a binary elliptic curve point represented in affine coordinates.
 *
 * @param[out] r			- the result.
 * @param[in] p				- the point to double.
 */
void eb_dbl_basic(eb_t r, eb_t p);

/**
 * Doubles a binary elliptic curve point represented in projective coordinates.
 *
 * @param[out] r			- the result.
 * @param[in] p				- the point to double.
 */
void eb_dbl_projc(eb_t r, eb_t p);

/**
 * Halves a point represented in affine coordinates.
 *
 * @param[out] r			- the result.
 * @param[in] p				- the point to halve.
 */
void eb_hlv(eb_t r, eb_t p);

/**
 * Computes the Frobenius map of a binary elliptic curve point represented
 * by affine coordinates.
 *
 * @param[out] r			- the result.
 * @param[in] p				- the point.
 */
void eb_frb_basic(eb_t r, eb_t p);

/**
 * Computes the Frobenius map of a binary elliptic curve point represented
 * by projective coordinates.
 *
 * @param[out] r			- the result.
 * @param[in] p				- the point.
 */
void eb_frb_projc(eb_t r, eb_t p);

/**
 * Multiplies a binary elliptic point by an integer using the binary method.
 *
 * @param[out] r			- the result.
 * @param[in] p				- the point to multiply.
 * @param[in] k				- the integer.
 */
void eb_mul_basic(eb_t r, eb_t p, bn_t k);

/**
 * Multiplies a binary elliptic point by an integer using the constant-time
 * L�pez-Dahab point multiplication method.
 *
 * @param[out] r			- the result.
 * @param[in] p				- the point to multiply.
 * @param[in] k				- the integer.
 */
void eb_mul_lodah(eb_t r, eb_t p, bn_t k);

/**
 * Multiplies a binary elliptic point by an integer using the left-to-right
 * w-NAF method. If the binary curve is a Koblitz curve, w-TNAF is used.
 *
 * @param[out] r			- the result.
 * @param[in] p				- the point to multiply.
 * @param[in] k				- the integer.
 */
void eb_mul_lwnaf(eb_t r, eb_t p, bn_t k);

/**
 * Multiplies a binary elliptic point by an integer using the right-to-left
 * w-NAF method. If the binary curve is a Koblitz curve, w-TNAF is used.
 *
 * @param[out] r			- the result.
 * @param[in] p				- the point to multiply.
 * @param[in] k				- the integer.
 */
void eb_mul_rwnaf(eb_t r, eb_t p, bn_t k);

/**
 * Multiplies a binary elliptic point by an integer using the halving method.
 *
 * @param[out] r			- the result.
 * @param[in] p				- the point to multiply.
 * @param[in] k				- the integer.
 */
void eb_mul_halve(eb_t r, eb_t p, bn_t k);

/**
 * Multiplies the generator of a binary elliptic curve by an integer.
 *
 * @param[out] r			- the result.
 * @param[in] k				- the integer.
 */
void eb_mul_gen(eb_t r, bn_t k);

/**
 * Multiplies a binary elliptic point by a small integer.
 *
 * @param[out] r			- the result.
 * @param[in] p				- the point to multiply.
 * @param[in] k				- the integer.
 */
void eb_mul_dig(eb_t r, eb_t p, dig_t k);

/**
 * Builds a precomputation table for multiplying a fixed binary elliptic point
 * using the binary method.
 *
 * @param[out] t			- the precomputation table.
 * @param[in] p				- the point to multiply.
 */
void eb_mul_pre_basic(eb_t *t, eb_t p);

/**
 * Builds a precomputation table for multiplying a fixed binary elliptic point
 * using Yao's windowing method.
 *
 * @param[out] t			- the precomputation table.
 * @param[in] p				- the point to multiply.
 */
void eb_mul_pre_yaowi(eb_t *t, eb_t p);

/**
 * Builds a precomputation table for multiplying a fixed binary elliptic point
 * using the NAF windowing method.
 *
 * @param[out] t			- the precomputation table.
 * @param[in] p				- the point to multiply.
 */
void eb_mul_pre_nafwi(eb_t *t, eb_t p);

/**
 * Builds a precomputation table for multiplying a fixed binary elliptic point
 * using the single-table comb method.
 *
 * @param[out] t			- the precomputation table.
 * @param[in] p				- the point to multiply.
 */
void eb_mul_pre_combs(eb_t *t, eb_t p);

/**
 * Builds a precomputation table for multiplying a fixed binary elliptic point
 * using the double-table comb method.
 *
 * @param[out] t			- the precomputation table.
 * @param[in] p				- the point to multiply.
 */
void eb_mul_pre_combd(eb_t *t, eb_t p);

/**
 * Builds a precomputation table for multiplying a fixed binary elliptic point
 * using the w-(T)NAF method.
 *
 * @param[out] t			- the precomputation table.
 * @param[in] p				- the point to multiply.
 */
void eb_mul_pre_lwnaf(eb_t *t, eb_t p);

/**
 * Multiplies a fixed binary elliptic point using a precomputation table and
 * the binary method.
 *
 * @param[out] r			- the result.
 * @param[in] t				- the precomputation table.
 * @param[in] k				- the integer.
 */
void eb_mul_fix_basic(eb_t r, eb_t *t, bn_t k);

/**
 * Multiplies a fixed binary elliptic point using a precomputation table and
 * Yao's windowing method
 *
 * @param[out] r			- the result.
 * @param[in] t				- the precomputation table.
 * @param[in] k				- the integer.
 */
void eb_mul_fix_yaowi(eb_t r, eb_t *t, bn_t k);

/**
 * Multiplies a fixed binary elliptic point using a precomputation table and
 * the w-(T)NAF method.
 *
 * @param[out] r			- the result.
 * @param[in] t				- the precomputation table.
 * @param[in] k				- the integer.
 */
void eb_mul_fix_nafwi(eb_t r, eb_t *t, bn_t k);

/**
 * Multiplies a fixed binary elliptic point using a precomputation table and
 * the single-table comb method.
 *
 * @param[out] r			- the result.
 * @param[in] t				- the precomputation table.
 * @param[in] k				- the integer.
 */
void eb_mul_fix_combs(eb_t r, eb_t *t, bn_t k);

/**
 * Multiplies a fixed binary elliptic point using a precomputation table and
 * the double-table comb method.
 *
 * @param[out] r			- the result.
 * @param[in] t				- the precomputation table.
 * @param[in] k				- the integer.
 */
void eb_mul_fix_combd(eb_t r, eb_t *t, bn_t k);

/**
 * Multiplies a fixed binary elliptic point using a precomputation table and
 * the w-(T)NAF method.
 *
 * @param[out] r			- the result.
 * @param[in] t				- the precomputation table.
 * @param[in] k				- the integer.
 */
void eb_mul_fix_lwnaf(eb_t r, eb_t *t, bn_t k);

/**
 * Multiplies and adds two binary elliptic curve points simultaneously using
 * scalar multiplication and point addition.
 *
 * @param[out] r			- the result.
 * @param[in] p				- the first point to multiply.
 * @param[in] k				- the first integer.
 * @param[in] q				- the second point to multiply.
 * @param[in] l				- the second integer,
 */
void eb_mul_sim_basic(eb_t r, eb_t p, bn_t k, eb_t q, bn_t l);

/**
 * Multiplies and adds two binary elliptic curve points simultaneously using
 * shamir's trick.
 *
 * @param[out] r			- the result.
 * @param[in] p				- the first point to multiply.
 * @param[in] k				- the first integer.
 * @param[in] q				- the second point to multiply.
 * @param[in] l				- the second integer,
 */
void eb_mul_sim_trick(eb_t r, eb_t p, bn_t k, eb_t q, bn_t l);

/**
 * Multiplies and adds two binary elliptic curve points simultaneously using
 * interleaving of NAFs.
 *
 * @param[out] r			- the result.
 * @param[in] p				- the first point to multiply.
 * @param[in] k				- the first integer.
 * @param[in] q				- the second point to multiply.
 * @param[in] l				- the second integer,
 */
void eb_mul_sim_inter(eb_t r, eb_t p, bn_t k, eb_t q, bn_t l);

/**
 * Multiplies and adds two binary elliptic curve points simultaneously using
 * Solinas' Joint Sparse Form.
 *
 * @param[out] r			- the result.
 * @param[in] p				- the first point to multiply.
 * @param[in] k				- the first integer.
 * @param[in] q				- the second point to multiply.
 * @param[in] l				- the second integer,
 */
void eb_mul_sim_joint(eb_t r, eb_t p, bn_t k, eb_t q, bn_t l);

/**
 * Multiplies and adds the generator and a binary elliptic curve point
 * simultaneously. Computes R = kG + lQ.
 *
 * @param[out] r			- the result.
 * @param[in] k				- the first integer.
 * @param[in] q				- the second point to multiply.
 * @param[in] l				- the second integer,
 */
void eb_mul_sim_gen(eb_t r, bn_t k, eb_t q, bn_t l);

/**
 * Converts a point to affine coordinates.
 *
 * @param[out] r			- the result.
 * @param[in] p				- the point to convert.
 */
void eb_norm(eb_t r, eb_t p);

/**
 * Converts multiple points to affine coordinates.
 *
 * @param[out] r			- the result.
 * @param[in] t				- the points to convert.
 * @param[in] n				- the number of points.
 */
void eb_norm_sim(eb_t * r, eb_t * t, int n);

/**
 * Maps a byte array to a point in a binary elliptic curve.
 *
 * @param[out] p			- the result.
 * @param[in] msg			- the byte array to map.
 * @param[in] len			- the array length in bytes.
 */
void eb_map(eb_t p, unsigned char *msg, int len);

/**
 * Compresses a point.
 *
 * @param[out] r			- the result.
 * @param[in] p				- the point to compress.
 */
void eb_pck(eb_t r, eb_t p);

/**
 * Decompresses a point.
 *
 * @param[out] r			- the result.
 * @param[in] p				- the point to decompress.
 * @return if the decompression was successful
 */
int eb_upk(eb_t r, eb_t p);

#endif /* !RELIC_EB_H */
