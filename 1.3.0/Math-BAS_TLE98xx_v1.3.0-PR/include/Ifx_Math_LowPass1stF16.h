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
 * \file Ifx_Math_LowPass1stF16.h
 * \brief Provides APIs to implement a 1st order low-pass filter.
 * The functions in this module are managing a 1st order low-pass filter. Each instance of a 1st order low-pass filter
 * needs one instance (declaration) of the data structure of the module.
 */

#ifndef IFX_MATH_LOWPASS1STF16_H
#define IFX_MATH_LOWPASS1STF16_H
#include "Ifx_Math.h"
#include "Ifx_Math_Add.h"
#include "Ifx_Math_DivShL.h"
#include "Ifx_Math_Sat.h"
#include "Ifx_Math_ShL.h"
#include "Ifx_Math_ShR.h"

/**
 * Maximum value representable in Q14
 */
#define IFX_MATH_LOWPASS1STF16_ONE_2Q14 (16384)

/**
 * \brief Data structure that stores all data of module instance.
 *
 */
typedef struct Ifx_Math_LowPass1stF16
{
    /**
     * The internal state variable of the  1st order low-pass filter, equivalent to the output of the filter at previous
     * function call cycle
     */
    Ifx_Math_Fract32 p_previousValue;

    /**
     * Time constant of the 1st order low-pass filter in microseconds
     */
    uint32 p_timeConstant_us;

    /**
     * Discrete sampling time of the 1st order low-pass filter in microseconds
     */
    uint32 p_samplingTime_us;

    /**
     * The discrete coefficient calculated based on the filter's time constant and the sampling time Ts:
     * \f$\begin{equation} TConstCoeff = \frac{T_s}{\mathcal{T}+T_s} \end{equation}\f$
     */
    Ifx_Math_Fract16 p_timeConstCoeff;

    /**
     * The gain of the low pass filter, in Q14 format.
     */
    Ifx_Math_Fract16 p_gainQ14;
} Ifx_Math_LowPass1stF16;

/**
 *  \brief Initializes the low pass filter parameters.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *
 */
void Ifx_Math_LowPass1stF16_init(Ifx_Math_LowPass1stF16* self);

/**
 *  \brief The time-discrete implementation of a 1st order low-pass filter.
 *
 *  This function provides the time-discrete implementation of a 1st order low-pass filter, described in continuous
 * Laplace domain by the following transfer function:
 *
 *  \f[\begin{equation} \frac{G(s)}{U(s)} = H\cdot \frac{1}{\mathcal{T} \cdot (s+1)} \end{equation}\f]
 *  where U(s) is the input, G(s) is the filtered output, H is the gain, and \f$\begin{equation} \mathcal{T}
 * \end{equation}\f$ is the time constant of the filter.
 *
 *  The time-discrete implementation of the 1st order low-pass filter is based on the continuous to discrete method \f$s
 * \rightarrow \frac{z - 1}{z \cdot T_s}\f$, resulting in the following optimized form:
 *
 *  \f[\begin{equation} y_n = TConstCoeff \cdot (H\cdot u_n - y_{n-1})+y_{n-1}  \end{equation}\f]
 *
 *  where:
 *  - \f$\begin{equation} u_n \end{equation}\f$ is the input at actual time (n),
 *  - \f$\begin{equation} y_n \end{equation}\f$ is the output at actual time (n),
 *  - \f$\begin{equation} y_{n-1} \end{equation}\f$ is the output at previous time cycle,
 *  - \f$ H \f$ is the gain of the filter,
 *  - \f$\begin{equation} TConstCoeff = \frac{T_s}{\mathcal{T}+T_s} \end{equation}\f$, where:
 *     - \f$\begin{equation} \mathcal{T} \end{equation}\f$ is the time constant of the filter.
 *     - \f$\begin{equation} T_s \end{equation}\f$ is the cycle time (sampling time)
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *  \param [in] input Input value of the filter
 *
 *  \return The output value of the 1st order low-pass filter
 */
Ifx_Math_Fract16 Ifx_Math_LowPass1stF16_execute(Ifx_Math_LowPass1stF16* self, Ifx_Math_Fract16 input);

/**
 *  \brief Set the output of the 1st order low-pass filter to the value passed to the "previousValue" parameter.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *  \param [in] previousValue The value to which the output of the filter must be set
 *
 */
static inline void Ifx_Math_LowPass1stF16_setPreviousValue(Ifx_Math_LowPass1stF16* self, Ifx_Math_Fract16
                                                           previousValue)
{
    self->p_previousValue = Ifx_Math_ShL_F32(previousValue, 15);
}


/**
 *  \brief Set the time constant of the 1st order low-pass filter to the value passed to the "timeConstant" parameter.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *  \param [in] timeConstant_us the time constant of the filter in microseconds, restricted to the range [0us, 2^31us]
 *
 */
static inline void Ifx_Math_LowPass1stF16_setTimeConstant_us(Ifx_Math_LowPass1stF16* self, uint32 timeConstant_us)
{
    /* timeConstCoeff = samplingTime/(samplingTime+timeConstant), calculated in Q15 since tConstCoeff is between 0 and 1
     * */
    Ifx_Math_Fract32 addTmp = Ifx_Math_Add_F32((Ifx_Math_Fract32)(self->p_samplingTime_us),
        (Ifx_Math_Fract32)(timeConstant_us));
    self->p_timeConstCoeff  = Ifx_Math_Sat_F16_F32(Ifx_Math_DivShL_F32((Ifx_Math_Fract32)(self->p_samplingTime_us),
        addTmp, 15u));
    self->p_timeConstant_us = timeConstant_us;
}


/**
 *  \brief Set the sampling time of the 1st order low-pass filter to the value passed to the "samplingTime" parameter.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *  \param [in] samplingTime_us the discrete sampling time period of the filter in microseconds, restricted to the range
 * [1us, 10^6us]
 *
 */
static inline void Ifx_Math_LowPass1stF16_setSamplingTime_us(Ifx_Math_LowPass1stF16* self, uint32 samplingTime_us)
{
    /* timeConstCoeff = samplingTime/(samplingTime+timeConstant), calculated in Q15 since tConstCoeff is between 0 and 1
     * */
    Ifx_Math_Fract32 addTmp = Ifx_Math_Add_F32((Ifx_Math_Fract32)(samplingTime_us),
        (Ifx_Math_Fract32)(self->p_timeConstant_us));
    self->p_timeConstCoeff  = Ifx_Math_Sat_F16_F32(Ifx_Math_DivShL_F32((Ifx_Math_Fract32)(samplingTime_us), addTmp,
        15u));
    self->p_samplingTime_us = samplingTime_us;
}


/**
 *  \brief Get the time constant of the 1st order low-pass filter.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *
 *  \return Time constant of the low pass filter
 */
static inline uint32 Ifx_Math_LowPass1stF16_getTimeConstant_us(Ifx_Math_LowPass1stF16* self)
{
    return self->p_timeConstant_us;
}


/**
 *  \brief Get the sampling time of the 1st order low-pass filter.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *
 *  \return Sampling time
 */
static inline uint32 Ifx_Math_LowPass1stF16_getSamplingTime_us(Ifx_Math_LowPass1stF16* self)
{
    return self->p_samplingTime_us;
}


/**
 *  \brief Set the gain of the low pass filter, limited between 0 and 1 in Q14.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *  \param [in] gainQ14 Gain value to set, in Q14 format.
 *
 */
static inline void Ifx_Math_LowPass1stF16_setGain(Ifx_Math_LowPass1stF16* self, Ifx_Math_Fract16 gainQ14)
{
    /* Limit the gain between 0 an 1 */
    if (gainQ14 > IFX_MATH_LOWPASS1STF16_ONE_2Q14)
    {
        self->p_gainQ14 = IFX_MATH_LOWPASS1STF16_ONE_2Q14;
    }
    else if (gainQ14 < 0)
    {
        self->p_gainQ14 = 0;
    }
    else
    {
        self->p_gainQ14 = gainQ14;
    }
}


/**
 *  \brief Get the gain of the low pass filter.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *
 *  \return Gain of the low pass filter
 */
static inline Ifx_Math_Fract16 Ifx_Math_LowPass1stF16_getGain(Ifx_Math_LowPass1stF16* self)
{
    return self->p_gainQ14;
}


/**
 *  \brief Get the previous value of the 1st order low pass filter.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *
 *  \return Previous value (state) of the low pass filter
 */
static inline Ifx_Math_Fract16 Ifx_Math_LowPass1stF16_getPreviousValue(Ifx_Math_LowPass1stF16* self)
{
    return (Ifx_Math_Fract16)Ifx_Math_ShR_F32(self->p_previousValue, 15);
}


#endif /*IFX_MATH_LOWPASS1STF16_H*/
