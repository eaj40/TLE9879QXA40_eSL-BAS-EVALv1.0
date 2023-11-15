/*
 * Copyright (c) 2022 Infineon Technologies AG. All Rights Reserved.
 *
 * Use of this file is subject to the terms of use agreed between (i) you or the company in which ordinary course of
 * business you are acting and (ii) Infineon Technologies AG, its affiliates or its licensees. If and as long as no
 * such terms of use are agreed, use of this file is subject to the Evaluation Software License Agreement distributed
 * along with this file within the software delivery package.
 *
 */

#include "Ifx_Math_LowPass1stF16.h"
#include "Ifx_Math_Sub.h"
#include "Ifx_Math_Mul.h"
#include "Ifx_Math_MulShR.h"

/* polyspace-begin MISRA-C3:D4.14 [Justified:Low] "The caller function has to guarantee that NULL is not passed as
 * argument." */
void Ifx_Math_LowPass1stF16_init(Ifx_Math_LowPass1stF16* self)
{
    /* Initialize all filter variables except the gain to 0 */
    self->p_timeConstCoeff  = 0;
    self->p_samplingTime_us = 0;
    self->p_timeConstant_us = 0;
    self->p_previousValue   = 0;

    /* Initialize the gain to 1 */
    self->p_gainQ14 = IFX_MATH_LOWPASS1STF16_ONE_2Q14;
}


Ifx_Math_Fract16 Ifx_Math_LowPass1stF16_execute(Ifx_Math_LowPass1stF16* self, Ifx_Math_Fract16 input)
{
    /* Variable declaration*/
    /* Input multiplied by the gain */
    Ifx_Math_Fract16 inputGain;

    /* Shifted previous value */
    Ifx_Math_Fract16 previousValueSh;

    /* Difference inputGain and prevValue */
    Ifx_Math_Fract16 err;

    /* Error multiplied by the time constant coefficient */
    Ifx_Math_Fract32 discErr;

    /* Temporary variable to hold output */
    Ifx_Math_Fract16 output;

    /* Assume input, output, and state have the same Q format */
    /* Multiply input with gain */
    inputGain = Ifx_Math_MulShR_F16(self->p_gainQ14, input, 15u);

    /* Convert previous value to 16 bit */
    previousValueSh = (Ifx_Math_Fract16)Ifx_Math_ShR_F32(self->p_previousValue, 16u);

    /* Subtract previous value from input */
    err = Ifx_Math_Sub_F16(inputGain, previousValueSh);

    /* Multiply error with Tcoef */
    discErr = Ifx_Math_Mul_F32_F16F16(self->p_timeConstCoeff, err);

    /* Update the state */
    self->p_previousValue = Ifx_Math_Add_F32(discErr, self->p_previousValue);

    /* Calculate the output */
    output = (Ifx_Math_Fract16)Ifx_Math_ShR_F32(self->p_previousValue, 15u);

    return output;
}


/* polyspace-end MISRA-C3:D4.14 [Justified:Low] "The caller function has to guarantee that NULL is not passed as
 * argument." */
