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
 * \file Ifx_Math_Atan2.h
 * \brief Provides a function to calculate the four quadrant arctangent of two values.
 */

#ifndef IFX_MATH_ATAN2_H
#define IFX_MATH_ATAN2_H
#include "Ifx_Math.h"

/**
 *  \brief Calculate the four quadrant arctangent of x and y.
 *
 *  The function returns a value in the range [0, 2^32], where 0 represents 0 and 2^32 represents \f$2*\pi\f$. The
 * return value is 0 if both inputs are 0.
 *  The resolution of the output can be modified by configuring the size of the lookup table. The size of the lookup
 * table is given by IFX_MATH_USRCFG_ATAN_LUT_SIZE. The valid configuration set is: IFX_MATH_USRCFG_ATAN_LUT_SIZE =
 * {IFX_MATH_USROPT_ATAN_LUT_SIZE_8; IFX_MATH_USROPT_ATAN_LUT_SIZE_10; IFX_MATH_USROPT_ATAN_LUT_SIZE_12}.
 *
 *  \param [in] y 16-bit fractional number with the format Q15
 *  \param [in] x 16-bit fractional number with the format Q15
 *
 *  \return 32-bit input angle, normalized by  \f$2*\pi\f$
 */
uint32 Ifx_Math_Atan2_F16(Ifx_Math_Fract16 y, Ifx_Math_Fract16 x);

#endif /*IFX_MATH_ATAN2_H*/
