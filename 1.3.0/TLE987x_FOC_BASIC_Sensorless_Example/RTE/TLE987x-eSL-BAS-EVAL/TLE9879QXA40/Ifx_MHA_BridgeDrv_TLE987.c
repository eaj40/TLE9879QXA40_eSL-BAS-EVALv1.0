/*
 * Copyright (c) 2022 Infineon Technologies AG. All Rights Reserved.
 *
 * Use of this file is subject to the terms of use agreed between (i) you or the company in which ordinary course of
 * business you are acting and (ii) Infineon Technologies AG, its affiliates or its licensees. If and as long as no
 * such terms of use are agreed, use of this file is subject to the Evaluation Software License Agreement distributed
 * along with this file within the software delivery package.
 *
 */

#include "Ifx_MHA_BridgeDrv_TLE987.h"
#include "Ifx_MHA_BridgeDrv_Cfg.h"
#include "bdrv.h"

/* Mask for unveiling all fault status-bits within the BDRV_IS register,
 * except of Pre-Driver short Interrupt Status bits and excepting reserved bits */
#define IFX_MHA_BRIDGEDRV_TLE987_IS_UMASK_OC_OV_UV_FAULTS (0x1F1FFC00UL)

/* Mask for unveiling the Over-Current status-bits within the BDRV_IS register */
#define IFX_MHA_BRIDGEDRV_TLE987_IS_UMASK_OC_FAULT        (0x0000FC00UL)

/* Mask for unveiling the Over-Voltage status-bits within the BDRV_IS register */
#define IFX_MHA_BRIDGEDRV_TLE987_IS_UMASK_OV_FAULT        (0x14140000UL)

/* Mask for unveiling the Under-Voltage status-bits within the BDRV_IS register */
#define IFX_MHA_BRIDGEDRV_TLE987_IS_UMASK_UV_FAULT        (0x0B0B0000UL)

/* Macros to define the component ID */
#define IFX_MHA_BRIDGEDRV_TLE987_COMPONENTID_SOURCEID     ((uint8)Ifx_ComponentID_SourceID_infineonTechnologiesAG)
#define IFX_MHA_BRIDGEDRV_TLE987_COMPONENTID_LIBRARYID    ((uint16)Ifx_ComponentID_LibraryID_mctrlHardwareAbstraction)

#define IFX_MHA_BRIDGEDRV_TLE987_COMPONENTID_MODULEID     (0U)
#define IFX_MHA_BRIDGEDRV_TLE987_COMPONENTID_COMPONENTID1 (1U)

#define IFX_MHA_BRIDGEDRV_TLE987_COMPONENTID_COMPONENTID2 ((uint8)Ifx_ComponentID_ComponentID2_basic)

/* Macros to define the component version */
#define IFX_MHA_BRIDGEDRV_TLE987_COMPONENTVERSION_MAJOR   (1U)
#define IFX_MHA_BRIDGEDRV_TLE987_COMPONENTVERSION_MINOR   (3U)
#define IFX_MHA_BRIDGEDRV_TLE987_COMPONENTVERSION_PATCH   (0U)
#define IFX_MHA_BRIDGEDRV_TLE987_COMPONENTVERSION_T       (0U)
#define IFX_MHA_BRIDGEDRV_TLE987_COMPONENTVERSION_REV     (0U)

/* *INDENT-OFF* */
/* Component ID */
static const Ifx_ComponentID      Ifx_MHA_BridgeDrv_TLE987_componentID = {
    .sourceID     = IFX_MHA_BRIDGEDRV_TLE987_COMPONENTID_SOURCEID,
    .libraryID    = IFX_MHA_BRIDGEDRV_TLE987_COMPONENTID_LIBRARYID,
    .moduleID     = IFX_MHA_BRIDGEDRV_TLE987_COMPONENTID_MODULEID,
    .componentID1 = IFX_MHA_BRIDGEDRV_TLE987_COMPONENTID_COMPONENTID1,
    .componentID2 = IFX_MHA_BRIDGEDRV_TLE987_COMPONENTID_COMPONENTID2,
};

/* Component Version */
static const Ifx_ComponentVersion Ifx_MHA_BridgeDrv_TLE987_componentVersion = {
    .major = IFX_MHA_BRIDGEDRV_TLE987_COMPONENTVERSION_MAJOR,
    .minor = IFX_MHA_BRIDGEDRV_TLE987_COMPONENTVERSION_MINOR,
    .patch = IFX_MHA_BRIDGEDRV_TLE987_COMPONENTVERSION_PATCH,
    .t     = IFX_MHA_BRIDGEDRV_TLE987_COMPONENTVERSION_T,
    .rev   = IFX_MHA_BRIDGEDRV_TLE987_COMPONENTVERSION_REV
};
/* *INDENT-ON* */
/* Checks if any fault occurred and acts accordingly */
static inline bool Ifx_MHA_BridgeDrv_TLE987_checkFaultStatus(Ifx_MHA_BridgeDrv_TLE987* self, bool clearFault);

/* Check HW faults of bridge driver */
static inline bool Ifx_MHA_BridgeDrv_TLE987_checkHwFaults(bool* faultOverCurrent, bool* faultOverVoltage,
                                                          bool* faultUnderVoltage, bool clearFault);

/* Bridge driver state machine implementation */
static inline void Ifx_MHA_BridgeDrv_TLE987_stateMachine(Ifx_MHA_BridgeDrv_TLE987* self);

/* Disable and enable the bridge driver */
static inline void Ifx_MHA_BridgeDrv_TLE987_actionDisable(void);
static inline void Ifx_MHA_BridgeDrv_TLE987_actionEnable(void);

/* Bridge driver in init state*/
static inline Ifx_MHA_BridgeDrv_TLE987_State Ifx_MHA_BridgeDrv_TLE987_stateInit(Ifx_MHA_BridgeDrv_TLE987* self);

/* Bridge driver in on state*/
static inline Ifx_MHA_BridgeDrv_TLE987_State Ifx_MHA_BridgeDrv_TLE987_stateOn(Ifx_MHA_BridgeDrv_TLE987* self, bool
                                                                              clearFault);

/* Bridge driver in off state*/
static inline Ifx_MHA_BridgeDrv_TLE987_State Ifx_MHA_BridgeDrv_TLE987_stateOff(Ifx_MHA_BridgeDrv_TLE987* self, bool
                                                                               clearFault);

/* Bridge driver in fault state*/
static inline Ifx_MHA_BridgeDrv_TLE987_State Ifx_MHA_BridgeDrv_TLE987_stateFault(Ifx_MHA_BridgeDrv_TLE987* self, bool
                                                                                 clearFault);

/* Temporary function to read the faults from HW */
static inline uint32 Ifx_MHA_BridgeDrv_TLE987_readFaults(void)
{
    return SCUPM->BDRV_IS.reg & IFX_MHA_BRIDGEDRV_TLE987_IS_UMASK_OC_OV_UV_FAULTS;
}


/* polyspace-begin MISRA2012:D4.14 [Justified:Low] "The caller function has to guarantee that NULL is not passed as
 * argument." */

/* Function to get the component ID */
void Ifx_MHA_BridgeDrv_TLE987_getID(const Ifx_ComponentID** componentID)
{
    *componentID = &Ifx_MHA_BridgeDrv_TLE987_componentID;
}


/* Function to get the component version */
void Ifx_MHA_BridgeDrv_TLE987_getVersion(const Ifx_ComponentVersion** componentVersion)
{
    *componentVersion = &Ifx_MHA_BridgeDrv_TLE987_componentVersion;
}


void Ifx_MHA_BridgeDrv_TLE987_execute(Ifx_MHA_BridgeDrv_TLE987* self)
{
    /* Execute state machine */
    Ifx_MHA_BridgeDrv_TLE987_stateMachine(self);
}


void Ifx_MHA_BridgeDrv_TLE987_init(Ifx_MHA_BridgeDrv_TLE987* self)
{
    /* Initialize internal parameters to 0 */
    self->_Super_Ifx_MHA_BridgeDrv.p_enable     = false;
    self->_Super_Ifx_MHA_BridgeDrv.p_clearFault = false;
    self->p_status.state                        = Ifx_MHA_BridgeDrv_TLE987_State_init;
    self->p_status.overcurrent                  = false;
    self->p_status.overvoltage                  = false;
    self->p_status.undervoltage                 = false;
}


/* ************************************************************************************************
* Local functions declaration section
************************************************************************************************ */
static inline bool Ifx_MHA_BridgeDrv_TLE987_faultStatus(bool swFaultStatus, uint32 hwFaultStatus, uint8
                                                        faultConfiguration, bool* faultStatusRet)
{
    bool faultOccured = false;

    /* 1. check individually for Over-current */
    /* 1.1 Check if this fault is configured as ENABLED */
    if (faultConfiguration >= IFX_MHA_BRIDGEDRV_FAULT_REACTION_ENABLE)
    {
        if (hwFaultStatus != 0)
        {
            /* set the Over-current fault information status bit */
            faultOccured = true;

            /* 1.2 Check if this fault is configured for REPORTING */
            if (faultConfiguration >= IFX_MHA_BRIDGEDRV_FAULT_REACTION_REPORT_ONLY)
            {
#if (IFX_MHA_BRIDGEDRV_CFG_ENABLE_FAULT_OUT == 1)

                /* report fault source ONLY the 1st time it occurs */
                if (swFaultStatus == false)
                {
                    /* report the fault through the user interface */
                    IFX_MHA_BRIDGEDRV_CFG_FAULT_OUT();
                }

#endif

                /* 1.3 Check if this fault is configured for REACTION */
                if (faultConfiguration >= IFX_MHA_BRIDGEDRV_FAULT_REACTION_REPORT_REACT)
                {
                    /* set the fault status for reacting */
                    *faultStatusRet = true;
                }
            }
        }
    }

    return faultOccured;
}


static inline bool Ifx_MHA_BridgeDrv_TLE987_checkHwFaults(bool* faultOverCurrent, bool* faultOverVoltage,
                                                          bool* faultUnderVoltage, bool clearFault)
{
    /* local copy of the BDRV_IS Register */
    /* read all status bits for OC, OV, UV from BDRV Interrupt Status register */
    uint32 regBdrvIs = Ifx_MHA_BridgeDrv_TLE987_readFaults();

    /* boolean output fault status initialized to false */
    bool   faultStatusRet = false;

    /* Check if any fault (OC or OV or UV) occur */
    if ((regBdrvIs != 0)
        || (clearFault == true))
    {
        /* 1. check for Over-current */
        uint32 overcurrentStatus = (regBdrvIs & IFX_MHA_BRIDGEDRV_TLE987_IS_UMASK_OC_FAULT);
        *faultOverCurrent = Ifx_MHA_BridgeDrv_TLE987_faultStatus(*faultOverCurrent, overcurrentStatus,
            IFX_MHA_BRIDGEDRV_CFG_FAULT_REACTION_OVERCURRENT, &faultStatusRet);

        /* 2 check for Over-voltage */
        uint32 overvoltageStatus = (regBdrvIs & IFX_MHA_BRIDGEDRV_TLE987_IS_UMASK_OV_FAULT);
        *faultOverVoltage = Ifx_MHA_BridgeDrv_TLE987_faultStatus(*faultOverVoltage, overvoltageStatus,
            IFX_MHA_BRIDGEDRV_CFG_FAULT_REACTION_OVERVOLT, &faultStatusRet);

        /* 3 check for Under-voltage */
        uint32 undervoltageStatus = (regBdrvIs & IFX_MHA_BRIDGEDRV_TLE987_IS_UMASK_UV_FAULT);
        *faultUnderVoltage = Ifx_MHA_BridgeDrv_TLE987_faultStatus(*faultUnderVoltage, undervoltageStatus,
            IFX_MHA_BRIDGEDRV_CFG_FAULT_REACTION_UNDERVOLT, &faultStatusRet);
    }

    return faultStatusRet;
}


static inline bool Ifx_MHA_BridgeDrv_TLE987_checkFaultStatus(Ifx_MHA_BridgeDrv_TLE987* self, bool clearFault)
{
    /* Initialize boolean output fault status to false */
    bool faultStatusRet = false;

    /* Save status of bdrv faults in local variables */
    bool faultOverCurrent  = self->p_status.overcurrent;
    bool faultOverVoltage  = self->p_status.overvoltage;
    bool faultUnderVoltage = self->p_status.undervoltage;

    /* Check if a clear fault was requested */
    if (clearFault == true)
    {
        /* Clear all hardware faults */
        BDRV_Clr_Sts(IFX_MHA_BRIDGEDRV_TLE987_IS_UMASK_OC_OV_UV_FAULTS);
    }

    /* Check HW faults */
    faultStatusRet = Ifx_MHA_BridgeDrv_TLE987_checkHwFaults(&faultOverCurrent, &faultOverVoltage, &faultUnderVoltage,
        clearFault);

    /* Update bdrv faults from local variables */
    self->p_status.overcurrent  = faultOverCurrent;
    self->p_status.overvoltage  = faultOverVoltage;
    self->p_status.undervoltage = faultUnderVoltage;

    /* return the fault status */
    return faultStatusRet;
}


static inline Ifx_MHA_BridgeDrv_TLE987_State Ifx_MHA_BridgeDrv_TLE987_stateInit(Ifx_MHA_BridgeDrv_TLE987* self)
{
    /* Variable to store the next state */
    Ifx_MHA_BridgeDrv_TLE987_State nextState = Ifx_MHA_BridgeDrv_TLE987_State_init;

    /* do Action BridgeDriver DISABLED to enable charge pump */
    Ifx_MHA_BridgeDrv_TLE987_actionDisable();

    /* do state transition to OFF when the charge pump voltage is ramped up */
    if ((IFX_MHA_BRIDGEDRV_TLE987_IS_UMASK_UV_FAULT & Ifx_MHA_BridgeDrv_TLE987_readFaults()) == 0)
    {
        nextState = Ifx_MHA_BridgeDrv_TLE987_State_off;

        /* Clear under voltage software flag */
        self->p_status.undervoltage = false;
    }
    else
    {
        /* Clear under voltage hardware flag */
        BDRV_Clr_Sts(IFX_MHA_BRIDGEDRV_TLE987_IS_UMASK_UV_FAULT);
    }

    return nextState;
}


static inline Ifx_MHA_BridgeDrv_TLE987_State Ifx_MHA_BridgeDrv_TLE987_stateOff(Ifx_MHA_BridgeDrv_TLE987* self, bool
                                                                               clearFault)
{
    /* Variable to store the next state */
    Ifx_MHA_BridgeDrv_TLE987_State nextState;

    /* Variable to store fault status of bridge driver */
    bool                           faultStatus;

    /* Check fault status of bridge driver */
    faultStatus = Ifx_MHA_BridgeDrv_TLE987_checkFaultStatus(self, clearFault);

    /* check condition for T1 */
    if ((self->_Super_Ifx_MHA_BridgeDrv.p_enable == true)
        && (faultStatus == false))
    {
        /* do state transition to ON */
        nextState = Ifx_MHA_BridgeDrv_TLE987_State_on;

        /* do Action BridgeDriver ENABLED */
        Ifx_MHA_BridgeDrv_TLE987_actionEnable();
    }

    /* check condition for T2 */
    else if ((self->_Super_Ifx_MHA_BridgeDrv.p_enable == false)
             && (faultStatus == true))
    {
        /* do state transition to FAULT */
        nextState = Ifx_MHA_BridgeDrv_TLE987_State_fault;
    }
    else
    {
        /* stay in OFF state */
        nextState = Ifx_MHA_BridgeDrv_TLE987_State_off;
    }

    return nextState;
}


static inline Ifx_MHA_BridgeDrv_TLE987_State Ifx_MHA_BridgeDrv_TLE987_stateOn(Ifx_MHA_BridgeDrv_TLE987* self, bool
                                                                              clearFault)
{
    /* Variable to store the next state */
    Ifx_MHA_BridgeDrv_TLE987_State nextState;

    /* Variable to store fault status of bridge driver */
    bool                           faultStatus;

    /* Check fault status of bridge driver */
    faultStatus = Ifx_MHA_BridgeDrv_TLE987_checkFaultStatus(self, clearFault);

    /* check condition for T2 */
    if (faultStatus == true)
    {
        /* do state transition to FAULT */
        nextState = Ifx_MHA_BridgeDrv_TLE987_State_fault;
    }

    /* check condition for T1 */
    else if (self->_Super_Ifx_MHA_BridgeDrv.p_enable == false)
    {
        /* do state transition to OFF */
        nextState = Ifx_MHA_BridgeDrv_TLE987_State_off;

        /* do Action BridgeDriver DISABLED */
        Ifx_MHA_BridgeDrv_TLE987_actionDisable();
    }
    else
    {
        nextState = Ifx_MHA_BridgeDrv_TLE987_State_on;
    }

    return nextState;
}


static inline Ifx_MHA_BridgeDrv_TLE987_State Ifx_MHA_BridgeDrv_TLE987_stateFault(Ifx_MHA_BridgeDrv_TLE987* self, bool
                                                                                 clearFault)
{
    /* Variable to store the next state */
    Ifx_MHA_BridgeDrv_TLE987_State nextState;

    /* Variable to store fault status of bridge driver */
    bool                           faultStatus;

    /* Check fault status of bridge driver */
    faultStatus = Ifx_MHA_BridgeDrv_TLE987_checkFaultStatus(self, clearFault);

    /* check condition for T1 */
    if ((self->_Super_Ifx_MHA_BridgeDrv.p_enable == true)
        && (faultStatus == false)
        && (clearFault == true))
    {
        /* do state transition to ON */
        nextState = Ifx_MHA_BridgeDrv_TLE987_State_on;

        /* do Action BridgeDriver ENABLED */
        Ifx_MHA_BridgeDrv_TLE987_actionEnable();
    }
    else if ((self->_Super_Ifx_MHA_BridgeDrv.p_enable == false)
             && (faultStatus == false)
             && (clearFault == true))
    {
        /* do state transition to OFF */
        nextState = Ifx_MHA_BridgeDrv_TLE987_State_off;

        /* do Action BridgeDriver DISABLED */
        Ifx_MHA_BridgeDrv_TLE987_actionDisable();
    }
    else
    {
        /* stay in FAULT */
        nextState = Ifx_MHA_BridgeDrv_TLE987_State_fault;
    }

    return nextState;
}


static inline void Ifx_MHA_BridgeDrv_TLE987_stateMachine(Ifx_MHA_BridgeDrv_TLE987* self)
{
    /* Variable to store the previous state */
    Ifx_MHA_BridgeDrv_TLE987_State previousState = self->p_status.state;

    /* Variable to store the next state */
    Ifx_MHA_BridgeDrv_TLE987_State nextState;

    /* Variable to store clear fault */
    bool                           clearFault = self->_Super_Ifx_MHA_BridgeDrv.p_clearFault;

    switch (previousState)
    {
        /* Bridge driver is in init state*/
        case Ifx_MHA_BridgeDrv_TLE987_State_init:
            nextState = Ifx_MHA_BridgeDrv_TLE987_stateInit(self);
            break;

        /* Bridge driver is in off state*/
        case Ifx_MHA_BridgeDrv_TLE987_State_off:
            nextState = Ifx_MHA_BridgeDrv_TLE987_stateOff(self, clearFault);
            break;

        /* Bridge driver is in on state*/
        case Ifx_MHA_BridgeDrv_TLE987_State_on:
            nextState = Ifx_MHA_BridgeDrv_TLE987_stateOn(self, clearFault);
            break;

        /* If bridge driver is in fault state*/
        case Ifx_MHA_BridgeDrv_TLE987_State_fault:
            nextState = Ifx_MHA_BridgeDrv_TLE987_stateFault(self, clearFault);
            break;

        /* do default transition to init */
        default:
            nextState = Ifx_MHA_BridgeDrv_TLE987_State_init;
            break;
    }

    /* Update state and reset clear fault */
    self->p_status.state = nextState;

    if (clearFault == true)
    {
        self->_Super_Ifx_MHA_BridgeDrv.p_clearFault = false;
    }
}


static inline void Ifx_MHA_BridgeDrv_TLE987_actionDisable(void)
{
    /* All channels disabled */
    BDRV_Set_Bridge(Ch_En, Ch_En, Ch_En, Ch_En, Ch_En, Ch_En);
}


static inline void Ifx_MHA_BridgeDrv_TLE987_actionEnable(void)
{
    /* All channels enabled with PWM (CCU6 connection) */
    BDRV_Set_Bridge(Ch_PWM, Ch_PWM, Ch_PWM, Ch_PWM, Ch_PWM, Ch_PWM);
}


/* polyspace-end MISRA2012:D4.14 [Justified:Low] "The caller function has to guarantee that NULL is not passed as
 * argument." */
