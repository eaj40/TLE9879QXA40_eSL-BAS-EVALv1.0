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
 * \file Ifx_Math_ShRRnd.h
 * \brief Provides functions which perform a right-shift operation with rounding.
 * The functions in this module are providing a right-shift operation with rounding: the fractional result of the
 * division that corresponds to the (x &gt;&gt; n) operation, that means the result of (x / 2^n) is rounded to the
 * nearest integer.
 */

#ifndef IFX_MATH_SHRRND_H
#define IFX_MATH_SHRRND_H
#include "Ifx_Math.h"
#include "Ifx_Math_Add.h"
#include "Ifx_Math_ShL.h"
#include "Ifx_Math_ShR.h"

/**
 *  \brief Perform a right-shift operation on a 16-bit input with rounding.
 *
 *  This function returns the result of the corresponding division of the 16-bit input parameter x by two powered by
 * shift value n, rounded up to the nearest integer. For example: 3/4 = 1, 5/4 = 1, 7/4 = 2. In case that the fractional
 * part equals 0.5, the result is rounded up. For example: 1/2 = 1, 3/2 = 2, -1/2 = 0.
 *
 *  \param [in] x 16-bit fractional number
 *  \param [in] n Number of bits to shift the input right, between 1 and 15
 *
 *  \return 16-bit fractional number
 */
static inline Ifx_Math_Fract16 Ifx_Math_ShRRnd_F16(Ifx_Math_Fract16 x, uint8 n)
{
    Ifx_Math_Fract16 result;
    Ifx_Math_Fract32 input;

    /* Addition of 0.5 in the input value and rounding off to nearest integer */
    input  = Ifx_Math_Add_F32((Ifx_Math_Fract32)x, Ifx_Math_ShL_F32(1, (n - 1U)));
    result = (Ifx_Math_Fract16)Ifx_Math_ShR_F32(input, n);

    return result;
}


/**
 *  \brief Perform a right-shift operation on a 32-bit input with rounding.
 *
 *  This function returns the result of the corresponding division of the 32-bit input parameter x by two powered by
 * shift value n, rounded up to the nearest integer. For example: 3/4 = 1, 5/4 = 1, 7/4 = 2. In case that the fractional
 * part equals 0.5, the result is rounded up. For example: 1/2 = 1, 3/2 = 2, -1/2 = 0.
 *
 *  \param [in] x 32-bit fractional number
 *  \param [in] n Number of bits to shift the input right, between 1 and 31
 *
 *  \return 32-bit fractional number
 */
static inline Ifx_Math_Fract32 Ifx_Math_ShRRnd_F32(Ifx_Math_Fract32 x, uint8 n)
{
    Ifx_Math_Fract32 result;
    Ifx_Math_Fract64 input;

    /* Addition of 0.5 in the input value and rounding off to nearest integer */
    input = (Ifx_Math_Fract64)x + Ifx_Math_ShL_F32(1, (n - 1U));

    /* polyspace +2 MISRA2012:10.1 [Justified:Low] "Bitwise operators on signed values are required by fixed point
     * arithmetic." */
    result = (Ifx_Math_Fract32)(input >> n);

    return result;
}


#endif /*IFX_MATH_SHRRND_H*/
