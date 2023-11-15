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
 * \file Ifx_Math_AddSat.h
 * \brief Provides functions to calculate the saturated fractional sum of two fractional numbers.
 * The functions in this module calculate the fractional sum of two fractional numbers. The result is limited between
 * the minimum and maximum of the corresponding return type of the function.
 */

#ifndef IFX_MATH_ADDSAT_H
#define IFX_MATH_ADDSAT_H
#include "Ifx_Math.h"
#include "Ifx_Math_Add.h"
#include "Ifx_Math_Sat.h"

/**
 *  \brief Calculates the 16-bit fractional sum of two 16-bit fractional numbers, with saturation.
 *
 *  This function calculates the 16-bit fractional sum of two 16-bit fractional numbers, x and y. The result is limited
 * between IFX_MATH_FRACT16_MIN and IFX_MATH_FRACT16_MAX.
 *
 *  \param [in] x The first operand of the addition
 *  \param [in] y The second operand of the addition
 *
 *  \return 16-bit saturated sum of the two input parameters
 */

/* polyspace-begin CODE-METRIC:CALLING [Justified:Low] "Common math library functions are expected to be called multiple
 * times." */
static inline Ifx_Math_Fract16 Ifx_Math_AddSat_F16(Ifx_Math_Fract16 x, Ifx_Math_Fract16 y)
{
    return Ifx_Math_Sat_F16_F32(Ifx_Math_Add_F32(x, y));
}


/* polyspace-end CODE-METRIC:CALLING [Justified:Low] "Common math library functions are expected to be called multiple
 * times." */

/**
 *  \brief Calculates the 32-bit fractional sum of two 32-bit fractional numbers, with saturation.
 *
 *  This function calculates the 32-bit fractional sum of two 32-bit fractional numbers, x and y. The result is limited
 * between IFX_MATH_FRACT32_MIN and IFX_MATH_FRACT32_MAX.
 *
 *  \param [in] x The first operand of the addition
 *  \param [in] y The second operand of the addition
 *
 *  \return 32-bit saturated sum of the two input parameters
 */
static inline Ifx_Math_Fract32 Ifx_Math_AddSat_F32(Ifx_Math_Fract32 x, Ifx_Math_Fract32 y)
{
    return Ifx_Math_Sat_F32_F64((Ifx_Math_Fract64)x + (Ifx_Math_Fract64)y);
}


#endif /*IFX_MATH_ADDSAT_H*/
