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
 * \file Ifx_Math_AbsSat.h
 * \brief Provides functions to calculate the absolute value of a fractional input  with saturation.
 * The functions in this module calculate the absolute value of the fractional input. The result is limited between the
 * minimum and maximum of the corresponding return type of the function.
 */

#ifndef IFX_MATH_ABSSAT_H
#define IFX_MATH_ABSSAT_H
#include "Ifx_Math.h"
#include "Ifx_Math_Abs.h"
#include "Ifx_Math_Sat.h"

/**
 *  \brief Calculates the 16-bit absolute value of the 16-bit fractional input with saturation.
 *
 *  This function calculates the 16-bit absolute value of the 16-bit fractional number x. The result is limited between
 * IFX_MATH_FRACT16_MIN and IFX_MATH_FRACT16_MAX and returned.
 *
 *  \param [in] x 16-bit signed fractional number
 *
 *  \return 16-bit saturated absolute value of the input parameter
 */
static inline Ifx_Math_Fract16 Ifx_Math_AbsSat_F16(Ifx_Math_Fract16 x)
{
    return Ifx_Math_Sat_F16_F32(Ifx_Math_Abs_F32(x));
}


/**
 *  \brief Calculates the 32-bit absolute value of the 32-bit fractional input.
 *
 *  This function calculate the 32-bit absolute value of the 32-bit fractional number x. The result is limited between
 * IFX_MATH_FRACT32_MIN and IFX_MATH_FRACT32_MAX and returned.
 *
 *  \param [in] x 32-bit signed fractional number
 *
 *  \return 32-bit saturated absolute value of the input parameter
 */
static inline Ifx_Math_Fract32 Ifx_Math_AbsSat_F32(Ifx_Math_Fract32 x)
{
    return Ifx_Math_Sat_F32_F64((x < (Ifx_Math_Fract32)(0)) ? -(Ifx_Math_Fract64)(x) : (Ifx_Math_Fract64)(x));
}


#endif /*IFX_MATH_ABSSAT_H*/
