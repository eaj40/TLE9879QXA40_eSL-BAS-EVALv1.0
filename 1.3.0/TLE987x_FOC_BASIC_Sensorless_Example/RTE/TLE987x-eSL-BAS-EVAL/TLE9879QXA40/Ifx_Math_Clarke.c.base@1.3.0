/*
 * Copyright (c) 2022 Infineon Technologies AG. All Rights Reserved.
 *
 * Use of this file is subject to the terms of use agreed between (i) you or the company in which ordinary course of
 * business you are acting and (ii) Infineon Technologies AG, its affiliates or its licensees. If and as long as no
 * such terms of use are agreed, use of this file is subject to the Evaluation Software License Agreement distributed
 * along with this file within the software delivery package.
 *
 */

#include "Ifx_Math_Clarke.h"
#include "Ifx_Math_Sub.h"
#include "Ifx_Math_ShR.h"
#include "Ifx_Math_MulShRSat.h"

Ifx_Math_CmpFract16 Ifx_Math_Clarke_F16(Ifx_Math_3PhaseFract16 phase)
{
    /* Instantiate the returning alphaBeta data container */
    Ifx_Math_CmpFract16 alphaBeta;

    /* Calculate alpha */
    alphaBeta.real = phase.u;

    /* Calculate Beta: */
    /* Downscale Iv and Iw to Q14 format and execute the subtraction (Iv - Iw) in Q14 */
    Ifx_Math_Fract16 ivSubIw = Ifx_Math_Sub_F16(Ifx_Math_ShR_F16(phase.v, 1u), Ifx_Math_ShR_F16(phase.w, 1u));

    /* Calculate (1/sqrt(3)) in Q15 * (Iv - Iw) in Q14) and convert to Q15 by 14-bit RSh */
    alphaBeta.imag = Ifx_Math_MulShRSat_F16(IFX_MATH_ONE_OVER_SQRT3_Q15, ivSubIw, 14u);

    return alphaBeta;
}
