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
 * \file Ifx_Math_PLLF16.h
 * \brief Phase Locked Loop (PLL) filter module.
 * This module takes as input the angle and applies a PLL filter, with a feed forward mechanism.
 * The feed forward signal is the average rate of change of the input angle, which has a configurable delay
 * (controllable by the option IFX_MATH_CFG_PLL_DELAY_LENGTH).
 *
 * The implementation expects that the product of the proportional gain and the sampling time fit in the range [-1, 1)
 * in Q15 representation. If this condition is not met, the filter output is undefined.
 */

#ifndef IFX_MATH_PLLF16_H
#define IFX_MATH_PLLF16_H
#include "Ifx_Math.h"
#include "Ifx_Math_ShL.h"
#include "Ifx_Math_ShR.h"

/**
 * \brief Data structure that stores all data of module instance.
 *
 */
typedef struct Ifx_Math_PLLF16
{
    /**
     * Buffer for the average rate of change of the input angle
     */
    uint32 p_buffer[IFX_MATH_CFG_PLL_DELAY_LENGTH];

    /**
     * Proportional gain of the PLL controller
     */
    Ifx_Math_Fract16Q p_propGain;

    /**
     * Sampling time of the PLL filter, in microseconds
     */
    uint32 p_samplingTime_us;

    /**
     * Previous output of the PLL filter
     */
    uint32 p_previousValue;

    /**
     * Stores the value of the proportional gain multiplied by the sampling time
     */
    Ifx_Math_Fract16 p_propGainSamplingTime;

    /**
     * Index of the current buffer position
     */
    uint8 p_bufferIndex;
} Ifx_Math_PLLF16;

/**
 * Output of the PLL, containing the angle output and the angle variation.
 */
typedef struct Ifx_Math_PLLF16_Type
{
    /**
     * Angle variation, from the average rate of change of the input angle
     */
    Ifx_Math_Fract16 deltaAngle;

    /**
     * Angle output from the PLL, represented in 32-bit format, in the range \f$[0, 2*\pi]\f$, normalized by\f$2*\pi\f$
     */
    uint32 angle;
} Ifx_Math_PLLF16_Type;

/**
 *  \brief Initializes all the PLL parameters to 0.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *
 */
void Ifx_Math_PLLF16_init(Ifx_Math_PLLF16* self);

/**
 *  \brief Executes the PLL filter.
 *
 *  The function takes as input the angle signal to track and outputs the filtered angle, together with the angle
 * variation.
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *  \param [in] input Input of the filter in a 32-bit angle value, in the range \f$[0, 2*\pi]\f$, normalized
 * by\f$2*\pi\f$
 *
 *  \return Structure containing the filtered angle and the angle variation from the average rate of change of the input
 * angle
 */
Ifx_Math_PLLF16_Type Ifx_Math_PLLF16_execute(Ifx_Math_PLLF16* self, uint32 input);

/**
 *  \brief Set the proportional gain of the PLL controller.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *  \param [in] propGain Proportional gain
 *
 */
void Ifx_Math_PLLF16_setPropGain(Ifx_Math_PLLF16* self, Ifx_Math_Fract16Q propGain);

/**
 *  \brief Set the discrete sampling time period in microseconds, in the range [1us, 10^6us]
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *  \param [in] samplingTime_us Sampling time, in microseconds
 *
 */
void Ifx_Math_PLLF16_setSamplingTime_us(Ifx_Math_PLLF16* self, uint32 samplingTime_us);

/**
 *  \brief Get the proportional gain of the PLL controller.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *
 *  \return Proportional gain and respective Q format
 */
static inline Ifx_Math_Fract16Q Ifx_Math_PLLF16_getPropGain(Ifx_Math_PLLF16* self)
{
    return self->p_propGain;
}


/**
 *  \brief Get the discrete sampling time period in microseconds.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *
 *  \return Sampling time, in microseconds
 */
static inline uint32 Ifx_Math_PLLF16_getSamplingTime_us(Ifx_Math_PLLF16* self)
{
    return self->p_samplingTime_us;
}


/**
 *  \brief Set the previous value of the PLL filter.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *  \param [in] previousValue Previous value to be set
 *
 */
static inline void Ifx_Math_PLLF16_setPreviousValue(Ifx_Math_PLLF16* self, uint32 previousValue)
{
    self->p_previousValue = previousValue;
}


/**
 *  \brief Get the previous value of the PLL filter.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *
 *  \return Previous value
 */
static inline uint32 Ifx_Math_PLLF16_getPreviousValue(Ifx_Math_PLLF16* self)
{
    return self->p_previousValue;
}


/**
 *  \brief Reset the buffer and buffer index
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *
 */
void Ifx_Math_PLLF16_resetBuffer(Ifx_Math_PLLF16* self);

#endif /*IFX_MATH_PLLF16_H*/
