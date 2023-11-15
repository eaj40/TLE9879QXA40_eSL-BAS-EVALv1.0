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
 * \file Ifx_Math_Abs.h
 * \brief Provides functions to calculate the absolute value of a fractional input  without saturation and any overflow
 * detection.
 */

#ifndef IFX_MATH_ABS_H
#define IFX_MATH_ABS_H
#include "Ifx_Math.h"
#include "Ifx_Math_Neg.h"

/**
 *  \brief Calculates the 16-bit absolute value of a 16-bit fractional input.
 *
 *  This function calculates the 16-bit absolute value of the 16-bit fractional number x. The result is returned without
 * saturation and any overflow detection.
 *
 *  \param [in] x 16-bit signed fractional number
 *
 *  \return 16-bit absolute value of the input parameter
 */
static inline Ifx_Math_Fract16 Ifx_Math_Abs_F16(Ifx_Math_Fract16 x)
{
    return (x < (Ifx_Math_Fract16)(0)) ? Ifx_Math_Neg_F16(x) : x;
}


/**
 *  \brief Calculates the 32-bit absolute value of a 32-bit fractional input.
 *
 *  This function calculates the 32-bit absolute value of the 32-bit fractional number x. The result is returned without
 * saturation and any overflow detection.
 *
 *  \param [in] x 32-bit signed fractional number
 *
 *  \return 32-bit absolute value of the input parameter
 */
static inline Ifx_Math_Fract32 Ifx_Math_Abs_F32(Ifx_Math_Fract32 x)
{
    return (x < (Ifx_Math_Fract32)(0)) ? Ifx_Math_Neg_F32(x) : x;
}


#endif /*IFX_MATH_ABS_H*/
