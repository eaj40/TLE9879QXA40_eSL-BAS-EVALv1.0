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
 * \file Ifx_Math_PiF16.h
 * \brief Provides APIs needed to implement a PI controller.
 */

#ifndef IFX_MATH_PIF16_H
#define IFX_MATH_PIF16_H
#include "Ifx_Math.h"
#include "Ifx_Math_ShL.h"
#include "Ifx_Math_ShR.h"

/**
 * \brief Data structure that stores all data of module instance.
 *
 */
typedef struct Ifx_Math_PiF16
{
    /**
     * State of the integrator
     */
    Ifx_Math_Fract32 p_integPreviousValue;

    /**
     * Integral gain multiplied by the sampling time of the PI controller
     */
    Ifx_Math_Fract16Q p_integGainSamplingTime;

    /**
     * Anti windup gain multiplied by the sampling time of the PI controller
     */
    Ifx_Math_Fract16Q p_antiWindupGainSamplingTime;

    /**
     * Proportional gain of the PI controller
     */
    Ifx_Math_Fract16Q p_propGain;

    /**
     * Upper limit of the PI controller
     */
    Ifx_Math_Fract16 p_upperLimit;

    /**
     * Lower limit of the PI controller
     */
    Ifx_Math_Fract16 p_lowerLimit;

    /**
     * Status of the output saturation of the module, has a value of 1 if the calculated output of the PI controller
     * before saturation is greater than the upper limit, -1 if the calculated output of the PI controller before
     * saturation is less than the lower limit, and 0 otherwise.
     */
    sint8 p_saturationStatus;

    /**
     * Q format of the error
     */
    Ifx_Math_FractQFormat p_qFormatError;

    /**
     * Q format of output of the PI controller
     */
    Ifx_Math_FractQFormat p_qFormatOutput;

    /**
     * Internal variable for Q format of the proportional gain
     */
    uint8 p_qFormatPropGain;

    /**
     * Internal variable for Q format of the integral gain
     */
    uint8 p_qFormatIntegGain;

    /**
     * Internal variable for Q format of the anti windup gain
     */
    uint8 p_qFormatAntiWindupGain;
} Ifx_Math_PiF16;

/**
 * Contains Qformats of proportional, integration, antiwindup gains, PI limiter and error
 */
typedef struct Ifx_Math_PiF16_Qformats
{
    /**
     * Q format of the anti windup gain multiplied by sampling time
     */
    Ifx_Math_FractQFormat qFormatAntiWindupGainSamplingTime;

    /**
     * Q format of the proportional gain
     */
    Ifx_Math_FractQFormat qFormatPropGain;

    /**
     * Q format of the integration gain multiplied by sampling time
     */
    Ifx_Math_FractQFormat qFormatIntegGainSamplingTime;

    /**
     * Q format of the output
     */
    Ifx_Math_FractQFormat qFormatOutput;

    /**
     * Q format of the Error
     */
    Ifx_Math_FractQFormat qFormatError;
} Ifx_Math_PiF16_Qformats;

/**
 *  Initialize all the PI controller parameters to 0.
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *  \param [in] qformatsPi Qformats of proportional, integration, antiwindup gains, PI limiter and error
 *
 */
void Ifx_Math_PiF16_init(Ifx_Math_PiF16* self, Ifx_Math_PiF16_Qformats qformatsPi);

/**
 *  \brief Calculates the output of the PI controller using the parallel form based on the error value, \( e \), and
 * discretization method chosen.
 *
 *  This function Calculates the output of the PI controller using the parallel form based on the error value, \f$ e
 * \f$, and discretization method chosen. The output is the sum of the proportional and integral part of the controller.
 *
 *  The Proportional part, \f$ U_p \f$, is calculated as:
 *
 *  \f[
 *  U_p(k) = k_p \cdot e(k)
 *  \f]
 *
 *  where \f$ k \f$ represents the current time instant.
 *
 *  The integral part, \f$ U_i \f$, depends on the discretization method used and initial conditions.
 *
 *  Using Forward Euler discretization:
 *
 *  \f[
 *  U_i(k) = T_s \cdot k_i \cdot e(k-1) + U_i(k-1)
 *  \f]
 *
 *  where \f$ T_s \f$ is the sampling time, and \f$ k_i \f$ is the integral gain.
 *
 *  Using Backward Euler discretization:
 *
 *  \f[
 *  U_i(k) = T_s \cdot k_i \cdot e(k) + U_i(k-1)
 *  \f]
 *
 *  Using Trapezoidal discretization:
 *
 *  \f[
 *  U_i(k) = \frac{T_s \cdot k_i \cdot e(k)}{2} +\frac{T_s \cdot k_i \cdot e(k-1)}{2} + U_i(k-1)
 *  \f]
 *
 *  The output is then the sum of the two parts
 *
 *  \f[
 *  U(k) = U_p(k) + U_i(k)
 *  \f]
 *
 *  and limited between the upper and lower limits.
 *
 *  If Anti-windup with back calculation is implemented, the output of the  anti-windup part is:
 *  \f[
 *  U_{aw}(k) = k_{aw}\cdot (U(k) - U_{sat}(k))
 *  \f]
 *
 *  where \f$ U_{sat} \f$ is the saturated output of the controller.
 *
 *  \f$ U_{aw} \f$ is then subtracted from the input to the integral resulting in a back-calculation anti-windup.
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *  \param [in] errorValue New input error value
 *  \param [in] antiWindupCtrl Control input which takes the values, 0, 1, or -1 and based on its value and the sign of
 * the input error, integration is enabled or disabled. If the value is 1 and the error is positive, or if the value -1
 * and the error is negative, then the internal state of the integrator is held constant. This input is only effective
 * if the clamp antiwindup option is used.
 *
 *  \return The output of the PI controller limited between the upper and lower limits defined
 */
Ifx_Math_Fract16 Ifx_Math_PiF16_execute(Ifx_Math_PiF16* self, Ifx_Math_Fract16 errorValue, sint8 antiWindupCtrl);

/**
 *  \brief Set the previous value of the integrator
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *  \param [in] integPreviousValue Previous value of the integrator
 *
 */
static inline void Ifx_Math_PiF16_setIntegPreviousValue(Ifx_Math_PiF16* self, Ifx_Math_Fract16 integPreviousValue)
{
    /* Store the integral previous value in the intermediate data type format */
    self->p_integPreviousValue = Ifx_Math_ShL_F32(integPreviousValue, 9);
}


/**
 *  \brief Get the previous value of the integrator.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *
 *  \return Previous value of the integrator
 */
static inline Ifx_Math_Fract16 Ifx_Math_PiF16_getIntegPreviousValue(Ifx_Math_PiF16* self)
{
    return (Ifx_Math_Fract16)Ifx_Math_ShR_F32(self->p_integPreviousValue, 9);
}


/**
 *  \brief Set the proportional gain value of the PI controller
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *  \param [in] propGain Proportional gain value
 *
 */
static inline void Ifx_Math_PiF16_setPropGain(Ifx_Math_PiF16* self, Ifx_Math_Fract16 propGain)
{
    self->p_propGain.value = propGain;
}


/**
 *  \brief Get the proportional gain value of the PI controller.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *
 *  \return Proportional gain value
 */
static inline Ifx_Math_Fract16 Ifx_Math_PiF16_getPropGain(Ifx_Math_PiF16* self)
{
    return self->p_propGain.value;
}


/**
 *  \brief Set the integral gain multiplied by sampling time value of the PI controller
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *  \param [in] integGainSamplingTime Integral gain multiplied by sampling time value
 *
 */
static inline void Ifx_Math_PiF16_setIntegGainSamplingTime(Ifx_Math_PiF16* self, Ifx_Math_Fract16
                                                           integGainSamplingTime)
{
    self->p_integGainSamplingTime.value = integGainSamplingTime;
}


/**
 *  \brief Get the integral gain multiplied by sampling time value of the PI controller.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *
 *  \return Integral gain multiplied by sampling time value
 */
static inline Ifx_Math_Fract16 Ifx_Math_PiF16_getIntegGainSamplingTime(Ifx_Math_PiF16* self)
{
    return self->p_integGainSamplingTime.value;
}


/**
 *  \brief Set the anti-windup gain multiplied by sampling time value of the PI controller
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *  \param [in] antiWindupGainSamplingTime Antiwindup gain multiplied by sampling time value
 *
 */
static inline void Ifx_Math_PiF16_setAntiWindupGainSamplingTime(Ifx_Math_PiF16* self, Ifx_Math_Fract16
                                                                antiWindupGainSamplingTime)
{
    self->p_antiWindupGainSamplingTime.value = antiWindupGainSamplingTime;
}


/**
 *  \brief Get the antiwindup gain multiplied by sampling time  value of the PI controller.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *
 *  \return Antiwindup gain multiplied by sampling time value
 */
static inline Ifx_Math_Fract16 Ifx_Math_PiF16_getAntiWindupGainSamplingTime(Ifx_Math_PiF16* self)
{
    return self->p_antiWindupGainSamplingTime.value;
}


/**
 *  \brief Set the output upper limit of the PI controller
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *  \param [in] upperLimit Upper limit of the PI limiter
 *
 */
static inline void Ifx_Math_PiF16_setUpperLimit(Ifx_Math_PiF16* self, Ifx_Math_Fract16 upperLimit)
{
    self->p_upperLimit = upperLimit;
}


/**
 *  \brief Get the saturation upper limit of the PI controller.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *
 *  \return Upper limit of the PI controller
 */
static inline Ifx_Math_Fract16 Ifx_Math_PiF16_getUpperLimit(Ifx_Math_PiF16* self)
{
    return self->p_upperLimit;
}


/**
 *  \brief Get the saturation lower limit of the PI controller.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *
 *  \return Lower limit of the PI controller
 */
static inline Ifx_Math_Fract16 Ifx_Math_PiF16_getLowerLimit(Ifx_Math_PiF16* self)
{
    return self->p_lowerLimit;
}


/**
 *  \brief Set the output lower limit of the PI controller
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *  \param [in] lowerLimit Lower limit of the PI limiter
 *
 */
static inline void Ifx_Math_PiF16_setLowerLimit(Ifx_Math_PiF16* self, Ifx_Math_Fract16 lowerLimit)
{
    self->p_lowerLimit = lowerLimit;
}


/**
 *  \brief Get the saturation status of the PI controller.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *
 *  \return Saturation status
 */
static inline sint8 Ifx_Math_PiF16_getSaturationStatus(Ifx_Math_PiF16* self)
{
    return self->p_saturationStatus;
}


#endif /*IFX_MATH_PIF16_H*/
