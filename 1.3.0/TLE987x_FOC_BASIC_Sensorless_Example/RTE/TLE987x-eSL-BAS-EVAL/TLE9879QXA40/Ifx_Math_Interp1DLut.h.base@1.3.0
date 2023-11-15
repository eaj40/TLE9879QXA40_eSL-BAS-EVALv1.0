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
 * \file Ifx_Math_Interp1DLut.h
 * \brief Provides a function to perform linear interpolation in a 1-D look up table.
 */

#ifndef IFX_MATH_INTERP1DLUT_H
#define IFX_MATH_INTERP1DLUT_H
#include "Ifx_Math.h"

/**
 * Structure containing the 1 dimensional lookup table and its size
 */
typedef struct Ifx_Math_Interp1DLut_Type
{
    /**
     * Pointer to the array containing the data points on the ordinate of the 1-D lookup table
     */
    const Ifx_Math_Fract16* data;

    /**
     * Number of elements of the table
     */
    uint16 size;
} Ifx_Math_Interp1DLut_Type;

/**
 *  \brief Performs 16-bit interpolation on 1-D lookup table
 *
 *  This function returns the interpolated values of a 1-D function at specific query points defined by a 1-D lookup
 * table using linear interpolation.
 *  The linear interpolation is calculated according to the following equation:
 *
 *  \f[\begin{equation} y = y_1 + \frac{y_2 - y_1}{x_2 - x_1}\cdot (x-x_1)  \end{equation}\f]
 *
 *  where:
 *
 *  \f$ y \f$ is the interpolated value, returned by the function
 *  \f$y_1\f$ and \f$y_2\f$ are the query points on the ordinate, given by the values of the 1-D loop up table array
 *  \f$x_1\f$ and \f$x_2\f$ are the related points on the abscissa, given by the index of the 1-D look up table
 * multiplied by a defined coefficient
 *  \f$ x \f$ is the abscissa value input to the function given by the user
 *
 *  If the value of the abscissa is bigger than the table size, the last value of the table is returned.
 *
 *  \param [in] x Abscissa value, between 0 and IFX_MATH_FRACT16_MAX
 *  \param [in] table Structure containing the lookup table to interpolate
 *  \param [in] shift Coefficient defining the ratio between the resolution of the input and the resolution of the
 * abscissa of the 1-D lookup table, between 0 and 14
 *
 *  \return 16-bit interpolated ordinate value
 */
Ifx_Math_Fract16 Ifx_Math_Interp1DLut_F16(Ifx_Math_Fract16 x, Ifx_Math_Interp1DLut_Type table, uint8 shift);

#endif /*IFX_MATH_INTERP1DLUT_H*/
