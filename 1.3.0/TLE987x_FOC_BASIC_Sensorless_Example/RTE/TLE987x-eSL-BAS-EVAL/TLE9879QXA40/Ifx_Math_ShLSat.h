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
 * \file Ifx_Math_ShLSat.h
 * \brief Provides functions which shift the fractional input x left by n bits and return the result limited between the
 * minimum and maximum of the return type of the corresponding function.
 */

#ifndef IFX_MATH_SHLSAT_H
#define IFX_MATH_SHLSAT_H
#include "Ifx_Math.h"
#include "Ifx_Math_Sat.h"

/**
 *  \brief Return the result of the 16-bit input parameter shifted left by n bits limited between IFX_MATH_FRACT16_MIN
 * and IFX_MATH_FRACT16_MAX.
 *
 *
 *  \param [in] x 16-bit fractional number with the format Qa
 *  \param [in] n Number of bits to shift the input left, between 0 and 15
 *
 *  \return 16-bit result of the operation
 */
static inline Ifx_Math_Fract16 Ifx_Math_ShLSat_F16(Ifx_Math_Fract16 x, uint8 n)
{
    /* polyspace +2 MISRA2012:10.1 [Justified:Low] "Bitwise operators on signed values are required by fixed point
     * arithmetic." */
    return Ifx_Math_Sat_F16_F32(((Ifx_Math_Fract32)x << n));
}


/**
 *  \brief Return the result of the 32-bit input parameter shifted left by n bits limited between IFX_MATH_FRACT32_MIN
 * and IFX_MATH_FRACT32_MAX.
 *
 *
 *  \param [in] x 32-bit fractional number with the format Qa
 *  \param [in] n Number of bits to shift the input left, between 0 and 31
 *
 *  \return 32-bit result of the operation
 */
static inline Ifx_Math_Fract32 Ifx_Math_ShLSat_F32(Ifx_Math_Fract32 x, uint8 n)
{
    /* polyspace +2 MISRA2012:10.1 [Justified:Low] "Bitwise operators on signed values are required by fixed point
     * arithmetic." */
    return Ifx_Math_Sat_F32_F64(((Ifx_Math_Fract64)x << n));
}


#endif /*IFX_MATH_SHLSAT_H*/
