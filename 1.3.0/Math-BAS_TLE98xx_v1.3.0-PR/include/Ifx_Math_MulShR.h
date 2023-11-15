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
 * \file Ifx_Math_MulShR.h
 * \brief Provides functions that calculate the shifted fractional product of two fractional numbers.
 * The functions in this module calculate the fractional product of two fractional numbers, right shifted by n, without
 * saturation and overflow detection.
 */

#ifndef IFX_MATH_MULSHR_H
#define IFX_MATH_MULSHR_H
#include "Ifx_Math.h"
#include "Ifx_Math_ShR.h"

/**
 *  \brief Calculates the bits need to be shifted right when multiplying two fractional types.
 *
 *
 *  \param [in] typeX The format of the first operand of multiplication
 *  \param [in] typeY The format of the second operand of multiplication
 *  \param [in] typeReturn The format of the product
 *
 *  \return Number of bits needed to be shifted right
 */
static inline uint8 Ifx_Math_MulShR_ShiftMul(Ifx_Math_FractQFormat typeX, Ifx_Math_FractQFormat typeY,
                                             Ifx_Math_FractQFormat typeReturn)
{
    uint8 rightShift;

    if (((uint8)typeX + (uint8)typeY) >= (uint8)typeReturn)
    {
        rightShift = ((uint8)typeX + (uint8)typeY) - (uint8)typeReturn;
    }
    else
    {
        rightShift = 0u;
    }

    return rightShift;
}


/**
 *  \brief Calculates the 16-bit shifted fractional product of two 16-bit fractional numbers.
 *
 *  This function calculates the 16-bit fractional product of two 16-bit fractional numbers, x and y, right shifted by
 * n, without saturation and overflow detection.
 *
 *  \param [in] x 16-bit fractional number with the format Qa
 *  \param [in] y 16-bit fractional number with the format Qb
 *  \param [in] n Number of bits to right shift the multiplication result, between 0 and 30
 *
 *  \return 16-bit fractional number with the format Q(a+b-n)
 */

/* polyspace-begin CODE-METRIC:CALLING [Justified:Low] "Common math library functions are expected to be called multiple
 * times." */
static inline Ifx_Math_Fract16 Ifx_Math_MulShR_F16(Ifx_Math_Fract16 x, Ifx_Math_Fract16 y, uint8 n)
{
    return (Ifx_Math_Fract16)Ifx_Math_ShR_F32(((Ifx_Math_Fract32)x * (Ifx_Math_Fract32)y), n);
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
 *  \param [in] n Number of bits to right shift the multiplication result, between 0 and 62
 *
 *  \return 32-bit fractional number with the format Q(a+b-n)
 */

/* polyspace-begin CODE-METRIC:CALLING [Justified:Low] "Common math library functions are expected to be called multiple
 * times." */
static inline Ifx_Math_Fract32 Ifx_Math_MulShR_F32(Ifx_Math_Fract32 x, Ifx_Math_Fract32 y, uint8 n)
{
    /* Stores the temporary result of the multiplication */
    Ifx_Math_Fract64 tmpMul;

    /* Stores the return value */
    Ifx_Math_Fract32 mulReturn;
    tmpMul = (Ifx_Math_Fract64)x * (Ifx_Math_Fract64)y;

    /* polyspace +2 MISRA2012:10.1 [Justified:Low] "Bitwise operators on signed values are required by fixed point
     * arithmetic." */
    mulReturn = (Ifx_Math_Fract32)(tmpMul >> n);

    return mulReturn;
}


/* polyspace-end CODE-METRIC:CALLING [Justified:Low] "Common math library functions are expected to be called multiple
 * times." */

/**
 *  \brief Calculates the 32-bit shifted fractional product of two 16-bit fractional numbers.
 *
 *  This function calculates the 32-bit fractional product of two 16-bit fractional numbers, x and y, right shifted by
 * n, without saturation and overflow detection.
 *
 *  \param [in] x 16-bit fractional number with the format Qa
 *  \param [in] y 16-bit fractional number with the format Qb
 *  \param [in] n Number of bits to right shift the multiplication result, between 0 and 30
 *
 *  \return 32-bit fractional number with the format Q(a+b-n)
 */

/* polyspace-begin CODE-METRIC:CALLING [Justified:Low] "Common math library functions are expected to be called multiple
 * times." */
static inline Ifx_Math_Fract32 Ifx_Math_MulShR_F32_F16F16(Ifx_Math_Fract16 x, Ifx_Math_Fract16 y, uint8 n)
{
    return Ifx_Math_ShR_F32(((Ifx_Math_Fract32)x * (Ifx_Math_Fract32)y), n);
}


/* polyspace-end CODE-METRIC:CALLING [Justified:Low] "Common math library functions are expected to be called multiple
 * times." */

#endif /*IFX_MATH_MULSHR_H*/
