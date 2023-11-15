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
 * \file Ifx_Math_NegSat.h
 * \brief Provides functions which return the saturated negative value of a fractional number.
 * The functions in this module return the negative value of the fractional number x. The result is limited between the
 * minimum and the maximum of the return type of the corresponding function.
 */

#ifndef IFX_MATH_NEGSAT_H
#define IFX_MATH_NEGSAT_H
#include "Ifx_Math.h"
#include "Ifx_Math_Neg.h"
#include "Ifx_Math_Sat.h"

/**
 *  \brief Returns the 16-bit negative value of the 16-bit input, limited between FRACT16_MIN and FRACT16_MAX.
 *
 *
 *  \param [in] x 16-bit signed fractional number
 *
 *  \return 16-bit  saturated negative value of the input
 */
static inline Ifx_Math_Fract16 Ifx_Math_NegSat_F16(Ifx_Math_Fract16 x)
{
    return Ifx_Math_Sat_F16_F32(Ifx_Math_Neg_F32(x));
}


/**
 *  \brief Returns the 32-bit negative value of the 32-bit input limited between FRACT32_MIN and FRACT32_MAX.
 *
 *
 *  \param [in] x 32-bit signed fractional number
 *
 *  \return 32-bit saturated negative value of the input
 */
static inline Ifx_Math_Fract32 Ifx_Math_NegSat_F32(Ifx_Math_Fract32 x)
{
    return Ifx_Math_Sat_F32_F64(-(Ifx_Math_Fract64)(x));
}


#endif /*IFX_MATH_NEGSAT_H*/
