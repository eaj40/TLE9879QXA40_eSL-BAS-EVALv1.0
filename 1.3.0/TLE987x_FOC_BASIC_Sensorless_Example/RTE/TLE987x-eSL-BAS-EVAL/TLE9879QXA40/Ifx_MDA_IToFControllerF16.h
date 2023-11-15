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
 * \file Ifx_MDA_IToFControllerF16.h
 * \brief I/f (also known as current to frequency) scalar control module.
 */

#ifndef IFX_MDA_ITOFCONTROLLERF16_H
#define IFX_MDA_ITOFCONTROLLERF16_H
#include "Ifx_ComponentID.h"
#include "Ifx_ComponentVersion.h"
#include "Ifx_MDA_IToFControllerF16_Cfg.h"
#include "Ifx_Math.h"

/**
 * Base current, in A
 * <table>
 *  <caption>Parameter Representation</caption>
 *      <tr>
 *          <th>Name</th>
 *          <th>Attribute</th>
 *          <th>Value</th>
 *      </tr>
 *      <tr>
 *          <td rowspan="6">BASE_CURRENT_A</td>
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
 *          <td>A</td>
 *      </tr>
 * </table>
 *
 */
#define IFX_MDA_ITOFCONTROLLERF16_BASE_CURRENT_A (IFX_MDA_ITOFCONTROLLERF16_CFG_BASE_CURRENT_A)

/**
 * Contains the module output variables.
 */
typedef struct Ifx_MDA_IToFControllerF16_Output
{
    /**
     * Real and imaginary reference currents as outputs of the IToF module, normalized by the base current, in Q15
     * format
     * <table>
     *  <caption>Parameter Representation</caption>
     *      <tr>
     *          <th>Name</th>
     *          <th>Attribute</th>
     *          <th>Value</th>
     *      </tr>
     *      <tr>
     *          <td rowspan="6">refCurrent.real</td>
     *          <td>Fractional bits</td>
     *          <td>15</td>
     *      </tr>
     *      <tr>
     *          <td>Gain</td>
     *          <td>IFX_MDA_ITOFCONTROLLERF16_BASE_CURRENT_A</td>
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
     *          <td>A</td>
     *      </tr><tr>
     *          <td rowspan="6">refCurrent.imag</td>
     *          <td>Fractional bits</td>
     *          <td>15</td>
     *      </tr>
     *      <tr>
     *          <td>Gain</td>
     *          <td>IFX_MDA_ITOFCONTROLLERF16_BASE_CURRENT_A</td>
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
     *          <td>A</td>
     *      </tr>
     * </table>
     *
     */
    Ifx_Math_CmpFract16 refCurrent;

    /**
     * Angle of the output current vector
     * <table>
     *  <caption>Parameter Representation</caption>
     *      <tr>
     *          <th>Name</th>
     *          <th>Attribute</th>
     *          <th>Value</th>
     *      </tr>
     *      <tr>
     *          <td rowspan="6">currentVecAngle_rad</td>
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
    uint32 currentVecAngle_rad;
} Ifx_MDA_IToFControllerF16_Output;

/**
 * \brief Data structure that stores all data of module instance.
 *
 */
typedef struct Ifx_MDA_IToFControllerF16
{
    /**
     * Contains the module output variables
     */
    Ifx_MDA_IToFControllerF16_Output p_output;

    /**
     * Previous value (state) for the angle integrator
     */
    uint32 p_anglePreviousValue;

    /**
     * Angle increment value, in Q14
     */
    Ifx_Math_Fract16 p_angleIncrementQ14;
} Ifx_MDA_IToFControllerF16;

/**
 *  \brief The initialization API of the IToF module.
 *
 *  The normalized parameters are initialized according to the data entered in Config Wizard.
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *
 */
void Ifx_MDA_IToFControllerF16_init(Ifx_MDA_IToFControllerF16* self);

/**
 *  \brief The cyclic execution API of the IToF module.
 *
 *  This API calculates the speed by integrating the input angle with time.
 *
 *  Inputs of this API:
 *  <ul>
 *      <li>Reference speed normalized by IFX_MDA_ITOFCONTROLLERF16_CFG_BASE_SPEED_RADPS in Q15 format</li>
 *  </ul>
 *
 *  Outputs of this API:
 *  <ul>
 *      <li>Calculated angle between 0 and 2*pi, normalized to  0 to 2^32-1</li>
 *  </ul>
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *  \param [in] speedQ15 Input speed, normalized
 *
 */
void Ifx_MDA_IToFControllerF16_execute(Ifx_MDA_IToFControllerF16* self, Ifx_Math_Fract16 speedQ15);

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
static inline void Ifx_MDA_IToFControllerF16_setAnglePreviousValue(Ifx_MDA_IToFControllerF16* self, uint32
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
static inline uint32 Ifx_MDA_IToFControllerF16_getAnglePreviousValue(Ifx_MDA_IToFControllerF16* self)
{
    return self->p_anglePreviousValue;
}


/**
 *  \brief The API  adds 180 degree in the current value of angle in IToF module.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *
 */
void Ifx_MDA_IToFControllerF16_addOneEightyDegreeInAngle(Ifx_MDA_IToFControllerF16* self);

/**
 *  \brief Get module output variables.
 * The output contains the d-q reference current and the previous value of the generated angle.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *  \param [out] output Structure containing the module output
 *
 */
static inline void Ifx_MDA_IToFControllerF16_getOutput(Ifx_MDA_IToFControllerF16       * self,
                                                       Ifx_MDA_IToFControllerF16_Output* output)
{
    *output = self->p_output;
}


/**
 *  \brief Set the reference current of the Q axis.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *  \param [in] refCurrentImagQ15 imaginary reference current, normalized by the base current, in Q15
 *
 */
static inline void Ifx_MDA_IToFControllerF16_setReferenceCurrentImag(Ifx_MDA_IToFControllerF16* self, Ifx_Math_Fract16
                                                                     refCurrentImagQ15)
{
    self->p_output.refCurrent.imag = refCurrentImagQ15;
}


/**
 *  \brief Get the reference current of the Q axis.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *
 *  \return imaginary reference current
 */
static inline Ifx_Math_Fract16 Ifx_MDA_IToFControllerF16_getReferenceCurrentImag(Ifx_MDA_IToFControllerF16* self)
{
    return self->p_output.refCurrent.imag;
}


/**
 *  \brief Returns the component ID
 *
 *
 *  \param [out] *componentID Variable to store the address of the component ID
 *
 */
void Ifx_MDA_IToFControllerF16_getID(const Ifx_ComponentID** componentID);

/**
 *  \brief Returns the component version
 *
 *
 *  \param [out] *componentVersion Variable to store the address of the component version
 *
 */
void Ifx_MDA_IToFControllerF16_getVersion(const Ifx_ComponentVersion** componentVersion);

#endif /*IFX_MDA_ITOFCONTROLLERF16_H*/
