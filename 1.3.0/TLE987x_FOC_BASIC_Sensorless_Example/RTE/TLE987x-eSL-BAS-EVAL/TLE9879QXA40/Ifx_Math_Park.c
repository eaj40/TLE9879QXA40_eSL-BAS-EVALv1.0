/*
 * Copyright (c) 2022 Infineon Technologies AG. All Rights Reserved.
 *
 * Use of this file is subject to the terms of use agreed between (i) you or the company in which ordinary course of
 * business you are acting and (ii) Infineon Technologies AG, its affiliates or its licensees. If and as long as no
 * such terms of use are agreed, use of this file is subject to the Evaluation Software License Agreement distributed
 * along with this file within the software delivery package.
 *
 */

#include "Ifx_Math_Park.h"
#include "Ifx_Math_MulSat.h"
#include "Ifx_Math_Cos.h"
#include "Ifx_Math_Sin.h"
#include "Ifx_Math_AddSat.h"
#include "Ifx_Math_SubSat.h"

Ifx_Math_CmpFract16 Ifx_Math_Park_F16(Ifx_Math_CmpFract16 stationary, uint32 angle)
{
    /* Declare data structure for return */
    Ifx_Math_CmpFract16 parkOutput;

    /* Declare locals for grouping the operations */
    /* I_alpha * cos(theta) */
    Ifx_Math_Fract16    iA;

    /* I_beta * sin(theta) */
    Ifx_Math_Fract16    iB;

    /* I_beta * cos(theta) */
    Ifx_Math_Fract16    iC;

    /* I_alpha * sin(theta) */
    Ifx_Math_Fract16    iD;

    /* cos(theta) */
    Ifx_Math_Fract16    cosTheta = Ifx_Math_Cos_F16(angle);

    /* sin(theta) */
    Ifx_Math_Fract16    sinTheta = Ifx_Math_Sin_F16(angle);

    /* Calculate the local operations group "I_alpha * cos(theta)" */
    iA = Ifx_Math_MulSat_F16(stationary.real, cosTheta);

    /* Calculate the local operations group "I_beta * sin(theta)" */
    iB = Ifx_Math_MulSat_F16(stationary.imag, sinTheta);

    /* Calculate the local operations group "I_beta * cos(theta)" */
    iC = Ifx_Math_MulSat_F16(stationary.imag, cosTheta);

    /* Calculate the local operations group "I_alpha * sin(theta)" */
    iD = Ifx_Math_MulSat_F16(stationary.real, sinTheta);

    /* Calculate Id from the local operations groups */
    parkOutput.real = Ifx_Math_AddSat_F16(iA, iB);

    /* Calculate Iq from the local operations groups */
    parkOutput.imag = Ifx_Math_SubSat_F16(iC, iD);

    return parkOutput;
}
