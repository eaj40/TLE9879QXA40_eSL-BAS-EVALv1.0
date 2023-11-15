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
 * \file Ifx_MDA_FocControllerF16.h
 * \brief 16-bit implementation of the field oriented controller.
 * This module takes as input the currents in alpha-beta format, the direct and quadrature reference currents and the
 * rotor flux angle, performs the current control using two PI controllers, and outputs the voltage command, in polar
 * format. It also takes the electrical speed as input and performs dq decoupling if enabled​.
 */

#ifndef IFX_MDA_FOCCONTROLLERF16_H
#define IFX_MDA_FOCCONTROLLERF16_H
#include "Ifx_ComponentID.h"
#include "Ifx_ComponentVersion.h"
#include "Ifx_MDA_FocControllerF16_Cfg.h"
#include "Ifx_Math.h"
#include "Ifx_Math_DqDecouplingF16.h"
#include "Ifx_Math_PiF16.h"

/**
 * Base voltage, in V
 * <table>
 *  <caption>Parameter Representation</caption>
 *      <tr>
 *          <th>Name</th>
 *          <th>Attribute</th>
 *          <th>Value</th>
 *      </tr>
 *      <tr>
 *          <td rowspan="6">BASE_VOLTAGE_V</td>
 *          <td>Fractional bits</td>
 *          <td></td>
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
 *          <td>V</td>
 *      </tr>
 * </table>
 *
 */
#define IFX_MDA_FOCCONTROLLERF16_BASE_VOLTAGE_V (IFX_MDA_FOCCONTROLLERF16_CFG_BASE_VOLTAGE_V)

/**
 * Contains the module output variables.
 */
typedef struct Ifx_MDA_FocControllerF16_Output
{
    /**
     * Voltage command in polar form
     * <table>
     *  <caption>Parameter Representation</caption>
     *      <tr>
     *          <th>Name</th>
     *          <th>Attribute</th>
     *          <th>Value</th>
     *      </tr>
     *      <tr>
     *          <td rowspan="6">voltageCommandPolar.amplitude</td>
     *          <td>Fractional bits</td>
     *          <td>15</td>
     *      </tr>
     *      <tr>
     *          <td>Gain</td>
     *          <td>IFX_MDA_FOCCONTROLLERF16_BASE_VOLTAGE_V</td>
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
     *          <td>V</td>
     *      </tr><tr>
     *          <td rowspan="6">voltageCommandPolar.angle</td>
     *          <td>Fractional bits</td>
     *          <td>32</td>
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
     *          <td>[0, 4294967295]</td>
     *      </tr>
     *      <tr>
     *          <td>Unit</td>
     *          <td>rad</td>
     *      </tr>
     * </table>
     *
     */
    Ifx_Math_PolarFract16 voltageCommandPolar;
} Ifx_MDA_FocControllerF16_Output;

/**
 * \brief Data structure that stores all data of module instance.
 *
 */
typedef struct Ifx_MDA_FocControllerF16
{
    /**
     * Instance of PI for the direct current
     */
    Ifx_Math_PiF16 currentDPi;

    /**
     * Current in the d-q reference frame
     */
    Ifx_Math_CmpFract16 currentDQ;

    /**
     * Instance of PI for the quadrature current
     */
    Ifx_Math_PiF16 currentQPi;

    /**
     * Instance of dqDecoupling
     */
    Ifx_Math_DqDecouplingF16 dqDecoupling;

    /**
     * Contains the module output variables
     */
    Ifx_MDA_FocControllerF16_Output p_output;

    /**
     * Voltage in the d-q reference frame
     */
    Ifx_Math_CmpFract16 voltageDQ;

    /**
     * D-PI controller  antiWindup control
     */
    sint8 p_dAntiwindupCtrl;

    /**
     * Q-PI controller  antiWindup control
     */
    sint8 p_qAntiwindupCtrl;
} Ifx_MDA_FocControllerF16;

/**
 *  \brief Initialize the module to the default values.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *
 */
void Ifx_MDA_FocControllerF16_init(Ifx_MDA_FocControllerF16* self);

/**
 *  \brief Execute the current regulation and output the voltage command.
 *
 *  This API executes the current regulation of the Field Oriented Controller (FOC). It performs the Park transformation
 * and executes the PI controllers of the D and Q currents and outputs the resulting voltage vector in polar
 * coordinates. Additionally, if enabled, it compensates for the d-q decoupling, and if enabled, it limits the amplitude
 * of the output voltage vector according to the available DC link voltage with a D component prioritization.
 *
 *  Inputs to this API:
 *  <ul>
 *      <li>Reference D and Q currents, normalized in Q15 format</li>
 *      <li>Measured alpha and beta currents, normalized in Q15 format</li>
 *      <li>Rotor flux angle between 0 and 2*pi, normalized to  0 to 2^32-1</li>
 *      <li>Electrical speed, needed to compensate for the d-q decoupling, normalized in Q15 format</li>
 *      <li>DC Link voltage, needed to limit the amplitude of the output voltage vector, normalized in Q15 format</li>
 *  </ul>
 *
 *  Outputs to this API:
 *  <ul>
 *      <li>Calculated voltage vector in polar coordinates containing the amplitude normalized in Q15 format and the
 * angle between 0 and 2*pi normalized to 0 to 2^32-1</li>
 *  </ul>
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *  \param [in] currentAlphaBeta Measured current, in alpha-beta reference frame
 *  \param [in] dqCommand Current command, in d-q reference frame
 *  \param [in] rotorFluxAngle Rotor flux angle
 *  \param [in] electricalSpeed Normalized electrical speed
 *  \param [in] dcLinkVoltageQ15 Normalized dcLinkVoltage
 *
 */
void Ifx_MDA_FocControllerF16_execute(Ifx_MDA_FocControllerF16* self, Ifx_Math_CmpFract16 currentAlphaBeta,
                                      Ifx_Math_CmpFract16 dqCommand, uint32 rotorFluxAngle, Ifx_Math_Fract16
                                      electricalSpeed, Ifx_Math_Fract16
                                      dcLinkVoltageQ15);

/**
 *  \brief Resets the FOC controller intermediate variables, outputs and PI controller previous values to 0.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *
 */
void Ifx_MDA_FocControllerF16_reset(Ifx_MDA_FocControllerF16* self);

/**
 *  \brief Get the module output variable, containing the voltage command.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *  \param [out] output Structure containing the module output
 *
 */
static inline void Ifx_MDA_FocControllerF16_getOutput(Ifx_MDA_FocControllerF16       * self,
                                                      Ifx_MDA_FocControllerF16_Output* output)
{
    *output = self->p_output;
}


/**
 *  \brief Get the saturation status of the voltage on the Q axis
 *
 *  Q voltage is in saturation if at least:
 *  <ul>
 *      <li>Q Pi controller is in saturation</li>
 *      <li>Q voltage has exceeded the maximum allowable voltage</li>
 *  </ul>
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *
 *  \return Saturation status of the voltage on the Q axis
 */
static inline sint8 Ifx_MDA_FocControllerF16_getQVoltageSatStatus(Ifx_MDA_FocControllerF16* self)
{
    sint8 qVoltageSatStatus;

    /* Get the saturation status of the Q PI controller */
    sint8 qVoltageSatStatusIntern = Ifx_Math_PiF16_getSaturationStatus(&(self->currentQPi));

    /* Check internal and external saturation status */
    if (self->p_qAntiwindupCtrl == qVoltageSatStatusIntern)
    {
        qVoltageSatStatus = self->p_qAntiwindupCtrl;
    }
    else
    {
        if (self->p_qAntiwindupCtrl * qVoltageSatStatusIntern == -1)
        {
            qVoltageSatStatus = -1;
        }
        else
        {
            qVoltageSatStatus = self->p_qAntiwindupCtrl + qVoltageSatStatusIntern;
        }
    }

    return qVoltageSatStatus;
}


/**
 *  \brief Returns the component ID
 *
 *
 *  \param [out] *componentID Variable to store the address of the component ID
 *
 */
void Ifx_MDA_FocControllerF16_getID(const Ifx_ComponentID** componentID);

/**
 *  \brief Returns the component version
 *
 *
 *  \param [out] *componentVersion Variable to store the address of the component version
 *
 */
void Ifx_MDA_FocControllerF16_getVersion(const Ifx_ComponentVersion** componentVersion);

#endif /*IFX_MDA_FOCCONTROLLERF16_H*/
