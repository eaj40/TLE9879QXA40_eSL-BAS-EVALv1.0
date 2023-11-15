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
 * \file Ifx_MDA_FluxEstimatorF16.h
 * \brief Estimator for the rotor flux position and speed.
 * This module takes as input the stator voltage and current and outputs the estimated normalized speed and the
 * estimated position of rotor flux (in radians).
 *
 * In order to perform the estimation, all the macros in the file <u>Ifx_MDA_FluxEstimatorF16_Cfg</u>.h must be
 * configured.
 */

#ifndef IFX_MDA_FLUXESTIMATORF16_H
#define IFX_MDA_FLUXESTIMATORF16_H
#include "Ifx_ComponentID.h"
#include "Ifx_ComponentVersion.h"
#include "Ifx_MDA_FluxEstimatorF16_Cfg.h"
#include "Ifx_Math.h"
#include "Ifx_Math_LowPass1stF16.h"
#include "Ifx_Math_PLLF16.h"

/**
 * Flux estimator base electrical speed, in rad/s
 * <table>
 *  <caption>Parameter Representation</caption>
 *      <tr>
 *          <th>Name</th>
 *          <th>Attribute</th>
 *          <th>Value</th>
 *      </tr>
 *      <tr>
 *          <td rowspan="6">BASE_ELEC_SPEED_RADPS</td>
 *          <td>Fractional bits</td>
 *          <td>0</td>
 *      </tr>
 *      <tr>
 *          <td>Gain</td>
 *          <td>1</td>
 *      </tr>
 *      <tr>
 *          <td>Offset</td>
 *          <td>0</td>
 *      </tr>
 *      <tr>
 *          <td>Accuracy</td>
 *          <td>-</td>
 *      </tr>
 *      <tr>
 *          <td>Range</td>
 *          <td>[-3.40282347E+38, 3.40282347E+38]</td>
 *      </tr>
 *      <tr>
 *          <td>Unit</td>
 *          <td>rad/s</td>
 *      </tr>
 * </table>
 *
 */
#define IFX_MDA_FLUXESTIMATORF16_BASE_ELEC_SPEED_RADPS (IFX_MDA_FLUXESTIMATORF16_CFG_BASE_ELEC_SPEED_RADPS)

/**
 * Contains the module output variables.
 */
typedef struct Ifx_MDA_FluxEstimatorF16_Output
{
    /**
     * Estimated speed, normalized
     * <table>
     *  <caption>Parameter Representation</caption>
     *      <tr>
     *          <th>Name</th>
     *          <th>Attribute</th>
     *          <th>Value</th>
     *      </tr>
     *      <tr>
     *          <td rowspan="6">speedQ15</td>
     *          <td>Fractional bits</td>
     *          <td>15</td>
     *      </tr>
     *      <tr>
     *          <td>Gain</td>
     *          <td>IFX_MDA_FLUXESTIMATORF16_BASE_ELEC_SPEED_RADPS</td>
     *      </tr>
     *      <tr>
     *          <td>Offset</td>
     *          <td>0</td>
     *      </tr>
     *      <tr>
     *          <td>Accuracy</td>
     *          <td>-</td>
     *      </tr>
     *      <tr>
     *          <td>Range</td>
     *          <td>[-32768, 32767]</td>
     *      </tr>
     *      <tr>
     *          <td>Unit</td>
     *          <td>rad/s</td>
     *      </tr>
     * </table>
     *
     */
    Ifx_Math_Fract16 speedQ15;

    /**
     * Estimated position of the rotor flux
     * <table>
     *  <caption>Parameter Representation</caption>
     *      <tr>
     *          <th>Name</th>
     *          <th>Attribute</th>
     *          <th>Value</th>
     *      </tr>
     *      <tr>
     *          <td rowspan="6">anglePLL</td>
     *          <td>Fractional bits</td>
     *          <td>32</td>
     *      </tr>
     *      <tr>
     *          <td>Gain</td>
     *          <td>6.2832</td>
     *      </tr>
     *      <tr>
     *          <td>Offset</td>
     *          <td>0</td>
     *      </tr>
     *      <tr>
     *          <td>Accuracy</td>
     *          <td>-</td>
     *      </tr>
     *      <tr>
     *          <td>Range</td>
     *          <td>[0, 4294967295]</td>
     *      </tr>
     *      <tr>
     *          <td>Unit</td>
     *          <td>rad</td>
     *      </tr>
     * </table>
     *
     */
    uint32 anglePLL;
} Ifx_MDA_FluxEstimatorF16_Output;

/**
 * Modes of the Flux Estimator
 */
typedef enum Ifx_MDA_FluxEstimatorF16_Mode
{
    Ifx_MDA_FluxEstimatorF16_Mode_disable = 0, /**<Estimation is disabled, speed is set to zero and estimated angle is
                                                * kept at last value*/
    Ifx_MDA_FluxEstimatorF16_Mode_enable  = 1  /**<Speed and angle estimation of the rotor flux is enabled*/
} Ifx_MDA_FluxEstimatorF16_Mode;

/**
 * \brief Data structure that stores all data of module instance.
 *
 */
typedef struct Ifx_MDA_FluxEstimatorF16
{
    /**
     * First order low pass filter for the stator alpha components
     */
    Ifx_Math_LowPass1stF16 p_alphaFilter;

    /**
     * First order low pass filter for the stator beta components
     */
    Ifx_Math_LowPass1stF16 p_betaFilter;

    /**
     * First order low pass filter for the speed
     */
    Ifx_Math_LowPass1stF16 p_speedFilter;

    /**
     * Phase Locked Loop filter for the angle and speed estimation
     */
    Ifx_Math_PLLF16 p_pllFilter;

    /**
     * Contains the output variables of the module.
     */
    Ifx_MDA_FluxEstimatorF16_Output p_output;

    /**
     * Sampling time, in microseconds
     */
    uint32 p_samplingTime_us;

    /**
     * Conversion factor from rad to rad/s
     */
    Ifx_Math_Fract16 p_radToRadPerSecondQ7;

    /**
     * Configured operation mode of the Flux Estimator
     */
    Ifx_MDA_FluxEstimatorF16_Mode p_mode;
} Ifx_MDA_FluxEstimatorF16;

/**
 *  \brief Initialize the module to the default values and to the values configured in Config Wizard.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *
 */
void Ifx_MDA_FluxEstimatorF16_init(Ifx_MDA_FluxEstimatorF16* self);

/**
 *  \brief Perform estimation of the rotor flux rotational speed and position.
 *
 *  If the module is enabled, this API performs the speed and angle estimation of the rotor flux. In case of mode
 * disable, this API does not perform calculations, but sets the returned speed to zero and the returned estimated angle
 * to the last value.
 *
 *  Inputs of this API:
 *  <ul>
 *      <li>Stator voltage in stator reference frame, normalized by IFX_MDA_FLUXESTIMATORF16_CFG_BASE_VOLTAGE_V in Q15
 * format</li>
 *      <li>Stator current in stator reference frame, normalized by IFX_MDA_FLUXESTIMATORF16_CFG_BASE_CURRENT_A in Q15
 * format</li>
 *  </ul>
 *
 *  Outputs of this API:
 *  <ul>
 *      <li>Estimated rotor flux speed, normalized by IFX_MDA_FLUXESTIMATORF16_BASE_ELECTRICAL_SPEED_RADPS in Q15
 * format</li>
 *      <li>Estimated rotor flux angle between 0 and 2*pi, normalized to  0 to 2^32-1</li>
 *  </ul>
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *  \param [in] statorVoltage Structure containing the normalized stator voltage, in 1Q15 format
 *  \param [in] statorCurrent Structure containing the normalized stator current, in 1Q15 format
 *
 */
void Ifx_MDA_FluxEstimatorF16_execute(Ifx_MDA_FluxEstimatorF16* self, Ifx_Math_CmpFract16 statorVoltage,
                                      Ifx_Math_CmpFract16 statorCurrent);

/**
 *  \brief Configure the mode of the Flux Estimator.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *  \param [in] mode Control input to set the mode of the FluxEstimator
 *
 */
static inline void Ifx_MDA_FluxEstimatorF16_configMode(Ifx_MDA_FluxEstimatorF16* self, Ifx_MDA_FluxEstimatorF16_Mode
                                                       mode)
{
    self->p_mode = mode;
}


/**
 *  \brief Set the discrete sampling time period in microseconds, between  [1us, 10^6us].
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *  \param [in] samplingTime_us Sampling time, in microseconds
 *
 *  <table>
 *      <caption>Parameter Representation</caption>
 *          <tr>
 *              <th>Name</th>
 *              <th>Attribute</th>
 *              <th>Value</th>
 *          </tr>
 *          <tr>
 *              <td rowspan="6">setSamplingTime_us</td>
 *              <td>Fractional bits</td>
 *              <td></td>
 *          </tr>
 *          <tr>
 *              <td>Gain</td>
 *              <td>1.0E-6</td>
 *          </tr>
 *          <tr>
 *              <td>Offset</td>
 *              <td>0</td>
 *          </tr>
 *          <tr>
 *              <td>Accuracy</td>
 *              <td>-</td>
 *          </tr>
 *          <tr>
 *              <td>Range</td>
 *              <td>[1.0E-6, 1]</td>
 *          </tr>
 *          <tr>
 *              <td>Unit</td>
 *              <td>s</td>
 *          </tr>
 *  </table>
 *
 */
void Ifx_MDA_FluxEstimatorF16_setSamplingTime_us(Ifx_MDA_FluxEstimatorF16* self, uint32 samplingTime_us);

/**
 *  \brief Get the discrete sampling time period in microseconds.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *
 *  \return Sampling time, in microseconds
 *  <table>
 *      <caption>Parameter Representation</caption>
 *          <tr>
 *              <th>Name</th>
 *              <th>Attribute</th>
 *              <th>Value</th>
 *          </tr>
 *          <tr>
 *              <td rowspan="6">getSamplingTime_us</td>
 *              <td>Fractional bits</td>
 *              <td></td>
 *          </tr>
 *          <tr>
 *              <td>Gain</td>
 *              <td>1.0E-6</td>
 *          </tr>
 *          <tr>
 *              <td>Offset</td>
 *              <td>0</td>
 *          </tr>
 *          <tr>
 *              <td>Accuracy</td>
 *              <td>-</td>
 *          </tr>
 *          <tr>
 *              <td>Range</td>
 *              <td>[1.0E-6, 1]</td>
 *          </tr>
 *          <tr>
 *              <td>Unit</td>
 *              <td>s</td>
 *          </tr>
 *  </table>
 *
 */
static inline uint32 Ifx_MDA_FluxEstimatorF16_getSamplingTime_us(Ifx_MDA_FluxEstimatorF16* self)
{
    return self->p_samplingTime_us;
}


/**
 *  \brief Get the module output variables.
 * The output contains the estimated speed, represented in Q15, and the estimated flux angle.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *  \param [out] output Structure containing the module outputs
 *
 */
static inline void Ifx_MDA_FluxEstimatorF16_getOutput(Ifx_MDA_FluxEstimatorF16       * self,
                                                      Ifx_MDA_FluxEstimatorF16_Output* output)
{
    *output = self->p_output;
}


/**
 *  \brief Returns the component ID
 *
 *
 *  \param [out] *componentID Variable to store the address of the component ID
 *
 */
void Ifx_MDA_FluxEstimatorF16_getID(const Ifx_ComponentID** componentID);

/**
 *  \brief Get the active mode of the Flux Estimator.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *
 *  \return Mode
 */
static inline Ifx_MDA_FluxEstimatorF16_Mode Ifx_MDA_FluxEstimatorF16_getMode(Ifx_MDA_FluxEstimatorF16* self)
{
    return self->p_mode;
}


/**
 *  \brief Returns the component version
 *
 *
 *  \param [out] *componentVersion Variable to store the address of the component version
 *
 */
void Ifx_MDA_FluxEstimatorF16_getVersion(const Ifx_ComponentVersion** componentVersion);

#endif /*IFX_MDA_FLUXESTIMATORF16_H*/
