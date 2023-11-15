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
#include "Ifx_MHA_MeasurementADC_TLE987.h"
#include "Ifx_MHA_MeasurementADC_Cfg.h"

/* Math library includes */
#include "Ifx_Math_Sat.h"
#include "Ifx_Math_ShR.h"

/* CMSIS includes */
#include "cmsis_compiler.h"

/* SDK includes */
#include "adc1.h"

/* Macros to define the component ID */
#define IFX_MHA_MEASUREMENTADC_TLE987_COMPONENTID_SOURCEID     ((uint8)Ifx_ComponentID_SourceID_infineonTechnologiesAG)
#define IFX_MHA_MEASUREMENTADC_TLE987_COMPONENTID_LIBRARYID              \
                                                               ((uint16) \
     Ifx_ComponentID_LibraryID_mctrlHardwareAbstraction)

#define IFX_MHA_MEASUREMENTADC_TLE987_COMPONENTID_MODULEID     (1U)
#define IFX_MHA_MEASUREMENTADC_TLE987_COMPONENTID_COMPONENTID1 (1U)

#define IFX_MHA_MEASUREMENTADC_TLE987_COMPONENTID_COMPONENTID2 ((uint8)Ifx_ComponentID_ComponentID2_basic)

/* Macros to define the component version */
#define IFX_MHA_MEASUREMENTADC_TLE987_COMPONENTVERSION_MAJOR   (1U)
#define IFX_MHA_MEASUREMENTADC_TLE987_COMPONENTVERSION_MINOR   (3U)
#define IFX_MHA_MEASUREMENTADC_TLE987_COMPONENTVERSION_PATCH   (0U)
#define IFX_MHA_MEASUREMENTADC_TLE987_COMPONENTVERSION_T       (0U)
#define IFX_MHA_MEASUREMENTADC_TLE987_COMPONENTVERSION_REV     (0U)

/* *INDENT-OFF* */
/* Component ID */
static const Ifx_ComponentID      Ifx_MHA_MeasurementADC_TLE987_componentID = {
    .sourceID     = IFX_MHA_MEASUREMENTADC_TLE987_COMPONENTID_SOURCEID,
    .libraryID    = IFX_MHA_MEASUREMENTADC_TLE987_COMPONENTID_LIBRARYID,
    .moduleID     = IFX_MHA_MEASUREMENTADC_TLE987_COMPONENTID_MODULEID,
    .componentID1 = IFX_MHA_MEASUREMENTADC_TLE987_COMPONENTID_COMPONENTID1,
    .componentID2 = IFX_MHA_MEASUREMENTADC_TLE987_COMPONENTID_COMPONENTID2,
};

/* Component Version */
static const Ifx_ComponentVersion Ifx_MHA_MeasurementADC_TLE987_componentVersion = {
    .major = IFX_MHA_MEASUREMENTADC_TLE987_COMPONENTVERSION_MAJOR,
    .minor = IFX_MHA_MEASUREMENTADC_TLE987_COMPONENTVERSION_MINOR,
    .patch = IFX_MHA_MEASUREMENTADC_TLE987_COMPONENTVERSION_PATCH,
    .t     = IFX_MHA_MEASUREMENTADC_TLE987_COMPONENTVERSION_T,
    .rev   = IFX_MHA_MEASUREMENTADC_TLE987_COMPONENTVERSION_REV
};
/* *INDENT-ON* */
/* Swap write index of the current measurement buffer after receiving second current measurement */
static inline void Ifx_MHA_MeasurementADC_TLE987_swapCurrentMeasurementWriteIndex(
    Ifx_MHA_MeasurementADC_TLE987* self);

/* DC link voltage and shunt currents calculation*/
static inline void Ifx_MHA_MeasurementADC_TLE987_calc(Ifx_MHA_MeasurementADC_TLE987* self);

/* Scale DC-Link currents to Q15 */
static inline Ifx_Math_Fract16 Ifx_MHA_MeasurementADC_TLE987_scaleCurrent(Ifx_MHA_MeasurementADC_TLE987* self, uint16
                                                                          rawCurrentMeasurement);

/* polyspace-begin MISRA2012:D4.14 [Justified:Low] "The caller function has to guarantee that NULL is not passed as
 * argument." */

/* polyspace-begin MISRA2012:5.1 [Justified:Low] "Violation is justified because all supported compilers can handle
 * notably more significant initial characters in identifiers than required in this case and the readability is
 * ensured." */

/* Function to get the component ID */
void Ifx_MHA_MeasurementADC_TLE987_getID(const Ifx_ComponentID** componentID)
{
    *componentID = &Ifx_MHA_MeasurementADC_TLE987_componentID;
}


/* Function to get the component version */
void Ifx_MHA_MeasurementADC_TLE987_getVersion(const Ifx_ComponentVersion** componentVersion)
{
    *componentVersion = &Ifx_MHA_MeasurementADC_TLE987_componentVersion;
}


/* polyspace-end MISRA2012:5.1 [Justified:Low] "Violation is justified because all supported compilers can handle
 * notably more significant initial characters in identifiers than required in this case and the readability is
 * ensured." */
void Ifx_MHA_MeasurementADC_TLE987_init(Ifx_MHA_MeasurementADC_TLE987* self)
{
    /* Reset variables used to store current measurements */
    self->p_rawCurrentMeasurements[0][0]     = 0U;
    self->p_rawCurrentMeasurements[0][1]     = 0U;
    self->p_rawCurrentMeasurements[1][0]     = 0U;
    self->p_rawCurrentMeasurements[1][1]     = 0U;
    self->p_rawCurrentMeasurementsReadIndex  = 1;
    self->p_rawCurrentMeasurementsWriteIndex = 0;

    /* Reset outputs */
    self->p_output.dcLinkVoltageQ15    = 0;
    self->p_output.shuntCurrentsQ15[0] = 0;
    self->p_output.shuntCurrentsQ15[1] = 0;

    /* Reset module parameters */
    self->p_currentAccumulator                   = 0UL;
    self->p_cycleCounter                         = 1U;
    self->p_offset                               = 0U;
    self->_Super_Ifx_MHA_MeasurementADC.p_enable = false;

    /* Initialize state */
    self->p_status.state = Ifx_MHA_MeasurementADC_TLE987_State_init;

    /* Initialize current and CSA gains */
    Ifx_MHA_MeasurementADC_TLE987_setCsaGain(self,
        (Ifx_MHA_MeasurementADC_TLE987_optionCsaGain)IFX_MHA_MEASUREMENTADC_CFG_CSA_GAIN);
}


/* Period match interrupt */
void Ifx_MHA_MeasurementADC_TLE987_periodMatch(Ifx_MHA_MeasurementADC_TLE987* self)
{
    /* Read ADC EIM result register */
    self->p_rawCurrentMeasurements[self->p_rawCurrentMeasurementsWriteIndex][0] = ADC1_EIM_Result_Get();
}


/* One match interrupt */
void Ifx_MHA_MeasurementADC_TLE987_oneMatch(Ifx_MHA_MeasurementADC_TLE987* self)
{
    /* Read ADC CSA result register */
    self->p_rawCurrentMeasurements[self->p_rawCurrentMeasurementsWriteIndex][1] = ADC1_CSA_Result_Get();

    /* Swap write index of ra current measurment buffer*/
    Ifx_MHA_MeasurementADC_TLE987_swapCurrentMeasurementWriteIndex(self);
}


__USED void Ifx_MHA_MeasurementADC_TLE987_execute(Ifx_MHA_MeasurementADC_TLE987* self)
{
    /* Variable to store the state */
    Ifx_MHA_MeasurementADC_TLE987_State previousState = self->p_status.state;
    Ifx_MHA_MeasurementADC_TLE987_State nextState     = previousState;

    switch (previousState)
    {
        /* Initialize ADC and CSA */
        case Ifx_MHA_MeasurementADC_TLE987_State_init:
            CSA_Power_Off();

            /* Set the state to OFF */
            nextState = Ifx_MHA_MeasurementADC_TLE987_State_off;
            break;

        /* Current sense amplifier powered on and ADC measuring outputs */
        case Ifx_MHA_MeasurementADC_TLE987_State_on:

            /* Check if module disabled */
            if (self->_Super_Ifx_MHA_MeasurementADC.p_enable == false)
            {
                /* Disable the current sense amplifier */
                CSA_Power_Off();

                /* Set the state to OFF */
                nextState = Ifx_MHA_MeasurementADC_TLE987_State_off;
            }
            else
            {
                /* DC link voltage and shunt currents calculation*/
                Ifx_MHA_MeasurementADC_TLE987_calc(self);
            }

            break;

        /* Current sense amplifier powered off */
        case Ifx_MHA_MeasurementADC_TLE987_State_off:

            /* Check if module enabled */
            if (self->_Super_Ifx_MHA_MeasurementADC.p_enable == true)
            {
                /* Enable the current sense amplifier */
                CSA_Power_On();

                /* Change ADC to SW mode and start conversion */
                ADC1_SetMode(SW_MODE);
                ADC1_SetSocSwMode(ADC1_CSA);
                nextState = Ifx_MHA_MeasurementADC_TLE987_State_calibration;
            }

            break;

        /* Calibration of the current sense amplifier */
        case Ifx_MHA_MeasurementADC_TLE987_State_calibration:

            /* Accumulate the input current */
            self->p_currentAccumulator += ADC1_CSA_Result_Get();

            /* Calibration is still ongoing */
            if (self->p_cycleCounter < IFX_MHA_MEASUREMENTADC_CFG_CALIBRATION_CYCLES)
            {
                /* Trigger another conversion */
                ADC1_SOC_Set();
                self->p_cycleCounter++;
            }

            /* Calibration is done, get the average current */
            else
            {
                /* Change ADC to sequencer mode */
                ADC1_SetMode(SEQ_MODE);

                /* Get the average value */
                self->p_offset = (uint16)(self->p_currentAccumulator / self->p_cycleCounter);

                /* Reset cycle counter and accumulator variable */
                self->p_cycleCounter       = 1;
                self->p_currentAccumulator = 0U;

                /* Automatic transition to ON */
                nextState = Ifx_MHA_MeasurementADC_TLE987_State_on;
            }

            break;

        default:

            /* do default transition to INIT */
            nextState = Ifx_MHA_MeasurementADC_TLE987_State_init;
            break;
    }

    self->p_status.state = nextState;
}


static inline Ifx_Math_Fract16 Ifx_MHA_MeasurementADC_TLE987_scaleCurrent(Ifx_MHA_MeasurementADC_TLE987* self, uint16
                                                                          rawCurrentMeasurement)
{
    /* Stores the result of the ADC without the CSA offset */
    sint16           adcWithoutOffset;

    /* Stores the value of the scaled current */
    Ifx_Math_Fract32 scaledCurrent;

    /* Scale shunt currents */
    adcWithoutOffset = (sint16)rawCurrentMeasurement - (sint16)self->p_offset;
    scaledCurrent    = (Ifx_Math_Fract32)adcWithoutOffset * self->p_currentGain.value;

    /* Convert to 16-bit and return scaled current */
    return Ifx_Math_Sat_F16_F32(Ifx_Math_ShR_F32(scaledCurrent, (uint8)self->p_currentGain.qFormat));
}


static inline void Ifx_MHA_MeasurementADC_TLE987_calc(Ifx_MHA_MeasurementADC_TLE987* self)
{
    /* Read DC-Link voltage and scale it */
    Ifx_Math_Fract32 scaledDcLink = (Ifx_Math_Fract32)ADC1_VDH_Result_Get() *
                                    IFX_MHA_MEASUREMENTADC_CFG_CONVERT_VDC_TO_Q15;
    self->p_output.dcLinkVoltageQ15 = Ifx_Math_Sat_F16_F32(Ifx_Math_ShR_F32(scaledDcLink,
        IFX_MHA_MEASUREMENTADC_CFG_VDC_BITS));

    /* Read and scale first shunt current */
    self->p_output.shuntCurrentsQ15[0] = Ifx_MHA_MeasurementADC_TLE987_scaleCurrent(self,
        self->p_rawCurrentMeasurements[self->p_rawCurrentMeasurementsReadIndex][0]);

    /* Read and scale second shunt current */
    self->p_output.shuntCurrentsQ15[1] = Ifx_MHA_MeasurementADC_TLE987_scaleCurrent(self,
        self->p_rawCurrentMeasurements[self->p_rawCurrentMeasurementsReadIndex][1]);
}


static inline void Ifx_MHA_MeasurementADC_TLE987_swapCurrentMeasurementWriteIndex(Ifx_MHA_MeasurementADC_TLE987* self)
{
    /* Store actual write index */
    uint8 writeIndex = self->p_rawCurrentMeasurementsWriteIndex;

    /* Swap actual write index with actual read index */
    self->p_rawCurrentMeasurementsWriteIndex = self->p_rawCurrentMeasurementsReadIndex;

    /* Swap actual read index with previously stored write index */
    self->p_rawCurrentMeasurementsReadIndex = writeIndex;
}


/* polyspace-end MISRA2012:D4.14 [Justified:Low] "The caller function has to guarantee that NULL is not passed as
 * argument." */
