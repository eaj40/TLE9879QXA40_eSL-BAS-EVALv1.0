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
 * \file Ifx_Math_DivShLRSat.h
 * \brief Provides functions to calculate the shifted fractional quotient of two fractional inputs, with saturation.
 * The functions in this module calculate the fractional quotient of the two input parameters, x and y, where x is the
 * numerator and y the denominator, shifted by n and saturated between the minimum and maximum of the corresponding
 * return type of the function. The result is shifted left if n is greater than zero and right if n is less than zero.
 * The division is protected from division by zero by returning the maximum of the return type of the function if the
 * numerator is greater or equal to zero and the minimum if the numerator is less than zero.
 */

#ifndef IFX_MATH_DIVSHLRSAT_H
#define IFX_MATH_DIVSHLRSAT_H
#include "Ifx_Math.h"
#include "Ifx_Math_DivShLR.h"
#include "Ifx_Math_Sat.h"
#include "Ifx_Math_ShLR.h"

/**
 *  \brief Calculates the 16-bit shifted fractional quotient of two 16-bit fractional inputs.
 *
 *  This function calculates the 16-bit fractional quotient of the two 16-bit fractional inputs, x and y, where x is the
 * numerator and y the denominator, shifted by n, saturated between IFX_MATH_FRACT16_MIN and IFX_MATH_FRACT16_MAX. The
 * division is protected from division by zero by returning IFX_MATH_FRACT16_MAX if x &gt;= 0 and IFX_MATH_FRACT16_MIN
 * if x &lt; 0.
 *
 *  \param [in] x 16-bit fractional number with the format Qa
 *  \param [in] y 16-bit fractional number with the format Qb
 *  \param [in] n Number of bits to shift the division result, between -16 and 16. Positive n indicates left shift,
 * negative n indicates right shift
 *
 *  \return 16-bit fractional number with the format Q(a-b+n)
 */
static inline Ifx_Math_Fract16 Ifx_Math_DivShLRSat_F16(Ifx_Math_Fract16 x, Ifx_Math_Fract16 y, sint8 n)
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
        divReturn = Ifx_Math_Sat_F16_F32((Ifx_Math_ShLR_F32((Ifx_Math_Fract32)x, n) / (Ifx_Math_Fract32)y));
    }

    return divReturn;
}


/**
 *  \brief Calculates the 32-bit shifted fractional quotient of two 32-bit fractional inputs.
 *
 *  This function calculates the 32-bit fractional quotient of the two 32-bit fractional inputs, x and y, where x is the
 * numerator and y the denominator, shifted by n, saturated between IFX_MATH_FRACT32_MIN and IFX_MATH_FRACT32_MAX. The
 * division is protected from division by zero by returning IFX_MATH_FRACT32_MAX if x &gt;= 0 and IFX_MATH_FRACT32_MIN
 * if x &lt; 0.
 *
 *  \param [in] x 32-bit fractional number with the format Qa
 *  \param [in] y 32-bit fractional number with the format Qb
 *  \param [in] n Number of bits to shift the division result, between -32 and 32. Positive n indicates left shift,
 * negative n indicates right shift
 *
 *  \return 32-bit fractional number with the format Q(a-b+n)
 */
static inline Ifx_Math_Fract32 Ifx_Math_DivShLRSat_F32(Ifx_Math_Fract32 x, Ifx_Math_Fract32 y, sint8 n)
{
    return Ifx_Math_Sat_F32_F64(Ifx_Math_DivShLR_p_F64_F32F32(x, y, n));
}


#endif /*IFX_MATH_DIVSHLRSAT_H*/
