/*
 * Copyright (c) 2022 Infineon Technologies AG. All Rights Reserved.
 *
 * Use of this file is subject to the terms of use agreed between (i) you or the company in which ordinary course of
 * business you are acting and (ii) Infineon Technologies AG, its affiliates or its licensees. If and as long as no
 * such terms of use are agreed, use of this file is subject to the Evaluation Software License Agreement distributed
 * along with this file within the software delivery package.
 *
 */

#include "Ifx_MS_FocSolutionF16.h"
#include "Ifx_MS_FocSolutionF16_Cfg.h"

#include "Ifx_Math_Abs.h"
#include "Ifx_Math_CartToPolar.h"
#include "Ifx_Math_Clarke.h"
#include "Ifx_Math_Park.h"
#include "Ifx_Math_PolarToCart.h"
#include "Ifx_Math_NegSat.h"
#include "Ifx_Math_Sub.h"
#include "Ifx_Math_AddSat.h"
#include "Ifx_Math_DivShLSat.h"
#include "Ifx_Math_SubSat.h"

/* Macro to saturate the value with wrapping */
#define IFX_MS_FOCSOLUTIONF16_2MAX                              (-2 * IFX_MATH_FRACT16_MIN)

/* 30 degrees in Q32 */
#define IFX_MS_FOCSOLUTIONF16_PIBY6                             (IFX_MATH_PI_INDEX / 6U)

/* 90 degrees in Q32 */
#define IFX_MS_FOCSOLUTIONF16_PIBY2                             (IFX_MATH_PI_INDEX / 2U)

/* Macro for transition mode when mode is direct transition */
#define IFX_MS_FOCSOLUTIONF16_TRANSITION_MODE_DIRECT_TRANSITION (0)

/* Macro for transition mode when mode is smooth transition */
#define IFX_MS_FOCSOLUTIONF16_TRANSITION_MODE_SMOOTH_TRANSITION (1)

/* Macros to define the component ID */
#define IFX_MS_FOCSOLUTION_TLE987_COMPONENTID_SOURCEID                   \
                                                                ((uint8) \
     Ifx_ComponentID_SourceID_infineonTechnologiesAG)
#define IFX_MS_FOCSOLUTION_TLE987_COMPONENTID_LIBRARYID         ((uint16)Ifx_ComponentID_LibraryID_mctrlSolution)
#define IFX_MS_FOCSOLUTION_TLE987_COMPONENTID_MODULEID          (0U)
#define IFX_MS_FOCSOLUTION_TLE987_COMPONENTID_COMPONENTID1      (1U)

#define IFX_MS_FOCSOLUTION_TLE987_COMPONENTID_COMPONENTID2      ((uint8)Ifx_ComponentID_ComponentID2_basic)

/* Macros to define the component version */
#define IFX_MS_FOCSOLUTION_TLE987_COMPONENTVERSION_MAJOR        (1U)
#define IFX_MS_FOCSOLUTION_TLE987_COMPONENTVERSION_MINOR        (3U)
#define IFX_MS_FOCSOLUTION_TLE987_COMPONENTVERSION_PATCH        (0U)
#define IFX_MS_FOCSOLUTION_TLE987_COMPONENTVERSION_T            (0U)
#define IFX_MS_FOCSOLUTION_TLE987_COMPONENTVERSION_REV          (0U)

/* *INDENT-OFF* */

/* Component ID */
static const Ifx_ComponentID      Ifx_MS_FocSolution_TLE987_componentID = {
    .sourceID     = IFX_MS_FOCSOLUTION_TLE987_COMPONENTID_SOURCEID,
    .libraryID    = IFX_MS_FOCSOLUTION_TLE987_COMPONENTID_LIBRARYID,
    .moduleID     = IFX_MS_FOCSOLUTION_TLE987_COMPONENTID_MODULEID,
    .componentID1 = IFX_MS_FOCSOLUTION_TLE987_COMPONENTID_COMPONENTID1,
    .componentID2 = IFX_MS_FOCSOLUTION_TLE987_COMPONENTID_COMPONENTID2,
};

/* Component Version */
static const Ifx_ComponentVersion Ifx_MS_FocSolution_TLE987_componentVersion = {
    .major = IFX_MS_FOCSOLUTION_TLE987_COMPONENTVERSION_MAJOR,
    .minor = IFX_MS_FOCSOLUTION_TLE987_COMPONENTVERSION_MINOR,
    .patch = IFX_MS_FOCSOLUTION_TLE987_COMPONENTVERSION_PATCH,
    .t     = IFX_MS_FOCSOLUTION_TLE987_COMPONENTVERSION_T,
    .rev   = IFX_MS_FOCSOLUTION_TLE987_COMPONENTVERSION_REV
};
/* *INDENT-ON* */

/* polyspace-begin MISRA2012:D4.14 [Justified:Low] "The caller function has to guarantee that NULL is not passed as
 * argument." */

/* Initialization functions called by Ifx_MS_FocSolutionF16_init() */
void               Ifx_MS_FocSolutionF16_initModules(Ifx_MS_FocSolutionF16* self);
static inline void Ifx_MS_FocSolutionF16_initDriveAlgo(Ifx_MS_FocSolutionF16* self);
static inline void Ifx_MS_FocSolutionF16_initSpeedPi(Ifx_MS_FocSolutionF16* self);
static inline void Ifx_MS_FocSolutionF16_initSpeedAccelerationLimiters(Ifx_MS_FocSolutionF16* self);
static inline void Ifx_MS_FocSolutionF16_initStartCurrentRateLimiter(Ifx_MS_FocSolutionF16* self);

/* Functions called by Ifx_MS_FocSolutionF16_executeControlMode() */
static inline Ifx_MHA_MeasurementADC_TLE987_Output Ifx_MS_FocSolutionF16_measureAndReconstruct(
    Ifx_MS_FocSolutionF16* self);
static inline uint32                Ifx_MS_FocSolutionF16_fluxEstimation(Ifx_MS_FocSolutionF16* self);
static inline Ifx_Math_PolarFract16 Ifx_MS_FocSolutionF16_regulationLoop(Ifx_MS_FocSolutionF16* self,
                                                                         uint32 estimatedAngle, Ifx_Math_Fract16
                                                                         dcLinkVoltageQ15);
static inline void Ifx_MS_FocSolutionF16_voltageGeneration(
    Ifx_MS_FocSolutionF16* self, Ifx_Math_PolarFract16 voltageCommandPolar, Ifx_MHA_MeasurementADC_TLE987_Output
    measurementADCOutput);
static void Ifx_MS_FocSolutionF16_stateMachine(Ifx_MS_FocSolutionF16* self, Ifx_Math_Fract16 speedQ15, bool
                                               faultStatus, Ifx_Math_CmpFract16 currentsDqRef,
                                               Ifx_MDA_IToFControllerF16_Output
                                               iToFOutput);

/* State functions called by Ifx_MS_FocSolutionF16_stateMachine function */
static inline Ifx_MS_FocSolutionF16_State Ifx_MS_FocSolutionF16_stateInit(Ifx_MS_FocSolutionF16* self);
static inline Ifx_MS_FocSolutionF16_State Ifx_MS_FocSolutionF16_stateOff(Ifx_MS_FocSolutionF16* self, bool
                                                                         faultStatus);
static inline Ifx_MS_FocSolutionF16_State Ifx_MS_FocSolutionF16_stateStandby(Ifx_MS_FocSolutionF16* self, bool
                                                                             faultStatus);
static inline Ifx_MS_FocSolutionF16_State Ifx_MS_FocSolutionF16_stateFault(Ifx_MS_FocSolutionF16* self);

/* Functions called by Ifx_MS_FocSolutionF16_stateFault() */
/* Requests a clear fault of the underlying modules */
static inline bool Ifx_MS_FocSolutionF16_clearModuleFaults(Ifx_MS_FocSolutionF16* self);
static inline bool Ifx_MS_FocSolutionF16_clearModuleFaultsMHA(Ifx_MS_FocSolutionF16* self);
static inline void Ifx_MS_FocSolutionF16_clearModuleFaultsMAS(Ifx_MS_FocSolutionF16* self);

#if IFX_MS_FOCSOLUTIONF16_CFG_INCLUDE_STARTANGLE_IDENT == 1
static inline void Ifx_MS_FocSolutionF16_clearModuleFaultsMDA(Ifx_MS_FocSolutionF16* self);
#endif /* IFX_MS_FOCSOLUTIONF16_CFG_INCLUDE_STARTANGLE_IDENT == 1 */
/* Returns true if any module has a clear fault pending and false otherwise */
static inline bool Ifx_MS_FocSolutionF16_anyClearFaultIsPending(Ifx_MS_FocSolutionF16* self);

/* Check if all faults of the underlying module are successfully cleared */
static inline bool                        Ifx_MS_FocSolutionF16_faultSuccessfullyCleared(Ifx_MS_FocSolutionF16* self);
static inline Ifx_MS_FocSolutionF16_State Ifx_MS_FocSolutionF16_stateRun(Ifx_MS_FocSolutionF16* self, Ifx_Math_Fract16
                                                                         speedQ15, bool faultStatus,
                                                                         Ifx_Math_CmpFract16 currentsDqRef,
                                                                         Ifx_MDA_IToFControllerF16_Output iToFOutput);
static inline Ifx_MS_FocSolutionF16_State Ifx_MS_FocSolutionF16_stateRampDown(Ifx_MS_FocSolutionF16* self, bool
                                                                              faultStatus, Ifx_Math_CmpFract16
                                                                              currentsDqRef,
                                                                              Ifx_MDA_IToFControllerF16_Output
                                                                              iToFOutput);

#if IFX_MS_FOCSOLUTIONF16_CFG_INCLUDE_STARTANGLE_IDENT == 1
static inline Ifx_MS_FocSolutionF16_State Ifx_MS_FocSolutionF16_stateStartAngleIdent(Ifx_MS_FocSolutionF16* self,
                                                                                     Ifx_Math_Fract16 speedQ15, bool
                                                                                     faultStatus,
                                                                                     Ifx_MDA_IToFControllerF16_Output
                                                                                     iToFOutput);

/* Set angle previous value of iToF controller to the estimated angle minus pi/2 offset and tolerance based on the sign
 * of the reference speed */
void Ifx_MS_FocSolutionF16_setAnglePreviousValue(Ifx_MS_FocSolutionF16* self, Ifx_Math_Fract16 speedQ15,
                                                 Ifx_MDA_StartAngleIdentF16_Output startAngleIdentOutput);
#endif /* IFX_MS_FOCSOLUTIONF16_CFG_INCLUDE_STARTANGLE_IDENT == 1 */
/* Execute the sub-state machine */
static inline void Ifx_MS_FocSolutionF16_subStateMachine(Ifx_MS_FocSolutionF16* self, Ifx_MDA_IToFControllerF16_Output
                                                         iToFOutput, Ifx_Math_CmpFract16 currentsDqRef);

/* State functions called by Ifx_MS_FocSolutionF16_subStateMachine function */
static inline Ifx_MS_FocSolutionF16_SubState Ifx_MS_FocSolutionF16_subStateOpenLoop(Ifx_MS_FocSolutionF16* self,
                                                                                    Ifx_MDA_IToFControllerF16_Output
                                                                                    iToFOutput, Ifx_Math_CmpFract16
                                                                                    currentsDqRef);

#if (IFX_MS_FOCSOLUTIONF16_CFG_TRANSITION_MODE == IFX_MS_FOCSOLUTIONF16_TRANSITION_MODE_SMOOTH_TRANSITION)
static inline Ifx_MS_FocSolutionF16_SubState Ifx_MS_FocSolutionF16_subStateTransitUp(Ifx_MS_FocSolutionF16* self,
                                                                                     Ifx_MDA_IToFControllerF16_Output
                                                                                     iToFOutput, Ifx_Math_CmpFract16
                                                                                     currentsDqRef);
static inline Ifx_MS_FocSolutionF16_SubState Ifx_MS_FocSolutionF16_subStateTransitDown(Ifx_MS_FocSolutionF16* self,
                                                                                       Ifx_MDA_IToFControllerF16_Output
                                                                                       iToFOutput, Ifx_Math_CmpFract16
                                                                                       currentsDqRef);
#endif /* (IFX_MS_FOCSOLUTIONF16_CFG_TRANSITION_MODE == IFX_MS_FOCSOLUTIONF16_TRANSITION_MODE_SMOOTH_TRANSITION) */
static inline Ifx_MS_FocSolutionF16_SubState Ifx_MS_FocSolutionF16_subStateClosedLoop(Ifx_MS_FocSolutionF16* self,
                                                                                      Ifx_Math_CmpFract16
                                                                                      currentsDqRef);

/* Execute on exit from closedLoop */
static inline Ifx_MS_FocSolutionF16_SubState Ifx_MS_FocSolutionF16_subStateClosedLoopExit(
    Ifx_MS_FocSolutionF16* self);

/* Execute I2f use the I2f angle for Park */
static inline void Ifx_MS_FocSolutionF16_openLoop(Ifx_MS_FocSolutionF16* self);

/* Closed loop implementation */
static inline void Ifx_MS_FocSolutionF16_closedLoop(Ifx_MS_FocSolutionF16* self, uint32 fluxEstimatorAngle);

/* Execute the fast loop operations with VToF */
static inline Ifx_Math_PolarFract16 Ifx_MS_FocSolutionF16_vToFLoop(Ifx_MS_FocSolutionF16* self);

/* Reset all the previous values present in the module */
static void Ifx_MS_FocSolutionF16_reset(Ifx_MS_FocSolutionF16* self);

/* Fault check for all the modules */
static bool Ifx_MS_FocSolutionF16_faultStatus(Ifx_MS_FocSolutionF16* self);

/* API to enable / disable all the modules */
static void Ifx_MS_FocSolutionF16_localEnable(Ifx_MS_FocSolutionF16* self, bool enable);

/* Enable/disable measurement ADC */
static void Ifx_MS_FocSolutionF16_enableMeasurementADC(Ifx_MS_FocSolutionF16* self, bool enable);

/* Enable/disable bridge driver */
static void Ifx_MS_FocSolutionF16_enableBridgeDrv(Ifx_MS_FocSolutionF16* self, bool enable);

/* Enable/disable pattern generator */
static void Ifx_MS_FocSolutionF16_enablePatternGen(Ifx_MS_FocSolutionF16* self, bool enable);

/* Speed and acceleration limit */
static inline void Ifx_MS_FocSolutionF16_limitSpeed(Ifx_MS_FocSolutionF16* self, Ifx_Math_Fract16 speedQ15);

/* Check if all modules are in the on state */
static inline bool Ifx_MS_FocSolutionF16_checkModulesStateOn(Ifx_MS_FocSolutionF16* self);

/* API to calculate the ref. q current in the speed loop or set it equal to the direct interface Q ref. current */
static inline Ifx_Math_Fract16 Ifx_MS_FocSolutionF16_calcCurrentQRef(Ifx_MS_FocSolutionF16* self, Ifx_Math_Fract16
                                                                     estimatedSpeedQ15, Ifx_Math_Fract16 currentQRef);

#if (IFX_MS_FOCSOLUTIONF16_CFG_TRANSITION_MODE == IFX_MS_FOCSOLUTIONF16_TRANSITION_MODE_SMOOTH_TRANSITION)

/* Smooth transition between open and close loop */
static inline bool Ifx_MS_FocSolutionF16_angleMergeCheck(Ifx_MS_FocSolutionF16* self, Ifx_MDA_IToFControllerF16_Output
                                                         iToFOutput, Ifx_Math_CmpFract16 currentsDqRef);

/* Calculation of angle error */
static inline Ifx_Math_Fract32 Ifx_MS_FocSolutionF16_angleError(Ifx_MS_FocSolutionF16          * self,
                                                                Ifx_MDA_IToFControllerF16_Output iToFOutput);

/* API to increase the amplitude of the reference Q current when transitioning/ramping down */
static inline void Ifx_MS_FocSolutionF16_increaseQCurrent(Ifx_MS_FocSolutionF16                * self,
                                                          Ifx_MDA_IToFControllerF16_Output const iToFOutput,
                                                          Ifx_Math_Fract16 const
                                                          transitionCounterQ15);

/* API to decrease amplitude of the reference Q current when transitioning up */
static inline void Ifx_MS_FocSolutionF16_decreaseQCurrent(Ifx_MS_FocSolutionF16                * self,
                                                          Ifx_MDA_IToFControllerF16_Output const iToFOutput);

/* API to decrease the amplitude of the reference D current when transitioning/ramping down */
static inline void Ifx_MS_FocSolutionF16_decreaseDCurrent(Ifx_MS_FocSolutionF16* self, Ifx_Math_Fract16 const
                                                          currentDRefQ15, Ifx_Math_Fract16 const
                                                          transitionCounterQ15);

#endif /* (IFX_MS_FOCSOLUTIONF16_CFG_TRANSITION_MODE == IFX_MS_FOCSOLUTIONF16_TRANSITION_MODE_SMOOTH_TRANSITION) */
/* API to initialize speed precontrol */
static inline void Ifx_MS_FocSolutionF16_initSpeedPreControl(Ifx_MS_FocSolutionF16* self);

/* Initialize reference current limiter */
static inline void Ifx_MS_FocSolutionF16_initRefCurrentLimiter(Ifx_MS_FocSolutionF16* self);

/* API to calculate sum of feedforward and feedback current and limit result */
static inline Ifx_Math_Fract16 Ifx_MS_FocSolutionF16_calcSumCurrentQRef(Ifx_MS_FocSolutionF16* self, Ifx_Math_Fract16
                                                                        speedPiOut);

/* Check if all modules are in the off state */
static inline bool Ifx_MS_FocSolutionF16_checkModulesStateOff(Ifx_MS_FocSolutionF16* self);

/* API to init. the iToF angle and set the dq ref currents, based on ref. speed */
static inline void Ifx_MS_FocSolutionF16_setDqCommand(Ifx_MS_FocSolutionF16* self, Ifx_MDA_IToFControllerF16_Output
                                                      iToFOutput);

/* API to set the Q command current based on speed */
static inline void Ifx_MS_FocSolutionF16_setQCommand(Ifx_MS_FocSolutionF16* self, Ifx_MDA_IToFControllerF16_Output
                                                     iToFOutput);

/* API to detect current Q command sign change */
static inline void Ifx_MS_FocSolutionF16_detectQCommandZeroCross(Ifx_MS_FocSolutionF16* self);

/* API to rotate dq ref. system by Pi */
static inline void Ifx_MS_FocSolutionF16_rotateDQRefSystem(Ifx_MS_FocSolutionF16* self);

/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/
/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/
/* Function to get the component ID */
void Ifx_MS_FocSolutionF16_getID(const Ifx_ComponentID** componentID)
{
    *componentID = &Ifx_MS_FocSolution_TLE987_componentID;
}


/* Function to get the component version */
void Ifx_MS_FocSolutionF16_getVersion(const Ifx_ComponentVersion** componentVersion)
{
    *componentVersion = &Ifx_MS_FocSolution_TLE987_componentVersion;
}


void Ifx_MS_FocSolutionF16_init(Ifx_MS_FocSolutionF16* self)
{
    /* Initialize modules from used libraries */
    Ifx_MS_FocSolutionF16_initModules(self);

    /* Initialize speed PI controller */
    Ifx_MS_FocSolutionF16_initSpeedPi(self);

    /* Initialize transition speeds */
    self->transitionSpeedUpQ15   = IFX_MS_FOCSOLUTIONF16_CFG_TRANSITION_SPEED_UP_Q15;
    self->transitionSpeedDownQ15 = IFX_MS_FOCSOLUTIONF16_CFG_TRANSITION_SPEED_DOWN_Q15;

#if (IFX_MS_FOCSOLUTIONF16_CFG_TRANSITION_MODE == IFX_MS_FOCSOLUTIONF16_TRANSITION_MODE_SMOOTH_TRANSITION)

    /* Init transition counter and limit */
    Ifx_MS_FocSolutionF16_setTransitionTimeLimit(self, (uint16)IFX_MS_FOCSOLUTIONF16_CFG_TRANSITION_TIME_CYCLES);
    self->p_transitionCounter_cycles = 0u;

    /* Init transition angle tolerance */
    self->p_transitionAngleTolerance = (sint32)IFX_MS_FOCSOLUTIONF16_CFG_ANGLE_ERROR_MIN;

    /* Init transition down D current scaling factor and target Id current */
    Ifx_MS_FocSolutionF16_setTransitionDownDCurrentScalingQ14(self,
        IFX_MS_FOCSOLUTIONF16_CFG_TRANSITION_D_CURRENT_SCALING_Q14);

    /* Initialize mid point between transition speed up and transition speed down */
    self->transitionSpeedMidQ15 = IFX_MS_FOCSOLUTIONF16_CFG_TRANSITION_SPEED_DOWN_Q15 +
                                  IFX_MS_FOCSOLUTIONF16_CFG_SPEED_BAND_HALF_Q15;
#endif /* (IFX_MS_FOCSOLUTIONF16_CFG_TRANSITION_MODE == IFX_MS_FOCSOLUTIONF16_TRANSITION_MODE_SMOOTH_TRANSITION) */
    /* Initialize speed ramp up/down rates */
    self->p_speedRampUpRateOpenLoopQ30     = IFX_MS_FOCSOLUTIONF16_CFG_OPEN_LOOP_RAMP_UP_RATE_Q30;
    self->p_speedRampDownRateOpenLoopQ30   = IFX_MS_FOCSOLUTIONF16_CFG_OPEN_LOOP_RAMP_DOWN_RATE_Q30;
    self->p_speedRampUpRateClosedLoopQ30   = IFX_MS_FOCSOLUTIONF16_CFG_CLOSED_LOOP_RAMP_UP_RATE_Q30;
    self->p_speedRampDownRateClosedLoopQ30 = IFX_MS_FOCSOLUTIONF16_CFG_CLOSED_LOOP_RAMP_DOWN_RATE_Q30;

    /* Initialize speed and acceleration limiters */
    Ifx_MS_FocSolutionF16_initSpeedAccelerationLimiters(self);

    /* Initialize startup current rate limiter */
    Ifx_MS_FocSolutionF16_initStartCurrentRateLimiter(self);

    /* Initialize internal variables */
    self->p_status.state            = Ifx_MS_FocSolutionF16_State_init;
    self->p_status.subState         = Ifx_MS_FocSolutionF16_SubState_openLoop;
    self->p_enablePowerStage        = false;
    self->p_enableControl           = false;
    self->p_clearFault              = false;
    self->p_clearFaultIsRequested   = false;
    self->p_qCurrentAtTransitionQ15 = IFX_MS_FOCSOLUTIONF16_CFG_Q_CURRENT_AT_TRANSITION_Q15;
    self->p_qCommandZeroCrossing    = false;
}


void Ifx_MS_FocSolutionF16_executeSpeedControl(Ifx_MS_FocSolutionF16* self, Ifx_Math_Fract16 speedQ15,
                                               Ifx_Math_CmpFract16 currentsDqRef)
{
    /* Fault status */
    bool                             faultStatus = Ifx_MS_FocSolutionF16_faultStatus(self);

    /* Get current to frequency output */
    Ifx_MDA_IToFControllerF16_Output iToFOutput;
    Ifx_MDA_IToFControllerF16_getOutput(&(self->iToF), &iToFOutput);

    if (self->p_clearFault == true)
    {
        /* Request to clear the underlying modules faults no matter whether FocSolution is in fault state to clear fault
         * bits of the underlying modules */
        (void)Ifx_MS_FocSolutionF16_clearModuleFaults(self);

        if (self->p_status.state == Ifx_MS_FocSolutionF16_State_fault)
        {
            /* Request to clear the fault state of the FocSolution */
            self->p_clearFaultIsRequested = true;
        }

        /* Reset clearFault flag */
        self->p_clearFault = false;
    }

    /* State Machine execute */
    Ifx_MS_FocSolutionF16_stateMachine(self, speedQ15, faultStatus, currentsDqRef, iToFOutput);

#if IFX_MS_FOCSOLUTIONF16_CFG_INCLUDE_STARTANGLE_IDENT == 1

    /* Disable start angle identification */
    self->p_enableStartAngleIdent = false;
#endif /* IFX_MS_FOCSOLUTIONF16_CFG_INCLUDE_STARTANGLE_IDENT == 1 */
}


static inline Ifx_Math_Fract16 Ifx_MS_FocSolutionF16_calcCurrentQRef(Ifx_MS_FocSolutionF16* self, Ifx_Math_Fract16
                                                                     estimatedSpeedQ15, Ifx_Math_Fract16 currentQRef)
{
    Ifx_Math_Fract16 refCurrent;
    sint8            qAntiwindupCtrl = Ifx_MDA_FocControllerF16_getQVoltageSatStatus(&(self->focController));

    /* Check if the direct interface is enabled */
    if (self->p_enableDirectInterface == true)
    {
        /* Set the ref. Q current to the input ref. Q current */
        refCurrent = currentQRef;

        /* Reset speed PI if direct interface is enabled with the input ref. Q current */
        Ifx_Math_PiF16_setIntegPreviousValue(&(self->speedPi), currentQRef);
    }
    else
    {
        /* Calculate Speed error */
        Ifx_Math_Fract16 rateLimitInSpeedQ14 = Ifx_Math_ShR_F16(self->rateLimitInSpeedQ15, 1u);
        Ifx_Math_Fract16 estimatedSpeedQ14   = Ifx_Math_ShR_F16(estimatedSpeedQ15, 1u);
        Ifx_Math_Fract16 errSpeedQ14         = Ifx_Math_Sub_F16(rateLimitInSpeedQ14, estimatedSpeedQ14);

        /* Execute speed PI */
        Ifx_Math_Fract16 speedPiOut = Ifx_Math_PiF16_execute(&(self->speedPi), errSpeedQ14, qAntiwindupCtrl);

        /* Check if speedPreControl is enabled */
        if (self->p_enableSpeedPreControl == true)
        {
            /* Calculate sum of feedforward and feedback current and limit result */
            refCurrent = Ifx_MS_FocSolutionF16_calcSumCurrentQRef(self, speedPiOut);
        }
        else
        {
            /* Reset state of SpeedPreControl with the last ref. speed */
            Ifx_Math_SpeedPreControlF16_setPreviousSpeed(&(self->speedPreControl), self->rateLimitInSpeedQ15);

            /* Set the ref. current to the output of the speed PI */
            refCurrent = speedPiOut;
        }
    }

    return refCurrent;
}


static inline Ifx_Math_Fract16 Ifx_MS_FocSolutionF16_calcSumCurrentQRef(Ifx_MS_FocSolutionF16* self, Ifx_Math_Fract16
                                                                        speedPiOut)
{
    /* Execute speed precontrol */
    Ifx_Math_Fract16 speedPreControlOut = Ifx_Math_SpeedPreControlF16_execute(&(self->speedPreControl),
        self->rateLimitInSpeedQ15);

    /* Sum the two reference currents from speed control and speed precontrol */
    Ifx_Math_Fract16 refCurrentSum = Ifx_Math_AddSat_F16(speedPiOut, speedPreControlOut);

    /* Limit the sum of the two reference currents */
    Ifx_Math_Fract16 refCurrentSumLimited = Ifx_Math_LimitF16_execute(&(self->p_refCurrentLimit), refCurrentSum);

    return refCurrentSumLimited;
}


/* API to execute the fast loop operations */
void Ifx_MS_FocSolutionF16_executeControlMode(Ifx_MS_FocSolutionF16* self)
{
    /* Local variables */
    Ifx_Math_PolarFract16                voltageCommandPolar;
    Ifx_MHA_MeasurementADC_TLE987_Output measurementADCOutput;
    uint32                               estimatedAngle;

    /* Return voltage measurement and perform current measurement and reconstruction */
    measurementADCOutput = Ifx_MS_FocSolutionF16_measureAndReconstruct(self);

    /* Perform the angle and speed estimation */
    estimatedAngle = Ifx_MS_FocSolutionF16_fluxEstimation(self);

    if ((self->p_status.state == Ifx_MS_FocSolutionF16_State_run)
        || (self->p_status.state == Ifx_MS_FocSolutionF16_State_rampDown))
    {
        if (self->p_status.actualControlMode == Ifx_MS_FocSolutionF16_ControlMode_foc)
        {
            voltageCommandPolar = Ifx_MS_FocSolutionF16_regulationLoop(self, estimatedAngle,
                measurementADCOutput.dcLinkVoltageQ15);
        }

        /* p_status.actualControlMode == Ifx_MS_FocSolutionF16_ControlMode_vToF */
        else
        {
            voltageCommandPolar = Ifx_MS_FocSolutionF16_vToFLoop(self);
        }
    }
    else
    {
#if IFX_MS_FOCSOLUTIONF16_CFG_INCLUDE_STARTANGLE_IDENT == 1

        /* Execute start angle identification */
        /* Ensure to call this function in all states, as otherwise clearfault will not be handled */
        Ifx_MDA_StartAngleIdentF16_execute(&(self->startAngleIdent), measurementADCOutput.dcLinkVoltageQ15,
            measurementADCOutput.shuntCurrentsQ15);
#endif /* IFX_MS_FOCSOLUTIONF16_CFG_INCLUDE_STARTANGLE_IDENT == 1 */
        /* Modulator output is 0 while in standby, fault, off, startAngleIdent, init */
        voltageCommandPolar.amplitude = 0;
        voltageCommandPolar.angle     = 0;
    }

    /* Generate voltage according to the command */
    Ifx_MS_FocSolutionF16_voltageGeneration(self, voltageCommandPolar, measurementADCOutput);
}


static inline void Ifx_MS_FocSolutionF16_rotateDQRefSystem(Ifx_MS_FocSolutionF16* self)
{
    /* Add 180deg. to the I2f angle */
    Ifx_MDA_IToFControllerF16_addOneEightyDegreeInAngle(&(self->iToF));

    /* Get current controllers previous value */
    Ifx_Math_Fract16 currentDPiPrevValue = Ifx_Math_PiF16_getIntegPreviousValue(&(self->focController.currentDPi));
    Ifx_Math_Fract16 currentQPiPrevValue = Ifx_Math_PiF16_getIntegPreviousValue(&(self->focController.currentQPi));

    /* Reset current PIs with negative of previous value */
    Ifx_Math_PiF16_setIntegPreviousValue(&(self->focController.currentDPi), Ifx_Math_Neg_F16(currentDPiPrevValue));
    Ifx_Math_PiF16_setIntegPreviousValue(&(self->focController.currentQPi), Ifx_Math_Neg_F16(currentQPiPrevValue));
}


static inline bool Ifx_MS_FocSolutionF16_clearModuleFaults(Ifx_MS_FocSolutionF16* self)
{
    /* Flag whether the clear fault request has been propagated to all underlying modules */
    bool clearFaultPropagated;

    /* Clear faults of MHA modules */
    clearFaultPropagated = Ifx_MS_FocSolutionF16_clearModuleFaultsMHA(self);

    /* Clear faults of MAS modules */
    Ifx_MS_FocSolutionF16_clearModuleFaultsMAS(self);

#if IFX_MS_FOCSOLUTIONF16_CFG_INCLUDE_STARTANGLE_IDENT == 1

    /* Clear faults of MDA modules */
    Ifx_MS_FocSolutionF16_clearModuleFaultsMDA(self);
#endif /* IFX_MS_FOCSOLUTIONF16_CFG_INCLUDE_STARTANGLE_IDENT == 1 */

    return clearFaultPropagated;
}


static inline bool Ifx_MS_FocSolutionF16_clearModuleFaultsMHA(Ifx_MS_FocSolutionF16* self)
{
    /* Flag whether the clear fault request has been propagated to all underlying modules */
    bool clearFaultPropagated;
    clearFaultPropagated = false;

    /* Clear faults of modules */
    Ifx_MHA_BridgeDrv_TLE987_clearFault(&self->bridgeDrvTLE987);
    Ifx_MHA_PatternGen_TLE987_clearFault(&self->patternGenTLE987);

    return clearFaultPropagated;
}


static inline void Ifx_MS_FocSolutionF16_clearModuleFaultsMAS(Ifx_MS_FocSolutionF16* self)
{
    /* Variable declaration */
    Ifx_MAS_ModulatorF16_Status modulatorStatus;

    /* Get the status of modules */
    modulatorStatus = Ifx_MAS_ModulatorF16_getStatus(&self->modulator);

    /* Clear faults of modules */
    if ((modulatorStatus.maxAmplitudeFlag != false)
        || (modulatorStatus.overmodulationFlag != false))
    {
        Ifx_MAS_ModulatorF16_clearFault(&self->modulator);
    }
}


#if IFX_MS_FOCSOLUTIONF16_CFG_INCLUDE_STARTANGLE_IDENT == 1
static inline void Ifx_MS_FocSolutionF16_clearModuleFaultsMDA(Ifx_MS_FocSolutionF16* self)
{
    /* Variable declaration */
    Ifx_MDA_StartAngleIdentF16_Status startAngleIdentStatus;

    /* Get the status of modules */
    startAngleIdentStatus = Ifx_MDA_StartAngleIdentF16_getStatus(&self->startAngleIdent);

    /* Clear faults of modules */
    if (startAngleIdentStatus.voltageFluctuation != false)
    {
        Ifx_MDA_StartAngleIdentF16_clearFault(&(self->startAngleIdent));
    }
}


#endif /* IFX_MS_FOCSOLUTIONF16_CFG_INCLUDE_STARTANGLE_IDENT == 1 */
static inline Ifx_MHA_MeasurementADC_TLE987_Output Ifx_MS_FocSolutionF16_measureAndReconstruct(
    Ifx_MS_FocSolutionF16* self)
{
    /* Get measured shunt currents from previous cycle */
    Ifx_MHA_MeasurementADC_TLE987_Output measurementADCOutput;
    Ifx_MHA_MeasurementADC_TLE987_execute(&(self->measurementADCTLE987));
    Ifx_MHA_MeasurementADC_TLE987_getOutput(&(self->measurementADCTLE987), &measurementADCOutput);

    /* Current reconstruction */
    self->currentsUVW = Ifx_Math_CurrentReconstruction_F16(self->previousCurrentReconstructionInfo,
        measurementADCOutput.shuntCurrentsQ15);

    /* Update current reconstruction information for the next state */
    self->previousCurrentReconstructionInfo = self->p_currentReconstructionInfo;

    /* Current Clark transformation (UVW to alpha-beta) */
    self->currentsAlphaBeta = Ifx_Math_Clarke_F16(self->currentsUVW);

    /* Return the measured DC link voltage */
    return measurementADCOutput;
}


static inline uint32 Ifx_MS_FocSolutionF16_fluxEstimation(Ifx_MS_FocSolutionF16* self)
{
    /* Flux estimator */
    Ifx_MDA_FluxEstimatorF16_execute(&(self->fluxEstimator), self->previousVoltageAlphaBeta, self->currentsAlphaBeta);

    /* Store voltage for next cycle */
    self->previousVoltageAlphaBeta = self->voltageAlphaBeta;

    /* Assign estimated speed output */
    Ifx_MDA_FluxEstimatorF16_Output fluxEstimatorOutput;
    Ifx_MDA_FluxEstimatorF16_getOutput(&(self->fluxEstimator), &fluxEstimatorOutput);
    self->p_output.estimatedSpeedQ15 = fluxEstimatorOutput.speedQ15;

    /* Return estimated angle */
    return fluxEstimatorOutput.anglePLL;
}


/* Execute the fast loop operations with IToF and FOC */
static inline Ifx_Math_PolarFract16 Ifx_MS_FocSolutionF16_regulationLoop(Ifx_MS_FocSolutionF16* self, uint32
                                                                         estimatedAngle, Ifx_Math_Fract16
                                                                         dcLinkVoltageQ15)
{
    /* Check if the Q command changes sign */
    if (self->p_qCommandZeroCrossing == true)
    {
        /* Rotate ref. DQ system if Q command changes sign */
        Ifx_MS_FocSolutionF16_rotateDQRefSystem(self);

        /* Set the current zero cross flag to false */
        self->p_qCommandZeroCrossing = false;
    }

    if (self->p_status.subState == Ifx_MS_FocSolutionF16_SubState_closedLoop)
    {
        /* Set the angle for Park transformation from FE */
        Ifx_MS_FocSolutionF16_closedLoop(self, estimatedAngle);
    }
    else
    {
        /* Execute iToF and set the angle for Park transformation from iToF */
        Ifx_MS_FocSolutionF16_openLoop(self);
    }

    /* Field Oriented Controller */
    Ifx_MDA_FocControllerF16_execute(&(self->focController), self->currentsAlphaBeta, self->dqCommand, self->angle,
        self->rateLimitInSpeedQ15, dcLinkVoltageQ15);
    Ifx_MDA_FocControllerF16_Output focControllerOutput;
    Ifx_MDA_FocControllerF16_getOutput(&(self->focController), &focControllerOutput);

    return focControllerOutput.voltageCommandPolar;
}


/* Open loop implementation */
static inline void Ifx_MS_FocSolutionF16_openLoop(Ifx_MS_FocSolutionF16* self)
{
    /* Execute I/f block to generate the angle */
    Ifx_MDA_IToFControllerF16_execute(&(self->iToF), self->rateLimitInSpeedQ15);
    Ifx_MDA_IToFControllerF16_Output iToFOutput;
    Ifx_MDA_IToFControllerF16_getOutput(&(self->iToF), &iToFOutput);

    /* Angle is set by iToF */
    self->angle = iToFOutput.currentVecAngle_rad;
}


/* Closed loop implementation */
static inline void Ifx_MS_FocSolutionF16_closedLoop(Ifx_MS_FocSolutionF16* self, uint32 fluxEstimatorAngle)
{
    /* Get the angle from the flux estimator */
    self->angle = fluxEstimatorAngle;
}


/* Execute the fast loop operations with VToF */
static inline Ifx_Math_PolarFract16 Ifx_MS_FocSolutionF16_vToFLoop(Ifx_MS_FocSolutionF16* self)
{
    Ifx_MDA_VToFControllerF16_Output vToFControllerOutput;
    Ifx_MDA_VToFControllerF16_execute(&(self->vToF), self->rateLimitInSpeedQ15);
    Ifx_MDA_VToFControllerF16_getOutput(&(self->vToF), &vToFControllerOutput);

    return vToFControllerOutput.voltageVector;
}


static inline void Ifx_MS_FocSolutionF16_voltageGeneration(Ifx_MS_FocSolutionF16* self, Ifx_Math_PolarFract16
                                                           voltageCommandPolar, Ifx_MHA_MeasurementADC_TLE987_Output
                                                           measurementADCOutput)
{
    /* Private variable to store the modulator output */
    Ifx_MAS_ModulatorF16_Output modulatorOutput;

    /* Private variables to store compare values and trigger times */
    uint16                    * compareValues_tick;
    uint16                    * triggerTime_tick;

#if IFX_MS_FOCSOLUTIONF16_CFG_INCLUDE_STARTANGLE_IDENT == 1

    /* Private variable to store the start angle identification output */
    Ifx_MDA_StartAngleIdentF16_Output startAngleIdentOutput;

    /* Call pattern generator with compare values depending on the state */
    if (self->p_status.state != Ifx_MS_FocSolutionF16_State_startAngleIdent)
#endif /* IFX_MS_FOCSOLUTIONF16_CFG_INCLUDE_STARTANGLE_IDENT == 1 */
    {
        /* Call modulator */
        /* Ensure to call this function in all states, as otherwise clearfault will not be handled */
        Ifx_MAS_ModulatorF16_execute(&(self->modulator), voltageCommandPolar, measurementADCOutput.dcLinkVoltageQ15,
            self->currentsUVW);
        Ifx_MAS_ModulatorF16_getOutput(&(self->modulator), &modulatorOutput);

        /* Store current reconstruction information */
        self->p_currentReconstructionInfo = modulatorOutput.currentReconstructionInfo;

        /* Convert actual voltage from modulator to cartesian, to be used by the flux estimator */
        self->voltageAlphaBeta = Ifx_Math_PolarToCart_F16(modulatorOutput.actualVoltage);

        /* Assign compare values and triggers */
        compareValues_tick = modulatorOutput.compareValues_tick;
        triggerTime_tick   = modulatorOutput.triggerTime_tick;
    }

#if IFX_MS_FOCSOLUTIONF16_CFG_INCLUDE_STARTANGLE_IDENT == 1
    else
    {
        /* Get start angle identification output */
        Ifx_MDA_StartAngleIdentF16_getOutput(&(self->startAngleIdent), &startAngleIdentOutput);

        /* Assign compare values and triggers */
        compareValues_tick = startAngleIdentOutput.compareValues_tick;
        triggerTime_tick   = startAngleIdentOutput.triggerTime_tick;
    }
#endif /* IFX_MS_FOCSOLUTIONF16_CFG_INCLUDE_STARTANGLE_IDENT == 1 */
    /* Call pattern generator */
    /* Ensure to call this function in all states, as otherwise clearfault will not be handled */
    Ifx_MHA_PatternGen_TLE987_execute(&(self->patternGenTLE987), compareValues_tick, triggerTime_tick);
}


/* Init all modules */
void Ifx_MS_FocSolutionF16_initModules(Ifx_MS_FocSolutionF16* self)
{
    /* Initialize hardware abstraction modules */
    Ifx_MHA_MeasurementADC_TLE987_init(&(self->measurementADCTLE987));
    Ifx_MHA_BridgeDrv_TLE987_init(&(self->bridgeDrvTLE987));
    Ifx_MHA_PatternGen_TLE987_init(&(self->patternGenTLE987));

    /* Initialize building blocks and drive algorithm modules */
    Ifx_MAS_ModulatorF16_init(&(self->modulator));
    Ifx_MS_FocSolutionF16_initDriveAlgo(self);

    /* Init speed precontrol */
    Ifx_MS_FocSolutionF16_initSpeedPreControl(self);

    /* Init reference current limiter */
    Ifx_MS_FocSolutionF16_initRefCurrentLimiter(self);
}


static inline void Ifx_MS_FocSolutionF16_initSpeedPreControl(Ifx_MS_FocSolutionF16* self)
{
    /*Static configuration */
    Ifx_Math_SpeedPreControlF16_StaticConfig speedPreControlStaticConfig;

    /* Friction constant */
    speedPreControlStaticConfig.fricitionConstant = IFX_MS_FOCSOLUTIONF16_CFG_VISCOUS_FRICTION_CONSTANT_Q15;

    /* J/Ts */
    Ifx_Math_Fract16Q rotorInertiaOverSamplingTime;
    rotorInertiaOverSamplingTime.value                       = IFX_MS_FOCSOLUTIONF16_CFG_INERTIA_BY_TS_Q;
    rotorInertiaOverSamplingTime.qFormat                     =
        (Ifx_Math_FractQFormat)IFX_MS_FOCSOLUTIONF16_CFG_INERTIA_BY_TS_Q_FORMAT;
    speedPreControlStaticConfig.rotorInertiaOverSamplingTime = rotorInertiaOverSamplingTime;

    /* Call init */
    Ifx_Math_SpeedPreControlF16_init(&(self->speedPreControl), speedPreControlStaticConfig);

    /* set Inverse torque ct. */
    Ifx_Math_SpeedPreControlF16_setInverseTorqueConstant(&(self->speedPreControl),
        IFX_MS_FOCSOLUTIONF16_CFG_INVERSE_TORQUE_CONSTANT_Q15);
}


static inline void Ifx_MS_FocSolutionF16_initRefCurrentLimiter(Ifx_MS_FocSolutionF16* self)
{
    /* Set upper limit of reference current limiter */
    Ifx_Math_LimitF16_setUpperLimit(&(self->p_refCurrentLimit), IFX_MS_FOCSOLUTIONF16_CFG_SPEED_PI_OUT_UPP_LIMIT_Q);

    /* Set lower limit of reference current limiter */
    Ifx_Math_LimitF16_setLowerLimit(&(self->p_refCurrentLimit), IFX_MS_FOCSOLUTIONF16_CFG_SPEED_PI_OUT_LOW_LIMIT_Q);
}


static inline void Ifx_MS_FocSolutionF16_initDriveAlgo(Ifx_MS_FocSolutionF16* self)
{
    Ifx_MDA_FluxEstimatorF16_init(&(self->fluxEstimator));
    Ifx_MDA_IToFControllerF16_init(&(self->iToF));
    Ifx_MDA_FocControllerF16_init(&(self->focController));
    Ifx_MDA_VToFControllerF16_init(&(self->vToF));

#if IFX_MS_FOCSOLUTIONF16_CFG_INCLUDE_STARTANGLE_IDENT == 1
    Ifx_MDA_StartAngleIdentF16_init(&(self->startAngleIdent));
#endif /* IFX_MS_FOCSOLUTIONF16_CFG_INCLUDE_STARTANGLE_IDENT == 1 */
}


static inline void Ifx_MS_FocSolutionF16_initSpeedAccelerationLimiters(Ifx_MS_FocSolutionF16* self)
{
    /* Speed limiter */
    Ifx_Math_LimitF16_setLowerLimit(&(self->speedLimit), IFX_MS_FOCSOLUTIONF16_CFG_MINIMUM_SPEED_Q15);
    Ifx_Math_LimitF16_setUpperLimit(&(self->speedLimit), IFX_MS_FOCSOLUTIONF16_CFG_MAXIMUM_SPEED_Q15);

    /* Acceleration limiter */
    Ifx_Math_AccelLimitF16_init(&(self->accelerationLimit));
    Ifx_Math_AccelLimitF16_setSpeedStepUpLimit(&(self->accelerationLimit), self->p_speedRampUpRateOpenLoopQ30);
    Ifx_Math_AccelLimitF16_setSpeedStepDownLimit(&(self->accelerationLimit), self->p_speedRampDownRateOpenLoopQ30);
}


static inline void Ifx_MS_FocSolutionF16_initStartCurrentRateLimiter(Ifx_MS_FocSolutionF16* self)
{
    /* Initialize rate limiter */
    Ifx_Math_RateLimitF16_init(&(self->p_startCurrentRateLimit));

    /* Set rate limiter up rate */
    Ifx_Math_RateLimitF16_setUpRate(&(self->p_startCurrentRateLimit),
        IFX_MS_FOCSOLUTIONF16_CFG_START_CURRENT_RAMP_UP_RATE_Q15);

    /* Set rate limiter down rate */
    Ifx_Math_RateLimitF16_setDownRate(&(self->p_startCurrentRateLimit),
        IFX_MS_FOCSOLUTIONF16_CFG_START_CURRENT_RAMP_UP_RATE_Q15);
}


static inline void Ifx_MS_FocSolutionF16_initSpeedPi(Ifx_MS_FocSolutionF16* self)
{
    /* Set Q formats */
    Ifx_Math_PiF16_Qformats piCtrlSpeedQform;
    piCtrlSpeedQform.qFormatPropGain                   =
        (Ifx_Math_FractQFormat)IFX_MS_FOCSOLUTIONF16_CFG_SPEED_PI_PROPGAIN_Q_FORMAT;
    piCtrlSpeedQform.qFormatIntegGainSamplingTime      =
        (Ifx_Math_FractQFormat)IFX_MS_FOCSOLUTIONF16_CFG_SPEED_PI_KI_TS_Q_FORMAT;
    piCtrlSpeedQform.qFormatAntiWindupGainSamplingTime =
        (Ifx_Math_FractQFormat)IFX_MS_FOCSOLUTIONF16_CFG_SPEED_PI_KAW_TS_Q_FORMAT;
    piCtrlSpeedQform.qFormatOutput                     =
        (Ifx_Math_FractQFormat)IFX_MS_FOCSOLUTIONF16_CFG_SPEED_PI_LIMIT_Q_FORMAT;
    piCtrlSpeedQform.qFormatError                      = Ifx_Math_FractQFormat_q14;

    /* Call init */
    Ifx_Math_PiF16_init(&(self->speedPi), piCtrlSpeedQform);

    /* Call setters */
    Ifx_Math_PiF16_setPropGain(&(self->speedPi), IFX_MS_FOCSOLUTIONF16_CFG_SPEED_PI_PROPGAIN_Q);
    Ifx_Math_PiF16_setIntegGainSamplingTime(&(self->speedPi), IFX_MS_FOCSOLUTIONF16_CFG_SPEED_PI_KI_TS_Q);
    Ifx_Math_PiF16_setAntiWindupGainSamplingTime(&(self->speedPi), IFX_MS_FOCSOLUTIONF16_CFG_SPEED_PI_KAW_TS_Q);
    Ifx_Math_PiF16_setUpperLimit(&(self->speedPi), IFX_MS_FOCSOLUTIONF16_CFG_SPEED_PI_OUT_UPP_LIMIT_Q);
    Ifx_Math_PiF16_setLowerLimit(&(self->speedPi), IFX_MS_FOCSOLUTIONF16_CFG_SPEED_PI_OUT_LOW_LIMIT_Q);
}


static void Ifx_MS_FocSolutionF16_enableMeasurementADC(Ifx_MS_FocSolutionF16* self, bool enable)
{
    /* Variables to store the status of the pattern generator and bridge driver */
    Ifx_MHA_BridgeDrv_TLE987_Status  bridgeDrvStatus;
    Ifx_MHA_PatternGen_TLE987_Status patternGenStatus;

    /* Get the bridge driver status */
    bridgeDrvStatus = Ifx_MHA_BridgeDrv_TLE987_getStatus(&(self->bridgeDrvTLE987));

    /* Get the pattern generator status */
    patternGenStatus = Ifx_MHA_PatternGen_TLE987_getStatus(&self->patternGenTLE987);

    if (enable == true)
    {
        Ifx_MHA_MeasurementADC_TLE987_enable(&(self->measurementADCTLE987), true);
    }
    else if ((bridgeDrvStatus.state == Ifx_MHA_BridgeDrv_TLE987_State_off)
             && (patternGenStatus.state == Ifx_MHA_PatternGen_TLE987_State_off))
    {
        Ifx_MHA_MeasurementADC_TLE987_enable(&(self->measurementADCTLE987), false);
    }
    else
    {
        /* Do nothing */
    }
}


static void Ifx_MS_FocSolutionF16_enableBridgeDrv(Ifx_MS_FocSolutionF16* self, bool enable)
{
    /* Variable to store the status of bridge driver */
    Ifx_MHA_PatternGen_TLE987_Status patternGenStatus;

    /* Get the pattern generator status */
    patternGenStatus = Ifx_MHA_PatternGen_TLE987_getStatus(&self->patternGenTLE987);

    if ((enable == false)
        && (patternGenStatus.state == Ifx_MHA_PatternGen_TLE987_State_off))
    {
        Ifx_MHA_BridgeDrv_TLE987_enable(&(self->bridgeDrvTLE987), false);
    }
    else if ((enable == true)
             && (patternGenStatus.state == Ifx_MHA_PatternGen_TLE987_State_on))
    {
        Ifx_MHA_BridgeDrv_TLE987_enable(&(self->bridgeDrvTLE987), true);
    }
    else
    {
        /* Do nothing */
    }
}


static void Ifx_MS_FocSolutionF16_enablePatternGen(Ifx_MS_FocSolutionF16* self, bool enable)
{
    /* Variables to store the status of the measurement ADC */
    Ifx_MHA_MeasurementADC_TLE987_Status measurementADCStatus;

    /* Get the measurement ADC status */
    measurementADCStatus = Ifx_MHA_MeasurementADC_TLE987_getStatus(&self->measurementADCTLE987);

    if ((enable == true)
        && (measurementADCStatus.state == Ifx_MHA_MeasurementADC_TLE987_State_on))
    {
        Ifx_MHA_PatternGen_TLE987_enable(&(self->patternGenTLE987), true);
    }
    else if (enable == false)
    {
        Ifx_MHA_PatternGen_TLE987_enable(&(self->patternGenTLE987), false);
    }
    else
    {
        /* Do nothing */
    }
}


static void Ifx_MS_FocSolutionF16_localEnable(Ifx_MS_FocSolutionF16* self, bool enable)
{
    /* Enable/disable measurement ADC */
    Ifx_MS_FocSolutionF16_enableMeasurementADC(self, enable);

    /* Enable/disable pattern generator */
    Ifx_MS_FocSolutionF16_enablePatternGen(self, enable);

    /* Enable/disable bridge driver */
    Ifx_MS_FocSolutionF16_enableBridgeDrv(self, enable);

    /* Enable/disable modulator */
    Ifx_MAS_ModulatorF16_enable(&(self->modulator), enable);
}


static inline bool Ifx_MS_FocSolutionF16_checkModulesStateOn(Ifx_MS_FocSolutionF16* self)
{
    /* Variable declaration */
    Ifx_MHA_MeasurementADC_TLE987_Status measurementADCstatus;
    Ifx_MHA_BridgeDrv_TLE987_Status      bridgeDrvStatus;
    Ifx_MHA_PatternGen_TLE987_Status     patternGenStatus;
    Ifx_MAS_ModulatorF16_Status          modulatorStatus;
    bool                                 returnValue;

    /* Get the status of all modules */
    measurementADCstatus = Ifx_MHA_MeasurementADC_TLE987_getStatus(&self->measurementADCTLE987);
    bridgeDrvStatus      = Ifx_MHA_BridgeDrv_TLE987_getStatus(&self->bridgeDrvTLE987);
    patternGenStatus     = Ifx_MHA_PatternGen_TLE987_getStatus(&self->patternGenTLE987);
    modulatorStatus      = Ifx_MAS_ModulatorF16_getStatus(&self->modulator);

    /* Check if all modules are in the on state */
    if ((measurementADCstatus.state == Ifx_MHA_MeasurementADC_TLE987_State_on)
        && (bridgeDrvStatus.state == Ifx_MHA_BridgeDrv_TLE987_State_on)
        && (patternGenStatus.state == Ifx_MHA_PatternGen_TLE987_State_on)
        && (modulatorStatus.state == Ifx_MAS_ModulatorF16_State_on))
    {
        returnValue = true;
    }
    else
    {
        returnValue = false;
    }

    return returnValue;
}


static inline bool Ifx_MS_FocSolutionF16_checkModulesStateOff(Ifx_MS_FocSolutionF16* self)
{
    /* Variable declaration */
    Ifx_MHA_MeasurementADC_TLE987_Status measurementADCstatus;
    Ifx_MHA_BridgeDrv_TLE987_Status      bridgeDrvStatus;
    Ifx_MHA_PatternGen_TLE987_Status     patternGenStatus;
    Ifx_MAS_ModulatorF16_Status          modulatorStatus;
    bool                                 returnValue;

    /* Get the status of all modules */
    measurementADCstatus = Ifx_MHA_MeasurementADC_TLE987_getStatus(&self->measurementADCTLE987);
    bridgeDrvStatus      = Ifx_MHA_BridgeDrv_TLE987_getStatus(&self->bridgeDrvTLE987);
    patternGenStatus     = Ifx_MHA_PatternGen_TLE987_getStatus(&self->patternGenTLE987);
    modulatorStatus      = Ifx_MAS_ModulatorF16_getStatus(&self->modulator);

    /* Check if all modules are in the off state */
    if ((measurementADCstatus.state == Ifx_MHA_MeasurementADC_TLE987_State_off)
        && (bridgeDrvStatus.state == Ifx_MHA_BridgeDrv_TLE987_State_off)
        && (patternGenStatus.state == Ifx_MHA_PatternGen_TLE987_State_off)
        && (modulatorStatus.state == Ifx_MAS_ModulatorF16_State_off))
    {
        returnValue = true;
    }
    else
    {
        returnValue = false;
    }

    return returnValue;
}


/* API to set the fault status of the all used modules */
/* fault check for all the modules */
static bool Ifx_MS_FocSolutionF16_faultStatus(Ifx_MS_FocSolutionF16* self)
{
    /* local variable to check fault status */
    bool faultStatusRet = false;

    /* Bridge driver fault */
    if (Ifx_MHA_BridgeDrv_TLE987_getStatus(&(self->bridgeDrvTLE987)).state == Ifx_MHA_BridgeDrv_TLE987_State_fault)
    {
        faultStatusRet = true;
    }

    /* Pattern generator fault */
    else if (Ifx_MHA_PatternGen_TLE987_getStatus(&(self->patternGenTLE987)).state ==
             Ifx_MHA_PatternGen_TLE987_State_fault)
    {
        faultStatusRet = true;
    }

    /* Modulator fault */
    else if (Ifx_MAS_ModulatorF16_getStatus(&(self->modulator)).state == Ifx_MAS_ModulatorF16_State_fault)
    {
        faultStatusRet = true;
    }

#if IFX_MS_FOCSOLUTIONF16_CFG_INCLUDE_STARTANGLE_IDENT == 1

    /* Start angle identification fault */
    else if (Ifx_MDA_StartAngleIdentF16_getStatus(&(self->startAngleIdent)).state ==
             Ifx_MDA_StartAngleIdentF16_State_fault)
    {
        faultStatusRet = true;
    }
#endif /* IFX_MS_FOCSOLUTIONF16_CFG_INCLUDE_STARTANGLE_IDENT == 1 */
    else
    {
        faultStatusRet = false;
    }

    /* Return the fault status */
    return faultStatusRet;
}


/* FOC state machine */
static inline void Ifx_MS_FocSolutionF16_stateMachine(Ifx_MS_FocSolutionF16* self, Ifx_Math_Fract16 speedQ15, bool
                                                      faultStatus, Ifx_Math_CmpFract16 currentsDqRef,
                                                      Ifx_MDA_IToFControllerF16_Output iToFOutput)
{
    /* Variable to store the state */
    Ifx_MS_FocSolutionF16_State previousState = self->p_status.state;
    Ifx_MS_FocSolutionF16_State nextState;

    switch (previousState)
    {
        /* Initialize the module */
        case Ifx_MS_FocSolutionF16_State_init:
            nextState = Ifx_MS_FocSolutionF16_stateInit(self);
            break;

        /* All the modules are enabled */
        case Ifx_MS_FocSolutionF16_State_standBy:
            nextState = Ifx_MS_FocSolutionF16_stateStandby(self, faultStatus);
            break;

        /* Run state */
        case Ifx_MS_FocSolutionF16_State_run:
            nextState = Ifx_MS_FocSolutionF16_stateRun(self, speedQ15, faultStatus, currentsDqRef, iToFOutput);
            break;

        /* FOC not running */
        case Ifx_MS_FocSolutionF16_State_off:
            nextState = Ifx_MS_FocSolutionF16_stateOff(self, faultStatus);
            break;

        /* FOC is in fault */
        case Ifx_MS_FocSolutionF16_State_fault:
            nextState = Ifx_MS_FocSolutionF16_stateFault(self);
            break;

        /* FOC is in ramp down state */
        case Ifx_MS_FocSolutionF16_State_rampDown:
            nextState = Ifx_MS_FocSolutionF16_stateRampDown(self, faultStatus, currentsDqRef, iToFOutput);
            break;

#if IFX_MS_FOCSOLUTIONF16_CFG_INCLUDE_STARTANGLE_IDENT == 1

        /*  FOC is in start angle identification state */
        case Ifx_MS_FocSolutionF16_State_startAngleIdent:
            nextState = Ifx_MS_FocSolutionF16_stateStartAngleIdent(self, speedQ15, faultStatus, iToFOutput);
            break;

#endif /* IFX_MS_FOCSOLUTIONF16_CFG_INCLUDE_STARTANGLE_IDENT == 1 */
        /* do default transition to INIT */
        default:
            nextState = Ifx_MS_FocSolutionF16_State_init;
            break;
    }

    self->p_status.state = nextState;
}


static inline Ifx_MS_FocSolutionF16_State Ifx_MS_FocSolutionF16_stateInit(Ifx_MS_FocSolutionF16* self)
{
    Ifx_MS_FocSolutionF16_State nextState;

    /* Check if all the modules are in OFF state */
    if (Ifx_MS_FocSolutionF16_checkModulesStateOff(self) == true)
    {
        /* Set the state to OFF */
        nextState = Ifx_MS_FocSolutionF16_State_off;
    }
    else
    {
        nextState = Ifx_MS_FocSolutionF16_State_init;
    }

    return nextState;
}


static inline Ifx_MS_FocSolutionF16_State Ifx_MS_FocSolutionF16_stateOff(Ifx_MS_FocSolutionF16* self, bool
                                                                         faultStatus)
{
    Ifx_MS_FocSolutionF16_State nextState;

    /* Reset previous values */
    Ifx_MS_FocSolutionF16_reset(self);

    if (faultStatus == true)
    {
        nextState = Ifx_MS_FocSolutionF16_State_fault;
    }

    /* Check if power stage is enabled */
    else if (self->p_enablePowerStage == true)
    {
        /* Enable underlying modules */
        Ifx_MS_FocSolutionF16_localEnable(self, true);

        /* Check if underlying modules are in the on state */
        if (Ifx_MS_FocSolutionF16_checkModulesStateOn(self) == true)
        {
            nextState = Ifx_MS_FocSolutionF16_State_standBy;
        }
        else
        {
            nextState = Ifx_MS_FocSolutionF16_State_off;
        }
    }
    else
    {
        nextState = Ifx_MS_FocSolutionF16_State_off;
    }

    return nextState;
}


static inline Ifx_MS_FocSolutionF16_State Ifx_MS_FocSolutionF16_stateStandby(Ifx_MS_FocSolutionF16* self, bool
                                                                             faultStatus)
{
    Ifx_MS_FocSolutionF16_State nextState;

    if (faultStatus == true)
    {
        /* Go into fault state */
        nextState = Ifx_MS_FocSolutionF16_State_fault;
    }
    else if (self->p_enablePowerStage == false)
    {
        /* Disable all underlying modules */
        Ifx_MS_FocSolutionF16_localEnable(self, false);

        /* Check if underlying modules are in the off state */
        if (Ifx_MS_FocSolutionF16_checkModulesStateOff(self) == true)
        {
            nextState = Ifx_MS_FocSolutionF16_State_off;
        }
        else
        {
            nextState = Ifx_MS_FocSolutionF16_State_standBy;
        }
    }

#if IFX_MS_FOCSOLUTIONF16_CFG_INCLUDE_STARTANGLE_IDENT == 1
    else if (self->p_enableStartAngleIdent == true)
    {
        /* Enable start angle identification module */
        Ifx_MDA_StartAngleIdentF16_enable(&(self->startAngleIdent), true);

        /* Go to start angle identification state */
        nextState = Ifx_MS_FocSolutionF16_State_startAngleIdent;
    }
#endif /* IFX_MS_FOCSOLUTIONF16_CFG_INCLUDE_STARTANGLE_IDENT == 1 */
    else if (self->p_enableControl == true)
    {
        /* Update control mode */
        self->p_status.actualControlMode = Ifx_MS_FocSolutionF16_getControlMode(self);

        /* Set open loop ramp up/down rates */
        Ifx_Math_AccelLimitF16_setSpeedStepUpLimit(&(self->accelerationLimit), self->p_speedRampUpRateOpenLoopQ30);
        Ifx_Math_AccelLimitF16_setSpeedStepDownLimit(&(self->accelerationLimit),
            self->p_speedRampDownRateOpenLoopQ30);

        /* Init. substate machine */
        self->p_status.subState = Ifx_MS_FocSolutionF16_SubState_openLoop;

        /* Enable flux estimator */
        Ifx_MDA_FluxEstimatorF16_configMode(&(self->fluxEstimator), Ifx_MDA_FluxEstimatorF16_Mode_enable);

        /* Go to run state */
        nextState = Ifx_MS_FocSolutionF16_State_run;
    }
    else
    {
        nextState = Ifx_MS_FocSolutionF16_State_standBy;
    }

    /* Always reset FocCtrl module in standby */
    Ifx_MS_FocSolutionF16_reset(self);

    return nextState;
}


static inline Ifx_MS_FocSolutionF16_State Ifx_MS_FocSolutionF16_stateRun(Ifx_MS_FocSolutionF16* self, Ifx_Math_Fract16
                                                                         speedQ15, bool faultStatus,
                                                                         Ifx_Math_CmpFract16 currentsDqRef,
                                                                         Ifx_MDA_IToFControllerF16_Output iToFOutput)
{
    Ifx_MS_FocSolutionF16_State nextState;

    /* Perform speed and acceleration limit */
    Ifx_MS_FocSolutionF16_limitSpeed(self, speedQ15);

    if (faultStatus == true)
    {
        /* Disable flux estimator */
        Ifx_MDA_FluxEstimatorF16_configMode(&(self->fluxEstimator), Ifx_MDA_FluxEstimatorF16_Mode_disable);

        /* Set the module to fault */
        nextState = Ifx_MS_FocSolutionF16_State_fault;
    }
    else if ((self->p_enableControl == false)
             || (self->p_enablePowerStage == false))
    {
#if (IFX_MS_FOCSOLUTIONF16_CFG_TRANSITION_MODE == IFX_MS_FOCSOLUTIONF16_TRANSITION_MODE_SMOOTH_TRANSITION)

        /* Reset transition counter */
        self->p_transitionCounter_cycles = 0;
#endif /* (IFX_MS_FOCSOLUTIONF16_CFG_TRANSITION_MODE == IFX_MS_FOCSOLUTIONF16_TRANSITION_MODE_SMOOTH_TRANSITION) */
        /* Set state to ramp down */
        nextState = Ifx_MS_FocSolutionF16_State_rampDown;
    }
    else
    {
        /* Execute sub-state machine */
        Ifx_MS_FocSolutionF16_subStateMachine(self, iToFOutput, currentsDqRef);

        /* Stay in run */
        nextState = Ifx_MS_FocSolutionF16_State_run;
    }

    return nextState;
}


static inline void Ifx_MS_FocSolutionF16_limitSpeed(Ifx_MS_FocSolutionF16* self, Ifx_Math_Fract16 speedQ15)
{
    /* Value limited speed */
    Ifx_Math_Fract16 limSpeed;

    /* Limit input speed */
    limSpeed = Ifx_Math_LimitF16_execute(&(self->speedLimit), speedQ15);

    /* Execute accel limited */
    self->rateLimitInSpeedQ15 = Ifx_Math_AccelLimitF16_execute(&(self->accelerationLimit), limSpeed);
}


static inline void Ifx_MS_FocSolutionF16_subStateMachine(Ifx_MS_FocSolutionF16* self, Ifx_MDA_IToFControllerF16_Output
                                                         iToFOutput, Ifx_Math_CmpFract16 currentsDqRef)
{
    /* Variable to store the state */
    Ifx_MS_FocSolutionF16_SubState previousSubState = self->p_status.subState;
    Ifx_MS_FocSolutionF16_SubState nextSubState;

    switch (previousSubState)
    {
        /* Sub state machine run is in open loop */
        case Ifx_MS_FocSolutionF16_SubState_openLoop:
            nextSubState = Ifx_MS_FocSolutionF16_subStateOpenLoop(self, iToFOutput, currentsDqRef);
            break;

#if (IFX_MS_FOCSOLUTIONF16_CFG_TRANSITION_MODE == IFX_MS_FOCSOLUTIONF16_TRANSITION_MODE_SMOOTH_TRANSITION)

        /* Sub state machine run is in transition up */
        case Ifx_MS_FocSolutionF16_SubState_transitionUp:
            nextSubState = Ifx_MS_FocSolutionF16_subStateTransitUp(self, iToFOutput, currentsDqRef);
            break;

#endif /* (IFX_MS_FOCSOLUTIONF16_CFG_TRANSITION_MODE == IFX_MS_FOCSOLUTIONF16_TRANSITION_MODE_SMOOTH_TRANSITION) */
        /* Sub state machine run is in closed loop */
        case Ifx_MS_FocSolutionF16_SubState_closedLoop:
            nextSubState = Ifx_MS_FocSolutionF16_subStateClosedLoop(self, currentsDqRef);
            break;

#if (IFX_MS_FOCSOLUTIONF16_CFG_TRANSITION_MODE == IFX_MS_FOCSOLUTIONF16_TRANSITION_MODE_SMOOTH_TRANSITION)

        /* Sub state machine run is in transition down */
        case Ifx_MS_FocSolutionF16_SubState_transitionDown:
            nextSubState = Ifx_MS_FocSolutionF16_subStateTransitDown(self, iToFOutput, currentsDqRef);
            break;

#endif /* (IFX_MS_FOCSOLUTIONF16_CFG_TRANSITION_MODE == IFX_MS_FOCSOLUTIONF16_TRANSITION_MODE_SMOOTH_TRANSITION) */
        /* do default transition to open loop */
        default:
            nextSubState = Ifx_MS_FocSolutionF16_SubState_openLoop;
            break;
    }

    self->p_status.subState = nextSubState;
}


static inline Ifx_MS_FocSolutionF16_SubState Ifx_MS_FocSolutionF16_subStateOpenLoop(Ifx_MS_FocSolutionF16* self,
                                                                                    Ifx_MDA_IToFControllerF16_Output
                                                                                    iToFOutput, Ifx_Math_CmpFract16
                                                                                    currentsDqRef)
{
    Ifx_MS_FocSolutionF16_SubState nextSubState;

    /* Check if the direct current interface is enabled or disabled */
    if (self->p_enableDirectInterface == true)
    {
        self->dqCommand.real = currentsDqRef.real;
        self->dqCommand.imag = currentsDqRef.imag;
    }
    else
    {
        /* Set dq ref current and iToF angle */
        Ifx_MS_FocSolutionF16_setDqCommand(self, iToFOutput);
    }

    /* Check the ref. speed to go to transition up or stay in open loop */
    if ((Ifx_Math_Abs_F16(self->rateLimitInSpeedQ15) > self->transitionSpeedUpQ15)
        && (self->p_status.actualControlMode == Ifx_MS_FocSolutionF16_ControlMode_foc))
    {
#if (IFX_MS_FOCSOLUTIONF16_CFG_TRANSITION_MODE == IFX_MS_FOCSOLUTIONF16_TRANSITION_MODE_SMOOTH_TRANSITION)

        /* Init. transition counter */
        self->p_transitionCounter_cycles = 0;

        /* Go to transition up */
        nextSubState = Ifx_MS_FocSolutionF16_SubState_transitionUp;
#endif /* (IFX_MS_FOCSOLUTIONF16_CFG_TRANSITION_MODE == IFX_MS_FOCSOLUTIONF16_TRANSITION_MODE_SMOOTH_TRANSITION) */

#if (IFX_MS_FOCSOLUTIONF16_CFG_TRANSITION_MODE == IFX_MS_FOCSOLUTIONF16_TRANSITION_MODE_DIRECT_TRANSITION)

        /* Set the integral previous value to the configured transition q current */
        if (self->rateLimitInSpeedQ15 > 0)
        {
            Ifx_Math_PiF16_setIntegPreviousValue(&(self->speedPi), self->p_qCurrentAtTransitionQ15);
        }
        else
        {
            Ifx_Math_PiF16_setIntegPreviousValue(&(self->speedPi), Ifx_Math_Neg_F16(self->p_qCurrentAtTransitionQ15));
        }

        /* Set closed loop ramp up/down rates */
        Ifx_Math_AccelLimitF16_setSpeedStepUpLimit(&(self->accelerationLimit), self->p_speedRampUpRateClosedLoopQ30);
        Ifx_Math_AccelLimitF16_setSpeedStepDownLimit(&(self->accelerationLimit),
            self->p_speedRampDownRateClosedLoopQ30);

        /* Go to closed loop */
        nextSubState = Ifx_MS_FocSolutionF16_SubState_closedLoop;
#endif /* (IFX_MS_FOCSOLUTIONF16_CFG_TRANSITION_MODE == IFX_MS_FOCSOLUTIONF16_TRANSITION_MODE_DIRECT_TRANSITION) */
    }
    else
    {
        /* Stay in open loop */
        nextSubState = Ifx_MS_FocSolutionF16_SubState_openLoop;
    }

    return nextSubState;
}


static inline void Ifx_MS_FocSolutionF16_setDqCommand(Ifx_MS_FocSolutionF16* self, Ifx_MDA_IToFControllerF16_Output
                                                      iToFOutput)
{
    /* Set ref. d current to I2f */
    self->dqCommand.real = iToFOutput.refCurrent.real;

    /* Set ref. q current to I2f depending on current and previous reference speed */
    Ifx_MS_FocSolutionF16_setQCommand(self, iToFOutput);

    /* Update previous ref. current */
    self->p_previousQCommand = self->dqCommand.imag;
}


static inline void Ifx_MS_FocSolutionF16_setQCommand(Ifx_MS_FocSolutionF16* self, Ifx_MDA_IToFControllerF16_Output
                                                     iToFOutput)
{
    /* effective Q command current */
    Ifx_Math_Fract16 effectiveRefQCurrent;
    effectiveRefQCurrent = Ifx_Math_RateLimitF16_execute(&(self->p_startCurrentRateLimit),
        iToFOutput.refCurrent.imag);

    /* Set ref. q current to I2f depending on current and previous reference speed */
    if (self->rateLimitInSpeedQ15 >= 0)
    {
        self->dqCommand.imag = effectiveRefQCurrent;
    }
    else
    {
        self->dqCommand.imag = Ifx_Math_Neg_F16(effectiveRefQCurrent);
    }

    /* Detect if the current Q command changed sign */
    Ifx_MS_FocSolutionF16_detectQCommandZeroCross(self);
}


static inline void Ifx_MS_FocSolutionF16_detectQCommandZeroCross(Ifx_MS_FocSolutionF16* self)
{
    if (self->p_previousQCommand < 0)
    {
        if (self->dqCommand.imag > 0)
        {
            self->p_qCommandZeroCrossing = true;
        }
    }

    /* self->p_previousQCommand >= 0 */
    else
    {
        if (self->dqCommand.imag < 0)
        {
            self->p_qCommandZeroCrossing = true;
        }
    }
}


#if (IFX_MS_FOCSOLUTIONF16_CFG_TRANSITION_MODE == IFX_MS_FOCSOLUTIONF16_TRANSITION_MODE_SMOOTH_TRANSITION)
static inline Ifx_MS_FocSolutionF16_SubState Ifx_MS_FocSolutionF16_subStateTransitUp(Ifx_MS_FocSolutionF16* self,
                                                                                     Ifx_MDA_IToFControllerF16_Output
                                                                                     iToFOutput, Ifx_Math_CmpFract16
                                                                                     currentsDqRef)
{
    Ifx_MS_FocSolutionF16_SubState nextSubState;

    /* Run smooth transition */
    bool                           mergeAngle;
    mergeAngle = Ifx_MS_FocSolutionF16_angleMergeCheck(self, iToFOutput, currentsDqRef);

    /* If angle merged */
    if (mergeAngle == true)
    {
        Ifx_Math_Fract16 estimatedSpeedQ15 = self->p_output.estimatedSpeedQ15;

        /* Calculate ref. Q current */
        self->dqCommand.imag = Ifx_MS_FocSolutionF16_calcCurrentQRef(self, estimatedSpeedQ15, currentsDqRef.imag);

        /* Set closed loop ramp up/down rates  */
        Ifx_Math_AccelLimitF16_setSpeedStepUpLimit(&(self->accelerationLimit), self->p_speedRampUpRateClosedLoopQ30);
        Ifx_Math_AccelLimitF16_setSpeedStepDownLimit(&(self->accelerationLimit),
            self->p_speedRampDownRateClosedLoopQ30);

        /* Go to closed loop */
        nextSubState = Ifx_MS_FocSolutionF16_SubState_closedLoop;
    }
    else
    {
        /* Stay in transition up */
        nextSubState = Ifx_MS_FocSolutionF16_SubState_transitionUp;
    }

    return nextSubState;
}


static inline bool Ifx_MS_FocSolutionF16_angleMergeCheck(Ifx_MS_FocSolutionF16* self, Ifx_MDA_IToFControllerF16_Output
                                                         iToFOutput, Ifx_Math_CmpFract16 currentsDqRef)
{
    /* Angle merge indication */
    bool   mergeAngle = false;

    /* Angle error between iToF and flux estimator */
    sint32 angleError = Ifx_MS_FocSolutionF16_angleError(self, iToFOutput);

    /* Check angle error minimum value and transition time */
    if ((Ifx_Math_Abs_F32(angleError) < self->p_transitionAngleTolerance)
        || (self->p_transitionCounter_cycles == self->p_transitionTimeLimit_cycles - 1))
    {
        /* Set the integral previous value to the last reached ref. q current */
        Ifx_Math_PiF16_setIntegPreviousValue(&(self->speedPi), self->dqCommand.imag);

        /* Set angle merge flag to true */
        mergeAngle = true;
    }

    /* Decrease ref. q current amplitude */
    else
    {
        /* Check if the direct current interface is enabled or disabled */
        if (self->p_enableDirectInterface == true)
        {
            self->dqCommand.real = currentsDqRef.real;
            self->dqCommand.imag = currentsDqRef.imag;
        }
        else
        {
            Ifx_MS_FocSolutionF16_decreaseQCurrent(self, iToFOutput);
        }
    }

    return mergeAngle;
}


static inline sint32 Ifx_MS_FocSolutionF16_angleError(Ifx_MS_FocSolutionF16* self, Ifx_MDA_IToFControllerF16_Output
                                                      iToFOutput)
{
    /* Get flux estimator output */
    Ifx_MDA_FluxEstimatorF16_Output fluxEstimatorOutput;
    Ifx_MDA_FluxEstimatorF16_getOutput(&(self->fluxEstimator), &fluxEstimatorOutput);

    /* I2f angle in uin16 */
    uint16                          refAngle = (uint16)(iToFOutput.currentVecAngle_rad >> 16);

    /* FE angle in uin16 */
    uint16                          estAngle = (uint16)(fluxEstimatorOutput.anglePLL >> 16);

    /* Angle error in sint32 */
    sint32                          angleErr = (sint32)refAngle - (sint32)estAngle;

    /* Angle overflowed */
    sint32                          outputAngle;

    /* Check if error > pi */
    if (angleErr > IFX_MATH_FRACT16_MAX)
    {
        outputAngle = Ifx_Math_Sub_F32(angleErr, IFX_MS_FOCSOLUTIONF16_2MAX);
    }

    /* Check if error < -pi */
    else if (angleErr <= IFX_MATH_FRACT16_MIN)
    {
        /* -1 is due to the asymmetry of the range on the negative (-32768) and positive (32767) limits */
        outputAngle = (Ifx_Math_Add_F32(angleErr, IFX_MS_FOCSOLUTIONF16_2MAX) - 1);
    }
    else
    {
        /* No wrapping needed, return the input */
        outputAngle = angleErr;
    }

    return outputAngle;
}


#endif /* (IFX_MS_FOCSOLUTIONF16_CFG_TRANSITION_MODE == IFX_MS_FOCSOLUTIONF16_TRANSITION_MODE_SMOOTH_TRANSITION) */
static inline Ifx_MS_FocSolutionF16_SubState Ifx_MS_FocSolutionF16_subStateClosedLoop(Ifx_MS_FocSolutionF16* self,
                                                                                      Ifx_Math_CmpFract16
                                                                                      currentsDqRef)
{
    Ifx_MS_FocSolutionF16_SubState nextSubState;

    /* Store estimated speed and absolute estimated speed in local variables for further processing */
    Ifx_Math_Fract16               estimatedSpeedQ15    = self->p_output.estimatedSpeedQ15;
    Ifx_Math_Fract16               estimatedSpeedAbsQ15 = Ifx_Math_Abs_F16(estimatedSpeedQ15);

    /* Calculate ref. Q current */
    self->dqCommand.imag = Ifx_MS_FocSolutionF16_calcCurrentQRef(self, estimatedSpeedQ15, currentsDqRef.imag);

    if (estimatedSpeedAbsQ15 < self->transitionSpeedDownQ15)
    {
        /* Perform all required actions for exit from closedLoop and determine next subState */
        nextSubState = Ifx_MS_FocSolutionF16_subStateClosedLoopExit(self);
    }

#if (IFX_MS_FOCSOLUTIONF16_CFG_TRANSITION_MODE == IFX_MS_FOCSOLUTIONF16_TRANSITION_MODE_SMOOTH_TRANSITION)

    else if (estimatedSpeedAbsQ15 < self->transitionSpeedMidQ15)
    {
        /* Calculate the rate of the target Id for smooth transition down resulting from the limited input speed. The
         * rate increases from 0.0 to 1.0 as the speed decreases from the mid point between transitionSpeedUp and
         * transitionSpeedDown to transitionSpeedDown. */
        Ifx_Math_Fract16 speedDifferenceQ15 = self->transitionSpeedMidQ15 - estimatedSpeedAbsQ15;
        Ifx_Math_Fract16 rate               = Ifx_Math_MulShRSat_F16(speedDifferenceQ15,
            IFX_MS_FOCSOLUTIONF16_CFG_SPEED_BAND_HALF_INV_Q, IFX_MS_FOCSOLUTIONF16_CFG_SPEED_BAND_HALF_INV_Q_FORMAT);

        /* Calculate Id */
        self->dqCommand.real = Ifx_Math_Mul_F16(rate, self->p_transitionDownTargetDCurrentQ15);

        /* Stay in closed loop */
        nextSubState = Ifx_MS_FocSolutionF16_SubState_closedLoop;
    }

#endif /*(IFX_MS_FOCSOLUTIONF16_CFG_TRANSITION_MODE == IFX_MS_FOCSOLUTIONF16_TRANSITION_MODE_SMOOTH_TRANSITION) */
    else
    {
        /* Set the direct real reference current (ready for field weakening) */
        self->dqCommand.real = currentsDqRef.real;

        /* Stay in closed loop */
        nextSubState = Ifx_MS_FocSolutionF16_SubState_closedLoop;
    }

    /* Return the substate for the next cycle */
    return nextSubState;
}


static inline Ifx_MS_FocSolutionF16_SubState Ifx_MS_FocSolutionF16_subStateClosedLoopExit(Ifx_MS_FocSolutionF16* self)
{
    Ifx_MS_FocSolutionF16_SubState  nextSubState;
    Ifx_Math_Fract16                estimatedSpeedQ15 = self->p_output.estimatedSpeedQ15;

    /* Get flux estimator output */
    Ifx_MDA_FluxEstimatorF16_Output fluxEstimator;
    Ifx_MDA_FluxEstimatorF16_getOutput(&(self->fluxEstimator), &fluxEstimator);

    /* Reset the iToF previous angle to last flux estimator angle */
    Ifx_MDA_IToFControllerF16_setAnglePreviousValue(&(self->iToF), fluxEstimator.anglePLL);

    /* Reset the acceleration limit state to the last estimated speed */
    Ifx_Math_AccelLimitF16_setSpeedStepPreviousValue(&(self->accelerationLimit), estimatedSpeedQ15);

    /* Set open loop ramp up/down rates */
    Ifx_Math_AccelLimitF16_setSpeedStepUpLimit(&(self->accelerationLimit), self->p_speedRampUpRateOpenLoopQ30);
    Ifx_Math_AccelLimitF16_setSpeedStepDownLimit(&(self->accelerationLimit), self->p_speedRampDownRateOpenLoopQ30);

    /* Store last ref. q current to calculate the slope in subStateTransitDown */
    self->p_previousQCommand = self->dqCommand.imag;

#if (IFX_MS_FOCSOLUTIONF16_CFG_TRANSITION_MODE == IFX_MS_FOCSOLUTIONF16_TRANSITION_MODE_SMOOTH_TRANSITION)

    /* Reset transition counter */
    self->p_transitionCounter_cycles = 0u;

    /* Go to transition down */
    nextSubState = Ifx_MS_FocSolutionF16_SubState_transitionDown;
#endif /* (IFX_MS_FOCSOLUTIONF16_CFG_TRANSITION_MODE == IFX_MS_FOCSOLUTIONF16_TRANSITION_MODE_SMOOTH_TRANSITION) */

#if (IFX_MS_FOCSOLUTIONF16_CFG_TRANSITION_MODE == IFX_MS_FOCSOLUTIONF16_TRANSITION_MODE_DIRECT_TRANSITION)

    /* Go to transition down */
    nextSubState = Ifx_MS_FocSolutionF16_SubState_openLoop;
#endif /* (IFX_MS_FOCSOLUTIONF16_CFG_TRANSITION_MODE == IFX_MS_FOCSOLUTIONF16_TRANSITION_MODE_DIRECT_TRANSITION) */

    return nextSubState;
}


#if (IFX_MS_FOCSOLUTIONF16_CFG_TRANSITION_MODE == IFX_MS_FOCSOLUTIONF16_TRANSITION_MODE_SMOOTH_TRANSITION)
static inline Ifx_MS_FocSolutionF16_SubState Ifx_MS_FocSolutionF16_subStateTransitDown(Ifx_MS_FocSolutionF16* self,
                                                                                       Ifx_MDA_IToFControllerF16_Output
                                                                                       iToFOutput, Ifx_Math_CmpFract16
                                                                                       currentsDqRef)
{
    Ifx_MS_FocSolutionF16_SubState nextSubState;

    /* Calculate the slope of the transition down based on the elapsed transition cycles */
    Ifx_Math_Fract16               transitionCounterQ15 = Ifx_Math_MulShR_F16(
        (Ifx_Math_Fract16)self->p_transitionCounter_cycles, self->p_transitionDeltaQ15, 0u);

    /* Increase amplitude of ref. Q current */
    Ifx_MS_FocSolutionF16_increaseQCurrent(self, iToFOutput, transitionCounterQ15);

    /* Decrease amplitude of ref. D current */
    Ifx_MS_FocSolutionF16_decreaseDCurrent(self, currentsDqRef.real, transitionCounterQ15);

    if (self->p_transitionCounter_cycles < (self->p_transitionTimeLimit_cycles - 1u))
    {
        /* Increment transition counter */
        self->p_transitionCounter_cycles = self->p_transitionCounter_cycles + 1u;

        /* Stay in transition down */
        nextSubState = Ifx_MS_FocSolutionF16_SubState_transitionDown;
    }
    else
    {
        /* Store last ref. q current to check for addition of PI */
        self->p_previousQCommand = self->dqCommand.imag;

        /* Go to open loop */
        nextSubState = Ifx_MS_FocSolutionF16_SubState_openLoop;
    }

    return nextSubState;
}


static inline void Ifx_MS_FocSolutionF16_increaseQCurrent(Ifx_MS_FocSolutionF16                * self,
                                                          Ifx_MDA_IToFControllerF16_Output const iToFOutput,
                                                          Ifx_Math_Fract16 const                 transitionCounterQ15)
{
    /* Increase amplitude of positive imaginary ref. current at slope defined */
    if (self->rateLimitInSpeedQ15 >= 0)
    {
        /* Store delta between i2f q current and last ref. q current from PI based on sign ref. speed */
        Ifx_Math_Fract16 currDeltaQ15 = Ifx_Math_Sub_F16(iToFOutput.refCurrent.imag, self->p_previousQCommand);

        /* Increase amplitude of ref. q current */
        self->dqCommand.imag = Ifx_Math_Add_F16(self->p_previousQCommand, Ifx_Math_Mul_F16(transitionCounterQ15,
            currDeltaQ15));
    }

    /* Increase amplitude of negative imaginary ref. current at slope defined */
    else
    {
        /* Store delta between i2f q current and last ref. q current from PI based on sign ref. speed */
        Ifx_Math_Fract16 currDeltaQ15 = Ifx_Math_Add_F16(iToFOutput.refCurrent.imag, self->p_previousQCommand);

        /* Increase amplitude of ref. q current */
        self->dqCommand.imag = Ifx_Math_Sub_F16(self->p_previousQCommand, Ifx_Math_Mul_F16(transitionCounterQ15,
            currDeltaQ15));
    }
}


static inline void Ifx_MS_FocSolutionF16_decreaseQCurrent(Ifx_MS_FocSolutionF16                * self,
                                                          Ifx_MDA_IToFControllerF16_Output const iToFOutput)
{
    /* Increment transition counter */
    self->p_transitionCounter_cycles = self->p_transitionCounter_cycles + 1u;

    /* Slope to increase or decrease the currents */
    Ifx_Math_Fract16 slope = Ifx_Math_DivShLSat_F16((Ifx_Math_Fract16)self->p_transitionCounter_cycles,
        (Ifx_Math_Fract16)self->p_transitionTimeLimit_cycles, 15u);

    /* Decrease amplitude of positive imaginary ref. current at slope defined */
    if (self->rateLimitInSpeedQ15 >= 0)
    {
        self->dqCommand.imag = Ifx_Math_SubSat_F16(iToFOutput.refCurrent.imag, Ifx_Math_Mul_F16(slope,
            iToFOutput.refCurrent.imag));
    }

    /* Decrease amplitude of negative imaginary ref. current at slope defined */
    else
    {
        self->dqCommand.imag = Ifx_Math_SubSat_F16(Ifx_Math_Mul_F16(slope, iToFOutput.refCurrent.imag),
            iToFOutput.refCurrent.imag);
    }
}


static inline void Ifx_MS_FocSolutionF16_decreaseDCurrent(Ifx_MS_FocSolutionF16* self, Ifx_Math_Fract16 const
                                                          currentDRefQ15, Ifx_Math_Fract16 const transitionCounterQ15)
{
    /* Calculate difference between target D current during transition down and reference D current */
    Ifx_Math_Fract16 currentDDiffQ15 = Ifx_Math_Sub_F16(self->p_transitionDownTargetDCurrentQ15, currentDRefQ15);

    /* Decrease Id from the target Id current for transition down to the input d command */
    self->dqCommand.real = Ifx_Math_Sub_F16(self->p_transitionDownTargetDCurrentQ15, Ifx_Math_Mul_F16(
        transitionCounterQ15, currentDDiffQ15));
}


#endif /* (IFX_MS_FOCSOLUTIONF16_CFG_TRANSITION_MODE == IFX_MS_FOCSOLUTIONF16_TRANSITION_MODE_SMOOTH_TRANSITION) */
static inline Ifx_MS_FocSolutionF16_State Ifx_MS_FocSolutionF16_stateFault(Ifx_MS_FocSolutionF16* self)
{
    Ifx_MS_FocSolutionF16_State nextState;
    bool                        faultCleared;

    /* Check whether clear fault state is requested */
    if (self->p_clearFaultIsRequested == true)
    {
        /* Check whether all faults in the underlying modules have been successfully cleared */
        faultCleared = Ifx_MS_FocSolutionF16_faultSuccessfullyCleared(self);

        if (faultCleared == true)
        {
            /* Disable underlying modules and go to off state */
            Ifx_MS_FocSolutionF16_localEnable(self, false);
            nextState = Ifx_MS_FocSolutionF16_State_off;
        }
        else
        {
            /* Fault of an underlying module has not been successfully cleared, stay in fault state */
            nextState = Ifx_MS_FocSolutionF16_State_fault;
        }
    }
    else
    {
        /* No clear fault requested, stay in fault state */
        nextState = Ifx_MS_FocSolutionF16_State_fault;
    }

    return nextState;
}


static inline Ifx_MS_FocSolutionF16_State Ifx_MS_FocSolutionF16_stateRampDown(Ifx_MS_FocSolutionF16* self, bool
                                                                              faultStatus, Ifx_Math_CmpFract16
                                                                              currentsDqRef,
                                                                              Ifx_MDA_IToFControllerF16_Output
                                                                              iToFOutput)
{
    Ifx_MS_FocSolutionF16_State nextState;

    /* Perform speed and acceleration limit */
    Ifx_MS_FocSolutionF16_limitSpeed(self, 0);

    if (faultStatus == true)
    {
        /* Disable flux estimator */
        Ifx_MDA_FluxEstimatorF16_configMode(&(self->fluxEstimator), Ifx_MDA_FluxEstimatorF16_Mode_disable);

        /* Go into fault state */
        nextState = Ifx_MS_FocSolutionF16_State_fault;
    }
    else if (Ifx_Math_Abs_F16(self->rateLimitInSpeedQ15) <= IFX_MS_FOCSOLUTIONF16_CFG_MIN_SPEED_THRESHOLD_Q15)
    {
        /* Disable flux estimator */
        Ifx_MDA_FluxEstimatorF16_configMode(&(self->fluxEstimator), Ifx_MDA_FluxEstimatorF16_Mode_disable);
        nextState = Ifx_MS_FocSolutionF16_State_standBy;
    }
    else
    {
        /* Execute sub-state machine */
        Ifx_MS_FocSolutionF16_subStateMachine(self, iToFOutput, currentsDqRef);
        nextState = Ifx_MS_FocSolutionF16_State_rampDown;
    }

    return nextState;
}


static inline bool Ifx_MS_FocSolutionF16_anyClearFaultIsPending(Ifx_MS_FocSolutionF16* self)
{
    /* Holds the overall result of the underlying modules clearFaultIsPending */
    bool clearFaultIsPending = false;

    /* MHA modules fault is pending */
    if (Ifx_MHA_BridgeDrv_TLE987_clearFaultIsPending(&(self->bridgeDrvTLE987)) == true)
    {
        clearFaultIsPending = true;
    }
    else if (Ifx_MHA_PatternGen_TLE987_clearFaultIsPending(&(self->patternGenTLE987)) == true)
    {
        clearFaultIsPending = true;
    }

    /* Modulator fault is pending */
    else if (Ifx_MAS_ModulatorF16_clearFaultIsPending(&(self->modulator)) == true)
    {
        clearFaultIsPending = true;
    }

#if IFX_MS_FOCSOLUTIONF16_CFG_INCLUDE_STARTANGLE_IDENT == 1

    /* StartAngleIdent fault is pending */
    else if (Ifx_MDA_StartAngleIdentF16_clearFaultIsPending(&(self->startAngleIdent)) == true)
    {
        clearFaultIsPending = true;
    }
#endif /* IFX_MS_FOCSOLUTIONF16_CFG_INCLUDE_STARTANGLE_IDENT == 1 */
    else
    {
        clearFaultIsPending = false;
    }

    /* Return whether clear fault of any underlying module is still pending */
    return clearFaultIsPending;
}


static inline bool Ifx_MS_FocSolutionF16_faultSuccessfullyCleared(Ifx_MS_FocSolutionF16* self)
{
    bool faultCleared;

    /* Check that none of the clear fault requests in the underlying modules are pending */
    if (Ifx_MS_FocSolutionF16_anyClearFaultIsPending(self) == false)
    {
        /* Check for faults in underlying modules */
        if (Ifx_MS_FocSolutionF16_faultStatus(self) == true)
        {
            faultCleared = false;
        }

        /* No faults detected any more, fault(s) successfully cleared */
        else
        {
            faultCleared = true;
        }

        /* Reset the clear fault requested flag */
        self->p_clearFaultIsRequested = false;
    }
    else
    {
        /* Clear fault has not finished execution in every module yet */
        faultCleared = false;
    }

    return faultCleared;
}


#if IFX_MS_FOCSOLUTIONF16_CFG_INCLUDE_STARTANGLE_IDENT == 1
static inline Ifx_MS_FocSolutionF16_State Ifx_MS_FocSolutionF16_stateStartAngleIdent(Ifx_MS_FocSolutionF16* self,
                                                                                     Ifx_Math_Fract16 speedQ15, bool
                                                                                     faultStatus,
                                                                                     Ifx_MDA_IToFControllerF16_Output
                                                                                     iToFOutput)
{
    Ifx_MS_FocSolutionF16_State       nextState;

    /* Get the status of the start angle estimation */
    Ifx_MDA_StartAngleIdentF16_Status startAngleEstStatus = Ifx_MDA_StartAngleIdentF16_getStatus(
        &(self->startAngleIdent));

    if (faultStatus == true)
    {
        /* Go to fault */
        nextState = Ifx_MS_FocSolutionF16_State_fault;
    }
    else if (startAngleEstStatus.state == Ifx_MDA_StartAngleIdentF16_State_estimationReady)
    {
        /* Disable start angle ident. module */
        Ifx_MDA_StartAngleIdentF16_enable(&(self->startAngleIdent), false);

        /* Stay in start angle ident. state */
        nextState = Ifx_MS_FocSolutionF16_State_startAngleIdent;
    }
    else if (startAngleEstStatus.state == Ifx_MDA_StartAngleIdentF16_State_off)
    {
        /* Variable to store start angle identification output */
        Ifx_MDA_StartAngleIdentF16_Output startAngleIdentOutput;

        /* Get start angle identification output */
        Ifx_MDA_StartAngleIdentF16_getOutput(&(self->startAngleIdent), &startAngleIdentOutput);

        /* Init i2f angle prev value */
        Ifx_MS_FocSolutionF16_setAnglePreviousValue(self, speedQ15, startAngleIdentOutput);

        if (self->p_enableControl == true)
        {
            /* Update control mode */
            self->p_status.actualControlMode = Ifx_MS_FocSolutionF16_getControlMode(self);

            /* Set Q command current */
            self->dqCommand.imag = iToFOutput.refCurrent.imag;

            /* Set open loop ramp up/down rates */
            Ifx_Math_AccelLimitF16_setSpeedStepUpLimit(&(self->accelerationLimit),
                self->p_speedRampUpRateOpenLoopQ30);
            Ifx_Math_AccelLimitF16_setSpeedStepDownLimit(&(self->accelerationLimit),
                self->p_speedRampDownRateOpenLoopQ30);

            /* Init. substate machine */
            self->p_status.subState = Ifx_MS_FocSolutionF16_SubState_openLoop;

            /* Enable flux estimator */
            Ifx_MDA_FluxEstimatorF16_configMode(&(self->fluxEstimator), Ifx_MDA_FluxEstimatorF16_Mode_enable);

            /* If estimation is done and speed control is enabled, store estimated angle and go to run */
            nextState = Ifx_MS_FocSolutionF16_State_run;
        }
        else
        {
            nextState = Ifx_MS_FocSolutionF16_State_standBy;
        }
    }
    else
    {
        /* Stay in start angle ident. state */
        nextState = Ifx_MS_FocSolutionF16_State_startAngleIdent;
    }

    return nextState;
}


void Ifx_MS_FocSolutionF16_setAnglePreviousValue(Ifx_MS_FocSolutionF16* self, Ifx_Math_Fract16 speedQ15,
                                                 Ifx_MDA_StartAngleIdentF16_Output startAngleIdentOutput)
{
    /* Init i2f angle prev value */
    if (speedQ15 > 0)
    {
        Ifx_MDA_IToFControllerF16_setAnglePreviousValue(&(self->iToF), startAngleIdentOutput.estimatedAngle -
            IFX_MS_FOCSOLUTIONF16_PIBY2 + IFX_MS_FOCSOLUTIONF16_PIBY6);
    }
    else if (speedQ15 < 0)
    {
        Ifx_MDA_IToFControllerF16_setAnglePreviousValue(&(self->iToF), startAngleIdentOutput.estimatedAngle -
            IFX_MS_FOCSOLUTIONF16_PIBY2 - IFX_MS_FOCSOLUTIONF16_PIBY6);
    }
    else
    {
        Ifx_MDA_IToFControllerF16_setAnglePreviousValue(&(self->iToF), startAngleIdentOutput.estimatedAngle -
            IFX_MS_FOCSOLUTIONF16_PIBY2);
    }
}


#endif /* IFX_MS_FOCSOLUTIONF16_CFG_INCLUDE_STARTANGLE_IDENT == 1 */
static void Ifx_MS_FocSolutionF16_reset(Ifx_MS_FocSolutionF16* self)
{
    /* Flux estimator */
    Ifx_Math_LowPass1stF16_setPreviousValue(&(self->fluxEstimator.p_alphaFilter), 0);
    Ifx_Math_LowPass1stF16_setPreviousValue(&(self->fluxEstimator.p_betaFilter), 0);
    Ifx_Math_LowPass1stF16_setPreviousValue(&(self->fluxEstimator.p_speedFilter), 0);
    Ifx_Math_PLLF16_resetBuffer(&self->fluxEstimator.p_pllFilter);
    Ifx_Math_PLLF16_setPreviousValue(&self->fluxEstimator.p_pllFilter, 0);

    /* PI controller */
    Ifx_Math_PiF16_setIntegPreviousValue(&(self->speedPi), 0);

    /* Field oriented controller */
    Ifx_MDA_FocControllerF16_reset(&(self->focController));

    /* Acceleration limiter and I2f */
    Ifx_Math_AccelLimitF16_setSpeedStepPreviousValue(&(self->accelerationLimit), 0);
    Ifx_Math_RateLimitF16_setPreviousValue(&(self->p_startCurrentRateLimit),
        IFX_MS_FOCSOLUTIONF16_CFG_INIT_START_UP_CURRENT_Q15);

    /* Set the initial startup current to the statically configured value */
    self->dqCommand.imag = IFX_MS_FOCSOLUTIONF16_CFG_INIT_START_UP_CURRENT_Q15;

    /* Speed and sector number */
    self->rateLimitInSpeedQ15                      = 0;
    self->previousCurrentReconstructionInfo.sector = 0;
}


/* polyspace-end MISRA2012:D4.14 [Justified:Low] "The caller function has to guarantee that NULL is not passed as
 * argument." */
