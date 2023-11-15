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
 * \file Ifx_MDA_StartAngleIdentF16.h
 * \brief This module is used to estimate the electrical rotor angle at standstill (with an error of 30 degrees).
 */

#ifndef IFX_MDA_STARTANGLEIDENTF16_H
#define IFX_MDA_STARTANGLEIDENTF16_H

#include "Ifx_ComponentID.h"
#include "Ifx_ComponentVersion.h"
#include "Ifx_Math.h"

/**
 * Macro for fault reaction when fault source is disabled
 */
#define IFX_MDA_STARTANGLEIDENTF16_FAULT_REACTION_DISABLE      (0)

/**
 * Macro for fault reaction when fault source is enabled
 */
#define IFX_MDA_STARTANGLEIDENTF16_FAULT_REACTION_ENABLE       (1)

/**
 * Macro for fault reaction when fault source is report only
 */
#define IFX_MDA_STARTANGLEIDENTF16_FAULT_REACTION_REPORT_ONLY  (2)

/**
 * Macro for fault reaction when fault source is report and react
 */
#define IFX_MDA_STARTANGLEIDENTF16_FAULT_REACTION_REPORT_REACT (3)

/**
 * Contains the module output variables
 */
typedef struct Ifx_MDA_StartAngleIdentF16_Output
{
    /**
     * Compare values for the switches: first three up count, second three down count
     */
    uint16 compareValues_tick[6];

    /**
     * Array of  trigger events
     */
    uint16 triggerTime_tick[2];

    /**
     * Estimated initial rotor electrical angle, normalized by 2*pi
     * <table>
     *  <caption>Parameter Representation</caption>
     *      <tr>
     *          <th>Name</th>
     *          <th>Attribute</th>
     *          <th>Value</th>
     *      </tr>
     *      <tr>
     *          <td rowspan="6">estimatedAngle</td>
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
    uint32 estimatedAngle;

    /**
     * Maximum phase admittance value
     */
    Ifx_Math_Fract32 maxAdmittance;
} Ifx_MDA_StartAngleIdentF16_Output;

/**
 * States of the module state machine
 */
typedef enum Ifx_MDA_StartAngleIdentF16_State
{
    Ifx_MDA_StartAngleIdentF16_State_init            = 0, /**<Initial state*/
    Ifx_MDA_StartAngleIdentF16_State_off             = 1, /**<Off state*/
    Ifx_MDA_StartAngleIdentF16_State_on              = 2, /**<On state*/
    Ifx_MDA_StartAngleIdentF16_State_estimationReady = 3, /**<State when the estimation is finished*/
    Ifx_MDA_StartAngleIdentF16_State_fault           = 4  /**<State machine is in fault state. Output is set based on
                                                           * the fault output behavior configuration.*/
} Ifx_MDA_StartAngleIdentF16_State;

/**
 * Status of the module containing the state and the error flag​.
 */
typedef struct Ifx_MDA_StartAngleIdentF16_Status
{
    /**
     * State of the module
     */
    Ifx_MDA_StartAngleIdentF16_State state;

    /**
     * Voltage fluctuation error flag
     */
    bool voltageFluctuation;
} Ifx_MDA_StartAngleIdentF16_Status;

/**
 * \brief Data structure that stores all data of module instance.
 *
 */
typedef struct Ifx_MDA_StartAngleIdentF16
{
    /**
     * Contains the module output variables
     */
    Ifx_MDA_StartAngleIdentF16_Output p_output;

    /**
     * Accumulator for the phase admittance
     */
    Ifx_Math_Fract32 p_phaseAdmittanceAccumulator[6];

    /**
     * Array containing 6 admittance measurements (one for each angle)
     */
    Ifx_Math_Fract32 p_phaseAdmittance[6];

    /**
     * Status of the module
     */
    Ifx_MDA_StartAngleIdentF16_Status p_status;

    /**
     * Voltage accumulator
     */
    Ifx_Math_Fract32 p_voltageAccumulator;

    /**
     * First current measurement
     */
    Ifx_Math_Fract16 p_firstCurrent;

    /**
     * Second current measurement
     */
    Ifx_Math_Fract16 p_secondCurrent;

    /**
     * Value of the DC-Link voltage in the previous cycle
     */
    Ifx_Math_Fract16 p_previousDcLinkVoltageQ15;

    /**
     * Pulse counter
     */
    uint8 p_pulseCounter;

    /**
     * Phase counter
     */
    uint8 p_phaseCounter;

    /**
     * Averaging counter
     */
    uint8 p_averageCounter;

    /**
     * State machine enable setting
     */
    bool p_enable;

    /**
     * State machine clear fault setting
     */
    bool p_clearFault;

    /**
     * Flag to indicate where fault checking is active
     */
    bool p_faultCheckingActive;
} Ifx_MDA_StartAngleIdentF16;

/**
 *  \brief Initializes the module.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *
 */
void Ifx_MDA_StartAngleIdentF16_init(Ifx_MDA_StartAngleIdentF16* self);

/**
 *  \brief Execute the state machine
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *  \param [in] dcLinkVoltageQ15
 *  \param [inout] shuntCurrentsQ15
 *
 */
void Ifx_MDA_StartAngleIdentF16_execute(Ifx_MDA_StartAngleIdentF16* self, Ifx_Math_Fract16 dcLinkVoltageQ15,
                                        Ifx_Math_Fract16* shuntCurrentsQ15);

/**
 *  \brief Returns the status of the module
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *
 *  \return
 */
static inline Ifx_MDA_StartAngleIdentF16_Status Ifx_MDA_StartAngleIdentF16_getStatus(Ifx_MDA_StartAngleIdentF16* self)
{
    return self->p_status;
}


/**
 *  \brief Returns the output data structure
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *  \param [inout] output
 *
 */
static inline void Ifx_MDA_StartAngleIdentF16_getOutput(Ifx_MDA_StartAngleIdentF16       * self,
                                                        Ifx_MDA_StartAngleIdentF16_Output* output)
{
    *output = self->p_output;
}


/**
 *  \brief Enable the module
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *  \param [in] enable
 *
 */
static inline void Ifx_MDA_StartAngleIdentF16_enable(Ifx_MDA_StartAngleIdentF16* self, bool enable)
{
    self->p_enable = enable;
}


/**
 *  \brief Clears all faults from the module.
 * The module will be set to disable state in the next call to execute().
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *
 */
static inline void Ifx_MDA_StartAngleIdentF16_clearFault(Ifx_MDA_StartAngleIdentF16* self)
{
    self->p_clearFault = true;
}


/**
 *  \brief Check if a clear fault request is pending.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *
 *  \return True if a clear fault request was made but still not processed by the module and False otherwise
 */
static inline bool Ifx_MDA_StartAngleIdentF16_clearFaultIsPending(Ifx_MDA_StartAngleIdentF16* self)
{
    return self->p_clearFault;
}


/**
 *  \brief Returns the component ID
 *
 *
 *  \param [out] *componentID Variable to store the address of the component ID
 *
 */
void Ifx_MDA_StartAngleIdentF16_getID(const Ifx_ComponentID** componentID);

/**
 *  \brief Returns the component version
 *
 *
 *  \param [out] *componentVersion Variable to store the address of the component version
 *
 */
void Ifx_MDA_StartAngleIdentF16_getVersion(const Ifx_ComponentVersion** componentVersion);

#endif /*IFX_MDA_STARTANGLEIDENTF16_H*/
