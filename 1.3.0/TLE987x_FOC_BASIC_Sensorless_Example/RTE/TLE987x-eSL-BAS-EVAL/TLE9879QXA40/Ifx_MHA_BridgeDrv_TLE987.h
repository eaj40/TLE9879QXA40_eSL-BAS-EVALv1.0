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
 * \file Ifx_MHA_BridgeDrv_TLE987.h
 * \brief A specialized bridge driver module for the TLE987x devices.
 */

#ifndef IFX_MHA_BRIDGEDRV_TLE987_H
#define IFX_MHA_BRIDGEDRV_TLE987_H

#include "Ifx_ComponentID.h"
#include "Ifx_ComponentVersion.h"
#include "Ifx_MHA_BridgeDrv.h"
#include "bdrv.h"

/**
 * Bridge driver state options
 */
typedef enum Ifx_MHA_BridgeDrv_TLE987_State
{
    Ifx_MHA_BridgeDrv_TLE987_State_init  = 0, /**<Bridge driver is initializing. Drivers are powered off at the end of
                                               * initialization.*/
    Ifx_MHA_BridgeDrv_TLE987_State_off   = 1, /**<Bridge driver is disabled. Drivers are powered off.*/
    Ifx_MHA_BridgeDrv_TLE987_State_on    = 2, /**<Bridge driver is enabled. Drivers are controlled by external input.*/
    Ifx_MHA_BridgeDrv_TLE987_State_fault = 3  /**<Bridge Driver is in fault. Drivers are in the state configured by
                                               * faultOutputBehavior.*/
} Ifx_MHA_BridgeDrv_TLE987_State;

/**
 * Status of the bridge driver, containing the bit coded errors and the state machine state
 */
typedef struct Ifx_MHA_BridgeDrv_TLE987_Status
{
    /**
     * State of the bridge driver
     */
    Ifx_MHA_BridgeDrv_TLE987_State state;

    /**
     * Overcurrent status flag
     */
    bool overcurrent;

    /**
     * Overvoltage status flag
     * This flag is set to 1 if any of the following bits in register BDRV_IS is set:
     * <ul>
     *  <li>bit 28 (VSD_UPTH_STS): VSD Upper Threshold Measurement (ADC2 channel 2) Status;</li>
     *  <li>bit 26 (VCP_UPTH_STS): VCP Upper Threshold Measurement (ADC2 channel 3) Status;</li>
     *  <li>bit 20 (VSD_UPTH_IS): VSD Upper Threshold Measurement (ADC2 channel 2) Interrupt Status;</li>
     *  <li>bit 18 (VCP_UPTH_IS): VCP Upper Threshold Measurement (ADC2 channel 3) Interrupt Status.</li>
     * </ul>
     */
    bool overvoltage;

    /**
     * Undervoltage status flag
     * This flag is set to 1 if any of the following bits in register BDRV_IS is set:
     * <ul>
     *  <li>bit 27 (VSD_LOWTH_STS): VSD Lower Threshold Measurement (ADC2 channel 2) Status;</li>
     *  <li>bit 25 (VCP_LOWTH1_STS): VCP Lower Threshold 1 Measurement (ADC2 channel 3) Status;</li>
     *  <li>bit 24 (VCP_LOWTH2_STS): VCP Lower Threshold 2 Measurement (VCP_LOW Signal from CP) Status;</li>
     *  <li>bit 19 (VSD_LOWTH_IS): VSD Lower Threshold Measurement (ADC2 channel 2) Interrupt Status;</li>
     *  <li>bit 17 (VCP_LOWTH1_IS): VCP Lower Threshold 1 Measurement (ADC2 channel 3) Interrupt Status;</li>
     *  <li>bit 16 (VCP_LOWTH2_IS): VCP Lower Threshold 2 Measurement (VCP_LOW Signal from CP) Interrupt Status.</li>
     * </ul>
     */
    bool undervoltage;
} Ifx_MHA_BridgeDrv_TLE987_Status;

/**
 * \brief Data structure that stores all data of module instance.
 *
 */
typedef struct Ifx_MHA_BridgeDrv_TLE987
{
    /**
     * Structure inherited from Ifx_MHA_BridgeDrv
     */
    Ifx_MHA_BridgeDrv _Super_Ifx_MHA_BridgeDrv;

    /**
     * Status of the bridge driver, containing the bit coded errors and the state machine state.
     */
    Ifx_MHA_BridgeDrv_TLE987_Status p_status;
} Ifx_MHA_BridgeDrv_TLE987;

/**
 *  \brief Advance the state machine of the module.
 *
 *  This API handles the state machine of the module, as described by the available state-diagram. Additionally, it
 * handles the faults as specified per configuration.
 *
 *  Inputs of this API:
 *  <ul>
 *      <li>HW faults, that are read via the SDK function from the bridge driver fault detection status HW register
 * BDRV_IS</li>
 *      <li>Only the faults which are configured for FAULT_REPORT_REACT are considered by fault detection (Bridge
 * drivers overcurrent, overvoltage and undervoltage)</li>
 *      <li>State machine "Enable" input, boolean data that is Enabled/Disabled by a call to the
 * Ifx_BDrv_TLE987_enable(true/false)</li>
 *  </ul>
 *
 *  Outputs of this API:
 *  <ul>
 *      <li>State variable: encoded on the "Status" output</li>
 *      <li>Error code: encoded on the "Status" output</li>
 *  </ul>
 *
 *  These outputs are set corresponding to the states and state transitions design. A fault, if it is not disabled, will
 * be detected and the configured action will be taken within 3 calls to this function.
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *
 */
void Ifx_MHA_BridgeDrv_TLE987_execute(Ifx_MHA_BridgeDrv_TLE987* self);

/**
 *  \brief Initialize the bridge driver to the default settings.
 *
 *  The peripheral initialization should be done by the user before calling this function.
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *
 */
void Ifx_MHA_BridgeDrv_TLE987_init(Ifx_MHA_BridgeDrv_TLE987* self);

/**
 *  \brief Returns the component ID
 *
 *
 *  \param [out] *componentID Variable to store the address of the component ID
 *
 */
void Ifx_MHA_BridgeDrv_TLE987_getID(const Ifx_ComponentID** componentID);

/**
 *  \brief Returns the component version
 *
 *
 *  \param [out] *componentVersion Variable to store the address of the component version
 *
 */
void Ifx_MHA_BridgeDrv_TLE987_getVersion(const Ifx_ComponentVersion** componentVersion);

/**
 *  \brief Get the status of the bridge driver, containing the state machine state and the bit coded errors.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *
 *  \return Bridge driver status
 */
static inline Ifx_MHA_BridgeDrv_TLE987_Status Ifx_MHA_BridgeDrv_TLE987_getStatus(Ifx_MHA_BridgeDrv_TLE987* self)
{
    return self->p_status;
}


/**
 *  \brief Clears all faults from the module.
 * The module will be set to enable or disable state in the next call to execute() depending on the enable setting.
 * New hardware fault will lead the module to fault state again.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *
 *  \note
 *  Inherited from Ifx_MHA_BridgeDrv
 */
static inline void Ifx_MHA_BridgeDrv_TLE987_clearFault(Ifx_MHA_BridgeDrv_TLE987* self)
{
    Ifx_MHA_BridgeDrv_clearFault(&(self->_Super_Ifx_MHA_BridgeDrv));
}


/**
 *  \brief Check if a clear fault request is pending.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *
 *  \return True if a clear fault request was made but still not processed by the module and False otherwise
 *  \note
 *  Inherited from Ifx_MHA_BridgeDrv
 */
static inline bool Ifx_MHA_BridgeDrv_TLE987_clearFaultIsPending(Ifx_MHA_BridgeDrv_TLE987* self)
{
    return Ifx_MHA_BridgeDrv_clearFaultIsPending(&(self->_Super_Ifx_MHA_BridgeDrv));
}


/**
 *  \brief This API enables or disables the module based on the input parameter enable.
 * If the input parameter is TRUE and no fault is detected, then the module will be enabled in the next call to
 * execute().
 * This API also set the HW-related members of the internal "self" data structure to the corresponding HW registers
 * according to the user manual.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *  \param [in] enable Parameter of boolean type to enable or disable the module
 *
 *  \note
 *  Inherited from Ifx_MHA_BridgeDrv
 */
static inline void Ifx_MHA_BridgeDrv_TLE987_enable(Ifx_MHA_BridgeDrv_TLE987* self, bool enable)
{
    Ifx_MHA_BridgeDrv_enable(&(self->_Super_Ifx_MHA_BridgeDrv), enable);
}


#endif /*IFX_MHA_BRIDGEDRV_TLE987_H*/
