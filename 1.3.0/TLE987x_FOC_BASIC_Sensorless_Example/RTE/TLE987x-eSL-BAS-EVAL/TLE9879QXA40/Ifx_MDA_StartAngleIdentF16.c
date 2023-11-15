/*
 * Copyright (c) 2022 Infineon Technologies AG. All Rights Reserved.
 *
 * Use of this file is subject to the terms of use agreed between (i) you or the company in which ordinary course of
 * business you are acting and (ii) Infineon Technologies AG, its affiliates or its licensees. If and as long as no
 * such terms of use are agreed, use of this file is subject to the Evaluation Software License Agreement distributed
 * along with this file within the software delivery package.
 *
 */

/* Module includes */
#include "Ifx_MDA_StartAngleIdentF16.h"
#include "Ifx_MDA_StartAngleIdentF16_Cfg.h"

/* Math library includes */
#include "Ifx_Math_Abs.h"
#include "Ifx_Math_DivShL.h"
#include "Ifx_Math_Mul.h"
#include "Ifx_Math_Sub.h"

/* Representing the phases */
#define IFX_MDA_STARTANGLEIDENTF16_PHASE_A               0U
#define IFX_MDA_STARTANGLEIDENTF16_NEGATIVE_PHASE_C      1U
#define IFX_MDA_STARTANGLEIDENTF16_PHASE_B               2U
#define IFX_MDA_STARTANGLEIDENTF16_NEGATIVE_PHASE_A      3U
#define IFX_MDA_STARTANGLEIDENTF16_PHASE_C               4U
#define IFX_MDA_STARTANGLEIDENTF16_NEGATIVE_PHASE_B      5U

/* Total number of phases */
#define IFX_MDA_STARTANGLEIDENTF16_N_PHASES              6U

/* Cycles where the current measurement will be performed */
#define IFX_MDA_STARTANGLEIDENTF16_MEASUREMENT_POINT1    (IFX_MDA_STARTANGLEIDENTF16_CFG_ZERO_PULSE_LENGTH_CYCLES + 1)
#define IFX_MDA_STARTANGLEIDENTF16_MEASUREMENT_POINT2                                                               \
                                                         (IFX_MDA_STARTANGLEIDENTF16_CFG_ZERO_PULSE_LENGTH_CYCLES + \
     IFX_MDA_STARTANGLEIDENTF16_CFG_POSITIVE_PULSE_LENGTH_CYCLES)

/* Positive pulse end */
#define IFX_MDA_STARTANGLEIDENTF16_POSITIVE_PULSE_END                                                               \
                                                         (IFX_MDA_STARTANGLEIDENTF16_CFG_ZERO_PULSE_LENGTH_CYCLES + \
     IFX_MDA_STARTANGLEIDENTF16_CFG_POSITIVE_PULSE_LENGTH_CYCLES)

/* Negative pulse end */
#define IFX_MDA_STARTANGLEIDENTF16_NEGATIVE_PULSE_END                                                               \
                                                         (IFX_MDA_STARTANGLEIDENTF16_CFG_ZERO_PULSE_LENGTH_CYCLES + \
     IFX_MDA_STARTANGLEIDENTF16_CFG_POSITIVE_PULSE_LENGTH_CYCLES +                                                  \
     IFX_MDA_STARTANGLEIDENTF16_CFG_NEGATIVE_PULSE_LENGTH_CYCLES)

/* Total pulse cycles */
#define IFX_MDA_STARTANGLEIDENTF16_TOTAL_PULSE_CYCLES                                                               \
                                                         (IFX_MDA_STARTANGLEIDENTF16_CFG_ZERO_PULSE_LENGTH_CYCLES + \
     IFX_MDA_STARTANGLEIDENTF16_CFG_POSITIVE_PULSE_LENGTH_CYCLES +                                                  \
     IFX_MDA_STARTANGLEIDENTF16_CFG_NEGATIVE_PULSE_LENGTH_CYCLES)

/* pi/3, Used to output the initial rotor angle */
#define IFX_MDA_STARTANGLEIDENTF16_PI_BY_3_INDEX         (IFX_MATH_PI_INDEX / 3U)

/* 0% and 100 % duty cycles */
#define IFX_MDA_STARTANGLEIDENTF16_ZERO_DUTY_CYCLE       (2 * IFX_MDA_STARTANGLEIDENTF16_CFG_PERIOD_TICK)
#define IFX_MDA_STARTANGLEIDENTF16_FULL_DUTY_CYCLE       (0)

/* Macros to define the component ID */
#define IFX_MDA_STARTANGLEIDENT_COMPONENTID_SOURCEID     ((uint8)Ifx_ComponentID_SourceID_infineonTechnologiesAG)
#define IFX_MDA_STARTANGLEIDENT_COMPONENTID_LIBRARYID    ((uint16)Ifx_ComponentID_LibraryID_mctrlDriveAlgorithm)
#define IFX_MDA_STARTANGLEIDENT_COMPONENTID_MODULEID     (3U)
#define IFX_MDA_STARTANGLEIDENT_COMPONENTID_COMPONENTID1 (1U)

#define IFX_MDA_STARTANGLEIDENT_COMPONENTID_COMPONENTID2 ((uint8)Ifx_ComponentID_ComponentID2_basic)

/* Macros to define the component version */
#define IFX_MDA_STARTANGLEIDENT_COMPONENTVERSION_MAJOR   (1U)
#define IFX_MDA_STARTANGLEIDENT_COMPONENTVERSION_MINOR   (3U)
#define IFX_MDA_STARTANGLEIDENT_COMPONENTVERSION_PATCH   (0U)
#define IFX_MDA_STARTANGLEIDENT_COMPONENTVERSION_T       (0U)
#define IFX_MDA_STARTANGLEIDENT_COMPONENTVERSION_REV     (0U)

/* *INDENT-OFF* */
/* Component ID */
static const Ifx_ComponentID      Ifx_MDA_StartAngleIdent_componentID = {
    .sourceID     = IFX_MDA_STARTANGLEIDENT_COMPONENTID_SOURCEID,
    .libraryID    = IFX_MDA_STARTANGLEIDENT_COMPONENTID_LIBRARYID,
    .moduleID     = IFX_MDA_STARTANGLEIDENT_COMPONENTID_MODULEID,
    .componentID1 = IFX_MDA_STARTANGLEIDENT_COMPONENTID_COMPONENTID1,
    .componentID2 = IFX_MDA_STARTANGLEIDENT_COMPONENTID_COMPONENTID2,
};

/* Component Version */
static const Ifx_ComponentVersion Ifx_MDA_StartAngleIdent_componentVersion = {
    .major = IFX_MDA_STARTANGLEIDENT_COMPONENTVERSION_MAJOR,
    .minor = IFX_MDA_STARTANGLEIDENT_COMPONENTVERSION_MINOR,
    .patch = IFX_MDA_STARTANGLEIDENT_COMPONENTVERSION_PATCH,
    .t     = IFX_MDA_STARTANGLEIDENT_COMPONENTVERSION_T,
    .rev   = IFX_MDA_STARTANGLEIDENT_COMPONENTVERSION_REV
};
/* *INDENT-ON* */

/* polyspace-begin MISRA2012:D4.14 [Justified:Low] "The caller function has to guarantee that NULL is not passed as
 * argument." */

/* Check if any fault occurred and update status variables */
static inline bool Ifx_MDA_StartAngleIdentF16_checkFaultStatus(Ifx_MDA_StartAngleIdentF16* self, Ifx_Math_Fract16
                                                               dcLinkVoltageQ15, uint8 faultReaction, bool
                                                               clearFault);

/* Implements the off state of the state machine */
static inline Ifx_MDA_StartAngleIdentF16_State Ifx_MDA_StartAngleIdentF16_stateOff(Ifx_MDA_StartAngleIdentF16* self);

/* Implements the on state of the state machine */
static inline Ifx_MDA_StartAngleIdentF16_State Ifx_MDA_StartAngleIdentF16_stateOn(Ifx_MDA_StartAngleIdentF16* self,
                                                                                  Ifx_Math_Fract16
                                                                                  dcLinkVoltageQ15,
                                                                                  Ifx_Math_Fract16          *
                                                                                  shuntCurrentsQ15,
                                                                                  bool
                                                                                  faultStatus);

/* Implements the fault state of the state machine */
static inline Ifx_MDA_StartAngleIdentF16_State Ifx_MDA_StartAngleIdentF16_stateFault(Ifx_MDA_StartAngleIdentF16* self,
                                                                                     bool faultStatus, bool
                                                                                     clearFault);

/* Calculate phase admittances for 6 phases */
static inline void Ifx_MDA_StartAngleIdentF16_calculatePhaseAdmittances(Ifx_MDA_StartAngleIdentF16* self,
                                                                        Ifx_Math_Fract16            dcLinkVoltageQ15,
                                                                        Ifx_Math_Fract16          * shuntCurrentsQ15);

/* Generates a pulse for a given phase, containing the zero, positive and negative parts */
static inline void Ifx_MDA_StartAngleIdentF16_generatePulse(Ifx_MDA_StartAngleIdentF16* self, uint8 pulseCounter,
                                                            uint8 phaseCounter);

/* Measure current in the start and end of the positive pulse */
static inline void Ifx_MDA_StartAngleIdentF16_measureCurrent(Ifx_MDA_StartAngleIdentF16* self, uint8 counter,
                                                             Ifx_Math_Fract16* shuntCurrentsQ15);

/* Measure and accumulate voltage in the positive pulse */
static inline void Ifx_MDA_StartAngleIdentF16_measureVoltage(Ifx_MDA_StartAngleIdentF16* self, uint8 counter,
                                                             Ifx_Math_Fract16 dcLinkVoltageQ15);

/* Generate compare values to output necessary pulses */
static inline void Ifx_MDA_StartAngleIdentF16_generateCompares(uint8 phase, uint16 pattern1, uint16 pattern2,
                                                               uint16* compareValues_tick);

/* Estimate rotor position and assign outputs, after all the averaging cycles are completed */
static inline void Ifx_MDA_StartAngleIdentF16_estimatePosition(Ifx_MDA_StartAngleIdentF16* self);

/* Assign compare values to the module outputs */
static inline void Ifx_MDA_StartAngleIdentF16_assignOutputs(Ifx_MDA_StartAngleIdentF16* self,
                                                            uint16                    * compareValues_tick);

/* Set compare values in active short */
static inline void Ifx_MDA_StartAngleIdentF16_setActiveShort(Ifx_MDA_StartAngleIdentF16* self, uint16
                                                             compareValue_tick);

/* Resets the temporary values used for the position estimation */
static inline void Ifx_MDA_StartAngleIdentF16_resetData(Ifx_MDA_StartAngleIdentF16* self);

/* Function to get the component ID */
void Ifx_MDA_StartAngleIdentF16_getID(const Ifx_ComponentID** componentID)
{
    *componentID = &Ifx_MDA_StartAngleIdent_componentID;
}


/* Function to get the component version */
void Ifx_MDA_StartAngleIdentF16_getVersion(const Ifx_ComponentVersion** componentVersion)
{
    *componentVersion = &Ifx_MDA_StartAngleIdent_componentVersion;
}


void Ifx_MDA_StartAngleIdentF16_init(Ifx_MDA_StartAngleIdentF16* self)
{
    /* Initialize state control variables */
    self->p_clearFault = false;
    self->p_enable     = false;

    /* Initialize status variable */
    self->p_status.state              = Ifx_MDA_StartAngleIdentF16_State_init;
    self->p_status.voltageFluctuation = false;

    /* Initialize internal variables */
    self->p_faultCheckingActive = false;

    /* Reset data used for the estimation */
    Ifx_MDA_StartAngleIdentF16_resetData(self);

    /* Set active short low */
    Ifx_MDA_StartAngleIdentF16_setActiveShort(self, IFX_MDA_STARTANGLEIDENTF16_ZERO_DUTY_CYCLE);

    /* Assign trigger initial values */
    /* First trigger is located at the first quarter of the period */
    self->p_output.triggerTime_tick[0] = IFX_MDA_STARTANGLEIDENTF16_ZERO_DUTY_CYCLE / 4;

    /* Second trigger is as late as possible to measure the maximum difference */
    self->p_output.triggerTime_tick[1] = IFX_MDA_STARTANGLEIDENTF16_ZERO_DUTY_CYCLE -
                                         (IFX_MDA_STARTANGLEIDENTF16_CFG_DEADTIME_TICK +
                                          IFX_MDA_STARTANGLEIDENTF16_CFG_RINGINGTIME_TICK +
                                          IFX_MDA_STARTANGLEIDENTF16_CFG_DRIVERDELAY_TICK);

    /* Reset outputs */
    self->p_output.estimatedAngle = 0U;
    self->p_output.maxAdmittance  = 0;
}


void Ifx_MDA_StartAngleIdentF16_execute(Ifx_MDA_StartAngleIdentF16* self, Ifx_Math_Fract16 dcLinkVoltageQ15,
                                        Ifx_Math_Fract16* shuntCurrentsQ15)
{
    /* Store clearFault variable at the beginning of the module */
    bool                             clearFault = self->p_clearFault;

    /* Variable to store the state */
    Ifx_MDA_StartAngleIdentF16_State previousState = self->p_status.state;
    Ifx_MDA_StartAngleIdentF16_State nextState     = previousState;

    /* Check if any fault occurred */
    bool                             faultStatus = Ifx_MDA_StartAngleIdentF16_checkFaultStatus(self, dcLinkVoltageQ15,
        IFX_MDA_STARTANGLEIDENTF16_CFG_FAULT_REACTION_VOLTAGE_FLUCTUATION, clearFault);

    switch (previousState)
    {
        /* Initialize the module */
        case Ifx_MDA_StartAngleIdentF16_State_init:

            /* Set the state to off */
            nextState = Ifx_MDA_StartAngleIdentF16_State_off;
            break;

        /* Start angle identification not running */
        case Ifx_MDA_StartAngleIdentF16_State_off:
            nextState = Ifx_MDA_StartAngleIdentF16_stateOff(self);
            break;

        /* Start angle identification is generating pulses */
        case Ifx_MDA_StartAngleIdentF16_State_on:
            nextState = Ifx_MDA_StartAngleIdentF16_stateOn(self, dcLinkVoltageQ15, shuntCurrentsQ15, faultStatus);
            break;

        /* Start angle identification is in fault */
        case Ifx_MDA_StartAngleIdentF16_State_fault:
            nextState = Ifx_MDA_StartAngleIdentF16_stateFault(self, faultStatus, clearFault);
            break;

        /* Start angle identification is ready */
        case Ifx_MDA_StartAngleIdentF16_State_estimationReady:

            if (self->p_enable == false)
            {
                nextState = Ifx_MDA_StartAngleIdentF16_State_off;
            }
            else
            {
                nextState = Ifx_MDA_StartAngleIdentF16_State_estimationReady;
            }

            break;

        /* Default transition to init */
        default:
            nextState = Ifx_MDA_StartAngleIdentF16_State_init;
            break;
    }

    self->p_status.state = nextState;

    /* Clear the internal variable */
    if (clearFault == true)
    {
        self->p_clearFault = false;
    }
}


static inline bool Ifx_MDA_StartAngleIdentF16_checkFaultStatus(Ifx_MDA_StartAngleIdentF16* self, Ifx_Math_Fract16
                                                               dcLinkVoltageQ15, uint8 faultReaction, bool clearFault)
{
    /* Variable to store the voltage fluctuation fault status */
    bool faultVoltageFluctuation;
    bool faultStatus = false;

    /* Clear flag if fault clear is true */
    if (clearFault == true)
    {
        faultVoltageFluctuation     = false;
        self->p_faultCheckingActive = false;
    }
    else
    {
        faultVoltageFluctuation = self->p_status.voltageFluctuation;
    }

    /* 1 Check if this fault is configured as ENABLED */
    if (faultReaction >= IFX_MDA_STARTANGLEIDENTF16_FAULT_REACTION_ENABLE)
    {
        /* Calculate the voltage tolerance based on last voltage measurement */
        Ifx_Math_Fract16 voltageTolerance = Ifx_Math_Mul_F16(self->p_previousDcLinkVoltageQ15,
            IFX_MDA_STARTANGLEIDENTF16_CFG_VOLTAGE_FLUCTUATION_RANGE_Q15);

        /* Calculate voltage difference between previous measurement and current measurement */
        Ifx_Math_Fract16 voltageDifference = Ifx_Math_Sub_F16(self->p_previousDcLinkVoltageQ15, dcLinkVoltageQ15);

        /* Store voltage for the next cycle */
        self->p_previousDcLinkVoltageQ15 = dcLinkVoltageQ15;

        /* Check if difference is bigger than tolerance */
        if ((Ifx_Math_Abs_F16(voltageDifference) > voltageTolerance)
            && (self->p_faultCheckingActive == true))
        {
            /* Set the fault information status bit */
            faultVoltageFluctuation = true;

            /* 2 Check if this fault is configured for REPORTING */
            if (faultReaction >= IFX_MDA_STARTANGLEIDENTF16_FAULT_REACTION_REPORT_ONLY)
            {
                /* report the fault through the user interface */
#if (IFX_MDA_STARTANGLEIDENTF16_CFG_ENABLE_FAULT_OUT == 1)

                /* report fault source ONLY */
                if (self->p_status.voltageFluctuation == false)
                {
                    /* report the fault through the user interface */
                    IFX_MDA_STARTANGLEIDENTF16_CFG_FAULT_OUT();
                }

#endif

                /* 3 Check if this fault is configured for REACTION */
                if (faultReaction >= IFX_MDA_STARTANGLEIDENTF16_FAULT_REACTION_REPORT_REACT)
                {
                    /* set the fault status for reacting */
                    faultStatus = true;
                }
            }
        }
    }

    self->p_status.voltageFluctuation = faultVoltageFluctuation;

    return faultStatus;
}


static inline Ifx_MDA_StartAngleIdentF16_State Ifx_MDA_StartAngleIdentF16_stateOff(Ifx_MDA_StartAngleIdentF16* self)
{
    Ifx_MDA_StartAngleIdentF16_State nextState;

    if (self->p_enable == true)
    {
        /* Reset data and transition to on */
        Ifx_MDA_StartAngleIdentF16_resetData(self);
        nextState = Ifx_MDA_StartAngleIdentF16_State_on;
    }
    else
    {
        nextState = Ifx_MDA_StartAngleIdentF16_State_off;
    }

    return nextState;
}


static inline Ifx_MDA_StartAngleIdentF16_State Ifx_MDA_StartAngleIdentF16_stateOn(Ifx_MDA_StartAngleIdentF16* self,
                                                                                  Ifx_Math_Fract16
                                                                                  dcLinkVoltageQ15,
                                                                                  Ifx_Math_Fract16          *
                                                                                  shuntCurrentsQ15,
                                                                                  bool
                                                                                  faultStatus)
{
    Ifx_MDA_StartAngleIdentF16_State nextState;

    /* If fault occurred, set active short low */
    if (faultStatus == true)
    {
        Ifx_MDA_StartAngleIdentF16_setActiveShort(self, IFX_MDA_STARTANGLEIDENTF16_ZERO_DUTY_CYCLE);
        nextState = Ifx_MDA_StartAngleIdentF16_State_fault;
    }

    /* Module is disabled */
    else if (self->p_enable == false)
    {
        Ifx_MDA_StartAngleIdentF16_setActiveShort(self, IFX_MDA_STARTANGLEIDENTF16_ZERO_DUTY_CYCLE);
        nextState = Ifx_MDA_StartAngleIdentF16_State_off;
    }

    /* Module is enabled */
    else
    {
        /* Calculate admittances for the defined cycles */
        if (self->p_averageCounter < IFX_MDA_STARTANGLEIDENTF16_CFG_AVERAGING_CYCLES)
        {
            Ifx_MDA_StartAngleIdentF16_calculatePhaseAdmittances(self, dcLinkVoltageQ15, shuntCurrentsQ15);
            nextState = Ifx_MDA_StartAngleIdentF16_State_on;
        }

        /* Estimate and output the rotor position */
        else
        {
            Ifx_MDA_StartAngleIdentF16_setActiveShort(self, IFX_MDA_STARTANGLEIDENTF16_ZERO_DUTY_CYCLE);
            Ifx_MDA_StartAngleIdentF16_estimatePosition(self);
            nextState = Ifx_MDA_StartAngleIdentF16_State_estimationReady;
        }
    }

    return nextState;
}


static inline Ifx_MDA_StartAngleIdentF16_State Ifx_MDA_StartAngleIdentF16_stateFault(Ifx_MDA_StartAngleIdentF16* self,
                                                                                     bool faultStatus, bool
                                                                                     clearFault)
{
    Ifx_MDA_StartAngleIdentF16_State nextState;

    /* Check if a fault clear was requested and a fault didn't occur */
    if ((clearFault == true)
        && (faultStatus == false))
    {
        nextState = Ifx_MDA_StartAngleIdentF16_State_off;
    }
    else
    {
        nextState = Ifx_MDA_StartAngleIdentF16_State_fault;
    }

    return nextState;
}


static inline void Ifx_MDA_StartAngleIdentF16_calculatePhaseAdmittances(Ifx_MDA_StartAngleIdentF16* self,
                                                                        Ifx_Math_Fract16            dcLinkVoltageQ15,
                                                                        Ifx_Math_Fract16          * shuntCurrentsQ15)
{
    /* Iterate all phases */
    if (self->p_phaseCounter < IFX_MDA_STARTANGLEIDENTF16_N_PHASES)
    {
        /* Generate pulse, measure currents and accumulate voltage */
        Ifx_MDA_StartAngleIdentF16_generatePulse(self, self->p_pulseCounter, self->p_phaseCounter);

        /* Measure current */
        Ifx_MDA_StartAngleIdentF16_measureCurrent(self, self->p_pulseCounter, shuntCurrentsQ15);

        /* Measure and accumulate voltage */
        Ifx_MDA_StartAngleIdentF16_measureVoltage(self, self->p_pulseCounter, dcLinkVoltageQ15);

        if (self->p_pulseCounter < IFX_MDA_STARTANGLEIDENTF16_TOTAL_PULSE_CYCLES)
        {
            /* Increment counter */
            self->p_pulseCounter++;
        }
        else
        {
            /* Calculate current variation */
            Ifx_Math_Fract16 deltaCurrent = Ifx_Math_Abs_F16(self->p_secondCurrent - self->p_firstCurrent);
            self->p_phaseAdmittance[self->p_phaseCounter] = Ifx_Math_DivShL_F32((Ifx_Math_Fract32)deltaCurrent,
                self->p_voltageAccumulator, 15u);

            /* Reset pulse counter and voltage accumulator */
            self->p_pulseCounter       = 1;
            self->p_voltageAccumulator = 0;

            /* Increment phaseCounter */
            self->p_phaseCounter++;
        }
    }

    /* Accumulate admittances per phase */
    else
    {
        for (uint8 i = 0; i < IFX_MDA_STARTANGLEIDENTF16_N_PHASES; i++)
        {
            self->p_phaseAdmittanceAccumulator[i] += self->p_phaseAdmittance[i];
        }

        /* Reset phase counter */
        self->p_phaseCounter = 0;

        /* Increment averaging counter */
        self->p_averageCounter++;
    }
}


static inline void Ifx_MDA_StartAngleIdentF16_measureCurrent(Ifx_MDA_StartAngleIdentF16* self, uint8 counter,
                                                             Ifx_Math_Fract16* shuntCurrentsQ15)
{
    if (counter == IFX_MDA_STARTANGLEIDENTF16_MEASUREMENT_POINT1)
    {
        /* Take first measurement */
        self->p_firstCurrent = shuntCurrentsQ15[0];
    }
    else if (counter == IFX_MDA_STARTANGLEIDENTF16_MEASUREMENT_POINT2)
    {
        /* Take second measurement */
        self->p_secondCurrent = shuntCurrentsQ15[1];
    }
    else
    {
        /* No current measurement is needed */
    }
}


static inline void Ifx_MDA_StartAngleIdentF16_measureVoltage(Ifx_MDA_StartAngleIdentF16* self, uint8 counter,
                                                             Ifx_Math_Fract16 dcLinkVoltageQ15)
{
    /* Measure voltage between during positive pulse */
    if ((counter >= IFX_MDA_STARTANGLEIDENTF16_MEASUREMENT_POINT1)
        && (counter <= IFX_MDA_STARTANGLEIDENTF16_MEASUREMENT_POINT2))
    {
        self->p_voltageAccumulator += dcLinkVoltageQ15;

        /* Activate fault checking only while measuring the voltage */
        self->p_faultCheckingActive = true;
    }
    else
    {
        self->p_faultCheckingActive = false;
    }
}


/* polyspace-begin CODE-METRIC:VOCF [Justified:Low] "Language scope code metric was not considered in order to improve
 * readability." */
static inline void Ifx_MDA_StartAngleIdentF16_generatePulse(Ifx_MDA_StartAngleIdentF16* self, uint8 pulseCounter,
                                                            uint8 phaseCounter)
{
    uint16 compareValues_tick[3];

    /* Apply patterns to generate positive pulse */
    if ((pulseCounter >= IFX_MDA_STARTANGLEIDENTF16_CFG_ZERO_PULSE_LENGTH_CYCLES)
        && (pulseCounter < IFX_MDA_STARTANGLEIDENTF16_POSITIVE_PULSE_END))
    {
        Ifx_MDA_StartAngleIdentF16_generateCompares(phaseCounter, IFX_MDA_STARTANGLEIDENTF16_FULL_DUTY_CYCLE,
            IFX_MDA_STARTANGLEIDENTF16_ZERO_DUTY_CYCLE, compareValues_tick);
    }

    /* Apply patterns to generate negative pulse */
    else if ((pulseCounter >= IFX_MDA_STARTANGLEIDENTF16_POSITIVE_PULSE_END)
             && (pulseCounter < IFX_MDA_STARTANGLEIDENTF16_NEGATIVE_PULSE_END))
    {
        Ifx_MDA_StartAngleIdentF16_generateCompares(phaseCounter, IFX_MDA_STARTANGLEIDENTF16_ZERO_DUTY_CYCLE,
            IFX_MDA_STARTANGLEIDENTF16_FULL_DUTY_CYCLE, compareValues_tick);
    }

    /* Beginning and end of the pulse, set active short */
    else
    {
        /* Set the first compare values */
        for (uint8 i = 0; i < 3; i++)
        {
            compareValues_tick[i] = IFX_MDA_STARTANGLEIDENTF16_ZERO_DUTY_CYCLE;
        }
    }

    /* Assign compare values to the module outputs */
    Ifx_MDA_StartAngleIdentF16_assignOutputs(self, compareValues_tick);
}


/* polyspace-end CODE-METRIC:VOCF [Justified:Low] "Language scope code metric was not considered in order to improve
 * readability." */
static inline void Ifx_MDA_StartAngleIdentF16_estimatePosition(Ifx_MDA_StartAngleIdentF16* self)
{
    /* Variables to store the maximum admittance and phase where it is measured */
    Ifx_Math_Fract32 maxAdmittance = 0;
    uint8            maxPhase      = 0;

    for (uint8 i = 0; i < IFX_MDA_STARTANGLEIDENTF16_N_PHASES; i++)
    {
        /* Calculate average admittance per phase */
        Ifx_Math_Fract32 averageAdmittance = self->p_phaseAdmittanceAccumulator[i] /
                                             IFX_MDA_STARTANGLEIDENTF16_CFG_AVERAGING_CYCLES;

        /* Find the maximumAdmittance and phase where it occurs */
        if (averageAdmittance > maxAdmittance)
        {
            maxAdmittance = averageAdmittance;
            maxPhase      = i;
        }
    }

    /* Assign outputs */
    self->p_output.maxAdmittance  = maxAdmittance;
    self->p_output.estimatedAngle = maxPhase * IFX_MDA_STARTANGLEIDENTF16_PI_BY_3_INDEX;
}


/* polyspace-begin CODE-METRIC:VOCF [Justified:Low] "Language scope code metric was not considered in order to improve
 * readability." */
static inline void Ifx_MDA_StartAngleIdentF16_generateCompares(uint8 phase, uint16 pattern1, uint16 pattern2,
                                                               uint16* compareValues_tick)
{
    /* Switch for the phases */
    switch (phase)
    {
        case IFX_MDA_STARTANGLEIDENTF16_PHASE_A:
            compareValues_tick[0] = pattern1;
            compareValues_tick[1] = pattern2;
            compareValues_tick[2] = pattern2;
            break;

        case IFX_MDA_STARTANGLEIDENTF16_NEGATIVE_PHASE_A:
            compareValues_tick[0] = pattern2;
            compareValues_tick[1] = pattern1;
            compareValues_tick[2] = pattern1;
            break;

        case IFX_MDA_STARTANGLEIDENTF16_PHASE_B:
            compareValues_tick[0] = pattern2;
            compareValues_tick[1] = pattern1;
            compareValues_tick[2] = pattern2;
            break;

        case IFX_MDA_STARTANGLEIDENTF16_NEGATIVE_PHASE_B:
            compareValues_tick[0] = pattern1;
            compareValues_tick[1] = pattern2;
            compareValues_tick[2] = pattern1;
            break;

        case IFX_MDA_STARTANGLEIDENTF16_PHASE_C:
            compareValues_tick[0] = pattern2;
            compareValues_tick[1] = pattern2;
            compareValues_tick[2] = pattern1;
            break;

        case IFX_MDA_STARTANGLEIDENTF16_NEGATIVE_PHASE_C:
            compareValues_tick[0] = pattern1;
            compareValues_tick[1] = pattern1;
            compareValues_tick[2] = pattern2;
            break;

        default:
            compareValues_tick[0] = IFX_MDA_STARTANGLEIDENTF16_ZERO_DUTY_CYCLE;
            compareValues_tick[1] = IFX_MDA_STARTANGLEIDENTF16_ZERO_DUTY_CYCLE;
            compareValues_tick[2] = IFX_MDA_STARTANGLEIDENTF16_ZERO_DUTY_CYCLE;
            break;
    }
}


/* polyspace-end CODE-METRIC:VOCF [Justified:Low] "Language scope code metric was not considered in order to improve
 * readability." */
static inline void Ifx_MDA_StartAngleIdentF16_assignOutputs(Ifx_MDA_StartAngleIdentF16* self,
                                                            uint16                    * compareValues_tick)
{
    for (uint8 i = 0; i < 3; i++)
    {
        /* Assign compare up values */
        self->p_output.compareValues_tick[i] = compareValues_tick[i];

        /* Assign compare down values */
        self->p_output.compareValues_tick[i + 3] = IFX_MDA_STARTANGLEIDENTF16_ZERO_DUTY_CYCLE - compareValues_tick[i];
    }
}


static inline void Ifx_MDA_StartAngleIdentF16_setActiveShort(Ifx_MDA_StartAngleIdentF16* self, uint16
                                                             compareValue_tick)
{
    /* Set the compare up values */
    self->p_output.compareValues_tick[0] = compareValue_tick;
    self->p_output.compareValues_tick[1] = compareValue_tick;
    self->p_output.compareValues_tick[2] = compareValue_tick;

    /* Set the compare down values */
    self->p_output.compareValues_tick[3] = IFX_MDA_STARTANGLEIDENTF16_ZERO_DUTY_CYCLE - compareValue_tick;
    self->p_output.compareValues_tick[4] = IFX_MDA_STARTANGLEIDENTF16_ZERO_DUTY_CYCLE - compareValue_tick;
    self->p_output.compareValues_tick[5] = IFX_MDA_STARTANGLEIDENTF16_ZERO_DUTY_CYCLE - compareValue_tick;
}


static inline void Ifx_MDA_StartAngleIdentF16_resetData(Ifx_MDA_StartAngleIdentF16* self)
{
    /* Reset variables to store the phase admittances */
    for (uint8 i = 0; i < 6; i++)
    {
        self->p_phaseAdmittance[i]            = 0;
        self->p_phaseAdmittanceAccumulator[i] = 0;
    }

    /* Reset counters */
    self->p_pulseCounter             = 1U;
    self->p_averageCounter           = 0U;
    self->p_phaseCounter             = 0U;
    self->p_firstCurrent             = 0;
    self->p_previousDcLinkVoltageQ15 = 0;
    self->p_secondCurrent            = 0;
    self->p_voltageAccumulator       = 0;
}


/* polyspace-end MISRA2012:D4.14 [Justified:Low] "The caller function has to guarantee that NULL is not passed as
 * argument." */
