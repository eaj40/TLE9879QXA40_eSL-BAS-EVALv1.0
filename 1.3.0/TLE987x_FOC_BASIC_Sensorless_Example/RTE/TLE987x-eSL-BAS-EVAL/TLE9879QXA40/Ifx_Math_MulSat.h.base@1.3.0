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
 * \file Ifx_Math_MulSat.h
 * \brief Provides functions to calculate the fractional product of two fractional numbers, x and y, shifted by the
 * corresponding number of bits, and saturated between the minimum and the maximum of the return type of the
 * corresponding function.
 */

#ifndef IFX_MATH_MULSAT_H
#define IFX_MATH_MULSAT_H
#include "Ifx_Math.h"
#include "Ifx_Math_MulShR.h"
#include "Ifx_Math_Sat.h"

/**
 *  \brief Calculates the 16-bit fractional product of two 16-bit fractional numbers x and y, shifted right by 15,
 * saturated between IFX_MATH_FRACT16_MIN and IFX_MATH_FRACT16_MAX.
 *
 *
 *  \param [in] x 16-bit fractional number with the format Qa
 *  \param [in] y 16-bit fractional number with the format Qb
 *
 *  \return 16-bit fractional number with the format Q(a+b-15)
 */
static inline Ifx_Math_Fract16 Ifx_Math_MulSat_F16(Ifx_Math_Fract16 x, Ifx_Math_Fract16 y)
{
    return Ifx_Math_Sat_F16_F32(Ifx_Math_MulShR_F32_F16F16(x, y, 15u));
}


/**
 *  \brief Calculates the 32-bit fractional product of 32-bit fractional numbers, x and y, shifted right by 31,
 * saturated between IFX_MATH_FRACT32_MIN and IFX_MATH_FRACT32_MAX.
 *
 *
 *  \param [in] x 32-bit fractional number with the format Qa
 *  \param [in] y 32-bit fractional number with the format Qb
 *
 *  \return 32-bit fractional number with the format Q(a+b-31)
 */
static inline Ifx_Math_Fract32 Ifx_Math_MulSat_F32(Ifx_Math_Fract32 x, Ifx_Math_Fract32 y)
{
    /* polyspace +2 MISRA2012:10.1 [Justified:Low] "Bitwise operators on signed values are required by fixed point
     * arithmetic." */
    return Ifx_Math_Sat_F32_F64((((Ifx_Math_Fract64)x * (Ifx_Math_Fract64)y) >> 31U));
}


#endif /*IFX_MATH_MULSAT_H*/
