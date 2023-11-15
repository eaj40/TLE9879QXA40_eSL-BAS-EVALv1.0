/*
 * Copyright (c) 2022 Infineon Technologies AG. All Rights Reserved.
 *
 * Use of this file is subject to the terms of use agreed between (i) you or the company in which ordinary course of
 * business you are acting and (ii) Infineon Technologies AG, its affiliates or its licensees. If and as long as no
 * such terms of use are agreed, use of this file is subject to the Evaluation Software License Agreement distributed
 * along with this file within the software delivery package.
 *
 */

#include "Ifx_Math_Atan2.h"
#include "Ifx_Math_CartToPolar.h"
#include "Ifx_Math_MulShR.h"
#include "Ifx_Math_AddSat.h"
#include "Ifx_Math_Interp1DLut.h"

/* Shift factor for getting index from Amplitude table*/
#define IFX_MATH_CARTTOPOLAR_SHIFT (15 - IFX_MATH_CFG_CARTTOPOLAR_LUT_SIZE)

/* Link to look up table*/
static const Ifx_Math_Interp1DLut_Type cartToPolarInterp1DLut = {
    .data = &Ifx_Math_Lut_CartToPolar_F16_table[0], .size = (((uint16)1 << IFX_MATH_CFG_CARTTOPOLAR_LUT_SIZE) + 1)
};
Ifx_Math_PolarFract16 Ifx_Math_CartToPolar_F16(Ifx_Math_CmpFract16 cartesian)
{
    /* Local variables to store the real and imaginary */
    Ifx_Math_Fract16      abcissa;
    Ifx_Math_Fract16      ordinate;

    /* Local variables to store the return value and square of amplitude */
    Ifx_Math_PolarFract16 polar;
    Ifx_Math_Fract16      ampsq;

    /* Storing the real and imaginary part of cartesian co-ordinates into local variables */
    abcissa  = cartesian.real;
    ordinate = cartesian.imag;

    /* Get angle from arc tangent */
    polar.angle = Ifx_Math_Atan2_F16(ordinate, abcissa);

    /* Calculation of amplitude*amplitude */
    ampsq = Ifx_Math_AddSat_F16(Ifx_Math_MulShR_F16(abcissa, abcissa, 15u), Ifx_Math_MulShR_F16(ordinate, ordinate,
        15u));

    /* Get amplitude via interpolation between adjoining LUT values*/
    polar.amplitude = Ifx_Math_Interp1DLut_F16(ampsq, cartToPolarInterp1DLut, IFX_MATH_CARTTOPOLAR_SHIFT);

    return polar;
}
