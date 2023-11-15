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
 * \file Ifx_MDA_VToFControllerF16.h
 * \brief The V/f (also called voltage to frequency) scalar control module.
 */

#ifndef IFX_MDA_VTOFCONTROLLERF16_H
#define IFX_MDA_VTOFCONTROLLERF16_H
#include "Ifx_ComponentID.h"
#include "Ifx_ComponentVersion.h"
#include "Ifx_MDA_VToFControllerF16_Cfg.h"
#include "Ifx_Math.h"

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
#define IFX_MDA_VTOFCONTROLLERF16_BASE_VOLTAGE_V (IFX_MDA_VTOFCONTROLLERF16_CFG_BASE_VOLTAGE_V)

/**
 * Structure to hold the normalized parameters of the V/f LUT, in Q15
 */
typedef struct Ifx_MDA_VToFControllerF16_VToFLutNorm
{
    /**
     * Normalized rated speed
     */
    Ifx_Math_Fract16 p_ratedSpeedQ15;

    /**
     * Normalized corner speed
     */
    Ifx_Math_Fract16 p_cornerSpeedQ15;

    /**
     * Normalized minimum voltage
     */
    Ifx_Math_Fract16 p_minVoltQ15;

    /**
     * Normalized corner voltage
     */
    Ifx_Math_Fract16 p_cornerVoltQ15;

    /**
     * Normalized rated voltage
     */
    Ifx_Math_Fract16 p_ratedVoltQ15;
} Ifx_MDA_VToFControllerF16_VToFLutNorm;

/**
 * Contains the module output variables.
 */
typedef struct Ifx_MDA_VToFControllerF16_Output
{
    /**
     * Angle and normalized amplitude of the output voltage vector, in Q15
     * <table>
     *  <caption>Parameter Representation</caption>
     *      <tr>
     *          <th>Name</th>
     *          <th>Attribute</th>
     *          <th>Value</th>
     *      </tr>
     *      <tr>
     *          <td rowspan="6">voltageVector.amplitude</td>
     *          <td>Fractional bits</td>
     *          <td>15</td>
     *      </tr>
     *      <tr>
     *          <td>Gain</td>
     *          <td>IFX_MDA_VTOFCONTROLLERF16_BASE_VOLTAGE_V</td>
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
     *          <td rowspan="6">voltageVector.angle</td>
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
    Ifx_Math_PolarFract16 voltageVector;
} Ifx_MDA_VToFControllerF16_Output;

/**
 * \brief Data structure that stores all data of module instance.
 *
 */
typedef struct Ifx_MDA_VToFControllerF16
{
    /**
     * VToF lookup table object containing the normalized points of the LUT
     */
    Ifx_MDA_VToFControllerF16_VToFLutNorm p_VToFLUTnorm;

    /**
     * Contains the module output variables
     */
    Ifx_MDA_VToFControllerF16_Output p_output;

    /**
     * Previous value (state) for the angle integrator
     */
    uint32 p_anglePreviousValue;

    /**
     * Angle increment value, in Q14
     */
    Ifx_Math_Fract16 p_angleIncrementQ14;
} Ifx_MDA_VToFControllerF16;

/**
 *  \brief The initialization API of the V2f module.
 *
 *  The normalized parameters are initialized according to the data entered in Config Wizard.
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *
 */
void Ifx_MDA_VToFControllerF16_init(Ifx_MDA_VToFControllerF16* self);

/**
 *  \brief The cyclic execution API of the VToF module.
 *
 *  This API calculates the angle by integrating the speed with respect to time, and calculates the command voltage
 * output based on the input referece speed by linear interpolation, using the defined lookup table VToFLUT.
 *
 *  Inputs of this API
 *  <ul>
 *      <li>Reference speed, normalized by IFX_MDA_VTOFCONTROLLERF16_CFG_BASE_SPEED_RADPS in Q15 format</li>
 *  </ul>
 *
 *  Outputs of this API
 *  <ul>
 *      <li>Calculated voltage amplitude, normalized by IFX_MDA_VTOFCONTROLLERF16_CFG_BASE_VOLTAGE_V in Q15 format</li>
 *      <li>Calculated angle between 0 and 2*pi, normalized to  0 to 2^32-1</li>
 *  </ul>
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *  \param [in] speedQ15 Input speed, normalized
 *
 */
void Ifx_MDA_VToFControllerF16_execute(Ifx_MDA_VToFControllerF16* self, Ifx_Math_Fract16 speedQ15);

/**
 *  \brief Set the previous value of the angle.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *  \param [in] anglePreviousValue The value to which the previous value of the angle must be set
 *
 *  <table>
 *      <caption>Parameter Representation</caption>
 *          <tr>
 *              <th>Name</th>
 *              <th>Attribute</th>
 *              <th>Value</th>
 *          </tr>
 *          <tr>
 *              <td rowspan="6">setAnglePreviousValue</td>
 *              <td>Fractional bits</td>
 *              <td>32</td>
 *          </tr>
 *          <tr>
 *              <td>Gain</td>
 *              <td>6.2832</td>
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
 *              <td>[0, 4294967295]</td>
 *          </tr>
 *          <tr>
 *              <td>Unit</td>
 *              <td>rad</td>
 *          </tr>
 *  </table>
 *
 */
static inline void Ifx_MDA_VToFControllerF16_setAnglePreviousValue(Ifx_MDA_VToFControllerF16* self, uint32
                                                                   anglePreviousValue)
{
    self->p_anglePreviousValue = anglePreviousValue;
}


/**
 *  \brief Get the angle previous value.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *
 *  \return The angle previous value
 *  <table>
 *      <caption>Parameter Representation</caption>
 *          <tr>
 *              <th>Name</th>
 *              <th>Attribute</th>
 *              <th>Value</th>
 *          </tr>
 *          <tr>
 *              <td rowspan="6">getAnglePreviousValue</td>
 *              <td>Fractional bits</td>
 *              <td>32</td>
 *          </tr>
 *          <tr>
 *              <td>Gain</td>
 *              <td>6.2832</td>
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
 *              <td>[0, 4294967295]</td>
 *          </tr>
 *          <tr>
 *              <td>Unit</td>
 *              <td>rad</td>
 *          </tr>
 *  </table>
 *
 */
static inline uint32 Ifx_MDA_VToFControllerF16_getAnglePreviousValue(Ifx_MDA_VToFControllerF16* self)
{
    return self->p_anglePreviousValue;
}


/**
 *  \brief Get the module output variables, containing the voltage vector.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *  \param [out] output Structure containing the module output
 *
 */
static inline void Ifx_MDA_VToFControllerF16_getOutput(Ifx_MDA_VToFControllerF16       * self,
                                                       Ifx_MDA_VToFControllerF16_Output* output)
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
void Ifx_MDA_VToFControllerF16_getID(const Ifx_ComponentID** componentID);

/**
 *  \brief Returns the component version
 *
 *
 *  \param [out] *componentVersion Variable to store the address of the component version
 *
 */
void Ifx_MDA_VToFControllerF16_getVersion(const Ifx_ComponentVersion** componentVersion);

#endif /*IFX_MDA_VTOFCONTROLLERF16_H*/
