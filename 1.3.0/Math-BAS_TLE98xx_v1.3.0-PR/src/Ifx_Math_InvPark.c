/*
 * Copyright (c) 2022 Infineon Technologies AG. All Rights Reserved.
 *
 * Use of this file is subject to the terms of use agreed between (i) you or the company in which ordinary course of
 * business you are acting and (ii) Infineon Technologies AG, its affiliates or its licensees. If and as long as no
 * such terms of use are agreed, use of this file is subject to the Evaluation Software License Agreement distributed
 * along with this file within the software delivery package.
 *
 */

#include "Ifx_Math_InvPark.h"
#include "Ifx_Math_MulSat.h"
#include "Ifx_Math_Cos.h"
#include "Ifx_Math_Sin.h"
#include "Ifx_Math_SubSat.h"
#include "Ifx_Math_AddSat.h"

Ifx_Math_CmpFract16 Ifx_Math_InvPark_F16(Ifx_Math_CmpFract16 rotating, uint32 angle)
{
    /* Data structure for return */
    Ifx_Math_CmpFract16 invParkOutput;

    /* Locals for grouping the operations */
    /* Ia = I_d * cos(theta) */
    Ifx_Math_Fract16    iA;

    /* I_q * sin(theta) */
    Ifx_Math_Fract16    iB;

    /* I_q * cos(theta) */
    Ifx_Math_Fract16    iC;

    /* I_d * sin(theta) */
    Ifx_Math_Fract16    iD;

    /* cos(theta) */
    Ifx_Math_Fract16    cosTheta = Ifx_Math_Cos_F16(angle);

    /* sin(theta) */
    Ifx_Math_Fract16    sinTheta = Ifx_Math_Sin_F16(angle);

    /* Calculate the local operations group "I_d * cos(theta)" */
    iA = Ifx_Math_MulSat_F16(rotating.real, cosTheta);

    /* Calculate the local operations group "I_q * sin(theta)" */
    iB = Ifx_Math_MulSat_F16(rotating.imag, sinTheta);

    /* Calculate the local operations group "I_q * cos(theta)" */
    iC = Ifx_Math_MulSat_F16(rotating.imag, cosTheta);

    /* Calculate the local operations group "I_d * sin(theta)" */
    iD = Ifx_Math_MulSat_F16(rotating.real, sinTheta);

    /* Calculate I_alpha from the local operations groups */
    invParkOutput.real = Ifx_Math_SubSat_F16(iA, iB);

    /* Calculate I_beta from the local operations groups */
    invParkOutput.imag = Ifx_Math_AddSat_F16(iC, iD);

    return invParkOutput;
}
