/*
 * Copyright (c) 2022 Infineon Technologies AG. All Rights Reserved.
 *
 * Use of this file is subject to the terms of use agreed between (i) you or the company in which ordinary course of
 * business you are acting and (ii) Infineon Technologies AG, its affiliates or its licensees. If and as long as no
 * such terms of use are agreed, use of this file is subject to the Evaluation Software License Agreement distributed
 * along with this file within the software delivery package.
 *
 */

/**
 * \file Ifx_Math.h
 * \brief Contains all the type and macro definitions which are common to all the modules.
 */

#ifndef IFX_MATH_H
#define IFX_MATH_H
#include "Ifx_Math_Cfg.h"
#include "Ifx_Math_UsrOpt.h"
#include "types.h"

/**
 * Maximum value which can be represented with a signed 16-bit format
 */
#define IFX_MATH_FRACT16_MAX             ((Ifx_Math_Fract16)0x7FFF)

/**
 * Maximum value which can be represented with a signed 32-bit format
 */
#define IFX_MATH_FRACT32_MAX             ((Ifx_Math_Fract32)0x7FFFFFFF)

/**
 * Minimum value which can be represented with a signed 16-bit format
 */
#define IFX_MATH_FRACT16_MIN             ((Ifx_Math_Fract16)0x8000)

/**
 * Minimum value which can be represented with a signed 32-bit format
 */
#define IFX_MATH_FRACT32_MIN             ((Ifx_Math_Fract32)0x80000000U)

/**
 * Represents \f$pi\f$, normalized by \f$2*pi\f$, represented in 32-bit
 */
#define IFX_MATH_PI_INDEX                (2147483648UL)

/**
 * Represents \f$1/sqrt(3)\f$ in Q15
 */
#define IFX_MATH_ONE_OVER_SQRT3_Q15      ((Ifx_Math_Fract16)18919)

/**
 * Factor to convert from microseconds to seconds
 */
#define IFX_MATH_MICROSECONDS_TO_SECONDS (1000000)

/**
 * 16-bit signed fractional type
 */
typedef sint16 Ifx_Math_Fract16;

/**
 * Format of a fractional Q number.
 * The number after Q specifies the bit width of the fractional part of a number.
 */
typedef enum Ifx_Math_FractQFormat
{
    Ifx_Math_FractQFormat_q0  = 0,  /**<0 fractional bits.*/
    Ifx_Math_FractQFormat_q1  = 1,  /**<1 fractional bit.*/
    Ifx_Math_FractQFormat_q2  = 2,  /**<2 fractional bits.*/
    Ifx_Math_FractQFormat_q3  = 3,  /**<3 fractional bits.*/
    Ifx_Math_FractQFormat_q4  = 4,  /**<4 fractional bits.*/
    Ifx_Math_FractQFormat_q5  = 5,  /**<5 fractional bits.*/
    Ifx_Math_FractQFormat_q6  = 6,  /**<6 fractional bits.*/
    Ifx_Math_FractQFormat_q7  = 7,  /**<7 fractional bits.*/
    Ifx_Math_FractQFormat_q8  = 8,  /**<8 fractional bits.*/
    Ifx_Math_FractQFormat_q9  = 9,  /**<9 fractional bits.*/
    Ifx_Math_FractQFormat_q10 = 10, /**<10 fractional bits.*/
    Ifx_Math_FractQFormat_q11 = 11, /**<11 fractional bits.*/
    Ifx_Math_FractQFormat_q12 = 12, /**<12 fractional bits.*/
    Ifx_Math_FractQFormat_q13 = 13, /**<13 fractional bits.*/
    Ifx_Math_FractQFormat_q14 = 14, /**<14 fractional bits.*/
    Ifx_Math_FractQFormat_q15 = 15, /**<15 fractional bits.*/
    Ifx_Math_FractQFormat_q16 = 16, /**<16 fractional bits.*/
    Ifx_Math_FractQFormat_q17 = 17, /**<17 fractional bits.*/
    Ifx_Math_FractQFormat_q18 = 18, /**<18 fractional bits.*/
    Ifx_Math_FractQFormat_q19 = 19, /**<19 fractional bits.*/
    Ifx_Math_FractQFormat_q20 = 20, /**<20 fractional bits.*/
    Ifx_Math_FractQFormat_q21 = 21, /**<21 fractional bits.*/
    Ifx_Math_FractQFormat_q22 = 22, /**<22 fractional bits.*/
    Ifx_Math_FractQFormat_q23 = 23, /**<23 fractional bits.*/
    Ifx_Math_FractQFormat_q24 = 24, /**<24 fractional bits.*/
    Ifx_Math_FractQFormat_q25 = 25, /**<25 fractional bits.*/
    Ifx_Math_FractQFormat_q26 = 26, /**<26 fractional bits.*/
    Ifx_Math_FractQFormat_q27 = 27, /**<27 fractional bits.*/
    Ifx_Math_FractQFormat_q28 = 28, /**<28 fractional bits.*/
    Ifx_Math_FractQFormat_q29 = 29, /**<29 fractional bits.*/
    Ifx_Math_FractQFormat_q30 = 30, /**<30 fractional bits.*/
    Ifx_Math_FractQFormat_q31 = 31  /**<31 fractional bits.*/
} Ifx_Math_FractQFormat;

/**
 * Single precision float type
 */
typedef float Ifx_Math_Float32;

/**
 * 32-bit signed fractional type
 */
typedef sint32 Ifx_Math_Fract32;

/**
 * 64-bit signed fractional type
 */
typedef sint64 Ifx_Math_Fract64;

/**
 * Data Container of a three phase value(U, V, W) in 16-bit signed fractional type.
 */
typedef struct Ifx_Math_3PhaseFract16
{
    /**
     * Phase U quantity
     */
    Ifx_Math_Fract16 u;

    /**
     * Phase V quantity
     */
    Ifx_Math_Fract16 v;

    /**
     * Phase W quantity
     */
    Ifx_Math_Fract16 w;
} Ifx_Math_3PhaseFract16;

/**
 * Complex signed fixed point 16-bit data type
 */
typedef struct Ifx_Math_CmpFract16
{
    /**
     * Real part
     */
    Ifx_Math_Fract16 real;

    /**
     * Imaginary part
     */
    Ifx_Math_Fract16 imag;
} Ifx_Math_CmpFract16;

/**
 * Polar signed fixed point 16-bit data type
 */
typedef struct Ifx_Math_PolarFract16
{
    /**
     * 16-bit Amplitude
     */
    Ifx_Math_Fract16 amplitude;

    /**
     * 32-bit angle
     */
    uint32 angle;
} Ifx_Math_PolarFract16;

/**
 * 16-bit fractional number with Q format
 */
typedef struct Ifx_Math_Fract16Q
{
    /**
     * 16-bit fraction number with variable Q format
     */
    Ifx_Math_Fract16 value;

    /**
     * Q format
     */
    Ifx_Math_FractQFormat qFormat;
} Ifx_Math_Fract16Q;

#endif /*IFX_MATH_H*/
