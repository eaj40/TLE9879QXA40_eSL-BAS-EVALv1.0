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
 * \file Ifx_Math_MulShRAdd.h
 * \brief Provides functions which calculate the fractional product of two fractional numbers, shift the result, and add
 * it to a third fractional number.
 * The functions in this module calculate the fractional product of two fractional numbers y and z, right shifted by n,
 * and added to the fractional number x, without saturation and overflow detection.
 */

#ifndef IFX_MATH_MULSHRADD_H
#define IFX_MATH_MULSHRADD_H
#include "Ifx_Math.h"
#include "Ifx_Math_Add.h"
#include "Ifx_Math_ShR.h"

/**
 *  \brief Calculate the 16-bit fractional product of two fractional numbers, y and z, right shifted by n, and add to
 * the 16-bit fractional number x, without saturation and overflow detection.
 *
 *
 *  \param [in] x 16-bit fractional number with the format Q(a+b-n)
 *  \param [in] y 16-bit fractional number with the format Qa
 *  \param [in] z 16-bit fractional number with the format Qb
 *  \param [in] n Number of bits to right shift the multiplication result, between 0 and 30
 *
 *  \return 16-bit fractional number with the format Q(a+b-n)
 */
static inline Ifx_Math_Fract16 Ifx_Math_MulShRAdd_F16(Ifx_Math_Fract16 x, Ifx_Math_Fract16 y, Ifx_Math_Fract16 z,
                                                      uint8 n)
{
    return Ifx_Math_Add_F16(x, Ifx_Math_MulShR_F16(y, z, n));
}


/**
 *  \brief Calculate the 32-bit fractional product of two 32-bit fractional numbers y and z, right shifted by n, and add
 * to the 32-bit fractional number x, without saturation and overflow detection.
 *
 *
 *  \param [in] x 32-bit fractional number with the format Q(a+b-n)
 *  \param [in] y 32-bit fractional number with the format Qa
 *  \param [in] z 32-bit fractional number with the format Qb
 *  \param [in] n Number of bits to right shift the multiplication result, between 0 and 62
 *
 *  \return 32-bit fractional number with the format Q(a+b-n)
 */
static inline Ifx_Math_Fract32 Ifx_Math_MulShRAdd_F32(Ifx_Math_Fract32 x, Ifx_Math_Fract32 y, Ifx_Math_Fract32 z,
                                                      uint8 n)
{
    return Ifx_Math_Add_F32(x, Ifx_Math_MulShR_F32(y, z, n));
}


/**
 *  \brief Calculate the 32-bit fractional product of two 16-bit fractional numbers y and z, right shifted by n, and add
 * to the 16-bit fractional number x, without saturation and overflow detection.
 *
 *
 *  \param [in] x 32-bit fractional number with the format Q(a+b-n)
 *  \param [in] y 16-bit fractional number with the format Qa
 *  \param [in] z 16-bit fractional number with the format Qb
 *  \param [in] n Number of bits to right shift the multiplication result, between 0 and 30
 *
 *  \return 32-bit fractional number with the format Q(a+b-n)
 */
static inline Ifx_Math_Fract32 Ifx_Math_MulShRAdd_F32_F32F16F16(Ifx_Math_Fract32 x, Ifx_Math_Fract16 y,
                                                                Ifx_Math_Fract16 z, uint8 n)
{
    return Ifx_Math_Add_F32(x, Ifx_Math_MulShR_F32_F16F16(y, z, n));
}


#endif /*IFX_MATH_MULSHRADD_H*/
