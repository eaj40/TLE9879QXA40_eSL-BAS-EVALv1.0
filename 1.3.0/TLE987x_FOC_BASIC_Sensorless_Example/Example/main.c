/*
 * Copyright (c) 2022 Infineon Technologies AG. All Rights Reserved.
 *
 * Use of this file is subject to the terms of use agreed between (i) you or the company in which ordinary course of
 * business you are acting and (ii) Infineon Technologies AG, its affiliates or its licensees. If and as long as no
 * such terms of use are agreed, use of this file is subject to the Evaluation Software License Agreement distributed
 * along with this file within the software delivery package.
 *
 */

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/

#include "tle_device.h"
#include "Ifx_MS_FocSolutionF16.h"
#include "Ifx_Math_DivSat.h"

#include "Ifx_MHA_BridgeDrv_TLE987.h"
#include "probe_scope.h"

#include "no_opt.h"

/* User input speed which can be set in rpm and currents in dq frame which can be set when direct interface is enabled
 * */
#if defined(__ARMCOMPILER_VERSION)
NO_OPT volatile Ifx_Math_Fract16    referenceSpeedQ0    = 0;
NO_OPT volatile Ifx_Math_CmpFract16 currentsDqReference = {0, 0};
#else
volatile Ifx_Math_Fract16           referenceSpeedQ0    = 0;
volatile Ifx_Math_CmpFract16        currentsDqReference = {0, 0};
#endif

/* FOC instance */
NO_OPT Ifx_MS_FocSolutionF16        FocDemoClosedLoop = {
    .dqCommand = {0, 0},
};

/* User inputs to enable FOC */
/* Variables are defined as uint8 instead of bool because of the error in microinspector */
NO_OPT volatile uint8               enablePowerStage = 1;
NO_OPT volatile uint8               enableControl    = 0;

/* Corresponds to enum Ifx_MS_FocSolutionF16_ControlMode; mode = 1 == IToF and FOC; mode = 0 == VToF */
NO_OPT volatile uint8               controlMode           = 1;
NO_OPT volatile uint8               enableDirectInterface = 0;

/* User input to clear FOC fault */
NO_OPT volatile uint8               clrFaultFoc           = 0;
NO_OPT volatile uint8               enableSpeedPreControl = 0;

/* User input to enable start angle ident. */
NO_OPT volatile uint8               enableStartAngleIdent = 0;

/* Counter for the rotor alignment */
NO_OPT volatile uint16              rotorAlignCounter = 0;

/* Duration rotor alignment (steps) = alignment(s) * speed loop freq (Hz) */
#define ROTORALIGNMENT_TIME_STEPS (666)

#if (IFX_MS_FOCSOLUTIONF16_CFG_CURRENT_LOOP_FACTOR > 1)

/* Counter for the current control loop execution */
NO_OPT volatile sint8 currentControlCounter = 0;

#endif

#if (IFX_MS_FOCSOLUTIONF16_CFG_CURRENT_LOOP_FACTOR > 1)

/* Fast loop execution call back, PendSV_Handler software interrupt defined in start up file */
void PendSV_Handler(void)
{
    Ifx_MS_FocSolutionF16_executeControlMode(&FocDemoClosedLoop);
}


#endif

/* CCU6 one match callback */
void Ifx_FOC_oneMatchCallback(void)
{
    Ifx_MHA_PatternGen_TLE987_onOneMatch(&(FocDemoClosedLoop.patternGenTLE987));
#if (IFX_MS_FOCSOLUTIONF16_CFG_CURRENT_LOOP_FACTOR > 1)

    /* Execute current control only in the defined cycles */
    if (currentControlCounter < (IFX_MS_FOCSOLUTIONF16_CFG_CURRENT_LOOP_FACTOR - 1))
    {
        /* Increment counter */
        currentControlCounter++;
    }
    else
    {
        Ifx_MHA_MeasurementADC_TLE987_oneMatch(&(FocDemoClosedLoop.measurementADCTLE987));

        /* Reset Pattern Generator cycle counter */
        Ifx_MHA_PatternGen_TLE987_reset(&(FocDemoClosedLoop.patternGenTLE987));

        /* Trigger PendSV interrupt and reset counter */
        currentControlCounter   = 0;
        CPU->ICSR.bit.PENDSVSET = 1U;
    }

#else
    Ifx_MHA_MeasurementADC_TLE987_oneMatch(&(FocDemoClosedLoop.measurementADCTLE987));
    Ifx_MS_FocSolutionF16_executeControlMode(&FocDemoClosedLoop);
#endif
}


/* CCU6 period match callback */
void Ifx_FOC_periodMatchCallback(void)
{
    Ifx_MHA_MeasurementADC_TLE987_periodMatch(&(FocDemoClosedLoop.measurementADCTLE987));
    Ifx_MHA_PatternGen_TLE987_onPeriodMatch(&(FocDemoClosedLoop.patternGenTLE987));
}


/* Slow loop execution call back which is called by timer GPT2.T6 */
NO_OPT void Ifx_FOC_speedLoop_callback(void)
{
    /* Variable to hold the scaled ref. speed */
    Ifx_Math_Fract16 referenceSpeedQ15;

    /* If speed control or powerstage disabled */
    if ((enableControl == 0)
        || (enablePowerStage == 0))
    {
        /* set ref. speed to 0 */
        referenceSpeedQ15 = 0;

        /* Set alignment counter to init. value */
        rotorAlignCounter = ROTORALIGNMENT_TIME_STEPS;
    }

    /* If speed control enabled */
    else if (enableControl == 1)
    {
        /* Rotor alignment finished */
        if (rotorAlignCounter == 0)
        {
            /* Calculate the scaled speed from the ref. and base speed */
            referenceSpeedQ15 = Ifx_Math_DivSat_F16(referenceSpeedQ0, IFX_MS_FOCSOLUTIONF16_CFG_BASE_MECH_SPEED_RPM);
        }

        /* Rotor alignment ongoing */
        else
        {
            /* During rotor alignment ref. speed = 0 */
            referenceSpeedQ15 = 0;

            /* Decrement rotor alignment counter */
            rotorAlignCounter = rotorAlignCounter - 1;
        }
    }

    /* Execute speed control */
    Ifx_MS_FocSolutionF16_executeSpeedControl(&FocDemoClosedLoop, referenceSpeedQ15, currentsDqReference);
}


int main(void)
{
    /*****************************************************************************
    ** initialization of the hardware modules based on the configuration done   **
    ** by using the IFXConfigWizard                                             **
    *****************************************************************************/

    /*****************************************************************************
    ** place your application code here                                         **
    *****************************************************************************/

    /* Initialize Hardware */
    TLE_Init();

    /* Start Timer3 high and low byte timer */
    TIMER3_T3HL_Start();

    /* Start speed loop timer */
    GPT12E_T6_Start();
#if (IFX_MS_FOCSOLUTIONF16_CFG_CURRENT_LOOP_FACTOR > 1)

    /* Set PendSV interrupt priority */
    NVIC_SetPriority(PendSV_IRQn, 2);
#endif

    /* Start CCU6 T12 timer */
    CCU6_StartTmr_T12();
//    ProbeScope_Init(10000);

    /* Initialize FOC */
    Ifx_MS_FocSolutionF16_init(&FocDemoClosedLoop);

    /*****************************************************************************
    ** main endless loop                                                        **
    *****************************************************************************/
    for ( ; ;)
    {
        /* Service Watchdog */
        (void)WDT1_Service();

        /* Call clear fault API if requested */
        if (clrFaultFoc == 1)
        {
            Ifx_MS_FocSolutionF16_clearFault(&FocDemoClosedLoop);
            clrFaultFoc = 0;
        }

        /* Call FOC enable */
        Ifx_MS_FocSolutionF16_enablePowerStage(&FocDemoClosedLoop, (bool)enablePowerStage);
        Ifx_MS_FocSolutionF16_enableControl(&FocDemoClosedLoop, (bool)enableControl);
        Ifx_MS_FocSolutionF16_setControlMode(&FocDemoClosedLoop, (Ifx_MS_FocSolutionF16_ControlMode)controlMode);
        Ifx_MS_FocSolutionF16_enableDirectInterface(&FocDemoClosedLoop, (bool)enableDirectInterface);

        /* Call Bridge Driver (fault handling) */
        Ifx_MHA_BridgeDrv_TLE987_execute(&(FocDemoClosedLoop.bridgeDrvTLE987));
        Ifx_MS_FocSolutionF16_enableSpeedPreControl(&FocDemoClosedLoop, (bool)enableSpeedPreControl);

        if (enableStartAngleIdent == 1)
        {
            Ifx_MS_FocSolutionF16_enableStartAngleIdent(&FocDemoClosedLoop);
            // enableControl = 1;
            enableStartAngleIdent = 0;
        }
    }
}
