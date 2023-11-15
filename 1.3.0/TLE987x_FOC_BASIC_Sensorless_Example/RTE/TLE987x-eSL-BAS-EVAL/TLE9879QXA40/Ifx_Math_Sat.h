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
 * \file Ifx_Math_Sat.h
 * \brief Provides functions which return the value of the input limited between the minimum and the maximum of the
 * return type of the corresponding function.
 */

#ifndef IFX_MATH_SAT_H
#define IFX_MATH_SAT_H
#include "Ifx_Math.h"
#include "arm_math.h"

/**
 *  \brief Return the value of the 32-bit fractional input limited between IFX_MATH_FRACT32_MIN and IFX_MATH_FRACT32_MAX
 *
 *
 *  \param [in] x 64-bit signed fractional number
 *
 *  \return 32-bit saturated value of the input
 */

/* polyspace-begin CODE-METRIC:CALLING [Justified:Low] "Common math library functions are expected to be called multiple
 * times." */
static inline Ifx_Math_Fract32 Ifx_Math_Sat_F32_F64(Ifx_Math_Fract64 x)
{
    return (Ifx_Math_Fract32)clip_q63_to_q31(x);
}


/* polyspace-end CODE-METRIC:CALLING [Justified:Low] "Common math library functions are expected to be called multiple
 * times." */

/**
 *  \brief Return the value of the 16-bit fractional input x limited between IFX_MATH_FRACT16_MIN and
 * IFX_MATH_FRACT16_MAX
 * \return 16-bit saturated value of the input
 *
 *
 *  \param [in] x 32-bit signed fractional number
 *
 *  \return
 */

/* polyspace-begin CODE-METRIC:CALLING [Justified:Low] "Common math library functions are expected to be called multiple
 * times." */
static inline Ifx_Math_Fract16 Ifx_Math_Sat_F16_F32(Ifx_Math_Fract32 x)
{
    /* polyspace +2 MISRA2012:9.1 [Justified:Low] "Caused by assembly instruction in the function __SSAT provided by
     * CMSIS package, using ARM GCC." */
    return (Ifx_Math_Fract16)(__SSAT(x, 16));
}


/* polyspace-end CODE-METRIC:CALLING [Justified:Low] "Common math library functions are expected to be called multiple
 * times." */

#endif /*IFX_MATH_SAT_H*/
