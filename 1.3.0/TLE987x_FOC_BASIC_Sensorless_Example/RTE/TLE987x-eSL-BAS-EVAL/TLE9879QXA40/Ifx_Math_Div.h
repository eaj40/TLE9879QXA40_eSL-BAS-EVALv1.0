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
 * \file Ifx_Math_Div.h
 * \brief Provides functions to calculate the fractional quotient of two fractional inputs.
 * The functions in this module calculate the fractional quotient of the two input parameters, x and y, where x is the
 * numerator and y the denominator, shifted left by 15 or 31 without saturation and overflow detection. The division is
 * protected from division by zero by returning the maximum of the return type of the function if the numerator is
 * greater or equal to zero and the minimum if the numerator is less than zero.
 */

#ifndef IFX_MATH_DIV_H
#define IFX_MATH_DIV_H
#include "Ifx_Math.h"
#include "Ifx_Math_DivShL.h"

/**
 *  \brief Calculates the 16-bit fractional quotient of two 16-bit fractional inputs.
 *
 *  This function calculates the 16-bit fractional quotient of the two 16-bit fractional inputs, x and y, where x is the
 * numerator and y the denominator, shifted left by 15 without saturation and overflow detection. The division is
 * protected from division by zero by returning IFX_MATH_FRACT16_MAX if x &gt;= 0 and IFX_MATH_FRACT16_MIN if x &lt; 0.
 *
 *  \param [in] x 16-bit fractional number with the format Qa
 *  \param [in] y 16-bit fractional number with the format Qb
 *
 *  \return 16-bit fractional number with the format Q(a-b+15)
 */
static inline Ifx_Math_Fract16 Ifx_Math_Div_F16(Ifx_Math_Fract16 x, Ifx_Math_Fract16 y)
{
    return Ifx_Math_DivShL_F16(x, y, 15u);
}


/**
 *  \brief Calculates the 32-bit fractional quotient of two 32-bit fractional inputs.
 *
 *  This function calculates the 32-bit fractional quotient of the two 32-bit fractional inputs, x and y, where x is the
 * numerator and y the denominator, shifted left by 31 without saturation and overflow detection. The division is
 * protected from division by zero by returning IFX_MATH_FRACT32_MAX if x &gt;= 0 and IFX_MATH_FRACT32_MIN if x &lt; 0.
 *
 *  \param [in] x 32-bit fractional number with the format Qa
 *  \param [in] y 32-bit fractional number with the format Qb
 *
 *  \return 32-bit fractional number with the format Q(a-b+31)
 */
static inline Ifx_Math_Fract32 Ifx_Math_Div_F32(Ifx_Math_Fract32 x, Ifx_Math_Fract32 y)
{
    return (Ifx_Math_Fract32)Ifx_Math_DivShL_p_F64_F32F32(x, y, 31u);
}


#endif /*IFX_MATH_DIV_H*/
