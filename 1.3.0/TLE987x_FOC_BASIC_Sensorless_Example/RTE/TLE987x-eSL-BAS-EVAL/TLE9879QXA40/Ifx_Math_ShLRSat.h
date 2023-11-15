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
 * \file Ifx_Math_ShLRSat.h
 * \brief Provides functions which shift the fractional input right or left by n bits.
 * The functions in this module shift the fractional input x right or left by n bits. For a left shift, the result is
 * limited between the minimum and the maximum of the return type of the corresponding function.
 */

#ifndef IFX_MATH_SHLRSAT_H
#define IFX_MATH_SHLRSAT_H
#include "Ifx_Math.h"
#include "Ifx_Math_ShLSat.h"
#include "Ifx_Math_ShR.h"

/**
 *  \brief Returns the result of the 16-bit input parameter shifted by n bits with saturation.
 * This function Returns the result of the 16-bit input parameter shifted by n bits with saturation. For a left shift,
 * the result is limited between IFX_MATH_FRACT16_MIN and IFX_MATH_FRACT16_MAX.
 *
 *
 *  \param [in] x 16-bit fractional number with the format Qa
 *  \param [in] n Number of bits to shift the input, between -15 and 15. Positive n indicates left shift, negative n
 * indicates right shift
 *
 *  \return 16-bit result of the operation
 */
static inline Ifx_Math_Fract16 Ifx_Math_ShLRSat_F16(Ifx_Math_Fract16 x, sint8 n)
{
    /* Return value of the function */
    Ifx_Math_Fract16 shiftReturn;

    /* Negate the shift for shifting right */
    sint8            negativeShift;

    if (n >= 0)
    {
        shiftReturn = Ifx_Math_ShLSat_F16(x, (uint8)(n));
    }
    else
    {
        negativeShift = -n;
        shiftReturn   = Ifx_Math_ShR_F16(x, (uint8)negativeShift);
    }

    return shiftReturn;
}


/**
 *  \brief Returns the result of the 32-bit input parameter shifted by n bits with saturation.
 *
 *  This function returns the result of the 32-bit input parameter shifted by n bits with saturation. For a left shift,
 * the result is limited between IFX_MATH_FRACT32_MIN and IFX_MATH_FRACT32_MAX.
 *
 *  \param [in] x 32-bit fractional number with the format Qa
 *  \param [in] n Number of bits to shift the input, between -31 and 31. Positive n indicates left shift, negative n
 * indicates right shift
 *
 *  \return 32-bit result of the operation
 */
static inline Ifx_Math_Fract32 Ifx_Math_ShLRSat_F32(Ifx_Math_Fract32 x, sint8 n)
{
    /* Return value of the function */
    Ifx_Math_Fract32 shiftReturn;

    /* Negate the shift for shifting right */
    uint8            negativeShift;

    if (n >= 0)
    {
        shiftReturn = Ifx_Math_ShLSat_F32(x, (uint8)(n));
    }
    else
    {
        /* polyspace +1 MISRA-C3:10.8 [Justified:Low] "Shift factor is in between -31 and 31 and will not overflow" */
        negativeShift = (uint8)(-n);
        shiftReturn   = Ifx_Math_ShR_F32(x, negativeShift);
    }

    return shiftReturn;
}


#endif /*IFX_MATH_SHLRSAT_H*/
