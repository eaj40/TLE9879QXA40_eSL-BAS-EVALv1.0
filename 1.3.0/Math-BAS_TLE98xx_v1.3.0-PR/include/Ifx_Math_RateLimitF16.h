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
 * \file Ifx_Math_RateLimitF16.h
 * \brief Provides APIs to limit the rate of change of a signal.
 * The functions in this module are used to limit the rate of change of a signal. The rising rate is the rate of change
 * of the input signal when the input is greater than the previous value and the falling rate is the rate of change of
 * the signal when the input is less than the previous value. It is assumed that the input, output, state and rates all
 * have the same Q format.
 *
 * The rate of change of a discrete signal s is
 *
 * \f[
 * ds = \frac{s(k+1) - s(k)}{T_s}
 * \f]
 *
 * where k is the sampling instant and  \f$ T_s\f$ is the sampling time.
 * This is reformulated as
 *
 * \f[
 * s(k+1) = s(k) + T_s\cdot ds
 * \f]
 *
 * where s(k) is the old value stored in the state and \f$ T_s\cdot ds \f$ is the rate of change.
 *
 * Thus for a rising signal
 *
 * \f[
 * s(k+1) = s(k) + upRate
 * \f]
 *
 * and for a falling signal
 *
 * \f[
 * s(k+1) = s(k) - downRate
 * \f]
 *
 * The input value is then saturated between \f$ previousValue + upRate \f$ and \f$ previousValue - downRate \f$.
 */

#ifndef IFX_MATH_RATELIMITF16_H
#define IFX_MATH_RATELIMITF16_H
#include "Ifx_Math.h"

/**
 * \brief Data structure that stores all data of module instance.
 *
 */
typedef struct Ifx_Math_RateLimitF16
{
    /**
     * Previous value of the ramp
     */
    Ifx_Math_Fract16 p_previousValue;

    /**
     * Rising rate multiplied by the sampling time, greater than zero
     */
    Ifx_Math_Fract16 p_upRate;

    /**
     * Falling rate of a signal multiplied by the sampling time, greater than zero
     */
    Ifx_Math_Fract16 p_downRate;
} Ifx_Math_RateLimitF16;

/**
 *  \brief Initialize the ramp previous value, rising rate and falling rate to 0.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *
 */
static inline void Ifx_Math_RateLimitF16_init(Ifx_Math_RateLimitF16* self)
{
    self->p_previousValue = 0;
    self->p_upRate        = 0;
    self->p_downRate      = 0;
}


/**
 *  \brief 16-bit implementation of the rate limiter.
 *
 *  This function implements the 16-bit version of the rate limiter. It returns the input value saturated between \f$
 * previousValue + upRate \f$ and \f$ previousValue - downRate \f$, and updates the state variable (stores the new
 * output in the previousValue variable).
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *  \param [in] input Input signal
 *
 *  \return The actual output value in 16-bit
 */
Ifx_Math_Fract16 Ifx_Math_RateLimitF16_execute(Ifx_Math_RateLimitF16* self, Ifx_Math_Fract16 input);

/**
 *  \brief Set the rising rate.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *  \param [in] upRate Rising rate
 *
 */
static inline void Ifx_Math_RateLimitF16_setUpRate(Ifx_Math_RateLimitF16* self, Ifx_Math_Fract16 upRate)
{
    self->p_upRate = upRate;
}


/**
 *  \brief Get the rising rate.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *
 *  \return Rising rate
 */
static inline Ifx_Math_Fract16 Ifx_Math_RateLimitF16_getUpRate(Ifx_Math_RateLimitF16* self)
{
    return self->p_upRate;
}


/**
 *  \brief Set the falling rate.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *  \param [in] downRate Falling rate
 *
 */
static inline void Ifx_Math_RateLimitF16_setDownRate(Ifx_Math_RateLimitF16* self, Ifx_Math_Fract16 downRate)
{
    self->p_downRate = downRate;
}


/**
 *  \brief Get the falling rate.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *
 *  \return Falling rate
 */
static inline Ifx_Math_Fract16 Ifx_Math_RateLimitF16_getDownRate(Ifx_Math_RateLimitF16* self)
{
    return self->p_downRate;
}


/**
 *  \brief Set the previous value of the ramp.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *  \param [in] previousValue Previous value of the ramp
 *
 */
static inline void Ifx_Math_RateLimitF16_setPreviousValue(Ifx_Math_RateLimitF16* self, Ifx_Math_Fract16 previousValue)
{
    self->p_previousValue = previousValue;
}


/**
 *  \brief Get the previous value of the ramp.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *
 *  \return Previous value
 */
static inline Ifx_Math_Fract16 Ifx_Math_RateLimitF16_getPreviousValue(Ifx_Math_RateLimitF16* self)
{
    return self->p_previousValue;
}


#endif /*IFX_MATH_RATELIMITF16_H*/
