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
 * \file Ifx_Math_Atan.h
 * \brief Provides a function to calculate the arctangent of a value.
 */

#ifndef IFX_MATH_ATAN_H
#define IFX_MATH_ATAN_H
#include "Ifx_Math.h"
#include "Ifx_Math_AbsSat.h"
#include "Ifx_Math_ShRRnd.h"

/**
 * Ratio between the integer representing the value of the abscissa in Q15 format and the lookup table index.
 */
#define IFX_MATH_ATAN_INPUT_TO_INDEX (15 - IFX_MATH_CFG_ATAN_LUT_SIZE)

/**
 * Array representing a 1-D Lookup table with index representing the converted angle values (abscissa) and values
 * representing the corresponding Atan (ordinate) between \f$atan(0)\f$ and \f$atan(1)\f$
 */
extern const uint16 Ifx_Math_Lut_Atan_F16_table[];

/**
 *  \brief Calculates the arctangent of the input.
 *
 *  This function returns a value in the range [0, 2^29] if the input is greater than or equal to zero, where 2^29
 * represents \f$\pi/4\f$. If the input is less than zero the function returns a value in the range [7/8*2^32-1,
 * 2^32-1], where 7/8*2^32-1 represents \f$7*\pi/4\f$. The resolution of the output can be modified by configuring the
 * size of the lookup table. The size of the lookup table is given by IFX_MATH_CFG_ATAN_LUT_SIZE. The valid
 * configuration set is: IFX_MATH_CFG_ATAN_LUT_SIZE = {IFX_MATH_USROPT_ATAN_LUT_SIZE_8;
 * IFX_MATH_USROPT_ATAN_LUT_SIZE_10; IFX_MATH_USROPT_ATAN_LUT_SIZE_12}.
 *
 *  \param [in] x 16-bit fractional number with the format Q15
 *
 *  \return 32-bit angle, normalized by  \f$2*\pi\f$
 */
static inline uint32 Ifx_Math_Atan_F16(Ifx_Math_Fract16 x)
{
    /* Local variable for storing the index of the lookup table */
    Ifx_Math_Fract16 input;

    /* Local variable for storing the return value */
    uint32           atanTemp;

    /* Local variable for storing the table value */
    uint32           tableValue;

    /* Converting input to absolute value */
    input = Ifx_Math_AbsSat_F16(x);

    /* Converting input to table index */
    input = Ifx_Math_ShRRnd_F16(input, IFX_MATH_ATAN_INPUT_TO_INDEX);

    /* Get the value from the table and change the sign accordingly */
    tableValue = ((uint32)Ifx_Math_Lut_Atan_F16_table[input]) << 16;

    if (x < 0)
    {
        atanTemp = (2 * IFX_MATH_PI_INDEX) - tableValue;
    }
    else
    {
        atanTemp = tableValue;
    }

    return atanTemp;
}


#endif /*IFX_MATH_ATAN_H*/
