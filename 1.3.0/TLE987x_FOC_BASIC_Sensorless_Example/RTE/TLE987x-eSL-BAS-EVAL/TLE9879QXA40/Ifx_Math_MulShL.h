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
 * \file Ifx_Math_MulShL.h
 * \brief Provides functions that calculate the shifted fractional product of two fractional numbers.
 * The functions in this module calculate the fractional product of two fractional numbers, left shifted by n, without
 * saturation and overflow detection.
 */

#ifndef IFX_MATH_MULSHL_H
#define IFX_MATH_MULSHL_H
#include "Ifx_Math.h"
#include "Ifx_Math_ShL.h"

/**
 *  \brief Calculates the 32-bit shifted fractional product of two 16-bit fractional numbers.
 *
 *  This function calculates the 32-bit fractional product of two 16-bit fractional numbers, x and y, left shifted by n,
 * without saturation and overflow detection.
 *
 *  \param [in] x 16-bit fractional number with the format Qa
 *  \param [in] y 16-bit fractional number with the format Qb
 *  \param [in] n Number of bits to left shift the multiplication result, between 0 and 30
 *
 *  \return 32-bit fractional number with the format Q(a+b+n)
 */
static inline Ifx_Math_Fract32 Ifx_Math_MulShL_F32_F16F16(Ifx_Math_Fract16 x, Ifx_Math_Fract16 y, uint8 n)
{
    return Ifx_Math_ShL_F32(((Ifx_Math_Fract32)x * (Ifx_Math_Fract32)y), n);
}


#endif /*IFX_MATH_MULSHL_H*/
