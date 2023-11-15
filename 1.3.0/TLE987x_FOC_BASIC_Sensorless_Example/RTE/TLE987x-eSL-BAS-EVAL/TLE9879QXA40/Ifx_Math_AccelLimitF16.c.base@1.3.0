/*
 * Copyright (c) 2022 Infineon Technologies AG. All Rights Reserved.
 *
 * Use of this file is subject to the terms of use agreed between (i) you or the company in which ordinary course of
 * business you are acting and (ii) Infineon Technologies AG, its affiliates or its licensees. If and as long as no
 * such terms of use are agreed, use of this file is subject to the Evaluation Software License Agreement distributed
 * along with this file within the software delivery package.
 *
 */

#include "Ifx_Math_AccelLimitF16.h"
#include "Ifx_Math_Add.h"
#include "Ifx_Math_Sub.h"
#include "Ifx_Math_Neg.h"

/**
 *  \brief Return the input limited between upper limit and lower limit
 *
 *
 *  \param [in] input Input to be limited
 *  \param [in] upperLimit Upper limit
 *  \param [in] lowerLimit Lower limit
 *
 *  \return Limited input
 */
static inline Ifx_Math_Fract32 Ifx_Math_AccelLimitF16_p_limit(Ifx_Math_Fract32 input, Ifx_Math_Fract32 upperLimit,
                                                              Ifx_Math_Fract32 lowerLimit);

/**
 *  Calculate the limits in the zero region, limit the input between the calculated limits and store the result in
 * the speed previous value
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *  \param [in] input Input signal
 *
 */
static inline void Ifx_Math_AccelLimitF16_p_calcZeroRegion(Ifx_Math_AccelLimitF16* self, Ifx_Math_Fract32 input);

/**
 *  Calculate the limits in the positive region, limit the input between the calculated limits and store the result in
 * the speed previous value
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *  \param [in] input Input signal
 *
 */
static inline void Ifx_Math_AccelLimitF16_p_calcPosRegion(Ifx_Math_AccelLimitF16* self, Ifx_Math_Fract32 input);

/**
 *  Calculate the limits in the negative region, limit the input between the calculated limits and store the result in
 * the speed previous value
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *  \param [in] input Input signal
 *
 */
static inline void Ifx_Math_AccelLimitF16_p_calcNegRegion(Ifx_Math_AccelLimitF16* self, Ifx_Math_Fract32 input);

/* polyspace-begin MISRA-C3:D4.14 [Justified:Low] "The caller function has to guarantee that NULL is not passed as
 * argument." */
Ifx_Math_Fract16 Ifx_Math_AccelLimitF16_execute(Ifx_Math_AccelLimitF16* self, Ifx_Math_Fract16 input)
{
    /* Convert input to 32-bit fractional type */
    Ifx_Math_Fract32 input32;
    input32 = Ifx_Math_ShL_F32(input, 15u);

    /* Update previous value depending on sign of prev. value */
    if (self->p_speedStepPreviousValue > 0)
    {
        /* Update previous value if in +ve region */
        Ifx_Math_AccelLimitF16_p_calcPosRegion(self, input32);
    }
    else if (self->p_speedStepPreviousValue < 0)
    {
        /* Update previous value if in -ve region */
        Ifx_Math_AccelLimitF16_p_calcNegRegion(self, input32);
    }
    else
    {
        /* Update previous value if in zero region */
        Ifx_Math_AccelLimitF16_p_calcZeroRegion(self, input32);
    }

    /* Convert to 16-bit fractional type and return the output */
    return (Ifx_Math_Fract16)Ifx_Math_ShR_F32(self->p_speedStepPreviousValue, 15u);
}


static inline void Ifx_Math_AccelLimitF16_p_calcZeroRegion(Ifx_Math_AccelLimitF16* self, Ifx_Math_Fract32 input)
{
    /* Local variables for new state when rate of change is down */
    Ifx_Math_Fract32 downNew;

    /* Positive region? */
    if (input > 0)
    {
        /* Update limited previous value */
        if (input > self->p_speedStepUpLimitQ30)
        {
            self->p_speedStepPreviousValue = self->p_speedStepUpLimitQ30;
        }
        else
        {
            self->p_speedStepPreviousValue = input;
        }
    }

    /* Negative Region */
    else
    {
        /* Calculate new state when rate of change is down */
        downNew = Ifx_Math_Neg_F32(self->p_speedStepDownLimitQ30);

        /* Update limited previous value */
        if (input < downNew)
        {
            self->p_speedStepPreviousValue = downNew;
        }
        else
        {
            self->p_speedStepPreviousValue = input;
        }
    }
}


static inline void Ifx_Math_AccelLimitF16_p_calcPosRegion(Ifx_Math_AccelLimitF16* self, Ifx_Math_Fract32 input)
{
    /* Local variables for Output and new state when rate of change is Up and Down */
    Ifx_Math_Fract32 upNew, downNew;

    /* Calculate new state when rate of change is up */
    upNew = Ifx_Math_Add_F32(self->p_speedStepPreviousValue, self->p_speedStepUpLimitQ30);

    /* Calculate new state when rate of change is down */
    downNew = Ifx_Math_Sub_F32(self->p_speedStepPreviousValue, self->p_speedStepDownLimitQ30);

    /* Limit inNew between upNew and downNew and update previous value */
    self->p_speedStepPreviousValue = Ifx_Math_AccelLimitF16_p_limit(input, upNew, downNew);
}


static inline void Ifx_Math_AccelLimitF16_p_calcNegRegion(Ifx_Math_AccelLimitF16* self, Ifx_Math_Fract32 input)
{
    /* Local variables for Output and new state when rate of change is Up and Down */
    Ifx_Math_Fract32 upNew, downNew;

    /* Calculate new state when rate of change is up */
    upNew = Ifx_Math_Add_F32(self->p_speedStepPreviousValue, self->p_speedStepDownLimitQ30);

    /* Calculate new state when rate of change is down */
    downNew = Ifx_Math_Sub_F32(self->p_speedStepPreviousValue, self->p_speedStepUpLimitQ30);

    /* Limit inNew between upNew and downNew and update previous value */
    self->p_speedStepPreviousValue = Ifx_Math_AccelLimitF16_p_limit(input, upNew, downNew);
}


/* polyspace-end MISRA-C3:D4.14 [Justified:Low] "The caller function has to guarantee that NULL is not passed as
 * argument." */

/* Limit input between upperLimit and lowerLimit */
static inline Ifx_Math_Fract32 Ifx_Math_AccelLimitF16_p_limit(Ifx_Math_Fract32 input, Ifx_Math_Fract32 upperLimit,
                                                              Ifx_Math_Fract32 lowerLimit)
{
    Ifx_Math_Fract32 output;

    if (input > upperLimit)
    {
        output = upperLimit;
    }
    else if (input < lowerLimit)
    {
        output = lowerLimit;
    }
    else
    {
        output = input;
    }

    return output;
}
