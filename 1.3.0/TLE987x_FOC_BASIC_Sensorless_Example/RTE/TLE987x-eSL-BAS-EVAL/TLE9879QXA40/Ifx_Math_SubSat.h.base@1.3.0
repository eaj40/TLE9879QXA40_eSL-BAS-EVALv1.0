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
 * \file Ifx_Math_SubSat.h
 * \brief Provides functions to calculate the saturated fractional difference of two fractional numbers.
 * The functions in this module return the fractional difference of two fractional numbers, x and y. The result is
 * limited between the minimum and the maximum of the return type of the corresponding function.
 */

#ifndef IFX_MATH_SUBSAT_H
#define IFX_MATH_SUBSAT_H
#include "Ifx_Math.h"
#include "Ifx_Math_Sat.h"
#include "Ifx_Math_Sub.h"

/**
 *  \brief Return the 16-bit saturated fractional difference of two 16-bit fractional numbers.
 *
 *  Return the 16-bit fractional difference of two 16-bit fractional numbers, x and y. The result is limited between
 * IFX_MATH_FRACT16_MIN and IFX_MATH_FRACT16_MAX.
 *
 *  \param [in] x The first operand of the subtraction
 *  \param [in] y The second operand of the subtraction
 *
 *  \return 16-bit saturated difference of the two input parameters
 */

/* polyspace-begin CODE-METRIC:CALLING [Justified:Low] "Common math library functions are expected to be called multiple
 * times." */
static inline Ifx_Math_Fract16 Ifx_Math_SubSat_F16(Ifx_Math_Fract16 x, Ifx_Math_Fract16 y)
{
    return Ifx_Math_Sat_F16_F32(Ifx_Math_Sub_F32(x, y));
}


/* polyspace-end CODE-METRIC:CALLING [Justified:Low] "Common math library functions are expected to be called multiple
 * times." */

/**
 *  \brief Return the 32-bit saturated fractional difference of two 32-bit fractional numbers.
 *
 *  Return the 32-bit fractional difference of two 32-bit fractional numbers, x and y. The result is limited between
 * IFX_MATH_FRACT32_MIN and IFX_MATH_FRACT32_MAX.
 *
 *  \param [in] x The first operand of the subtraction
 *  \param [in] y The second operand of the subtraction
 *
 *  \return 32-bit saturated difference of the two input parameters
 */
static inline Ifx_Math_Fract32 Ifx_Math_SubSat_F32(Ifx_Math_Fract32 x, Ifx_Math_Fract32 y)
{
    return Ifx_Math_Sat_F32_F64((Ifx_Math_Fract64)x - (Ifx_Math_Fract64)y);
}


#endif /*IFX_MATH_SUBSAT_H*/
