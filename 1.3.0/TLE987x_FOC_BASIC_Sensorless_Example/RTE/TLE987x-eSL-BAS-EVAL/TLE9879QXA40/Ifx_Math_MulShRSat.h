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
 * \file Ifx_Math_MulShRSat.h
 * \brief Provides functions to calculate the fractional product of two fractional numbers, x and y, shifted by n, and
 * saturated.
 * The functions in this module calculate the fractional product of two fractional numbers, x and y, right shifted by n,
 * and saturated between the minimum and the maximum of the return type of the function.
 */

#ifndef IFX_MATH_MULSHRSAT_H
#define IFX_MATH_MULSHRSAT_H
#include "Ifx_Math.h"
#include "Ifx_Math_Sat.h"
#include "Ifx_Math_ShR.h"

/**
 *  \brief Calculates the 16-bit fractional product of two 16-bit fractional numbers, x and y, right shifted by n,
 * saturated between IFX_MATH_FRACT16_MIN and IFX_MATH_FRACT16_MAX.
 *
 *
 *  \param [in] x 16-bit fractional number with the format Qa
 *  \param [in] y 16-bit fractional number with the format Qb
 *  \param [in] n Number of bits to right shift the multiplication result, between 0 and 30
 *
 *  \return 16-bit fractional number with the format Q(a+b-n)
 */
static inline Ifx_Math_Fract16 Ifx_Math_MulShRSat_F16(Ifx_Math_Fract16 x, Ifx_Math_Fract16 y, uint8 n)
{
    return Ifx_Math_Sat_F16_F32(Ifx_Math_ShR_F32(((Ifx_Math_Fract32)x * (Ifx_Math_Fract32)y), n));
}


/**
 *  \brief Calculates the 32-bit fractional product of two 32-bit fractional numbers, x and y, right shifted by n,
 * saturated between IFX_MATH_FRACT32_MIN and IFX_MATH_FRACT32_MAX.
 *
 *
 *  \param [in] x 32-bit fractional number with the format Qa
 *  \param [in] y 32-bit fractional number with the format Qb
 *  \param [in] n Number of bits to right shift the multiplication result, between 0 and 62
 *
 *  \return 32-bit fractional number with the format Q(a+b-n)
 */
static inline Ifx_Math_Fract32 Ifx_Math_MulShRSat_F32(Ifx_Math_Fract32 x, Ifx_Math_Fract32 y, uint8 n)
{
    /* Stores the temporary result of the multiplication */
    Ifx_Math_Fract64 tmpMul;

    /* Stores the return value */
    Ifx_Math_Fract32 mulReturn;
    tmpMul = (Ifx_Math_Fract64)x * (Ifx_Math_Fract64)y;

    /* polyspace +2 MISRA2012:10.1 [Justified:Low] "Bitwise operators on signed values are required by fixed point
     * arithmetic." */
    mulReturn = Ifx_Math_Sat_F32_F64(tmpMul >> n);

    return mulReturn;
}


#endif /*IFX_MATH_MULSHRSAT_H*/
