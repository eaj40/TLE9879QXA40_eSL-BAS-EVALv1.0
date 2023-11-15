/*
 * Copyright (c) 2022 Infineon Technologies AG. All Rights Reserved.
 *
 * Use of this file is subject to the terms of use agreed between (i) you or the company in which ordinary course of
 * business you are acting and (ii) Infineon Technologies AG, its affiliates or its licensees. If and as long as no
 * such terms of use are agreed, use of this file is subject to the Evaluation Software License Agreement distributed
 * along with this file within the software delivery package.
 *
 */

#include "Ifx_MHA_PatternGen_TLE987.h"
#include "Ifx_MHA_PatternGen_Cfg.h"
#include "int.h"
#include "timer3.h"
#include "port.h"

/* Macro for enabling CCU6 outputs */
#define IFX_MHA_PATTERNGEN_TLE987_PWM_PATTERN_SHADOW_OUT      (0x3FUL)

/* Macro for triggering T13 in T12 zero match */
#define IFX_MHA_PATTERNGEN_TLE987_TRIGGER_T13_ON_ZERO_MATCH   (0x7AU)

/* Macro for triggering T13 in T12 period match */
#define IFX_MHA_PATTERNGEN_TLE987_TRIGGER_T13_ON_PERIOD_MATCH (0x76U)

/* Total number of compare values */
#define IFX_MHA_PATTERNGEN_TLE987_N_COMPARE_VALUES            (6U)

/* Macros to define the component ID */
#define IFX_MHA_PATTERNGEN_TLE987_COMPONENTID_SOURCEID        ((uint8)Ifx_ComponentID_SourceID_infineonTechnologiesAG)
#define IFX_MHA_PATTERNGEN_TLE987_COMPONENTID_LIBRARYID                 \
                                                              ((uint16) \
     Ifx_ComponentID_LibraryID_mctrlHardwareAbstraction)

#define IFX_MHA_PATTERNGEN_TLE987_COMPONENTID_MODULEID        (2U)
#define IFX_MHA_PATTERNGEN_TLE987_COMPONENTID_COMPONENTID1    (1U)

#define IFX_MHA_PATTERNGEN_TLE987_COMPONENTID_COMPONENTID2    ((uint8)Ifx_ComponentID_ComponentID2_basic)

/* Macros to define the component version */
#define IFX_MHA_PATTERNGEN_TLE987_COMPONENTVERSION_MAJOR      (1U)
#define IFX_MHA_PATTERNGEN_TLE987_COMPONENTVERSION_MINOR      (3U)
#define IFX_MHA_PATTERNGEN_TLE987_COMPONENTVERSION_PATCH      (0U)
#define IFX_MHA_PATTERNGEN_TLE987_COMPONENTVERSION_T          (0U)
#define IFX_MHA_PATTERNGEN_TLE987_COMPONENTVERSION_REV        (0U)

/* *INDENT-OFF* */
/* Component ID */
static const Ifx_ComponentID      Ifx_MHA_PatternGen_TLE987_componentID = {
    .sourceID     = IFX_MHA_PATTERNGEN_TLE987_COMPONENTID_SOURCEID,
    .libraryID    = IFX_MHA_PATTERNGEN_TLE987_COMPONENTID_LIBRARYID,
    .moduleID     = IFX_MHA_PATTERNGEN_TLE987_COMPONENTID_MODULEID,
    .componentID1 = IFX_MHA_PATTERNGEN_TLE987_COMPONENTID_COMPONENTID1,
    .componentID2 = IFX_MHA_PATTERNGEN_TLE987_COMPONENTID_COMPONENTID2,
};

/* Component Version */
static const Ifx_ComponentVersion Ifx_MHA_PatternGen_TLE987_componentVersion = {
    .major = IFX_MHA_PATTERNGEN_TLE987_COMPONENTVERSION_MAJOR,
    .minor = IFX_MHA_PATTERNGEN_TLE987_COMPONENTVERSION_MINOR,
    .patch = IFX_MHA_PATTERNGEN_TLE987_COMPONENTVERSION_PATCH,
    .t     = IFX_MHA_PATTERNGEN_TLE987_COMPONENTVERSION_T,
    .rev   = IFX_MHA_PATTERNGEN_TLE987_COMPONENTVERSION_REV
};
/* *INDENT-ON* */
/* Checks if any fault occurred and acts accordingly */
static inline bool Ifx_MHA_PatternGen_TLE987_checkFaultStatus(Ifx_MHA_PatternGen_TLE987* self, bool clearFault);

/* Pattern generator state machine implementation */
static inline void Ifx_MHA_PatternGen_TLE987_stateMachine(Ifx_MHA_PatternGen_TLE987* self, bool faultStatus, bool
                                                          clearFault);

/* Pattern generator in on state */
static inline Ifx_MHA_PatternGen_TLE987_State Ifx_MHA_PatternGen_TLE987_stateOn(Ifx_MHA_PatternGen_TLE987* self, bool
                                                                                faultStatus);

/* Pattern generator in off state */
static inline Ifx_MHA_PatternGen_TLE987_State Ifx_MHA_PatternGen_TLE987_stateOff(Ifx_MHA_PatternGen_TLE987* self, bool
                                                                                 faultStatus);

/* Pattern generator in fault state */
static inline Ifx_MHA_PatternGen_TLE987_State Ifx_MHA_PatternGen_TLE987_stateFault(Ifx_MHA_PatternGen_TLE987* self,
                                                                                   bool faultStatus, bool clearFault);

/* Load shadow registers and request shadow transfer */
static inline void Ifx_MHA_PatternGen_TLE987_transferDownCounting(Ifx_MHA_PatternGen_TLE987* self);
static inline void Ifx_MHA_PatternGen_TLE987_transferUpCounting(Ifx_MHA_PatternGen_TLE987* self);

/* Disable and enable the pattern generator */
static inline void Ifx_MHA_PatternGen_TLE987_actionDisable(void);
static inline void Ifx_MHA_PatternGen_TLE987_actionEnable(void);

#if (IFX_MHA_PATTERNGEN_CFG_CURRENT_LOOP_FACTOR > 1)

/* Update values from the shadow variables to the used ones */
static inline void Ifx_MHA_PatternGen_TLE987_updateCompareAndTriggers(Ifx_MHA_PatternGen_TLE987* self);
#endif

/* polyspace-begin MISRA2012:D4.14 [Justified:Low] "The caller function has to guarantee that NULL is not passed as
 * argument." */

/* Function to get the component ID */
void Ifx_MHA_PatternGen_TLE987_getID(const Ifx_ComponentID** componentID)
{
    *componentID = &Ifx_MHA_PatternGen_TLE987_componentID;
}


/* Function to get the component version */
void Ifx_MHA_PatternGen_TLE987_getVersion(const Ifx_ComponentVersion** componentVersion)
{
    *componentVersion = &Ifx_MHA_PatternGen_TLE987_componentVersion;
}


void Ifx_MHA_PatternGen_TLE987_init(Ifx_MHA_PatternGen_TLE987* self)
{
    /* Minimum dead time defined in Config Wizard */
    self->_Super_Ifx_MHA_PatternGen.p_deadTimeMin_ns = IFX_MHA_PATTERNGEN_CFG_MIN_DEADTIME;

    /* Initialize internal parameters to 0 */
    self->_Super_Ifx_MHA_PatternGen.p_enable     = false;
    self->_Super_Ifx_MHA_PatternGen.p_clearFault = false;
    self->p_status.state                         = Ifx_MHA_PatternGen_TLE987_State_init;
    self->p_status.trap                          = false;

    /* Reset the trigger and the compare values to be used in the interrupts */
    self->p_triggerTime_tick[0]       = 0U;
    self->p_triggerTime_tick[1]       = 0U;
    self->p_triggerTimeShadow_tick[0] = 0U;
    self->p_triggerTimeShadow_tick[1] = 0U;

    for (uint8 i = 0; i < IFX_MHA_PATTERNGEN_TLE987_N_COMPARE_VALUES; i++)
    {
        self->p_compareValues_tick[i]       = 0U;
        self->p_compareValuesShadow_tick[i] = 0U;
    }

    /* Reset executed flag and cycle counter */
    self->p_executed     = false;
    self->p_cycleCounter = 0;

    /* Start T13 on the period match to generate trigger for second current measurement */
    CCU6_SetT13Trigger(IFX_MHA_PATTERNGEN_TLE987_TRIGGER_T13_ON_PERIOD_MATCH);

    /* If the trap fault is disable, the trap flag does not need to be cleared */
#if IFX_MHA_PATTERNGEN_CFG_FAULT_REACTION_TRAP != 0

    /* Clear the trap interrupt flag if it is high */
    uint8 trapPortStatus;

    /* Check which port is used for trap and get the port status */
    if (CCU6->PISEL0.bit.ISTRP == 0)
    {
        trapPortStatus = PORT_P24_Get();
    }
    else
    {
        trapPortStatus = PORT_P23_Get();
    }

    /* If the port is high, clear the trap flag */
    if (trapPortStatus != 0)
    {
        CCU6_TRAP_Int_Clr();
    }

#endif
}


void Ifx_MHA_PatternGen_TLE987_execute(Ifx_MHA_PatternGen_TLE987* self, uint16 compareValues[6], uint16
                                       triggerTime_tick[2])
{
    /* Store clearFault variable at the beginning of the module */
    bool clearFault = self->_Super_Ifx_MHA_PatternGen.p_clearFault;

/* If the current loop factor is 1, then don't use the shadow variables */
#if (IFX_MHA_PATTERNGEN_CFG_CURRENT_LOOP_FACTOR == 1)

    /* Assign the trigger and the compare values to be used in the interrupts */
    self->p_triggerTime_tick[0] = triggerTime_tick[0];
    self->p_triggerTime_tick[1] = triggerTime_tick[1];

    for (uint8 i = 0; i < IFX_MHA_PATTERNGEN_TLE987_N_COMPARE_VALUES; i++)
    {
        self->p_compareValues_tick[i] = compareValues[i];
    }

#else

    /* Assign the trigger and the compare values to the shadow variables */
    self->p_triggerTimeShadow_tick[0] = triggerTime_tick[0];
    self->p_triggerTimeShadow_tick[1] = triggerTime_tick[1];

    for (uint8 i = 0; i < IFX_MHA_PATTERNGEN_TLE987_N_COMPARE_VALUES; i++)
    {
        self->p_compareValuesShadow_tick[i] = compareValues[i];
    }

#endif

    /* Check if any fault occurred and execute state machine */
    bool faultStatusLocal = Ifx_MHA_PatternGen_TLE987_checkFaultStatus(self, clearFault);
    Ifx_MHA_PatternGen_TLE987_stateMachine(self, faultStatusLocal, clearFault);
}


/* Period match interrupt */
void Ifx_MHA_PatternGen_TLE987_onPeriodMatch(Ifx_MHA_PatternGen_TLE987* self)
{
    /* Last cycle and execute was already called, update compare values and transfer it to the CCU6 */
    if ((self->p_cycleCounter == (IFX_MHA_PATTERNGEN_CFG_CURRENT_LOOP_FACTOR - 1))
        && (self->p_executed == true))
    {
/* Only update the values if the current loop factor is bigger than 1 */
#if (IFX_MHA_PATTERNGEN_CFG_CURRENT_LOOP_FACTOR > 1)

        /* Update compare and trigger values */
        Ifx_MHA_PatternGen_TLE987_updateCompareAndTriggers(self);
#endif

        /* Transfer compare values and reset counter and flag */
        Ifx_MHA_PatternGen_TLE987_transferUpCounting(self);
    }

    /* Last cycle and PatternGen execute was not called, compare values will be updated by execute */
    else if (self->p_cycleCounter == (IFX_MHA_PATTERNGEN_CFG_CURRENT_LOOP_FACTOR - 1))
    {
        self->p_cycleCounter++;
    }

    /* Not the last cycle, transfer current shadow values to the CCU6 */
    else
    {
        /* Transfer compare values and increment cycle counter */
        Ifx_MHA_PatternGen_TLE987_transferUpCounting(self);
        self->p_cycleCounter++;
    }
}


/* One match interrupt */
void Ifx_MHA_PatternGen_TLE987_onOneMatch(Ifx_MHA_PatternGen_TLE987* self)
{
    /* Transfer compare values */
    Ifx_MHA_PatternGen_TLE987_transferDownCounting(self);
}


static inline void Ifx_MHA_PatternGen_TLE987_transferUpCounting(Ifx_MHA_PatternGen_TLE987* self)
{
    /* Load shadow registers with the up counting compare values */
    CCU6_LoadShadowRegister_CC60(self->p_compareValues_tick[0]);
    CCU6_LoadShadowRegister_CC61(self->p_compareValues_tick[1]);
    CCU6_LoadShadowRegister_CC62(self->p_compareValues_tick[2]);

    /* Load shadow register for first trigger */
    TIMER3_Set_Cmp_Value(self->p_triggerTime_tick[0] >> 1U);

    /* Enable shadow transfer */
    CCU6_T12_Str_En();
}


static inline void Ifx_MHA_PatternGen_TLE987_transferDownCounting(Ifx_MHA_PatternGen_TLE987* self)
{
    /* Store T12 period value */
    uint16 periodValue = CCU6_T12_Period_Value_Get() + 1;
    uint16 timerPeriod = 2 * periodValue;

    /* Load shadow registers with the down counting compare values */
    CCU6_LoadShadowRegister_CC60(timerPeriod - self->p_compareValues_tick[3]);
    CCU6_LoadShadowRegister_CC61(timerPeriod - self->p_compareValues_tick[4]);
    CCU6_LoadShadowRegister_CC62(timerPeriod - self->p_compareValues_tick[5]);

    /* Load shadow register for second trigger */
    CCU6_LoadShadowRegister_CC63(self->p_triggerTime_tick[1] - periodValue);

    /* Enable shadow transfer */
    CCU6_T12_Str_En();
    CCU6_T13_Str_En();
}


#if (IFX_MHA_PATTERNGEN_CFG_CURRENT_LOOP_FACTOR > 1)
static inline void Ifx_MHA_PatternGen_TLE987_updateCompareAndTriggers(Ifx_MHA_PatternGen_TLE987* self)
{
    /* Update compare values */
    self->p_triggerTime_tick[0] = self->p_triggerTimeShadow_tick[0];
    self->p_triggerTime_tick[1] = self->p_triggerTimeShadow_tick[1];

    for (uint8 i = 0; i < IFX_MHA_PATTERNGEN_TLE987_N_COMPARE_VALUES; i++)
    {
        self->p_compareValues_tick[i] = self->p_compareValuesShadow_tick[i];
    }
}


#endif

static inline Ifx_MHA_PatternGen_TLE987_State Ifx_MHA_PatternGen_TLE987_stateOn(Ifx_MHA_PatternGen_TLE987* self, bool
                                                                                faultStatus)
{
    /* Variable to store the next state */
    Ifx_MHA_PatternGen_TLE987_State nextState;

    if (faultStatus == true)
    {
        /* Set the state to fault */
        nextState = Ifx_MHA_PatternGen_TLE987_State_fault;
    }

    /* Check if module disabled */
    else if (self->_Super_Ifx_MHA_PatternGen.p_enable == false)
    {
        /* Set the state to OFF */
        nextState = Ifx_MHA_PatternGen_TLE987_State_off;

        /* Disable the pattern generator */
        Ifx_MHA_PatternGen_TLE987_actionDisable();
    }
    else
    {
        /* Disable CCU6 SR1 node */
        NVIC_Node5_Dis();

        /* Transfer values if the last period match was already executed */
        if (self->p_cycleCounter == IFX_MHA_PATTERNGEN_CFG_CURRENT_LOOP_FACTOR)
        {
#if (IFX_MHA_PATTERNGEN_CFG_CURRENT_LOOP_FACTOR > 1)

            /* Update compare and trigger values and transfer them to CCU6 */
            Ifx_MHA_PatternGen_TLE987_updateCompareAndTriggers(self);
#endif
            Ifx_MHA_PatternGen_TLE987_transferUpCounting(self);
        }
        else
        {
            /* Signal to the period match function that this function was executed */
            self->p_executed = true;
        }

        nextState = Ifx_MHA_PatternGen_TLE987_State_on;

        /* Enable CCU6 SR1 node */
        NVIC_Node5_En();
    }

    return nextState;
}


static inline Ifx_MHA_PatternGen_TLE987_State Ifx_MHA_PatternGen_TLE987_stateOff(Ifx_MHA_PatternGen_TLE987* self, bool
                                                                                 faultStatus)
{
    /* Variable to store the next state */
    Ifx_MHA_PatternGen_TLE987_State nextState;

    /* Check if any fault occurred */
    if (faultStatus == true)
    {
        /* Set the state to fault */
        nextState = Ifx_MHA_PatternGen_TLE987_State_fault;
    }

    /* Check if module enabled */
    else if (self->_Super_Ifx_MHA_PatternGen.p_enable == true)
    {
        /* Transition to ON if module enabled on the next execution cycle */
        nextState = Ifx_MHA_PatternGen_TLE987_State_on;

        /* Enable the pattern generator */
        Ifx_MHA_PatternGen_TLE987_actionEnable();
    }
    else
    {
        nextState = Ifx_MHA_PatternGen_TLE987_State_off;
    }

    return nextState;
}


static inline Ifx_MHA_PatternGen_TLE987_State Ifx_MHA_PatternGen_TLE987_stateFault(Ifx_MHA_PatternGen_TLE987* self,
                                                                                   bool faultStatus, bool clearFault)
{
    /* Variable to store the next state */
    Ifx_MHA_PatternGen_TLE987_State nextState;

    if ((faultStatus == false)
        && (clearFault == true))
    {
        /* Transition to off if module is in fault in the next execution cycle */
        nextState = Ifx_MHA_PatternGen_TLE987_State_off;

        /* Disable the pattern generator*/
        Ifx_MHA_PatternGen_TLE987_actionDisable();
    }
    else
    {
        nextState = Ifx_MHA_PatternGen_TLE987_State_fault;
    }

    return nextState;
}


static inline void Ifx_MHA_PatternGen_TLE987_stateMachine(Ifx_MHA_PatternGen_TLE987* self, bool faultStatus, bool
                                                          clearFault)
{
    /* Variable to store the state */
    Ifx_MHA_PatternGen_TLE987_State previousState = self->p_status.state;
    Ifx_MHA_PatternGen_TLE987_State nextState     = previousState;

    switch (previousState)
    {
        /* Initialize the module */
        case Ifx_MHA_PatternGen_TLE987_State_init:

            /* Set the state to OFF */
            nextState = Ifx_MHA_PatternGen_TLE987_State_off;
            break;

        /* Pattern generator is running */
        case Ifx_MHA_PatternGen_TLE987_State_on:
            nextState = Ifx_MHA_PatternGen_TLE987_stateOn(self, faultStatus);
            break;

        /* Pattern generator not running */
        case Ifx_MHA_PatternGen_TLE987_State_off:
            nextState = Ifx_MHA_PatternGen_TLE987_stateOff(self, faultStatus);
            break;

        /* Pattern generator is in fault */
        case Ifx_MHA_PatternGen_TLE987_State_fault:
            nextState = Ifx_MHA_PatternGen_TLE987_stateFault(self, faultStatus, clearFault);
            break;

        default:

            /* do default transition to INIT */
            nextState = Ifx_MHA_PatternGen_TLE987_State_init;
            break;
    }

    self->p_status.state = nextState;

    /* Clear the internal variable */
    if (clearFault == true)
    {
        self->_Super_Ifx_MHA_PatternGen.p_clearFault = false;
    }
}


static inline bool Ifx_MHA_PatternGen_TLE987_checkFaultStatus(Ifx_MHA_PatternGen_TLE987* self, bool clearFault)
{
    /* boolean output fault status initialized to false */
    bool faultStatusRet = false;

    /* local copy of the previously detected faults */
    bool faultTrap = self->p_status.trap;

    /* first check if a fault clear request was placed */
    if (clearFault == true)
    {
        /* Clear the trap fault in case a fault clear request was done */
        faultTrap = false;

        /* Clear the trap flag */
        CCU6_TRAP_Int_Clr();
    }

    /* Check if the trap fault is enabled */
#if IFX_MHA_PATTERNGEN_CFG_FAULT_REACTION_TRAP >= IFX_MHA_PATTERNGEN_FAULT_REACTION_ENABLE

    /* Read the status bits for trap fault from pattern generator Interrupt Status register */
    if (CCU6_Trap_Flag_Int_Sts() != 0)
    {
        /* set the trap fault information status bit */
        faultTrap = true;

        /* Check if this fault is configured for REPORTING */
#if IFX_MHA_PATTERNGEN_CFG_FAULT_REACTION_TRAP >= IFX_MHA_PATTERNGEN_FAULT_REACTION_REPORT_ONLY

#if (IFX_MHA_PATTERNGEN_CFG_ENABLE_FAULT_OUT == 1)

        /* report fault source ONLY the first time it occurs */
        if (self->p_status.trap == false)
        {
            /* report the fault through the user interface */
            IFX_MHA_PATTERNGEN_CFG_FAULT_OUT();
        }

#endif

        /* Check if this fault is configured for REACTION */
#if IFX_MHA_PATTERNGEN_CFG_FAULT_REACTION_TRAP >= IFX_MHA_PATTERNGEN_FAULT_REACTION_REPORT_REACT

        /* set the fault status for reacting */
        faultStatusRet = true;
#endif
#endif
    }

#endif

    self->p_status.trap = faultTrap;

    return faultStatusRet;
}


static inline void Ifx_MHA_PatternGen_TLE987_actionDisable(void)
{
    /* Sets Multi-Channel PWM Pattern shadow register */
    CCU6_Multi_Ch_PWM_Shadow_Reg_Load((uint16)0x0000);

    /* Enables shadow transfer request by software */
    CCU6_MCM_PWM_Str_SW_En();
}


static inline void Ifx_MHA_PatternGen_TLE987_actionEnable(void)
{
    /* Sets Multi-Channel PWM Pattern shadow register */
    CCU6_Multi_Ch_PWM_Shadow_Reg_Load((uint16)IFX_MHA_PATTERNGEN_TLE987_PWM_PATTERN_SHADOW_OUT);

    /* Enables shadow transfer request by software*/
    CCU6_MCM_PWM_Str_SW_En();
}


/* polyspace-end MISRA2012:D4.14 [Justified:Low] "The caller function has to guarantee that NULL is not passed as
 * argument." */
