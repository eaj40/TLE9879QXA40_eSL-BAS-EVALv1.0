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
 * \file Ifx_Math_DivShLSatNZ.h
 * \brief Provides functions to calculate the shifted fractional quotient of two fractional inputs, with saturation.
 * The functions in this module calculate the fractional quotient of the two input parameters, x and y, where x is the
 * numerator and y the denominator, shifted by n and saturated between the minimum and maximum of the corresponding
 * return type of the function. The division is not protected from division by zero.
 */

#ifndef IFX_MATH_DIVSHLSATNZ_H
#define IFX_MATH_DIVSHLSATNZ_H
#include "Ifx_Math.h"
#include "Ifx_Math_Sat.h"
#include "Ifx_Math_ShL.h"

/**
 *  \brief Calculates the 16-bit shifted fractional quotient of two 16-bit fractional inputs.
 *
 *  This function calculates the 16-bit fractional quotient of the two 16-bit fractional inputs, x and y, where x is the
 * numerator and y the denominator, shifted by n, saturated between IFX_MATH_FRACT16_MIN and IFX_MATH_FRACT16_MAX. The
 * division is not protected from division by zero.
 *
 *  \param [in] x 16-bit fractional number with the format Qa
 *  \param [in] y 16-bit fractional number with the format Qb
 *  \param [in] n Number of bits to left shift the division result, between 0 and 16
 *
 *  \return 16-bit fractional number with the format Q(a-b+n)
 */
static inline Ifx_Math_Fract16 Ifx_Math_DivShLSatNZ_F16(Ifx_Math_Fract16 x, Ifx_Math_Fract16 y, uint8 n)
{
    return Ifx_Math_Sat_F16_F32((Ifx_Math_ShL_F32((Ifx_Math_Fract32)x, n) / (Ifx_Math_Fract32)y));
}


#endif /*IFX_MATH_DIVSHLSATNZ_H*/
