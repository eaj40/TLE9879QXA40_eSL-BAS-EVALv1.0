/*
 * Copyright (c) 2022 Infineon Technologies AG. All Rights Reserved.
 *
 * Use of this file is subject to the terms of use agreed between (i) you or the company in which ordinary course of
 * business you are acting and (ii) Infineon Technologies AG, its affiliates or its licensees. If and as long as no
 * such terms of use are agreed, use of this file is subject to the Evaluation Software License Agreement distributed
 * along with this file within the software delivery package.
 *
 */

#include "Ifx_Math_InvClarke.h"
#include "Ifx_Math_Mul.h"
#include "Ifx_Math_Neg.h"
#include "Ifx_Math_ShR.h"
#include "Ifx_Math_AddSat.h"
#include "Ifx_Math_SubSat.h"

/* Represents sqrt(3)/2 in Q15 */
#define IFX_MATH_INVCLARKE_SQRT3_DIV_2_Q15 (28377)

Ifx_Math_3PhaseFract16 Ifx_Math_InvClarke_F16(Ifx_Math_CmpFract16 stationary)
{
    /* Instantiate the returning 3-Phase Iu,Iv,Iw data container */
    Ifx_Math_3PhaseFract16 t3Phase;

    /* Calculate Iu */
    t3Phase.u = stationary.real;

    /* Calculate the temporary result of sqrt(3)/2*I_Beta */
    Ifx_Math_Fract16 betaTmp = Ifx_Math_Mul_F16(IFX_MATH_INVCLARKE_SQRT3_DIV_2_Q15, stationary.imag);

    /* Calculate the temporary result of -I_Alpha/2 */
    Ifx_Math_Fract16 alphaTmp = Ifx_Math_Neg_F16(Ifx_Math_ShR_F16(stationary.real, 1));

    /* Calculate Iv =  -I_Alpha/2 + sqrt(3)/2*I_Beta */
    t3Phase.v = Ifx_Math_AddSat_F16(alphaTmp, betaTmp);

    /* Calculate Iw =  -0.5*I_Alpha -sqrt(3)/2*I_Beta  */
    t3Phase.w = Ifx_Math_SubSat_F16(alphaTmp, betaTmp);

    return t3Phase;
}
