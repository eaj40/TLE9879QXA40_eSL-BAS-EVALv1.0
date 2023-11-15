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
 * \file Ifx_Math_Sin.h
 * \brief Provides a function to calculate the sinus of an input value.
 */

#ifndef IFX_MATH_SIN_H
#define IFX_MATH_SIN_H
#include "Ifx_Math.h"

/**
 * Converts 32-bit angle format to lookup table index
 */
#define IFX_MATH_SIN_ANGLE_TO_INDEX (16 - IFX_MATH_CFG_SIN_LUT_SIZE)

/**
 * Lookup table between \f$sin(0)\f$ and \f$sin(2*\pi)\f$ in Q15
 */
extern const Ifx_Math_Fract16 Ifx_Math_Lut_Sincos_F16_table[];

/**
 *  \brief Calculate the sinus value of the 32-bit input angle.
 *
 *  This function calculates the sinus value of a 32-bit angle value, in the range \f$[0, 2*\pi]\f$, normalized
 * by\f$2*\pi\f$. The output resolution is configurable by controlling the sine lookup table size. The size of the
 * lookup table is given by IFX_MATH_CFG_SIN_LUT_SIZE. The valid configuration set is: IFX_MATH_CFG_SIN_LUT_SIZE =
 * {IFX_MATH_USROPT_SIN_LUT_SIZE_8; IFX_MATH_USROPT_SIN_LUT_SIZE_10; IFX_MATH_USROPT_SIN_LUT_SIZE_12}.
 *
 *  \param [in] x 32-bit angle value, in the range \f$[0, 2*\pi]\f$, normalized by\f$2*\pi\f$
 *
 *  \return 16-bit sinus value in Q15 format
 */
static inline Ifx_Math_Fract16 Ifx_Math_Sin_F16(uint32 x)
{
    /* Stores the table index */
    uint16 tableIndex;

    /* Stores the 16-bit angle value */
    uint16 xQ16 = (uint16)(x >> 16);

    /* Addition of 0.5 in the input value and rounding off to nearest integer */
    uint32 xRounded = (uint32)xQ16 + (1UL << (IFX_MATH_SIN_ANGLE_TO_INDEX - 1U));

    /* Converts the rounded angle to table index */
    tableIndex = (uint16)(xRounded >> IFX_MATH_SIN_ANGLE_TO_INDEX);

    /* Get the table result */
    return Ifx_Math_Lut_Sincos_F16_table[tableIndex];
}


#endif /*IFX_MATH_SIN_H*/
