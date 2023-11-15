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
 * \file Ifx_Math_AccelLimitF16.h
 * \brief Provides functions to limit the acceleration and deceleration of a signal.
 * The functions in this module are used to limit the acceleration and deceleration of a signal. The absolute rate of
 * change of a signal is limited taking the sign of the input and previous value into account.
 *
 * In the case that the input signal and previous value are both positive and the input is higher than the previous
 * value, then the output is a signal which accelerates limited by the acceleration rate till it reaches the input
 * value, and if the input is less than the previous value, then the output is a signal which decelerates limited by the
 * deceleration rate till it reaches the input value.
 *
 * In the case that the input signal and previous value are both negative and the input is higher than the previous
 * value, then the output is a signal which decelerates limited by the deceleration rate till it reaches the input
 * value, and if the input is less than the previous value, then the output is a signal which accelerates limited by the
 * acceleration rate till it reaches the input value.
 *
 * In the case that the input signal and previous value have opposite signs, the output is a signal which decelerates to
 * zero limited by the deceleration rate, and then accelerates to the input value limited by the acceleration rate.
 */

#ifndef IFX_MATH_ACCELLIMITF16_H
#define IFX_MATH_ACCELLIMITF16_H
#include "Ifx_Math.h"
#include "Ifx_Math_ShL.h"
#include "Ifx_Math_ShR.h"

/**
 * \brief Data structure that stores all data of module instance.
 *
 */
typedef struct Ifx_Math_AccelLimitF16
{
    /**
     * Previous value of the speed
     */
    Ifx_Math_Fract32 p_speedStepPreviousValue;

    /**
     * Acceleration limit represented in Q30
     */
    Ifx_Math_Fract32 p_speedStepUpLimitQ30;

    /**
     * Deceleration limit represented in Q30
     */
    Ifx_Math_Fract32 p_speedStepDownLimitQ30;
} Ifx_Math_AccelLimitF16;

/**
 *  \brief Initialize the previous value, rising rate and falling rate to 0.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *
 */
static inline void Ifx_Math_AccelLimitF16_init(Ifx_Math_AccelLimitF16* self)
{
    self->p_speedStepPreviousValue = 0;
    self->p_speedStepUpLimitQ30    = 0;
    self->p_speedStepDownLimitQ30  = 0;
}


/**
 *  \brief Implements the 16-bit version of the Acceleration limiter
 *
 *  This API implements the 16-bit version of the Acceleration limiter. It returns the input value saturated between
 * (speedStepPreviousValue + speedStepUpLim) and (speedStepPreviousValue - speedStepDownLim), and updates the state
 * variable (stores the new output in the speedStepPreviousValue variable).
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *  \param [in] input Input signal
 *
 *  \return The actual output value in 16-bit
 */
Ifx_Math_Fract16 Ifx_Math_AccelLimitF16_execute(Ifx_Math_AccelLimitF16* self, Ifx_Math_Fract16 input);

/**
 *  \brief Set the acceleration rate, represented in Q30.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *  \param [in] stepUpQ30 Rising rate, represented in Q30
 *
 */
static inline void Ifx_Math_AccelLimitF16_setSpeedStepUpLimit(Ifx_Math_AccelLimitF16* self, Ifx_Math_Fract32
                                                              stepUpQ30)
{
    self->p_speedStepUpLimitQ30 = stepUpQ30;
}


/**
 *  \brief Get the acceleration rate, represented in Q30.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *
 *  \return Rising rate
 */
static inline Ifx_Math_Fract32 Ifx_Math_AccelLimitF16_getSpeedStepUpLimit(Ifx_Math_AccelLimitF16* self)
{
    return self->p_speedStepUpLimitQ30;
}


/**
 *  \brief Set the deceleration rate, represented in Q30.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *  \param [in] stepDownQ30 Falling rate, represented in Q30
 *
 */
static inline void Ifx_Math_AccelLimitF16_setSpeedStepDownLimit(Ifx_Math_AccelLimitF16* self, Ifx_Math_Fract32
                                                                stepDownQ30)
{
    self->p_speedStepDownLimitQ30 = stepDownQ30;
}


/**
 *  \brief Get the deceleration rate, represented in Q30.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *
 *  \return Falling rate
 */
static inline Ifx_Math_Fract32 Ifx_Math_AccelLimitF16_getSpeedStepDownLimit(Ifx_Math_AccelLimitF16* self)
{
    return self->p_speedStepDownLimitQ30;
}


/**
 *  \brief Set the previous value of the acceleration limiter.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *  \param [in] previousValue Previous value of the ramp
 *
 */
static inline void Ifx_Math_AccelLimitF16_setSpeedStepPreviousValue(Ifx_Math_AccelLimitF16* self, Ifx_Math_Fract16
                                                                    previousValue)
{
    self->p_speedStepPreviousValue = Ifx_Math_ShL_F32(previousValue, 15);
}


/**
 *  \brief Get the previous value of the ramp.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *
 *  \return Previous value
 */
static inline Ifx_Math_Fract16 Ifx_Math_AccelLimitF16_getSpeedStepPreviousValue(Ifx_Math_AccelLimitF16* self)
{
    return (Ifx_Math_Fract16)Ifx_Math_ShR_F32(self->p_speedStepPreviousValue, 15);
}


#endif /*IFX_MATH_ACCELLIMITF16_H*/
