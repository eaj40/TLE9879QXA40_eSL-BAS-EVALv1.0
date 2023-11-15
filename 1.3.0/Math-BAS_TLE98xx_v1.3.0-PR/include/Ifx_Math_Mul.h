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
 * \file Ifx_Math_Mul.h
 * \brief Provides functions to calculate the fractional product of two fractional numbers.
 * The functions in this module calculate the fractional product of two fractional numbers, shifted by the corresponding
 * number of bits, without saturation and overflow detection.
 */

#ifndef IFX_MATH_MUL_H
#define IFX_MATH_MUL_H
#include "Ifx_Math.h"
#include "Ifx_Math_MulShR.h"

/**
 *  \brief Calculate the 16-bit fractional product of two 16-bit fractional numbers.
 *
 *  This function calculates the 16-bit fractional product of two 16-bit fractional numbers, x and y, shifted right by
 * 15, without saturation and overflow detection.
 *
 *  \param [in] x 16-bit fractional number with the format Qa
 *  \param [in] y 16-bit fractional number with the format Qb
 *
 *  \return 16-bit fractional number with the format Q(a+b-15)
 */
static inline Ifx_Math_Fract16 Ifx_Math_Mul_F16(Ifx_Math_Fract16 x, Ifx_Math_Fract16 y)
{
    return Ifx_Math_MulShR_F16(x, y, 15u);
}


/**
 *  \brief Calculates 32-bit the fractional product of two 32-bit fractional numbers.
 *
 *  This function calculates the 32-bit the fractional product of two 32-bit fractional numbers, x and y, shifted right
 * by 31, without saturation and overflow detection.
 *
 *  \param [in] x 32-bit fractional number with the format Qa
 *  \param [in] y 32-bit fractional number with the format Qb
 *
 *  \return 32-bit fractional number with the format Q(a+b-31)
 */
static inline Ifx_Math_Fract32 Ifx_Math_Mul_F32(Ifx_Math_Fract32 x, Ifx_Math_Fract32 y)
{
    return Ifx_Math_MulShR_F32(x, y, 31u);
}


/**
 *  \brief Calculate the 32-bit fractional product of two 16-bit fractional numbers,, shifted left by 1.
 *
 *  This function calculates the 32-bit fractional product of two 16-bit fractional numbers, x and y, shifted left by 1,
 * without saturation and overflow detection.
 *
 *  \param [in] x 16-bit fractional number with the format Qa
 *  \param [in] y 16-bit fractional number with the format Qb
 *
 *  \return 32-bit fractional number with the format Q(a+b+1)
 */
static inline Ifx_Math_Fract32 Ifx_Math_Mul_F32_F16F16(Ifx_Math_Fract16 x, Ifx_Math_Fract16 y)
{
    /* polyspace +2 MISRA2012:10.1 [Justified:Low] "Bitwise operators on signed values are required by fixed point
     * arithmetic." */
    return (Ifx_Math_Fract32)(((Ifx_Math_Fract32)x * (Ifx_Math_Fract32)y) << 1U);
}


#endif /*IFX_MATH_MUL_H*/
