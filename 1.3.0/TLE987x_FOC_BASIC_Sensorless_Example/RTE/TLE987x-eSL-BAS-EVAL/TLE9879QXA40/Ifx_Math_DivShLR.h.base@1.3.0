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
 * \file Ifx_Math_DivShLR.h
 * \brief Provides functions to calculate the shifted fractional quotient of two fractional inputs.
 * The functions in this module calculate the fractional quotient of the two input parameters, x and y, where x is the
 * numerator and y the denominator, shifted by n without saturation and overflow detection. The result is shifted left
 * if n is greater than zero and right if n is less than zero.
 * The division is protected from division by zero by returning the maximum of the return type of the function if the
 * numerator is greater or equal to zero and the minimum if the numerator is less than zero.
 */

#ifndef IFX_MATH_DIVSHLR_H
#define IFX_MATH_DIVSHLR_H
#include "Ifx_Math.h"
#include "Ifx_Math_ShLR.h"

/**
 *  \brief Calculate the bits need to be shifted when dividing two fractional types
 *
 *
 *  \param [in] typeX The format of the first operand of division
 *  \param [in] typeY The format of the second operand of division
 *  \param [in] typeReturn The format of the quotient
 *
 *  \return Shift value
 */
static inline sint8 Ifx_Math_DivShLR_ShiftDiv(Ifx_Math_FractQFormat typeX, Ifx_Math_FractQFormat typeY,
                                              Ifx_Math_FractQFormat typeReturn)
{
    return -(sint8)typeX + (sint8)typeY + (sint8)typeReturn;
}


/**
 *  \brief Calculates the 16-bit shifted fractional quotient of two 16-bit fractional inputs.
 *
 *  This function calculates the fractional quotient between x and y, where x is the numerator and y the denominator,
 * shifted by n without saturation and overflow detection.  The division is protected from division by zero by returning
 * IFX_MATH_FRACT16_MAX if x &gt;= 0 and IFX_MATH_FRACT16_MIN if x &lt; 0.
 *
 *  \param [in] x 16-bit fractional number with the format Qa
 *  \param [in] y 16-bit fractional number with the format Qb
 *  \param [in] n Number of bits to shift the division result, between -16 and 16. Positive n indicates left shift,
 * negative n indicates right shift
 *
 *  \return 16-bit fractional number with the format Q(a-b+n)
 */
static inline Ifx_Math_Fract16 Ifx_Math_DivShLR_F16(Ifx_Math_Fract16 x, Ifx_Math_Fract16 y, sint8 n)
{
    /* Stores the return value */
    Ifx_Math_Fract16 divReturn;

    /* Protect against division by 0 */
    if (y == 0)
    {
        if (x >= 0)
        {
            divReturn = IFX_MATH_FRACT16_MAX;
        }
        else
        {
            divReturn = IFX_MATH_FRACT16_MIN;
        }
    }
    else
    {
        divReturn = (Ifx_Math_Fract16)(Ifx_Math_ShLR_F32((Ifx_Math_Fract32)x, n) / (Ifx_Math_Fract32)y);
    }

    return divReturn;
}


/**
 *  \brief Calculates the 64-bit fractional quotient of two 32-bit fractional inputs.
 *
 *  This functions calculates the 64-bit fractional quotient of the two 32-bit fractional inputs, x and y, where x is
 * the numerator and y the denominator, shifted by n without saturation and overflow detection. The division is
 * protected from division by zero by returning IFX_MATH_FRACT32_MAX if x &gt;= 0 and IFX_MATH_FRACT32_MIN if x &lt; 0.
 *
 *  \param [in] x 32-bit fractional number with the format Qa
 *  \param [in] y 32-bit fractional number with the format Qb
 *  \param [in] n Number of bits to shift the division result, between -16 and 16. Positive n indicates left shift,
 * negative n indicates right shift
 *
 *  \return 64-bit fractional number with the format Q(a-b+n)
 */
static inline Ifx_Math_Fract64 Ifx_Math_DivShLR_p_F64_F32F32(Ifx_Math_Fract32 x, Ifx_Math_Fract32 y, sint8 n)
{
    /* Stores the return value */
    Ifx_Math_Fract64 divReturn;

    /* Protect against division by 0 */
    if (y == 0)
    {
        if (x >= 0)
        {
            divReturn = (Ifx_Math_Fract64)IFX_MATH_FRACT32_MAX;
        }
        else
        {
            divReturn = (Ifx_Math_Fract64)IFX_MATH_FRACT32_MIN;
        }
    }
    else
    {
        if (n >= 0)
        {
            /* polyspace +2 MISRA2012:10.1 [Justified:Low] "Bitwise operators on signed values are required by fixed
             * point arithmetic." */
            divReturn = ((Ifx_Math_Fract64)x << n) / (Ifx_Math_Fract64)y;
        }
        else
        {
            /* polyspace +2 MISRA2012:10.1 [Justified:Low] "Bitwise operators on signed values are required by fixed
             * point arithmetic." */
            divReturn = ((Ifx_Math_Fract64)x >> -n) / (Ifx_Math_Fract64)y;
        }
    }

    return divReturn;
}


/**
 *  \brief Calculates the 32-bit shifted fractional quotient of two 32-bit fractional inputs.
 *
 *  This functions calculates the 32-bit fractional quotient of the two 32-bit fractional inputs x and y, where x is the
 * numerator and y the denominator, shifted by n without saturation and overflow detection. The division is protected
 * from division by zero by returning IFX_MATH_FRACT32_MAX if x &gt;= 0 and IFX_MATH_FRACT32_MIN if x &lt; 0.
 *
 *  \param [in] x 32-bit fractional number with the format Qa
 *  \param [in] y 32-bit fractional number with the format Qb
 *  \param [in] n Number of bits to shift the division result, between -16 and 16. Positive n indicates left shift,
 * negative n indicates right shift
 *
 *  \return 32-bit fractional number with the format Q(a-b+n)
 */
static inline Ifx_Math_Fract32 Ifx_Math_DivShLR_F32(Ifx_Math_Fract32 x, Ifx_Math_Fract32 y, sint8 n)
{
    return (Ifx_Math_Fract32)Ifx_Math_DivShLR_p_F64_F32F32(x, y, n);
}


#endif /*IFX_MATH_DIVSHLR_H*/
