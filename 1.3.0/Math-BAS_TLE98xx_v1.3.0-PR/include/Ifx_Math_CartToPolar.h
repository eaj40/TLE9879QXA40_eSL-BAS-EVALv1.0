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
 * \file Ifx_Math_CartToPolar.h
 * \brief Provides a function which performs the Cartesian to Polar coordinate transformation.
 */

#ifndef IFX_MATH_CARTTOPOLAR_H
#define IFX_MATH_CARTTOPOLAR_H
#include "Ifx_Math.h"

/**
 * The array representing a 1-D lookup table where the index represent the converted values of the square of the
 * amplitude and the array data values represent the corresponding square root of the amplitude which is in between 0
 * and 1. The amplitude is represented in Q15 format.
 */
extern const Ifx_Math_Fract16 Ifx_Math_Lut_CartToPolar_F16_table[];

/**
 *  \brief Calculates the 16-bit Cartesian to Polar transformation.
 *
 *  This function is calculates the co-ordinates from Cartesian (Real, Imaginary) system to Polar (Amplitude, Angle)
 * system. The angle is calculated using the Ifx_Math_Atan2 function. The amplitude is calculated based on the formula:
 *
 *  \f[\begin{equation} Amplitude = \sqrt{(real*real)+(imag*imag)}\end{equation}\f]
 *
 *  The resolution of the Amplitude table which ranges from [0,1] can be modified by configuring the size of the lookup
 * table. The size of the lookup table is given by IFX_MATH_CFG_CART2POLAR_LUT_SIZE. The valid configuration is set as:
 * IFX_MATH_CFG_CART2POLAR_LUT_SIZE = {IFX_MATH_USROPT_CART2POLAR_LUT_SIZE_8; IFX_MATH_USROPT_CART2POLAR_LUT_SIZE_10;
 * IFX_MATH_USROPT_CART2POLAR_LUT_SIZE_12}.
 *
 *  \param [in] cartesian Data Container of the cartesian co-ordinates {real,imag}
 *
 *  \return Data Container of the Polar quantities: amplitude and angle
 */
Ifx_Math_PolarFract16 Ifx_Math_CartToPolar_F16(Ifx_Math_CmpFract16 cartesian);

#endif /*IFX_MATH_CARTTOPOLAR_H*/
