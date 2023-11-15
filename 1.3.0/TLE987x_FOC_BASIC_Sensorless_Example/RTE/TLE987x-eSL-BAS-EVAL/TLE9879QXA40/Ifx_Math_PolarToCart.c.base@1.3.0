/*
 * Copyright (c) 2022 Infineon Technologies AG. All Rights Reserved.
 *
 * Use of this file is subject to the terms of use agreed between (i) you or the company in which ordinary course of
 * business you are acting and (ii) Infineon Technologies AG, its affiliates or its licensees. If and as long as no
 * such terms of use are agreed, use of this file is subject to the Evaluation Software License Agreement distributed
 * along with this file within the software delivery package.
 *
 */

#include "Ifx_Math_PolarToCart.h"
#include "Ifx_Math_Mul.h"
#include "Ifx_Math_Sin.h"
#include "Ifx_Math_Cos.h"

/* \brief Calculates the 16-bit polar to cartesian transformation. */
Ifx_Math_CmpFract16 Ifx_Math_PolarToCart_F16(Ifx_Math_PolarFract16 polar)
{
    /* Variable declaration of the result = real +j*imag */
    Ifx_Math_CmpFract16 result;

    /* real = Amp*cos(theta) */
    result.real = Ifx_Math_Mul_F16(polar.amplitude, Ifx_Math_Cos_F16(polar.angle));

    /* imag = Amp*sin(theta) */
    result.imag = Ifx_Math_Mul_F16(polar.amplitude, Ifx_Math_Sin_F16(polar.angle));

    return result;
}
