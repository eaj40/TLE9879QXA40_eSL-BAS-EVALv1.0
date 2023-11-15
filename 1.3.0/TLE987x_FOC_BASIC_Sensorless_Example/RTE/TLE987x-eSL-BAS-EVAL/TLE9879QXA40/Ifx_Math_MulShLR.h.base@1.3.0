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
 * \file Ifx_Math_MulShLR.h
 * \brief Provides functions that calculate the shifted fractional product of two fractional numbers.
 * The functions in this module calculate the fractional product of two fractional numbers, shifted by n, without
 * saturation and overflow detection. The result is shifted right if n is greater than zero and left if n is less than
 * zero.
 */

#ifndef IFX_MATH_MULSHLR_H
#define IFX_MATH_MULSHLR_H
#include "Ifx_Math.h"
#include "Ifx_Math_ShLR.h"

/**
 *  \brief Calculates the bits need to be shifted when multiplying two fractional types.
 *
 *
 *  \param [in] typeX The format of the first operand of multiplication
 *  \param [in] typeY The format of the second operand of multiplication
 *  \param [in] typeReturn The format of the product
 *
 *  \return Number of bits needed to be shifted
 */
static inline sint8 Ifx_Math_MulShLR_ShiftMul(Ifx_Math_FractQFormat typeX, Ifx_Math_FractQFormat typeY,
                                              Ifx_Math_FractQFormat typeReturn)
{
    return (sint8)typeX + (sint8)typeY - (sint8)typeReturn;
}


/**
 *  \brief Calculates the 16-bit shifted fractional product of two 16-bit fractional numbers.
 *
 *  This function calculates the 16-bit fractional product of two 16-bit fractional numbers, x and y, shifted by n,
 * without saturation and overflow detection. The result is shifted right if n is greater than zero and left if n is
 * less than zero.
 *
 *  \param [in] x 16-bit fractional number with the format Qa
 *  \param [in] y 16-bit fractional number with the format Qb
 *  \param [in] n Number of bits to shift the multiplication result, between -15 and 30. Positive n indicates right
 * shift, negative n indicates left shift
 *
 *  \return 16-bit fractional number with the format Q(a+b-n)
 */

/* polyspace-begin CODE-METRIC:CALLING [Justified:Low] "Common math library functions are expected to be called multiple
 * times." */
static inline Ifx_Math_Fract16 Ifx_Math_MulShLR_F16(Ifx_Math_Fract16 x, Ifx_Math_Fract16 y, sint8 n)
{
    return (Ifx_Math_Fract16)Ifx_Math_ShLR_F32(((Ifx_Math_Fract32)x * (Ifx_Math_Fract32)y), -n);
}


/* polyspace-end CODE-METRIC:CALLING [Justified:Low] "Common math library functions are expected to be called multiple
 * times." */

/**
 *  \brief Calculates the 32-bit shifted fractional product of two 32-bit fractional numbers.
 *
 *  This function calculates the 32-bit fractional product of two 32-bit fractional numbers, x and y, shifted by n,
 * without saturation and overflow detection.
 *
 *  \param [in] x 32-bit fractional number with the format Qa
 *  \param [in] y 32-bit fractional number with the format Qb
 *  \param [in] n Number of bits to shift the multiplication result, between -31 and 62. Positive n indicates right
 * shift, negative n indicates left shift
 *
 *  \return 32-bit fractional number with the format Q(a+b-n)
 */

/* polyspace-begin CODE-METRIC:CALLING [Justified:Low] "Common math library functions are expected to be called multiple
 * times." */
static inline Ifx_Math_Fract32 Ifx_Math_MulShLR_F32(Ifx_Math_Fract32 x, Ifx_Math_Fract32 y, sint8 n)
{
    /* Stores the temporary result of the multiplication */
    Ifx_Math_Fract64 tmpMul;

    /* Stores the return value */
    Ifx_Math_Fract32 mulReturn;
    tmpMul = (Ifx_Math_Fract64)x * (Ifx_Math_Fract64)y;

    if (n >= 0)
    {
        /* polyspace +2 MISRA2012:10.1 [Justified:Low] "Bitwise operators on signed values are required by fixed point
         * arithmetic." */
        mulReturn = (Ifx_Math_Fract32)(tmpMul >> n);
    }
    else
    {
        /* polyspace +2 MISRA2012:10.1 [Justified:Low] "Bitwise operators on signed values are required by fixed point
         * arithmetic." */
        mulReturn = (Ifx_Math_Fract32)(tmpMul << -n);
    }

    return mulReturn;
}


/* polyspace-end CODE-METRIC:CALLING [Justified:Low] "Common math library functions are expected to be called multiple
 * times." */

/**
 *  \brief Calculates the 32-bit shifted fractional product of two 16-bit fractional numbers.
 *
 *  This function calculates the 32-bit fractional product of two 16-bit fractional numbers, x and y, shifted by n,
 * without saturation and overflow detection.
 *
 *  \param [in] x 16-bit fractional number with the format Qa
 *  \param [in] y 16-bit fractional number with the format Qb
 *  \param [in] n Number of bits to shift the multiplication result, between -31 and 30. Positive n indicates right
 * shift, negative n indicates left shift
 *
 *  \return 32-bit fractional number with the format Q(a+b-n)
 */
static inline Ifx_Math_Fract32 Ifx_Math_MulShLR_F32_F16F16(Ifx_Math_Fract16 x, Ifx_Math_Fract16 y, sint8 n)
{
    return Ifx_Math_ShLR_F32(((Ifx_Math_Fract32)x * (Ifx_Math_Fract32)y), -n);
}


#endif /*IFX_MATH_MULSHLR_H*/
