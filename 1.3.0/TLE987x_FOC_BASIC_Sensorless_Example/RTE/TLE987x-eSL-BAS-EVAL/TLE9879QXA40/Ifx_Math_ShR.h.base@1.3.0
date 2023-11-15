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
 * \file Ifx_Math_ShR.h
 * \brief Provides functions which shift the fractional input x right by n bits and return the result without any
 * saturation or overflow detection.
 */

#ifndef IFX_MATH_SHR_H
#define IFX_MATH_SHR_H
#include "Ifx_Math.h"

/**
 *  \brief Return the result of the 16-bit fractional input parameter shifted right by n bits without saturation or
 * overflow detection.
 *
 *
 *  \param [in] x 16-bit fractional number with the format Qa
 *  \param [in] n Number of bits to shift the input right, between 0 and 15
 *
 *  \return 16-bit shifted value of the input
 */

/* polyspace-begin CODE-METRIC:CALLING [Justified:Low] "Common math library functions are expected to be called multiple
 * times." */
static inline Ifx_Math_Fract16 Ifx_Math_ShR_F16(Ifx_Math_Fract16 x, uint8 n)
{
    /* polyspace +2 MISRA2012:10.1 [Justified:Low] "Bitwise operators on signed values are required by fixed point
     * arithmetic." */
    return (Ifx_Math_Fract16)(x >> n);
}


/* polyspace-end CODE-METRIC:CALLING [Justified:Low] "Common math library functions are expected to be called multiple
 * times." */

/**
 *  \brief Return the result of the 32-bit fractional input parameter shifted right by n bits without saturation or
 * overflow detection.
 *
 *
 *  \param [in] x 32-bit fractional number with the format Qa
 *  \param [in] n Number of bits to shift the input right, between 0 and 31
 *
 *  \return 32-bit shifted value of the input
 */

/* polyspace-begin CODE-METRIC:CALLING [Justified:Low] "Common math library functions are expected to be called multiple
 * times." */
static inline Ifx_Math_Fract32 Ifx_Math_ShR_F32(Ifx_Math_Fract32 x, uint8 n)
{
    /* polyspace +2 MISRA2012:10.1 [Justified:Low] "Bitwise operators on signed values are required by fixed point
     * arithmetic." */
    return (Ifx_Math_Fract32)(x >> n);
}


/* polyspace-end CODE-METRIC:CALLING [Justified:Low] "Common math library functions are expected to be called multiple
 * times." */

#endif /*IFX_MATH_SHR_H*/
