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
 * \file Ifx_Math_PolarToCart.h
 * \brief Provided function to perform the polar to cartesian transformation.
 */

#ifndef IFX_MATH_POLARTOCART_H
#define IFX_MATH_POLARTOCART_H
#include "Ifx_Math.h"

/**
 *  \brief Calculates the 16-bit polar to cartesian transformation.
 *
 *  The polar to cartesian transformation function is calculating the co-ordinates from polar (amplitude, angle) system
 * to cartesian (real, imag) system.
 *  The output resolution is configurable by controlling the sine lookup table size. The size of the lookup table is
 * given by IFX_MATH_CFG_SIN_LUT_SIZE.
 *  The valid configuration set is: IFX_MATH_CFG_SIN_LUT_SIZE =
 * {IFX_MATH_USROPT_SIN_LUT_SIZE_8;IFX_MATH_USROPT_SIN_LUT_SIZE_10; IFX_MATH_USROPT_SIN_LUT_SIZE_12}.
 *
 *  \param [in] polar Data container of the polar coordinates: amplitude and angle
 *
 *  \return Data Container of the cartesian quantities: real and imaginary
 */
Ifx_Math_CmpFract16 Ifx_Math_PolarToCart_F16(Ifx_Math_PolarFract16 polar);

#endif /*IFX_MATH_POLARTOCART_H*/
