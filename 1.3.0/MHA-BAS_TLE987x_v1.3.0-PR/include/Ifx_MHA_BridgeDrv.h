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
 * \file Ifx_MHA_BridgeDrv.h
 * \brief A generalized bridge driver module with attributes and operations common and available to other bridge driver
 * modules.
 */

#ifndef IFX_MHA_BRIDGEDRV_H
#define IFX_MHA_BRIDGEDRV_H
#include "Ifx_Math.h"

/**
 * Macro for fault reaction when fault source is disabled
 */
#define IFX_MHA_BRIDGEDRV_FAULT_REACTION_DISABLE      (0)

/**
 * Macro for fault reaction when fault source is enabled
 */
#define IFX_MHA_BRIDGEDRV_FAULT_REACTION_ENABLE       (1)

/**
 * Macro for fault reaction when fault source is report only
 */
#define IFX_MHA_BRIDGEDRV_FAULT_REACTION_REPORT_ONLY  (2)

/**
 * Macro for fault reaction when fault source is report and react
 */
#define IFX_MHA_BRIDGEDRV_FAULT_REACTION_REPORT_REACT (3)

/**
 * \brief Data structure that stores all data of module instance.
 *
 */
typedef struct Ifx_MHA_BridgeDrv
{
    /**
     * State machine enable setting
     */
    bool p_enable;

    /**
     * State machine fault clear setting
     */
    bool p_clearFault;
} Ifx_MHA_BridgeDrv;

/**
 *  \brief Initialize the bridge driver to the default settings.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *
 */
void Ifx_MHA_BridgeDrv_init(Ifx_MHA_BridgeDrv* self);

/**
 *  \brief Advance the state machine of the module.
 *
 *  This API handles the state machine of the module, as described by the available state-diagram. Additionally, it
 * handles the faults as specified per configuration.
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *
 */
void Ifx_MHA_BridgeDrv_execute(Ifx_MHA_BridgeDrv* self);

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
 */
static inline void Ifx_MHA_BridgeDrv_enable(Ifx_MHA_BridgeDrv* self, bool enable)
{
    self->p_enable = enable;
}


/**
 *  \brief Clears all faults from the module.
 * The module will be set to enable or disable state in the next call to execute() depending on the enable setting.
 * New hardware fault will lead the module to fault state again.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *
 */
static inline void Ifx_MHA_BridgeDrv_clearFault(Ifx_MHA_BridgeDrv* self)
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
static inline bool Ifx_MHA_BridgeDrv_clearFaultIsPending(Ifx_MHA_BridgeDrv* self)
{
    return self->p_clearFault;
}


#endif /*IFX_MHA_BRIDGEDRV_H*/
