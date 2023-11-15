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
 * \file Ifx_Math_Neg.h
 * \brief Provides functions which return the negative value of the fractional number x without saturation and any
 * overflow detection.
 */

#ifndef IFX_MATH_NEG_H
#define IFX_MATH_NEG_H
#include "Ifx_Math.h"

/**
 *  \brief Returns the 16-bit negative value of the 16-bit fractional number x without saturation and any overflow
 * detection.
 *
 *
 *  \param [in] x 16-bit signed fractional number
 *
 *  \return 16-bit negative value of the input
 */
static inline Ifx_Math_Fract16 Ifx_Math_Neg_F16(Ifx_Math_Fract16 x)
{
    return (Ifx_Math_Fract16)(-x);
}


/**
 *  \brief Returns the 32-bit negative value of the 32-bit fractional number x without saturation and any overflow
 * detection.
 *
 *
 *  \param [in] x 32-bit signed fractional number
 *
 *  \return 32-bit negative value of the input
 */
static inline Ifx_Math_Fract32 Ifx_Math_Neg_F32(Ifx_Math_Fract32 x)
{
    return (Ifx_Math_Fract32)(-x);
}


#endif /*IFX_MATH_NEG_H*/
