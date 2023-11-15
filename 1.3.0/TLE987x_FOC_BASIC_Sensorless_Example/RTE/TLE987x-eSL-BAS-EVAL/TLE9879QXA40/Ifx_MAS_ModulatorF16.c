/*
 * Copyright (c) 2022 Infineon Technologies AG. All Rights Reserved.
 *
 * Use of this file is subject to the terms of use agreed between (i) you or the company in which ordinary course of
 * business you are acting and (ii) Infineon Technologies AG, its affiliates or its licensees. If and as long as no
 * such terms of use are agreed, use of this file is subject to the Evaluation Software License Agreement distributed
 * along with this file within the software delivery package.
 *
 */

#include "Ifx_MAS_ModulatorF16.h"

/* Math library includes */
#include "Ifx_Math_All.h"
#include "Ifx_Math_Cfg.h"

/* CMSIS includes */
#include "cmsis_compiler.h"

/* Size of the lookup table used  */
#define IFX_MAS_MODULATORF16_TABLESIN60SQRT3_LUT_SIZE  (12)

/* Maximum index of the lookup table used, 2^(size) -1  */
#define IFX_MAS_MODULATORF16_TABLESIN60SQRT3_MAX_INDEX (4095U)

/* Macro to convert angle value to lookup table index */
#define IFX_MAS_MODULATORF16_ANGLE_TO_INDEX            (15U - IFX_MAS_MODULATORF16_TABLESIN60SQRT3_LUT_SIZE)

/* Modulation index max for linear region, normalized by 2/pi */
#define IFX_MAS_MODULATORF16_CONST_MI_0907_Q15         (18921)

/* Ratio from period ticks to compare value */
#define IFX_MAS_MODULATORF16_MAX_COMPARE_VALUE_RATIO   (2)
#define IFX_MAS_MODULATORF16_MED_COMPARE_VALUE_RATIO   (4)

/* Macros to detect individual faults */
#define IFX_MAS_MODULATORF16_MAXAMPLITUDE_FAULT_STS    (0x20U)
#define IFX_MAS_MODULATORF16_OVERMODULATION_FAULT_STS  (0x40U)

/* Macros to define the component ID */
#define IFX_MAS_MODULATORF16_COMPONENTID_SOURCEID      ((uint8)Ifx_ComponentID_SourceID_infineonTechnologiesAG)
#define IFX_MAS_MODULATORF16_COMPONENTID_LIBRARYID     ((uint16)Ifx_ComponentID_LibraryID_mctrlActuatorSensor)
#define IFX_MAS_MODULATORF16_COMPONENTID_MODULEID      (0U)
#define IFX_MAS_MODULATORF16_COMPONENTID_COMPONENTID1  (1U)

#define IFX_MAS_MODULATORF16_COMPONENTID_COMPONENTID2  ((uint8)Ifx_ComponentID_ComponentID2_basic)

/* Macros to define the component version */
#define IFX_MAS_MODULATORF16_COMPONENTVERSION_MAJOR    (1U)
#define IFX_MAS_MODULATORF16_COMPONENTVERSION_MINOR    (3U)
#define IFX_MAS_MODULATORF16_COMPONENTVERSION_PATCH    (0U)
#define IFX_MAS_MODULATORF16_COMPONENTVERSION_T        (0U)
#define IFX_MAS_MODULATORF16_COMPONENTVERSION_REV      (0U)

/* *INDENT-OFF* */
/* Component ID */
static const Ifx_ComponentID      Ifx_MHA_ModulatorF16_componentID = {
    .sourceID     = IFX_MAS_MODULATORF16_COMPONENTID_SOURCEID,
    .libraryID    = IFX_MAS_MODULATORF16_COMPONENTID_LIBRARYID,
    .moduleID     = IFX_MAS_MODULATORF16_COMPONENTID_MODULEID,
    .componentID1 = IFX_MAS_MODULATORF16_COMPONENTID_COMPONENTID1,
    .componentID2 = IFX_MAS_MODULATORF16_COMPONENTID_COMPONENTID2,
};

/* Component Version */
static const Ifx_ComponentVersion Ifx_MHA_ModulatorF16_componentVersion = {
    .major = IFX_MAS_MODULATORF16_COMPONENTVERSION_MAJOR,
    .minor = IFX_MAS_MODULATORF16_COMPONENTVERSION_MINOR,
    .patch = IFX_MAS_MODULATORF16_COMPONENTVERSION_PATCH,
    .t     = IFX_MAS_MODULATORF16_COMPONENTVERSION_T,
    .rev   = IFX_MAS_MODULATORF16_COMPONENTVERSION_REV
};
/* *INDENT-ON* */

/* polyspace-begin MISRA2012:D4.14 [Justified:Low] "The caller function has to guarantee that NULL is not passed as
 * argument." */

/* API to initialize configuration related to current measurement */
static inline void Ifx_MAS_ModulatorF16_initCurrMeasCfg(Ifx_MAS_ModulatorF16* self);

/* API to check the fault status */
static inline bool Ifx_MAS_ModulatorF16_checkFaultStatus(Ifx_MAS_ModulatorF16* self, uint32 faults);

/* APIs to check the fault status of each fault individually */
static inline bool Ifx_MAS_ModulatorF16_maxAmplitudeFaultStatus(Ifx_MAS_ModulatorF16* self, bool* faultStatusRet,
                                                                uint32 faults);
static inline bool Ifx_MAS_ModulatorF16_overmodulationFaultStatus(Ifx_MAS_ModulatorF16* self, bool* faultStatusRet,
                                                                  uint32 faults);

/* API to set the compare values and triggers */
static inline void Ifx_MAS_ModulatorF16_setActiveShort(Ifx_MAS_ModulatorF16* self, sint16 cmprValues);

/* API to check and limit the inputs */
static inline uint8 Ifx_MAS_ModulatorF16_limitInputs(Ifx_MAS_ModulatorF16* self, Ifx_Math_Fract16 refVoltageAmp,
                                                     Ifx_Math_Fract16 dcLinkVoltage,
                                                     Ifx_Math_Fract16* voltageOverDcLinkVoltage);

/* API to calculate compare values based on symmetric switching */
static inline void Ifx_MAS_ModulatorF16_symmetricSwitching(sint16* cmprVal, sint16* switchingTimes, uint8 sector);

#if IFX_MAS_MODULATORF16_CFG_MIN_ON_TIME_TICK != 0

/* API to modify the switching times to get a minimum sensing time (minimum on time feature active) */
static inline void Ifx_MAS_ModulatorF16_modSwitch(Ifx_MAS_ModulatorF16* self, sint16* compVal, sint16* switchingTimes,
                                                  uint8 isSevenSegment);
#else

/* API to modify the switching times to get a minimum sensing time (minimum on time feature inactive) */
static inline void Ifx_MAS_ModulatorF16_modSwitch(Ifx_MAS_ModulatorF16* self, sint16* compVal,
                                                  sint16* switchingTimes);
#endif /* IFX_MAS_MODULATORF16_CFG_MIN_ON_TIME_TICK != 0 */
/* API to limit the switching times */
static inline void Ifx_MAS_ModulatorF16_limitSwitchTimes(Ifx_MAS_ModulatorF16* self, sint16* cmprVal);

/* API to calculate the triggers based on sector */
static inline void Ifx_MAS_ModulatorF16_calcTriggers(Ifx_MAS_ModulatorF16* self, sint16* cmprVal);

/* API to set the triggers for current measurement */
static inline void Ifx_MAS_ModulatorF16_measBegin(Ifx_MAS_ModulatorF16* self, sint16* cmprVal);

/* API to assign the compare values and triggers */
static inline void Ifx_MAS_ModulatorF16_assignOutputs(Ifx_MAS_ModulatorF16* self, sint16* cmprVal);

/* API to calculate the effective times of the inverter switching states */
static inline void Ifx_MAS_ModulatorF16_updatePolar7SegQ16(Ifx_MAS_ModulatorF16* self, uint32 angle, Ifx_Math_Fract16
                                                           voltageOverDcLinkVoltage, Ifx_Math_3PhaseFract16 currents);

#if IFX_MAS_MODULATORF16_CFG_ENABLE_DEADTIME_COMPENSATION == 1

/* API to calculate the 7 segment switching times based on t_R, t_L, t_0, and sector */
static inline void Ifx_MAS_ModulatorF16_calcSwitchTimes(Ifx_MAS_ModulatorF16* self, sint16* switchingTimes,
                                                        Ifx_Math_3PhaseFract16 currents, Ifx_Math_Fract16
                                                        voltageOverDcLinkVoltage);
static inline void Ifx_MAS_ModulatorF16_compDeadTime(Ifx_MAS_ModulatorF16* self, sint16* cmprVal,
                                                     Ifx_Math_3PhaseFract16 currents);

/* API to compensate for the deadtime */
static inline void Ifx_MAS_ModulatorF16_compPhaseDeadTime(Ifx_MAS_ModulatorF16* self, sint16* cmprVal,
                                                          Ifx_Math_Fract16 phaseCurrent, uint8 phaseOffset);
#else

/* API to calculate the 7 segment switching times based on t_R, t_L, t_0, and sector */
static inline void Ifx_MAS_ModulatorF16_calcSwitchTimes(Ifx_MAS_ModulatorF16* self, sint16* switchingTimes,
                                                        Ifx_Math_Fract16 voltageOverDcLinkVoltage);
#endif /* IFX_MAS_MODULATORF16_CFG_ENABLE_DEADTIME_COMPENSATION */

#if IFX_MAS_MODULATORF16_CFG_MIN_ON_TIME_TICK != 0

/* API to set the triggers for current measurement */
static inline uint8 Ifx_MAS_ModulatorF16_checkMinOnTime(Ifx_MAS_ModulatorF16* self, sint16* cmprVal);

#endif /* IFX_MAS_MODULATORF16_CFG_MIN_ON_TIME_TICK */
/* API to modify the switching times at low amplitudes */
static inline void Ifx_MAS_ModulatorF16_modSwitchSmallAmplitude(Ifx_MAS_ModulatorF16* self,
                                                                sint16              * cmprVal);
static inline void                       Ifx_MAS_ModulatorF16_measEnd(Ifx_MAS_ModulatorF16* self, sint16* cmprVal);
static inline Ifx_MAS_ModulatorF16_State Ifx_MAS_ModulatorF16_stateOff(Ifx_MAS_ModulatorF16* self, bool
                                                                       enterFaultState, uint32 angle, Ifx_Math_Fract16
                                                                       voltageOverDcLinkVoltage,
                                                                       Ifx_Math_3PhaseFract16 currents);
static inline Ifx_MAS_ModulatorF16_State Ifx_MAS_ModulatorF16_stateOn(Ifx_MAS_ModulatorF16* self, bool
                                                                      enterFaultState, uint32 angle, Ifx_Math_Fract16
                                                                      voltageOverDcLinkVoltage, Ifx_Math_3PhaseFract16
                                                                      currents);
static inline Ifx_MAS_ModulatorF16_State Ifx_MAS_ModulatorF16_stateBrake(Ifx_MAS_ModulatorF16* self, bool
                                                                         enterFaultState);
static inline Ifx_MAS_ModulatorF16_State Ifx_MAS_ModulatorF16_stateFault(Ifx_MAS_ModulatorF16* self);

/* Function to get the component ID */
void Ifx_MAS_ModulatorF16_getID(const Ifx_ComponentID** componentID)
{
    *componentID = &Ifx_MHA_ModulatorF16_componentID;
}


/* Function to get the component version */
void Ifx_MAS_ModulatorF16_getVersion(const Ifx_ComponentVersion** componentVersion)
{
    *componentVersion = &Ifx_MHA_ModulatorF16_componentVersion;
}


/* Initialize the modulator */
void Ifx_MAS_ModulatorF16_init(Ifx_MAS_ModulatorF16* self)
{
    /* Initialize the period */
    self->p_period_tick = IFX_MAS_MODULATORF16_CFG_PERIOD_TICK;

    /* Initialize the deadtime */
    self->p_deadTime_tick = IFX_MAS_MODULATORF16_CFG_DEADTIME_TICK;

    /* Active low and trigs to 0 */
    Ifx_MAS_ModulatorF16_setActiveShort(self, self->p_period_tick);

    /* Set the state machine to INIT */
    self->p_status.state              = Ifx_MAS_ModulatorF16_State_init;
    self->p_status.maxAmplitudeFlag   = false;
    self->p_status.overmodulationFlag = false;

    /* Disable module */
    Ifx_MAS_ModulatorF16_enable(self, false);

    /* Initialize current measurement configuration */
    Ifx_MAS_ModulatorF16_initCurrMeasCfg(self);

    /* Minimum sensing time */
    Ifx_MAS_ModulatorF16_p_recalculateMinSenseTime(self);

    /* Maximum amplitude */
    self->p_maxAmplitudeQ15 = IFX_MAS_MODULATORF16_CFG_MAX_AMPLITUDE_Q15;

    /* Initialize internal variables */
    self->p_clearFault = false;

    /* Set flag for second trigger */
    self->p_output.currentReconstructionInfo.secondTriggerIsSum = true;

    /* Set substate to bidirectional two phase shift */
    self->p_status.subState = Ifx_MAS_ModulatorF16_SubState_bidirectionalTwoPhase;

    /* Set the value for deadtime compensation */
    self->p_deadTimeCompensation = IFX_MAS_MODULATORF16_CFG_DEADTIME_TICK / 2;

    /* Minimum on time */
    self->p_minTurnOnTimeHalf_tick = (IFX_MAS_MODULATORF16_CFG_MIN_ON_TIME_TICK +
                                      IFX_MAS_MODULATORF16_CFG_DEADTIME_TICK) / 2;

    /* Disable brake */
    self->p_enableBrake = false;
}


static inline void Ifx_MAS_ModulatorF16_initCurrMeasCfg(Ifx_MAS_ModulatorF16* self)
{
    /* Current measurement config */
    self->p_currentMeasurement.p_driverDelay_tick     = IFX_MAS_MODULATORF16_CFG_DRIVERDELAY_TICK;
    self->p_currentMeasurement.p_ringingTime_tick     = IFX_MAS_MODULATORF16_CFG_RINGINGTIME_TICK;
    self->p_currentMeasurement.p_measurementTime_tick = IFX_MAS_MODULATORF16_CFG_MEASUREMENTTIME_TICK;

    /* Delta to be added if measuring from the beginning */
    Ifx_MAS_ModulatorF16_p_recalculateDeltaBegin(self);

    /* Current measurement trigger point */
    self->p_currentMeasurement.p_measurementPoint =
        (Ifx_MAS_ModulatorF16_measurementPoint)IFX_MAS_MODULATORF16_CFG_MEASUREMENTPOINT;

    /* Delta to be subtracted if measuring from the end */
    Ifx_MAS_ModulatorF16_p_recalculateDeltaEnd(self);
}


static inline bool Ifx_MAS_ModulatorF16_checkFaultStatus(Ifx_MAS_ModulatorF16* self, uint32 faults)
{
    /* boolean output fault status initialized to false */
    bool faultStatusRet = false;

    /* Check if any fault occurred */
    if (faults != 0u)
    {
        /* 1. check for maximum amplitude */
        self->p_status.maxAmplitudeFlag = Ifx_MAS_ModulatorF16_maxAmplitudeFaultStatus(self, &faultStatusRet, faults);

        /* 2. check for overmodulation */
        self->p_status.overmodulationFlag = Ifx_MAS_ModulatorF16_overmodulationFaultStatus(self, &faultStatusRet,
            faults);
    }

    /* Check if a fault clear request was placed */
    else if (self->p_clearFault == true)
    {
        /* Clear all faults in status in case a fault clear request was done */
        self->p_status.maxAmplitudeFlag   = false;
        self->p_status.overmodulationFlag = false;
    }
    else
    {
        /* Final else-clause */
    }

    /* return the fault status */
    return faultStatusRet;
}


static inline bool Ifx_MAS_ModulatorF16_maxAmplitudeFaultStatus(Ifx_MAS_ModulatorF16* self, bool* faultStatusRet,
                                                                uint32 faults)
{
    /* Variable to store the max. amplitude fault status */
    bool faultMaxAmplitude = self->p_status.maxAmplitudeFlag;

    /* 1. check individually for maximum amplitude */
    /* 1.1 Check if this fault is configured as ENABLED */
#if IFX_MAS_MODULATORF16_CFG_FAULT_REACTION_MAX_AMPLITUDE >= IFX_MAS_MODULATORF16_FAULT_REACTION_ENABLE

    if ((faults & IFX_MAS_MODULATORF16_MAXAMPLITUDE_FAULT_STS) != 0)
    {
        /* Set the fault information status bit */
        faultMaxAmplitude = true;

        /* 1.2 Check if this fault is configured for REPORTING */
#if IFX_MAS_MODULATORF16_CFG_FAULT_REACTION_MAX_AMPLITUDE >= IFX_MAS_MODULATORF16_FAULT_REACTION_REPORT_ONLY

#if (IFX_MAS_MODULATORF16_CFG_ENABLE_FAULT_OUT == 1)

        /* report fault source ONLY the 1st time it occurs */
        if (self->p_status.maxAmplitudeFlag == false)
        {
            /* report the fault through the user interface */
            IFX_MAS_MODULATORF16_CFG_FAULT_OUT();
        }

#endif

        /* 1.3 Check if this fault is configured for REACTION */
#if IFX_MAS_MODULATORF16_CFG_FAULT_REACTION_MAX_AMPLITUDE >= IFX_MAS_MODULATORF16_FAULT_REACTION_REPORT_REACT

        /* set the fault status for reacting */
        *faultStatusRet = true;
#endif
#endif
    }

#endif

    return faultMaxAmplitude;
}


static inline bool Ifx_MAS_ModulatorF16_overmodulationFaultStatus(Ifx_MAS_ModulatorF16* self, bool* faultStatusRet,
                                                                  uint32 faults)
{
    /* Variable to store the max. amplitude fault status */
    bool faultOvermodulation = self->p_status.overmodulationFlag;

    /* 2 check individually for Overmodulation */
    /* 2.1 Check if this fault is configured as ENABLED */
#if IFX_MAS_MODULATORF16_CFG_FAULT_REACTION_OVERMODULATION >= IFX_MAS_MODULATORF16_FAULT_REACTION_ENABLE

    if ((faults & IFX_MAS_MODULATORF16_OVERMODULATION_FAULT_STS) != 0)
    {
        /* set the fault information status bit */
        faultOvermodulation = true;

#if IFX_MAS_MODULATORF16_CFG_FAULT_REACTION_OVERMODULATION >= IFX_MAS_MODULATORF16_FAULT_REACTION_REPORT_ONLY

#if (IFX_MAS_MODULATORF16_CFG_ENABLE_FAULT_OUT == 1)

        /* report fault source ONLY the 1st time it occurs */
        if (self->p_status.overmodulationFlag == false)
        {
            /* report the fault through the user interface */
            IFX_MAS_MODULATORF16_CFG_FAULT_OUT();
        }

#endif

        /* 2.3 Check if this fault is configured for REACTION */
#if IFX_MAS_MODULATORF16_CFG_FAULT_REACTION_OVERMODULATION >= IFX_MAS_MODULATORF16_FAULT_REACTION_REPORT_REACT

        /* set the fault status for reacting */
        *faultStatusRet = true;
#endif
#endif
    }

#endif

    return faultOvermodulation;
}


/* polyspace-begin CODE-METRIC:CALLING [Justified:Low] "This is a common function expected to be called multiple times."
 * */
static inline void Ifx_MAS_ModulatorF16_setActiveShort(Ifx_MAS_ModulatorF16* self, sint16 cmprValues)
{
    /* Set the compare up values */
    self->p_output.compareValues_tick[0] = (uint16)cmprValues;
    self->p_output.compareValues_tick[1] = (uint16)cmprValues;
    self->p_output.compareValues_tick[2] = (uint16)cmprValues;

    /* Set the compare down values */
    sint16 downCompareValue = self->p_period_tick - cmprValues;
    self->p_output.compareValues_tick[3] = (uint16)downCompareValue;
    self->p_output.compareValues_tick[4] = (uint16)downCompareValue;
    self->p_output.compareValues_tick[5] = (uint16)downCompareValue;

    /* Set the current triggers */
    self->p_output.triggerTime_tick[0] = 0;
    self->p_output.triggerTime_tick[1] = 0;
}


/* polyspace-end CODE-METRIC:CALLING [Justified:Low] "This is a common function expected to be called multiple times."
 * */
void Ifx_MAS_ModulatorF16_execute(Ifx_MAS_ModulatorF16* self, Ifx_Math_PolarFract16 refVoltage, Ifx_Math_Fract16
                                  dcLinkVoltageQ15, Ifx_Math_3PhaseFract16 currents)
{
    /* Stores the occurred faults */
    uint8                      faultStatus;

    /* Stores if the state machine will be set to false */
    bool                       enterFaultState;

    /* Variable to store the state */
    Ifx_MAS_ModulatorF16_State previousState = (self->p_status.state);
    Ifx_MAS_ModulatorF16_State nextState     = previousState;

    /* Stores the ratio between amplitude of needed voltage and dc voltage */
    Ifx_Math_Fract16           voltageOverDcLinkVoltage;

    /* Limit the inputs and check if any fault occurred */
    faultStatus = Ifx_MAS_ModulatorF16_limitInputs(self, refVoltage.amplitude, dcLinkVoltageQ15,
        &voltageOverDcLinkVoltage);

    /* Update the actual output voltage angle to the ref. voltage angle */
    self->p_output.actualVoltage.angle = refVoltage.angle;

    /* Check fault configuration and return if state machine will be set to false */
    enterFaultState = Ifx_MAS_ModulatorF16_checkFaultStatus(self, faultStatus);

    /* Handles the state machine */
    switch (previousState)
    {
        /* Initialize the module */
        case Ifx_MAS_ModulatorF16_State_init:

            /* do state transition to OFF at the 1st state machine execution cycle */
            nextState = Ifx_MAS_ModulatorF16_State_off;
            break;

        /* Modulator not running */
        case Ifx_MAS_ModulatorF16_State_off:

            /* Call API when state is in off */
            nextState = Ifx_MAS_ModulatorF16_stateOff(self, enterFaultState, refVoltage.angle,
                voltageOverDcLinkVoltage, currents);
            break;

        /* Modulator running */
        case Ifx_MAS_ModulatorF16_State_on:
            nextState = Ifx_MAS_ModulatorF16_stateOn(self, enterFaultState, refVoltage.angle,
                voltageOverDcLinkVoltage, currents);
            break;

        /* Modulator in fault */
        case Ifx_MAS_ModulatorF16_State_fault:

            /* Call API when state is in off */
            nextState = Ifx_MAS_ModulatorF16_stateFault(self);
            break;

        /* Modulator in brake state*/
        case Ifx_MAS_ModulatorF16_State_brake:
            nextState = Ifx_MAS_ModulatorF16_stateBrake(self, enterFaultState);
            break;

        /* Invalid state, transition to init */
        default:

            /* Default transition to INIT */
            nextState = Ifx_MAS_ModulatorF16_State_init;

            /* Active low and trigs to 0 */
            Ifx_MAS_ModulatorF16_setActiveShort(self, self->p_period_tick);
            break;
    }

    /* Clear internal clear fault variable */
    self->p_clearFault = false;

    /* Update state */
    self->p_status.state = nextState;
}


static inline uint8 Ifx_MAS_ModulatorF16_limitInputs(Ifx_MAS_ModulatorF16* self, Ifx_Math_Fract16 refVoltageAmp,
                                                     Ifx_Math_Fract16 dcLinkVoltage,
                                                     Ifx_Math_Fract16* voltageOverDcLinkVoltage)
{
    /* Stores the occurred faults */
    uint8            faultStatus = 0;

    /* Stores actual voltage */
    Ifx_Math_Fract16 limitedAmp;
    Ifx_Math_Fract16 modIndex;
    Ifx_Math_Fract16 dcCheckedAmp;
    Ifx_Math_Fract16 limModIndex;
    Ifx_Math_Fract16 actualVoltageAmp;

    /* Limit amplitude to the maximum amplitude parameter */
    if (refVoltageAmp > self->p_maxAmplitudeQ15)
    {
        limitedAmp   = self->p_maxAmplitudeQ15;
        faultStatus |= (uint8)IFX_MAS_MODULATORF16_MAXAMPLITUDE_FAULT_STS;
    }
    else
    {
        limitedAmp = refVoltageAmp;
    }

    /* Limit DC-Link voltage to positive numbers */
    if (dcLinkVoltage <= 0)
    {
        modIndex     = 0;
        dcCheckedAmp = 0;
    }

    /* Calculate the ratio between amplitude of needed voltage and dc voltage */
    else
    {
        modIndex     = Ifx_Math_DivShLSatNZ_F16(limitedAmp, dcLinkVoltage, 15u);
        dcCheckedAmp = limitedAmp;
    }

    /* Limit modulation index to the Linear Region */
    if (modIndex > IFX_MAS_MODULATORF16_CONST_MI_0907_Q15)
    {
        limModIndex  = IFX_MAS_MODULATORF16_CONST_MI_0907_Q15;
        faultStatus |= (uint8)IFX_MAS_MODULATORF16_OVERMODULATION_FAULT_STS;

        /* Write the actual voltage (DC-Link voltage * linear modulation limit) */
        actualVoltageAmp = Ifx_Math_Mul_F16(dcLinkVoltage, IFX_MAS_MODULATORF16_CONST_MI_0907_Q15);
    }
    else
    {
        limModIndex      = modIndex;
        actualVoltageAmp = dcCheckedAmp;
    }

    /* Write actual voltage output and modulation index */
    *voltageOverDcLinkVoltage              = limModIndex;
    self->p_output.actualVoltage.amplitude = actualVoltageAmp;

    return faultStatus;
}


static inline Ifx_MAS_ModulatorF16_State Ifx_MAS_ModulatorF16_stateOff(Ifx_MAS_ModulatorF16* self, bool
                                                                       enterFaultState, uint32 angle, Ifx_Math_Fract16
                                                                       voltageOverDcLinkVoltage,
                                                                       Ifx_Math_3PhaseFract16 currents)
{
    Ifx_MAS_ModulatorF16_State nextState;

    /* Transition to FAULT and execute configured software reaction */
    if (enterFaultState == true)
    {
        nextState = Ifx_MAS_ModulatorF16_State_fault;
#if IFX_MAS_MODULATORF16_CFG_FAULT_OUT_BEHAVIOR == IFX_MAS_MODULATORF16_OUTPUT_BEHAVIOR_ACTIVE_SHORT_LOW
        Ifx_MAS_ModulatorF16_setActiveShort(self, self->p_period_tick);
#elif IFX_MAS_MODULATORF16_CFG_FAULT_OUT_BEHAVIOR == IFX_MAS_MODULATORF16_OUTPUT_BEHAVIOR_ACTIVE_SHORT_HIGH
        Ifx_MAS_ModulatorF16_setActiveShort(self, 0);
#elif IFX_MAS_MODULATORF16_CFG_FAULT_OUT_BEHAVIOR == IFX_MAS_MODULATORF16_OUTPUT_BEHAVIOR_ACTIVE_SHORT_HIGH_LOW
        Ifx_MAS_ModulatorF16_setActiveShort(self, (self->p_period_tick /
                                                   IFX_MAS_MODULATORF16_MED_COMPARE_VALUE_RATIO));
#endif
    }

    /* Transition to brake and execute configured software reaction */
    else if ((self->p_enableBrake == true)
             && (self->p_enable == true))
    {
        nextState = Ifx_MAS_ModulatorF16_State_brake;
#if IFX_MAS_MODULATORF16_CFG_BRAKE_OUT_BEHAVIOR == IFX_MAS_MODULATORF16_OUTPUT_BEHAVIOR_ACTIVE_SHORT_LOW
        Ifx_MAS_ModulatorF16_setActiveShort(self, self->p_period_tick);
#elif IFX_MAS_MODULATORF16_CFG_BRAKE_OUT_BEHAVIOR == IFX_MAS_MODULATORF16_OUTPUT_BEHAVIOR_ACTIVE_SHORT_HIGH
        Ifx_MAS_ModulatorF16_setActiveShort(self, 0);
#elif IFX_MAS_MODULATORF16_CFG_BRAKE_OUT_BEHAVIOR == IFX_MAS_MODULATORF16_OUTPUT_BEHAVIOR_ACTIVE_SHORT_HIGH_LOW
        Ifx_MAS_ModulatorF16_setActiveShort(self, (self->p_period_tick /
                                                   IFX_MAS_MODULATORF16_MED_COMPARE_VALUE_RATIO));
#endif
    }

    /* Transition to ON if module enabled on the next execution cycle */
    else if (self->p_enable == true)
    {
        nextState = Ifx_MAS_ModulatorF16_State_on;

        /* Set substate to bidirectional two phase shift */
        self->p_status.subState = Ifx_MAS_ModulatorF16_SubState_bidirectionalTwoPhase;

        /* Calculate the new compare values */
        Ifx_MAS_ModulatorF16_updatePolar7SegQ16(self, angle, voltageOverDcLinkVoltage, currents);
    }
    else
    {
        /* Stay in off */
        nextState = Ifx_MAS_ModulatorF16_State_off;
    }

    return nextState;
}


static inline Ifx_MAS_ModulatorF16_State Ifx_MAS_ModulatorF16_stateOn(Ifx_MAS_ModulatorF16* self, bool
                                                                      enterFaultState, uint32 angle, Ifx_Math_Fract16
                                                                      voltageOverDcLinkVoltage, Ifx_Math_3PhaseFract16
                                                                      currents)
{
    Ifx_MAS_ModulatorF16_State nextState;

    /* Transition to FAULT and execute configured software reaction */
    if (enterFaultState == true)
    {
        nextState = Ifx_MAS_ModulatorF16_State_fault;
#if IFX_MAS_MODULATORF16_CFG_FAULT_OUT_BEHAVIOR == IFX_MAS_MODULATORF16_OUTPUT_BEHAVIOR_ACTIVE_SHORT_LOW
        Ifx_MAS_ModulatorF16_setActiveShort(self, self->p_period_tick);
#elif IFX_MAS_MODULATORF16_CFG_FAULT_OUT_BEHAVIOR == IFX_MAS_MODULATORF16_OUTPUT_BEHAVIOR_ACTIVE_SHORT_HIGH
        Ifx_MAS_ModulatorF16_setActiveShort(self, 0);
#elif IFX_MAS_MODULATORF16_CFG_FAULT_OUT_BEHAVIOR == IFX_MAS_MODULATORF16_OUTPUT_BEHAVIOR_ACTIVE_SHORT_HIGH_LOW
        Ifx_MAS_ModulatorF16_setActiveShort(self, (self->p_period_tick /
                                                   IFX_MAS_MODULATORF16_MED_COMPARE_VALUE_RATIO));
#endif
    }

    /* Transition to brake and execute configured software reaction */
    else if ((self->p_enableBrake == true)
             && (self->p_enable == true))
    {
        /* Set the state to brake */
        nextState = Ifx_MAS_ModulatorF16_State_brake;
#if IFX_MAS_MODULATORF16_CFG_BRAKE_OUT_BEHAVIOR == IFX_MAS_MODULATORF16_OUTPUT_BEHAVIOR_ACTIVE_SHORT_LOW
        Ifx_MAS_ModulatorF16_setActiveShort(self, self->p_period_tick);
#elif IFX_MAS_MODULATORF16_CFG_BRAKE_OUT_BEHAVIOR == IFX_MAS_MODULATORF16_OUTPUT_BEHAVIOR_ACTIVE_SHORT_HIGH
        Ifx_MAS_ModulatorF16_setActiveShort(self, 0);
#elif IFX_MAS_MODULATORF16_CFG_BRAKE_OUT_BEHAVIOR == IFX_MAS_MODULATORF16_OUTPUT_BEHAVIOR_ACTIVE_SHORT_HIGH_LOW
        Ifx_MAS_ModulatorF16_setActiveShort(self, (self->p_period_tick /
                                                   IFX_MAS_MODULATORF16_MED_COMPARE_VALUE_RATIO));
#endif
    }

    /* Modulator enabled */
    else if (self->p_enable == true)
    {
        /* Calculate the new compare values */
        Ifx_MAS_ModulatorF16_updatePolar7SegQ16(self, angle, voltageOverDcLinkVoltage, currents);

        /* Set the state to on */
        nextState = Ifx_MAS_ModulatorF16_State_on;
    }
    else

    /* Modulator disabled */
    {
        /* Set the state to OFF */
        nextState = Ifx_MAS_ModulatorF16_State_off;

        /* Active low and trigs to 0 */
        Ifx_MAS_ModulatorF16_setActiveShort(self, self->p_period_tick);
    }

    return nextState;
}


static inline Ifx_MAS_ModulatorF16_State Ifx_MAS_ModulatorF16_stateBrake(Ifx_MAS_ModulatorF16* self, bool
                                                                         enterFaultState)
{
    Ifx_MAS_ModulatorF16_State nextState;

    /* Transition to FAULT and execute configured software reaction */
    if (enterFaultState == true)
    {
        nextState = Ifx_MAS_ModulatorF16_State_fault;
#if IFX_MAS_MODULATORF16_CFG_FAULT_OUT_BEHAVIOR == IFX_MAS_MODULATORF16_OUTPUT_BEHAVIOR_ACTIVE_SHORT_LOW
        Ifx_MAS_ModulatorF16_setActiveShort(self, self->p_period_tick);
#elif IFX_MAS_MODULATORF16_CFG_FAULT_OUT_BEHAVIOR == IFX_MAS_MODULATORF16_OUTPUT_BEHAVIOR_ACTIVE_SHORT_HIGH
        Ifx_MAS_ModulatorF16_setActiveShort(self, 0);
#elif IFX_MAS_MODULATORF16_CFG_FAULT_OUT_BEHAVIOR == IFX_MAS_MODULATORF16_OUTPUT_BEHAVIOR_ACTIVE_SHORT_HIGH_LOW
        Ifx_MAS_ModulatorF16_setActiveShort(self, (self->p_period_tick /
                                                   IFX_MAS_MODULATORF16_MED_COMPARE_VALUE_RATIO));
#endif
    }

    /* Transition to OFF and execute configured software reaction */
    else if ((self->p_enableBrake == false)
             && (self->p_enable == false))
    {
        /* do state transition to OFF */
        nextState = Ifx_MAS_ModulatorF16_State_off;

        /* Active low and trigs to 0 */
        Ifx_MAS_ModulatorF16_setActiveShort(self, self->p_period_tick);
    }

    /* Transition to ON if module enabled on the next execution cycle */
    else if (self->p_enableBrake == false)
    {
        nextState = Ifx_MAS_ModulatorF16_State_on;

        /* Set substate to bidirectional two phase shift */
        self->p_status.subState = Ifx_MAS_ModulatorF16_SubState_bidirectionalTwoPhase;
    }
    else
    {
        /* stay in brake */
        nextState = Ifx_MAS_ModulatorF16_State_brake;
    }

    return nextState;
}


static inline Ifx_MAS_ModulatorF16_State Ifx_MAS_ModulatorF16_stateFault(Ifx_MAS_ModulatorF16* self)
{
    Ifx_MAS_ModulatorF16_State nextState;

    if (self->p_clearFault == true)
    {
        /* do state transition to OFF */
        nextState = Ifx_MAS_ModulatorF16_State_off;

        /* Active low and trigs to 0 */
        Ifx_MAS_ModulatorF16_setActiveShort(self, self->p_period_tick);
    }
    else
    {
        nextState = Ifx_MAS_ModulatorF16_State_fault;
    }

    return nextState;
}


__USED static inline void Ifx_MAS_ModulatorF16_updatePolar7SegQ16(Ifx_MAS_ModulatorF16* self, uint32 angle,
                                                                  Ifx_Math_Fract16 voltageOverDcLinkVoltage,
                                                                  Ifx_Math_3PhaseFract16 currents)
{
    /* Sector number [0..5] */
    uint8  sector;

    /* Lookup table index */
    uint16 tableIndex;

    /* Array to store the switching times ={t_Right, t_Left, t_0} */
    sint16 switchingTimes[3];
    uint32 angleTimesSix;

    /* Calculate sector number */
    angleTimesSix = (angle >> 17) * 6;
    sector        = (uint8)(angleTimesSix >> 15);

    /* Assign sector value */
    self->p_output.currentReconstructionInfo.sector = sector;

    /* Calculate angle in sin lookup table: index [0..4095] */
    tableIndex = (uint16)(angleTimesSix >>
                          (IFX_MAS_MODULATORF16_ANGLE_TO_INDEX)&IFX_MAS_MODULATORF16_TABLESIN60SQRT3_MAX_INDEX);

    /* Get sine and cosine values from the lookup table */
    Ifx_Math_Fract16 sinValue = Ifx_MAS_ModulatorF16_lutSin60Sqrt3[tableIndex];
    Ifx_Math_Fract16 cosValue = Ifx_MAS_ModulatorF16_lutSin60Sqrt3[(IFX_MAS_MODULATORF16_TABLESIN60SQRT3_MAX_INDEX -
                                                                    tableIndex)];

    /* Calculate effective switching times, t_R, t_L, and t_0 */
    /* t_Right */
    switchingTimes[0] = Ifx_Math_MulShR_F16((self->p_period_tick / IFX_MAS_MODULATORF16_MAX_COMPARE_VALUE_RATIO),
        Ifx_Math_Mul_F16(voltageOverDcLinkVoltage, cosValue), 14u);

    /* t_Left */
    switchingTimes[1] = Ifx_Math_MulShR_F16((self->p_period_tick / IFX_MAS_MODULATORF16_MAX_COMPARE_VALUE_RATIO),
        Ifx_Math_Mul_F16(voltageOverDcLinkVoltage, sinValue), 14u);

    /* t_0 */
    switchingTimes[2] = ((self->p_period_tick / IFX_MAS_MODULATORF16_MAX_COMPARE_VALUE_RATIO) - switchingTimes[0] -
                         switchingTimes[1]) / 2;

#if IFX_MAS_MODULATORF16_CFG_ENABLE_DEADTIME_COMPENSATION == 1
    Ifx_MAS_ModulatorF16_calcSwitchTimes(self, switchingTimes, currents, voltageOverDcLinkVoltage);
#else
    Ifx_MAS_ModulatorF16_calcSwitchTimes(self, switchingTimes, voltageOverDcLinkVoltage);
    (void)currents;
#endif /* IFX_MAS_MODULATORF16_CFG_ENABLE_DEADTIME_COMPENSATION */
}


#if IFX_MAS_MODULATORF16_CFG_ENABLE_DEADTIME_COMPENSATION == 1
static inline void Ifx_MAS_ModulatorF16_calcSwitchTimes(Ifx_MAS_ModulatorF16* self, sint16* switchingTimes,
                                                        Ifx_Math_3PhaseFract16 currents, Ifx_Math_Fract16
                                                        voltageOverDcLinkVoltage)
#else
static inline void Ifx_MAS_ModulatorF16_calcSwitchTimes(Ifx_MAS_ModulatorF16*self, sint16*switchingTimes,
                                                        Ifx_Math_Fract16 voltageOverDcLinkVoltage)
#endif /* IFX_MAS_MODULATORF16_CFG_ENABLE_DEADTIME_COMPENSATION */
{
    /* Temporary values to hold the compare values. The array is organized as following:
     * The three phase pulses have different length: long, middle, short.
     * 0 and 3: switch on/off time of the long pulse.
     * 1 and 4: switch on/off time of the middle pulse.
     * 2 and 5: switch on/off time of the short pulse.
     */
    sint16 cmprVal[6];

    /* Calculate compare values based on symmetric switching */
    Ifx_MAS_ModulatorF16_symmetricSwitching(cmprVal, switchingTimes, self->p_output.currentReconstructionInfo.sector);

#if IFX_MAS_MODULATORF16_CFG_ENABLE_DEADTIME_COMPENSATION == 1

    /* Compensate deadtime */
    Ifx_MAS_ModulatorF16_compDeadTime(self, cmprVal, currents);
#endif /* IFX_MAS_MODULATORF16_CFG_ENABLE_DEADTIME_COMPENSATION */

#if IFX_MAS_MODULATORF16_CFG_MIN_ON_TIME_TICK != 0

    /* Check the minimum on time and store the modulation mode. Per default the modulator is in 7 segment mode, in case
     * a pulse is
     * eliminated the modulator will be in 5 segment mode. */
    uint8 isSevenSegment = Ifx_MAS_ModulatorF16_checkMinOnTime(self, cmprVal);
#endif /* IFX_MAS_MODULATORF16_CFG_MIN_ON_TIME_TICK */
    /* Assign compare values symmetrically */
    cmprVal[3] = cmprVal[0];
    cmprVal[4] = cmprVal[1];
    cmprVal[5] = cmprVal[2];

    /* Modify the corresponding compare value for the different asymmetric case */
    if ((voltageOverDcLinkVoltage > IFX_MAS_MODULATORF16_CFG_BI_DIRECTIONAL_SHIFTING_THRESHOLD_HIGH_Q15)
        || ((voltageOverDcLinkVoltage > IFX_MAS_MODULATORF16_CFG_BI_DIRECTIONAL_SHIFTING_THRESHOLD_LOW_Q15)
            && (self->p_status.subState == Ifx_MAS_ModulatorF16_SubState_bidirectionalThreePhase)))
    {
        self->p_status.subState = Ifx_MAS_ModulatorF16_SubState_bidirectionalThreePhase;
#if IFX_MAS_MODULATORF16_CFG_MIN_ON_TIME_TICK != 0
        Ifx_MAS_ModulatorF16_modSwitch(self, cmprVal, switchingTimes, isSevenSegment);
#else
        Ifx_MAS_ModulatorF16_modSwitch(self, cmprVal, switchingTimes);
#endif /* IFX_MAS_MODULATORF16_CFG_MIN_ON_TIME_TICK */
    }
    else
    {
        self->p_status.subState = Ifx_MAS_ModulatorF16_SubState_bidirectionalTwoPhase;
        Ifx_MAS_ModulatorF16_modSwitchSmallAmplitude(self, cmprVal);
    }

    /* Limit switching times between the defined limits */
    Ifx_MAS_ModulatorF16_limitSwitchTimes(self, cmprVal);

    /* Calculate the triggers based on sector */
    Ifx_MAS_ModulatorF16_calcTriggers(self, cmprVal);

    /* Assign compare values and triggers */
    Ifx_MAS_ModulatorF16_assignOutputs(self, cmprVal);
}


static inline void Ifx_MAS_ModulatorF16_symmetricSwitching(sint16* cmprVal, sint16* switchingTimes, uint8 sector)
{
    if ((sector & 1u) == 1u)
    {
        /* sector 1,3,5 */
        cmprVal[0] = switchingTimes[2];
        cmprVal[1] = switchingTimes[1] + switchingTimes[2];
        cmprVal[2] = switchingTimes[0] + cmprVal[1];
    }
    else
    {
        /* sector 0,2,4 */
        cmprVal[0] = switchingTimes[2];
        cmprVal[1] = switchingTimes[0] + switchingTimes[2];
        cmprVal[2] = switchingTimes[1] + cmprVal[1];
    }
}


#if IFX_MAS_MODULATORF16_CFG_MIN_ON_TIME_TICK != 0

/* polyspace-begin CODE-METRIC:VOCF [Justified:Low] "Violation of language scope metric is justified because the second
 * parameter actually points to an array of unique compare values" */
static inline void Ifx_MAS_ModulatorF16_modSwitch(Ifx_MAS_ModulatorF16* self, sint16* compVal, sint16* switchingTimes,
                                                  uint8 isSevenSegment)
#else

/* polyspace-begin CODE-METRIC:VOCF [Justified:Low] "Violation of language scope metric is justified because the second
 * parameter actually points to an array of unique compare values" */
static inline void Ifx_MAS_ModulatorF16_modSwitch(Ifx_MAS_ModulatorF16*self, sint16*compVal, sint16*switchingTimes)
#endif /* IFX_MAS_MODULATORF16_CFG_MIN_ON_TIME_TICK != 0 */
{
    /* Ticks to shift the pulses in order to have a large enough measurement window */
    sint16 shiftValue;

    /* In this mode the second trigger always measures two currents */
    self->p_output.currentReconstructionInfo.secondTriggerIsSum = true;

    /* Is at least one of the measurement windows (tL and/or tR) too small to be measured? */
    if ((switchingTimes[0] < self->p_minSenseTime_tick)
        || (switchingTimes[1] < self->p_minSenseTime_tick))
    {
        /* Calculate the number of ticks by which pulses must be shifted to make the smaller measurement window large
         * enough to enable current measurement
         * Note: shiftValue is negative!*/
        if (switchingTimes[0] < switchingTimes[1])
        {
            shiftValue = switchingTimes[0] - self->p_minSenseTime_tick;
        }
        else
        {
            shiftValue = switchingTimes[1] - self->p_minSenseTime_tick;
        }

        /* Instead of shifting only the middle pulse to the right, it is possible to shift the middle pulse to the right
         * and the large/small pulses to the left by half of the required shift value. This enables current measurement
         * closer to the maximum amplitude. */
        sint16 shiftValueHalfFixed = (sint16)(shiftValue / 2);

        /* Modify count up (compVal[0...2]) and count down (compVal[3...5]) values:
         * Always shift large pulse half value to the left */
        compVal[0] = compVal[0] + shiftValueHalfFixed;
        compVal[3] = compVal[3] - shiftValueHalfFixed;

#if IFX_MAS_MODULATORF16_CFG_MIN_ON_TIME_TICK != 0

        /* The short pulse can only be shifted to the left if it has not already been eliminated by the minimum on time
         * feature.
         * The conditional shift value equals:
         *  - shift value in case of 5 segment mode (short pulse has been eliminated)
         *  - half shift value in case of 7 segment mode */

        /* polyspace +2 MISRA-C3:10.1 [Justified:Low] "Right shift of signed integer is used for better performance and
         * justified since arithmetic shift is supported on all target architectures/compilers." */
        sint16 shiftValueCond = (sint16)(shiftValue >> isSevenSegment);

        /* Shift middle pulse to the right, full or half depending on minimum on time */
        compVal[1] = compVal[1] - shiftValueCond;
        compVal[4] = compVal[4] + shiftValueCond;

        /* Shift short pulse half value to the left only if it has not been eliminated */
        compVal[2] = compVal[2] + ((sint16)isSevenSegment * shiftValueCond);
        compVal[5] = compVal[5] - ((sint16)isSevenSegment * shiftValueCond);

#else

        /* Shift middle pulse half value to the right */
        compVal[1] = compVal[1] - shiftValueHalfFixed;
        compVal[4] = compVal[4] + shiftValueHalfFixed;

        /* Shift short pulse half value to the left */
        compVal[2] = compVal[2] + shiftValueHalfFixed;
        compVal[5] = compVal[5] - shiftValueHalfFixed;

#endif /* IFX_MAS_MODULATORF16_CFG_MIN_ON_TIME_TICK != 0 */
    }
}


/* polyspace-end CODE-METRIC:VOCF [Justified:Low] "Language scope code metric is justified because the second
 * parameter actually points to an array of unique compare values" */

/* polyspace-begin CODE-METRIC:VOCF [Justified:Low] "Violation of language scope metric is justified because the second
 * parameter actually points to an array of unique compare values" */
static inline void Ifx_MAS_ModulatorF16_modSwitchSmallAmplitude(Ifx_MAS_ModulatorF16* self, sint16* cmprVal)
{
    sint16 secondLastCmprVal;
    sint16 lastCmprVal;
    sint16 marginMeasurementWindowLeft;
    sint16 marginMeasurementWindowRight;

    /* Get the margin of the current measurement window for the first measurement trigger */
    marginMeasurementWindowLeft = cmprVal[1] - cmprVal[0] - self->p_minSenseTime_tick;

    /* Shift the long pulse to the left if the window is not enough for a current measurement */
    if (marginMeasurementWindowLeft < 0)
    {
        /* Modify count up */
        cmprVal[0] = cmprVal[0] + marginMeasurementWindowLeft;

        /* Modify count down */
        cmprVal[3] = cmprVal[3] - marginMeasurementWindowLeft;
    }

    /* After the shift of the long pulse, the last switching event could be the fixed pulse or the shift-to-left
     * pulse, find the second last switching event to decide the current measurement window */
    if (cmprVal[3] > cmprVal[4])
    {
        secondLastCmprVal = cmprVal[3];
        lastCmprVal       = cmprVal[4];
    }
    else
    {
        secondLastCmprVal = cmprVal[4];
        lastCmprVal       = cmprVal[3];
    }

    /* Get the current measurement window for the second measurement trigger */
    marginMeasurementWindowRight = cmprVal[5] - secondLastCmprVal - self->p_minSenseTime_tick;

    /* Shift short pulse to the right if the window is not enough */
    if (marginMeasurementWindowRight < 0)
    {
        /* Find the last compare value, the phase to be shifted right should orient on the last compare value */
        sint16 marginMeasurementWindowRightNew = cmprVal[2] - lastCmprVal + self->p_minSenseTime_tick;

        /* Modify count up */
        cmprVal[2] = cmprVal[2] + marginMeasurementWindowRightNew;

        /* Modify count down */
        cmprVal[5] = cmprVal[5] - marginMeasurementWindowRightNew;

        /* Set the flag that the second trigger measures only one current */
        self->p_output.currentReconstructionInfo.secondTriggerIsSum = false;
    }
    else
    {
        /* Set the flag that the second trigger measure two currents */
        self->p_output.currentReconstructionInfo.secondTriggerIsSum = true;
    }
}


/* polyspace-end CODE-METRIC:VOCF [Justified:Low] "Language scope code metric is justified because the second
 * parameter actually points to an array of unique compare values" */
static inline void Ifx_MAS_ModulatorF16_limitSwitchTimes(Ifx_MAS_ModulatorF16* self, sint16* cmprVal)
{
    uint8 i;

    /* Limit compare values */
    for (i = 0; i < 6; i++)
    {
        /* Set input to upper limit */
        if (cmprVal[i] > (self->p_period_tick / IFX_MAS_MODULATORF16_MAX_COMPARE_VALUE_RATIO))
        {
            cmprVal[i] = (self->p_period_tick / IFX_MAS_MODULATORF16_MAX_COMPARE_VALUE_RATIO);
        }

        /* Set input to lower limit */
        else if (cmprVal[i] < 0)
        {
            cmprVal[i] = 0;
        }
        else
        {
            /* Do nothing */
        }
    }
}


static inline void Ifx_MAS_ModulatorF16_calcTriggers(Ifx_MAS_ModulatorF16* self, sint16* cmprVal)
{
    if (self->p_currentMeasurement.p_measurementPoint == Ifx_MAS_ModulatorF16_measurementPoint_beginning)
    {
        /* Measuring from the begin */
        Ifx_MAS_ModulatorF16_measBegin(self, cmprVal);
    }
    else
    {
        /* Measuring from the end */
        Ifx_MAS_ModulatorF16_measEnd(self, cmprVal);
    }
}


static inline void Ifx_MAS_ModulatorF16_measBegin(Ifx_MAS_ModulatorF16* self, sint16* cmprVal)
{
    /* The measurement trigger during up-counting phase */
    self->p_output.triggerTime_tick[0] = (uint16)((sint16)(cmprVal[0] +
                                                           self->p_currentMeasurement.p_deltaBegin_tick));

    /* The measurement trigger during up-counting phase depends on the shifting pattern. */
    if (self->p_output.currentReconstructionInfo.secondTriggerIsSum == true)
    {
        self->p_output.triggerTime_tick[1] = (uint16)((sint16)((sint16)(self->p_period_tick - cmprVal[5]) +
                                                               self->p_currentMeasurement.p_deltaBegin_tick));
    }
    else
    {
        sint16 startingMeasurementWindow = cmprVal[3] < cmprVal[4] ? cmprVal[3] : cmprVal[4];
        self->p_output.triggerTime_tick[1] = (uint16)((sint16)((sint16)(self->p_period_tick -
                                                                        startingMeasurementWindow) +
                                                               self->p_currentMeasurement.p_deltaBegin_tick));
    }
}


static inline void Ifx_MAS_ModulatorF16_measEnd(Ifx_MAS_ModulatorF16* self, sint16* cmprVal)
{
    /* Temporary variables to hold the deltas */
    sint16 deltaEnd = self->p_currentMeasurement.p_deltaEnd_tick;

    /* Measuring from the beginning up */

    /* If the middle pulse is shifted to the right in bidirectional three phase shifting, the second switching event
     * could be the
     * middle pulse or the short pulse, find the second switching event to decide the current measurement window */
    sint16 endFirstMeasurementWindow = cmprVal[1] < cmprVal[2] ? cmprVal[1] : cmprVal[2];
    self->p_output.triggerTime_tick[0] = (uint16)((sint16)(endFirstMeasurementWindow - deltaEnd));

    /* Measuring from the end down */
    if (self->p_output.currentReconstructionInfo.secondTriggerIsSum == true)
    {
        /* After the shift of the long pulse, the last switching event could be the fixed pulse or the shift-to-left
         * pulse, find the second last switching event to decide the current measurement window */
        sint16 endSecondMeasurementWindow = cmprVal[3] < cmprVal[4] ? cmprVal[4] : cmprVal[3];
        self->p_output.triggerTime_tick[1] = (uint16)((sint16)((sint16)(self->p_period_tick -
                                                                        endSecondMeasurementWindow) - deltaEnd));
    }
    else
    {
        self->p_output.triggerTime_tick[1] = (uint16)((sint16)((sint16)(self->p_period_tick - cmprVal[5]) -
                                                               deltaEnd));
    }
}


/* polyspace-begin CODE-METRIC:VOCF [Justified:Low] "For performance reasons, the assignment of the compare values
 * according to the correct sector is implemented as a switch-case" */
static inline void Ifx_MAS_ModulatorF16_assignOutputs(Ifx_MAS_ModulatorF16* self, sint16* cmprVal)
{
    switch (self->p_output.currentReconstructionInfo.sector)
    {
        case 0:
            self->p_output.compareValues_tick[0] = (uint16)cmprVal[0];
            self->p_output.compareValues_tick[1] = (uint16)cmprVal[1];
            self->p_output.compareValues_tick[2] = (uint16)cmprVal[2];
            self->p_output.compareValues_tick[3] = (uint16)((sint16)(self->p_period_tick - cmprVal[3]));
            self->p_output.compareValues_tick[4] = (uint16)((sint16)(self->p_period_tick - cmprVal[4]));
            self->p_output.compareValues_tick[5] = (uint16)((sint16)(self->p_period_tick - cmprVal[5]));
            break;

        case 1:
            self->p_output.compareValues_tick[1] = (uint16)cmprVal[0];
            self->p_output.compareValues_tick[0] = (uint16)cmprVal[1];
            self->p_output.compareValues_tick[2] = (uint16)cmprVal[2];
            self->p_output.compareValues_tick[4] = (uint16)((sint16)(self->p_period_tick - cmprVal[3]));
            self->p_output.compareValues_tick[3] = (uint16)((sint16)(self->p_period_tick - cmprVal[4]));
            self->p_output.compareValues_tick[5] = (uint16)((sint16)(self->p_period_tick - cmprVal[5]));
            break;

        case 2:
            self->p_output.compareValues_tick[1] = (uint16)cmprVal[0];
            self->p_output.compareValues_tick[2] = (uint16)cmprVal[1];
            self->p_output.compareValues_tick[0] = (uint16)cmprVal[2];
            self->p_output.compareValues_tick[4] = (uint16)((sint16)(self->p_period_tick - cmprVal[3]));
            self->p_output.compareValues_tick[5] = (uint16)((sint16)(self->p_period_tick - cmprVal[4]));
            self->p_output.compareValues_tick[3] = (uint16)((sint16)(self->p_period_tick - cmprVal[5]));
            break;

        case 3:
            self->p_output.compareValues_tick[2] = (uint16)cmprVal[0];
            self->p_output.compareValues_tick[1] = (uint16)cmprVal[1];
            self->p_output.compareValues_tick[0] = (uint16)cmprVal[2];
            self->p_output.compareValues_tick[5] = (uint16)((sint16)(self->p_period_tick - cmprVal[3]));
            self->p_output.compareValues_tick[4] = (uint16)((sint16)(self->p_period_tick - cmprVal[4]));
            self->p_output.compareValues_tick[3] = (uint16)((sint16)(self->p_period_tick - cmprVal[5]));
            break;

        case 4:
            self->p_output.compareValues_tick[2] = (uint16)cmprVal[0];
            self->p_output.compareValues_tick[0] = (uint16)cmprVal[1];
            self->p_output.compareValues_tick[1] = (uint16)cmprVal[2];
            self->p_output.compareValues_tick[5] = (uint16)((sint16)(self->p_period_tick - cmprVal[3]));
            self->p_output.compareValues_tick[3] = (uint16)((sint16)(self->p_period_tick - cmprVal[4]));
            self->p_output.compareValues_tick[4] = (uint16)((sint16)(self->p_period_tick - cmprVal[5]));
            break;

        default:
            self->p_output.compareValues_tick[0] = (uint16)cmprVal[0];
            self->p_output.compareValues_tick[2] = (uint16)cmprVal[1];
            self->p_output.compareValues_tick[1] = (uint16)cmprVal[2];
            self->p_output.compareValues_tick[3] = (uint16)((sint16)(self->p_period_tick - cmprVal[3]));
            self->p_output.compareValues_tick[5] = (uint16)((sint16)(self->p_period_tick - cmprVal[4]));
            self->p_output.compareValues_tick[4] = (uint16)((sint16)(self->p_period_tick - cmprVal[5]));
            break;
    }
}


/* polyspace-end CODE-METRIC:VOCF [Justified:Low] "For performance reasons, the assignment of the compare values
 * according to the correct sector is implemented as a switch-case" */

#if IFX_MAS_MODULATORF16_CFG_ENABLE_DEADTIME_COMPENSATION == 1
static inline void Ifx_MAS_ModulatorF16_compDeadTime(Ifx_MAS_ModulatorF16* self, sint16* cmprVal,
                                                     Ifx_Math_3PhaseFract16 currents)
{
    uint8 sector = self->p_output.currentReconstructionInfo.sector;

    /* Map the long, middle, short pulse back to phases */
    uint8 vecMapPhaseU[] = {0, 1, 2, 2, 1, 0};
    uint8 vecMapPhaseV[] = {1, 0, 0, 1, 2, 2};
    uint8 vecMapPhaseW[] = {2, 2, 1, 0, 0, 1};

    /* Compensate the dead time of each phases */
    Ifx_MAS_ModulatorF16_compPhaseDeadTime(self, cmprVal, currents.u, vecMapPhaseU[sector]);
    Ifx_MAS_ModulatorF16_compPhaseDeadTime(self, cmprVal, currents.v, vecMapPhaseV[sector]);
    Ifx_MAS_ModulatorF16_compPhaseDeadTime(self, cmprVal, currents.w, vecMapPhaseW[sector]);
}


static inline void Ifx_MAS_ModulatorF16_compPhaseDeadTime(Ifx_MAS_ModulatorF16* self, sint16* cmprVal,
                                                          Ifx_Math_Fract16 phaseCurrent, uint8 phaseOffset)
{
    if (phaseCurrent >= 0)
    {
        cmprVal[phaseOffset] = cmprVal[phaseOffset] - self->p_deadTimeCompensation;
    }
    else
    {
        cmprVal[phaseOffset] = cmprVal[phaseOffset] + self->p_deadTimeCompensation;
    }
}


#endif /* IFX_MAS_MODULATORF16_CFG_ENABLE_DEADTIME_COMPENSATION */

#if IFX_MAS_MODULATORF16_CFG_MIN_ON_TIME_TICK != 0
static inline uint8 Ifx_MAS_ModulatorF16_checkMinOnTime(Ifx_MAS_ModulatorF16* self, sint16* cmprVal)
{
    /* Unsigned integer is used instead of bool to improve performance */
    uint8  isSevenSegment;

    /* Calculate the half of the minimum pulse length */
    sint16 halfMinimumPulseLength = (self->p_period_tick / 2) - cmprVal[2];

    /* Calculate switching time tLeft */
    sint16 tLeft = cmprVal[2] - cmprVal[1];

    /* Check if short pulse is smaller than the minimum turn on time and middle pulse is large enough for current
     * measurement */
    if ((halfMinimumPulseLength < self->p_minTurnOnTimeHalf_tick)
        && (tLeft > self->p_minSenseTime_tick / 2))
    {
        /* Compensate other phases */
        cmprVal[0] += halfMinimumPulseLength;
        cmprVal[1] += halfMinimumPulseLength;
        cmprVal[2] += halfMinimumPulseLength;

        /* After pulse elimination modulator is in 5 segment */
        isSevenSegment = 0u;
    }
    else
    {
        /* No elimination */
        isSevenSegment = 1u;
    }

    return isSevenSegment;
}


#endif /* IFX_MAS_MODULATORF16_CFG_MIN_ON_TIME_TICK */

/* polyspace-end MISRA2012:D4.14 [Justified:Low] "The caller function has to guarantee that NULL is not passed as
 * argument." */
