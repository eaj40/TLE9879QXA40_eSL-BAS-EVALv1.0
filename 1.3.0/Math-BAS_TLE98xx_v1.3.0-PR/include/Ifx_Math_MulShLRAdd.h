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
 * \file Ifx_Math_MulShLRAdd.h
 * \brief Provides functions which calculate the fractional product of two fractional numbers, shift the result, and add
 * it to a third  fractional number.
 * The functions in this module calculate the fractional product of two fractional numbers y and z, shifted by n, and
 * added to the fractional number x, without saturation and overflow detection. The result is shifted right if n is
 * greater than zero and left if n is less than zero.
 */

#ifndef IFX_MATH_MULSHLRADD_H
#define IFX_MATH_MULSHLRADD_H
#include "Ifx_Math.h"
#include "Ifx_Math_Add.h"
#include "Ifx_Math_MulShLR.h"

/**
 *  \brief Calculate the 16-bit fractional product of two fractional numbers, y and z, shifted by n, and add to the
 * 16-bit fractional number x, without saturation and overflow detection.
 *
 *
 *  \param [in] x 16-bit fractional number with the format Q(a+b-n)
 *  \param [in] y 16-bit fractional number with the format Qa
 *  \param [in] z 16-bit fractional number with the format Qb
 *  \param [in] n Number of bits to shift the multiplication result, between -15 and 30. Positive n indicates right
 * shift, negative n indicates left shift
 *
 *  \return 16-bit fractional number with the format Q(a+b-n)
 */
static inline Ifx_Math_Fract16 Ifx_Math_MulShLRAdd_F16(Ifx_Math_Fract16 x, Ifx_Math_Fract16 y, Ifx_Math_Fract16 z,
                                                       sint8 n)
{
    return Ifx_Math_Add_F16(x, Ifx_Math_MulShLR_F16(y, z, n));
}


/**
 *  \brief Calculate the 32-bit fractional product of two 32-bit fractional numbers y and z, shifted by n, and add to
 * the 32-bit fractional number x, without saturation and overflow detection.
 *
 *
 *  \param [in] x 32-bit fractional number with the format Q(a+b-n)
 *  \param [in] y 32-bit fractional number with the format Qa
 *  \param [in] z 32-bit fractional number with the format Qb
 *  \param [in] n Number of bits to shift the multiplication result, between -31 and 62. Positive n indicates right
 * shift, negative n indicates left shift
 *
 *  \return 32-bit fractional number with the format Q(a+b-n)
 */
static inline Ifx_Math_Fract32 Ifx_Math_MulShLRAdd_F32(Ifx_Math_Fract32 x, Ifx_Math_Fract32 y, Ifx_Math_Fract32 z,
                                                       sint8 n)
{
    return Ifx_Math_Add_F32(x, Ifx_Math_MulShLR_F32(y, z, n));
}


/**
 *  \brief Calculate the 32-bit fractional product of two 16-bit fractional numbers y and z, shifted by n, and add to
 * the 16-bit fractional number x, without saturation and overflow detection.
 *
 *
 *  \param [in] x 32-bit fractional number with the format Q(a+b-n)
 *  \param [in] y 16-bit fractional number with the format Qa
 *  \param [in] z 16-bit fractional number with the format Qb
 *  \param [in] n Number of bits to shift the multiplication result, between -31 and 30. Positive n indicates right
 * shift, negative n indicates left shift
 *
 *  \return 32-bit fractional number with the format Q(a+b-n)
 */
static inline Ifx_Math_Fract32 Ifx_Math_MulShLRAdd_F32_F32F16F16(Ifx_Math_Fract32 x, Ifx_Math_Fract16 y,
                                                                 Ifx_Math_Fract16 z, sint8 n)
{
    return Ifx_Math_Add_F32(x, Ifx_Math_MulShLR_F32_F16F16(y, z, n));
}


#endif /*IFX_MATH_MULSHLRADD_H*/
