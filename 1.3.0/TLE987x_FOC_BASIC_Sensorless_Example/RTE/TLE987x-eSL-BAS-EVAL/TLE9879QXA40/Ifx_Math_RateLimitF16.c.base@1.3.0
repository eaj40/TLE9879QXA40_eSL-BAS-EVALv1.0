/*
 * Copyright (c) 2022 Infineon Technologies AG. All Rights Reserved.
 *
 * Use of this file is subject to the terms of use agreed between (i) you or the company in which ordinary course of
 * business you are acting and (ii) Infineon Technologies AG, its affiliates or its licensees. If and as long as no
 * such terms of use are agreed, use of this file is subject to the Evaluation Software License Agreement distributed
 * along with this file within the software delivery package.
 *
 */

#include "Ifx_Math_RateLimitF16.h"
#include "Ifx_Math_AddSat.h"
#include "Ifx_Math_SubSat.h"

/* polyspace-begin MISRA-C3:D4.14 [Justified:Low] "The caller function has to guarantee that NULL is not passed as
 * argument." */
Ifx_Math_Fract16 Ifx_Math_RateLimitF16_execute(Ifx_Math_RateLimitF16* self, Ifx_Math_Fract16 input)
{
    /* Local variables for Output and new state when rate of change is Up and Down */
    Ifx_Math_Fract16 output;
    Ifx_Math_Fract16 upNew;
    Ifx_Math_Fract16 downNew;

    /* Calculate new state when rate of change is up */
    upNew = Ifx_Math_AddSat_F16(self->p_previousValue, self->p_upRate);

    /* Calculate new state when rate of change is down */
    downNew = Ifx_Math_SubSat_F16(self->p_previousValue, self->p_downRate);

    /* Input state is greater than upNew limit it to upNewState */
    if (input > upNew)
    {
        output = upNew;
    }

    /* Input state is less than downNew limit it to downNewState */
    else if (input < downNew)
    {
        output = downNew;
    }
    else
    {
        output = input;
    }

    /* Update previous value */
    self->p_previousValue = output;

    return output;
}


/* polyspace-end MISRA-C3:D4.14 [Justified:Low] "The caller function has to guarantee that NULL is not passed as
 * argument." */
