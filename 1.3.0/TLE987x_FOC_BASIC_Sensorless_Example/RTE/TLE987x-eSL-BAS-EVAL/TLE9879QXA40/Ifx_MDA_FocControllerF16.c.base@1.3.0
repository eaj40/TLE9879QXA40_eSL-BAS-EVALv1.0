/*
 * Copyright (c) 2022 Infineon Technologies AG. All Rights Reserved.
 *
 * Use of this file is subject to the terms of use agreed between (i) you or the company in which ordinary course of
 * business you are acting and (ii) Infineon Technologies AG, its affiliates or its licensees. If and as long as no
 * such terms of use are agreed, use of this file is subject to the Evaluation Software License Agreement distributed
 * along with this file within the software delivery package.
 *
 */

#include "Ifx_MDA_FocControllerF16.h"
#include "Ifx_MDA_FocControllerF16_Cfg.h"

/* Math library includes */
#include "Ifx_Math_CartToPolar.h"
#include "Ifx_Math_Interp1DLut.h"
#include "Ifx_Math_Park.h"
#include "Ifx_Math_PolarToCart.h"
#include "Ifx_Math_Arithmetic.h"

/* CMSIS includes */
#include "cmsis_compiler.h"

/* Macros to define the component ID */
#define IFX_MDA_FOCCONTROLLER_COMPONENTID_SOURCEID     ((uint8)Ifx_ComponentID_SourceID_infineonTechnologiesAG)
#define IFX_MDA_FOCCONTROLLER_COMPONENTID_LIBRARYID    ((uint16)Ifx_ComponentID_LibraryID_mctrlDriveAlgorithm)
#define IFX_MDA_FOCCONTROLLER_COMPONENTID_MODULEID     (1U)
#define IFX_MDA_FOCCONTROLLER_COMPONENTID_COMPONENTID1 (1U)

#define IFX_MDA_FOCCONTROLLER_COMPONENTID_COMPONENTID2 ((uint8)Ifx_ComponentID_ComponentID2_basic)

/* Macros to define the component version */
#define IFX_MDA_FOCCONTROLLER_COMPONENTVERSION_MAJOR   (1U)
#define IFX_MDA_FOCCONTROLLER_COMPONENTVERSION_MINOR   (3U)
#define IFX_MDA_FOCCONTROLLER_COMPONENTVERSION_PATCH   (0U)
#define IFX_MDA_FOCCONTROLLER_COMPONENTVERSION_T       (0U)
#define IFX_MDA_FOCCONTROLLER_COMPONENTVERSION_REV     (0U)

/* *INDENT-OFF* */
/* Component ID */
static const Ifx_ComponentID      Ifx_MDA_FocController_componentID = {
    .sourceID     = IFX_MDA_FOCCONTROLLER_COMPONENTID_SOURCEID,
    .libraryID    = IFX_MDA_FOCCONTROLLER_COMPONENTID_LIBRARYID,
    .moduleID     = IFX_MDA_FOCCONTROLLER_COMPONENTID_MODULEID,
    .componentID1 = IFX_MDA_FOCCONTROLLER_COMPONENTID_COMPONENTID1,
    .componentID2 = IFX_MDA_FOCCONTROLLER_COMPONENTID_COMPONENTID2,
};

/* Component Version */
static const Ifx_ComponentVersion Ifx_MDA_FocController_componentVersion = {
    .major = IFX_MDA_FOCCONTROLLER_COMPONENTVERSION_MAJOR,
    .minor = IFX_MDA_FOCCONTROLLER_COMPONENTVERSION_MINOR,
    .patch = IFX_MDA_FOCCONTROLLER_COMPONENTVERSION_PATCH,
    .t     = IFX_MDA_FOCCONTROLLER_COMPONENTVERSION_T,
    .rev   = IFX_MDA_FOCCONTROLLER_COMPONENTVERSION_REV
};
/* *INDENT-ON* */
/* Anti-windup strategy flag, NO saturation */
#define IFX_MDA_FOCCONTROLLER_FLAG_AW_NO  (0)

/* Anti-windup strategy flag, positive limitation is active */
#define IFX_MDA_FOCCONTROLLER_FLAG_AW_POS (1)

/* Anti-windup strategy flag, negative limitation is active */
#define IFX_MDA_FOCCONTROLLER_FLAG_AW_NEG (-1)

#if IFX_MDA_FOCCONTROLLERF16_CFG_DQDECOUPLINGENABLE

/* Local function to execute d-q decoupling */
static inline void Ifx_MDA_FocControllerF16_dqDecoupling(Ifx_MDA_FocControllerF16* self, Ifx_Math_Fract16
                                                         electricalSpeed);
#endif /* IFX_MDA_FOCCONTROLLERF16_CFG_DQDECOUPLINGENABLE */
/* Local functions to initialize d and q PI controllers */
static inline void Ifx_MDA_FocControllerF16_initDPi(Ifx_MDA_FocControllerF16* self);
static inline void Ifx_MDA_FocControllerF16_initQPi(Ifx_MDA_FocControllerF16* self);

#if IFX_MDA_FOCCONTROLLERF16_CFG_LIMIT_VOLT_VECTOR_D_PRIO

/* Local functions to limit D and Q voltages */
static inline void Ifx_MDA_FocControllerF16_limitDQVoltage(Ifx_MDA_FocControllerF16* self, Ifx_Math_Fract16
                                                           dcLinkVoltageQ15);
static inline void Ifx_MDA_FocControllerF16_limitDVoltage(Ifx_MDA_FocControllerF16* self, Ifx_Math_Fract16 maxAmpQ15);
static inline void Ifx_MDA_FocControllerF16_limitQVoltage(Ifx_MDA_FocControllerF16* self, Ifx_Math_Fract16 maxAmpQ15);
#endif /* IFX_MDA_FOCCONTROLLERF16_CFG_LIMIT_VOLT_VECTOR_D_PRIO */
/* Shift factor for getting index from Amplitude table */
#define IFX_MDA_FOCCONTROLLERF16_SHIFT (15 - IFX_MATH_CFG_CARTTOPOLAR_LUT_SIZE)

#if IFX_MDA_FOCCONTROLLERF16_CFG_LIMIT_VOLT_VECTOR_D_PRIO

/* Link to lookup table */
static const Ifx_Math_Interp1DLut_Type focControllerInterp1DLut = {
    .data = &Ifx_Math_Lut_CartToPolar_F16_table[0], .size = (((uint16)1 << IFX_MATH_CFG_CARTTOPOLAR_LUT_SIZE) + 1)
};

#endif /* IFX_MDA_FOCCONTROLLERF16_CFG_LIMIT_VOLT_VECTOR_D_PRIO */
/* Modulation index max for linear region, normalized by 2/pi */
#define IFX_MDA_FOCCONTROLLERF16_CONST_MI_0907_Q15 (18921)

/* polyspace-begin MISRA2012:D4.14 [Justified:Low] "The caller function has to guarantee that NULL is not passed as
 * argument." */

/* Function to get the component ID */
void Ifx_MDA_FocControllerF16_getID(const Ifx_ComponentID** componentID)
{
    *componentID = &Ifx_MDA_FocController_componentID;
}


/* Function to get the component version */
void Ifx_MDA_FocControllerF16_getVersion(const Ifx_ComponentVersion** componentVersion)
{
    *componentVersion = &Ifx_MDA_FocController_componentVersion;
}


/**
 *  Initialize the module to the default values.
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *
 */
void Ifx_MDA_FocControllerF16_init(Ifx_MDA_FocControllerF16* self)
{
    /* Initialize internal variables and outputs to 0 */
    self->currentDQ.real                         = 0;
    self->currentDQ.imag                         = 0;
    self->voltageDQ.real                         = 0;
    self->voltageDQ.imag                         = 0;
    self->p_output.voltageCommandPolar.amplitude = 0;
    self->p_output.voltageCommandPolar.angle     = 0;

    /* Initialize Id PI controller */
    Ifx_MDA_FocControllerF16_initDPi(self);

    /* Initialize Iq PI controller */
    Ifx_MDA_FocControllerF16_initQPi(self);

    /* Initialize d-q decoupling */
#if IFX_MDA_FOCCONTROLLERF16_CFG_DQDECOUPLINGENABLE
    Ifx_Math_DqDecouplingF16_init(&(self->dqDecoupling));
    Ifx_Math_DqDecouplingF16_setInductanceD(&(self->dqDecoupling),
        IFX_MDA_FOCCONTROLLERF16_CFG_DIRECT_INDUCTANCE_Q15);
    Ifx_Math_DqDecouplingF16_setInductanceQ(&(self->dqDecoupling),
        IFX_MDA_FOCCONTROLLERF16_CFG_QUADRATURE_INDUCTANCE_Q15);
#endif /* IFX_MDA_FOCCONTROLLERF16_CFG_DQDECOUPLINGENABLE */
}


/**
 *  Execute the current regulation and output the voltage command.
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *  \param [in] currentAlphaBeta Measured current, in alpha-beta reference frame
 *  \param [in] dqCommand Current command, in d-q reference frame
 *  \param [in] rotorFluxAngle Rotor flux angle
 *
 */

/* polyspace +2 CODE-METRIC:PARAM [Justified:Medium] "Increased complexity is justified to keep the functionality of
 * the component the same." */
__USED void Ifx_MDA_FocControllerF16_execute(Ifx_MDA_FocControllerF16* self, Ifx_Math_CmpFract16 currentAlphaBeta,
                                             Ifx_Math_CmpFract16 dqCommand, uint32 rotorFluxAngle, Ifx_Math_Fract16
                                             electricalSpeed, Ifx_Math_Fract16
                                             dcLinkVoltageQ15)
{
    /* Variable declaration */
    Ifx_Math_CmpFract16 errorCurrentDQ;

    /* Current Park trans. (alpha-beta to d-q) */
    self->currentDQ = Ifx_Math_Park_F16(currentAlphaBeta, rotorFluxAngle);

    /* Calculate current errors */
    errorCurrentDQ.real = Ifx_Math_Sub_F16(Ifx_Math_ShR_F16(dqCommand.real, 1), Ifx_Math_ShR_F16(self->currentDQ.real,
        1));
    errorCurrentDQ.imag = Ifx_Math_Sub_F16(Ifx_Math_ShR_F16(dqCommand.imag, 1), Ifx_Math_ShR_F16(self->currentDQ.imag,
        1));

    /* Execute current PI controllers */
    self->voltageDQ.real = Ifx_Math_PiF16_execute(&(self->currentDPi), errorCurrentDQ.real, self->p_dAntiwindupCtrl);
    self->voltageDQ.imag = Ifx_Math_PiF16_execute(&(self->currentQPi), errorCurrentDQ.imag, self->p_qAntiwindupCtrl);

    /* Execute d-q decoupling */
#if IFX_MDA_FOCCONTROLLERF16_CFG_DQDECOUPLINGENABLE
    Ifx_MDA_FocControllerF16_dqDecoupling(self, electricalSpeed);
#endif /* IFX_MDA_FOCCONTROLLERF16_CFG_DQDECOUPLINGENABLE */

#if IFX_MDA_FOCCONTROLLERF16_CFG_LIMIT_VOLT_VECTOR_D_PRIO

    /* Limit calculated D and Q voltages according to the available DC link voltage with a D component prioritization */
    Ifx_MDA_FocControllerF16_limitDQVoltage(self, dcLinkVoltageQ15);
#endif /* IFX_MDA_FOCCONTROLLERF16_CFG_LIMIT_VOLT_VECTOR_D_PRIO */
    /* Voltage cartesian to polar (d-q to angle-amp) */
    self->p_output.voltageCommandPolar = Ifx_Math_CartToPolar_F16(self->voltageDQ);

    /* Add rotor flux angle to convert from d-q to alpha-beta */
    self->p_output.voltageCommandPolar.angle += rotorFluxAngle;
}


void Ifx_MDA_FocControllerF16_reset(Ifx_MDA_FocControllerF16* self)
{
    /* Reset intermediate variables and outputs */
    self->currentDQ.real                         = 0;
    self->currentDQ.imag                         = 0;
    self->voltageDQ.real                         = 0;
    self->voltageDQ.imag                         = 0;
    self->p_output.voltageCommandPolar.amplitude = 0;
    self->p_output.voltageCommandPolar.angle     = 0;

    /* Reset PI controllers previous values */
    Ifx_Math_PiF16_setIntegPreviousValue(&(self->currentDPi), 0);
    Ifx_Math_PiF16_setIntegPreviousValue(&(self->currentQPi), 0);
}


/* Functions called by Ifx_MDA_FocControllerF16_init() */
static inline void Ifx_MDA_FocControllerF16_initDPi(Ifx_MDA_FocControllerF16* self)
{
    /* Set Q formats */
    Ifx_Math_PiF16_Qformats currentDPiQForm;
    currentDPiQForm.qFormatPropGain                   =
        (Ifx_Math_FractQFormat)IFX_MDA_FOCCONTROLLERF16_CFG_ID_PI_PROPGAIN_Q_FORMAT;
    currentDPiQForm.qFormatIntegGainSamplingTime      =
        (Ifx_Math_FractQFormat)IFX_MDA_FOCCONTROLLERF16_CFG_ID_PI_KI_TS_Q_FORMAT;
    currentDPiQForm.qFormatAntiWindupGainSamplingTime =
        (Ifx_Math_FractQFormat)IFX_MDA_FOCCONTROLLERF16_CFG_ID_PI_KAW_TS_Q_FORMAT;
    currentDPiQForm.qFormatOutput                     =
        (Ifx_Math_FractQFormat)IFX_MDA_FOCCONTROLLERF16_CFG_ID_PI_LIMIT_Q_FORMAT;
    currentDPiQForm.qFormatError                      = Ifx_Math_FractQFormat_q14;

    /* Call init */
    Ifx_Math_PiF16_init(&(self->currentDPi), currentDPiQForm);

    /* Call setters */
    Ifx_Math_PiF16_setPropGain(&(self->currentDPi), IFX_MDA_FOCCONTROLLERF16_CFG_ID_PI_PROPGAIN_Q);
    Ifx_Math_PiF16_setIntegGainSamplingTime(&(self->currentDPi), IFX_MDA_FOCCONTROLLERF16_CFG_ID_PI_KI_TS_Q);
    Ifx_Math_PiF16_setAntiWindupGainSamplingTime(&(self->currentDPi), IFX_MDA_FOCCONTROLLERF16_CFG_ID_PI_KAW_TS_Q);
    Ifx_Math_PiF16_setUpperLimit(&(self->currentDPi), IFX_MDA_FOCCONTROLLERF16_CFG_ID_PI_OUT_UPP_LIMIT_Q);
    Ifx_Math_PiF16_setLowerLimit(&(self->currentDPi), IFX_MDA_FOCCONTROLLERF16_CFG_ID_PI_OUT_LOW_LIMIT_Q);

    /* Set antiwindup flag to inactive */
    self->p_dAntiwindupCtrl = IFX_MDA_FOCCONTROLLER_FLAG_AW_NO;
}


static inline void Ifx_MDA_FocControllerF16_initQPi(Ifx_MDA_FocControllerF16* self)
{
    /* Set Q formats */
    Ifx_Math_PiF16_Qformats currentQPiQForm;
    currentQPiQForm.qFormatPropGain                   =
        (Ifx_Math_FractQFormat)IFX_MDA_FOCCONTROLLERF16_CFG_IQ_PI_PROPGAIN_Q_FORMAT;
    currentQPiQForm.qFormatIntegGainSamplingTime      =
        (Ifx_Math_FractQFormat)IFX_MDA_FOCCONTROLLERF16_CFG_IQ_PI_KI_TS_Q_FORMAT;
    currentQPiQForm.qFormatAntiWindupGainSamplingTime =
        (Ifx_Math_FractQFormat)IFX_MDA_FOCCONTROLLERF16_CFG_IQ_PI_KAW_TS_Q_FORMAT;
    currentQPiQForm.qFormatOutput                     =
        (Ifx_Math_FractQFormat)IFX_MDA_FOCCONTROLLERF16_CFG_IQ_PI_LIMIT_Q_FORMAT;
    currentQPiQForm.qFormatError                      = Ifx_Math_FractQFormat_q14;

    /* Call init */
    Ifx_Math_PiF16_init(&(self->currentQPi), currentQPiQForm);

    /* Call setters */
    Ifx_Math_PiF16_setPropGain(&(self->currentQPi), IFX_MDA_FOCCONTROLLERF16_CFG_IQ_PI_PROPGAIN_Q);
    Ifx_Math_PiF16_setIntegGainSamplingTime(&(self->currentQPi), IFX_MDA_FOCCONTROLLERF16_CFG_IQ_PI_KI_TS_Q);
    Ifx_Math_PiF16_setAntiWindupGainSamplingTime(&(self->currentQPi), IFX_MDA_FOCCONTROLLERF16_CFG_IQ_PI_KAW_TS_Q);
    Ifx_Math_PiF16_setUpperLimit(&(self->currentQPi), IFX_MDA_FOCCONTROLLERF16_CFG_IQ_PI_OUT_UPP_LIMIT_Q);
    Ifx_Math_PiF16_setLowerLimit(&(self->currentQPi), IFX_MDA_FOCCONTROLLERF16_CFG_IQ_PI_OUT_LOW_LIMIT_Q);

    /* Set antiwindup flag to inactive */
    self->p_qAntiwindupCtrl = IFX_MDA_FOCCONTROLLER_FLAG_AW_NO;
}


#if IFX_MDA_FOCCONTROLLERF16_CFG_DQDECOUPLINGENABLE

/* Functions called by Ifx_MDA_FocControllerF16_execute() */
static inline void Ifx_MDA_FocControllerF16_dqDecoupling(Ifx_MDA_FocControllerF16* self, Ifx_Math_Fract16
                                                         electricalSpeed)
{
    /* Calculate and apply d-q decoupling */
    Ifx_Math_CmpFract16 compensationVoltageDQ = Ifx_Math_DqDecouplingF16_execute(&(self->dqDecoupling),
        self->currentDQ, electricalSpeed);
    self->voltageDQ.real = Ifx_Math_SubSat_F16(self->voltageDQ.real, compensationVoltageDQ.real);
    self->voltageDQ.imag = Ifx_Math_AddSat_F16(self->voltageDQ.imag, compensationVoltageDQ.imag);
}


#endif /* IFX_MDA_FOCCONTROLLERF16_CFG_DQDECOUPLINGENABLE */

#if IFX_MDA_FOCCONTROLLERF16_CFG_LIMIT_VOLT_VECTOR_D_PRIO
static inline void Ifx_MDA_FocControllerF16_limitDQVoltage(Ifx_MDA_FocControllerF16* self, Ifx_Math_Fract16
                                                           dcLinkVoltageQ15)
{
    /* Calculate maximum modulation amplitude based on actual DC-Link voltage  */
    Ifx_Math_Fract16 maxAmpQ15 = Ifx_Math_MulSat_F16(dcLinkVoltageQ15, IFX_MDA_FOCCONTROLLERF16_CONST_MI_0907_Q15);

    /* Limit D voltage to max. amplitude */
    Ifx_MDA_FocControllerF16_limitDVoltage(self, maxAmpQ15);

    /* Limit Q voltage to the remaining available voltage vector */
    Ifx_MDA_FocControllerF16_limitQVoltage(self, maxAmpQ15);
}


static inline void Ifx_MDA_FocControllerF16_limitDVoltage(Ifx_MDA_FocControllerF16* self, Ifx_Math_Fract16 maxAmpQ15)
{
    if (Ifx_Math_Abs_F16(self->voltageDQ.real) >= maxAmpQ15)
    {
        /* Check D voltage sign */
        if (self->voltageDQ.real >= 0)
        {
            /* Set D antiwindup flag to positive saturation */
            self->p_dAntiwindupCtrl = IFX_MDA_FOCCONTROLLER_FLAG_AW_POS;

            /* Limit positive D */
            self->voltageDQ.real = maxAmpQ15;
        }
        else
        {
            /* Set D antiwindup flag to negative saturation */
            self->p_dAntiwindupCtrl = IFX_MDA_FOCCONTROLLER_FLAG_AW_NEG;

            /* Limit negative D */
            self->voltageDQ.real = Ifx_Math_Neg_F16(maxAmpQ15);
        }
    }
    else
    {
        /* Set D antiwindup flag to no saturation */
        self->p_dAntiwindupCtrl = IFX_MDA_FOCCONTROLLER_FLAG_AW_NO;
    }
}


static inline void Ifx_MDA_FocControllerF16_limitQVoltage(Ifx_MDA_FocControllerF16* self, Ifx_Math_Fract16 maxAmpQ15)
{
    /* Calculate square of maximum modulation amplitude */
    Ifx_Math_Fract16 maxAmpSqrQ15 = Ifx_Math_MulShR_F16(maxAmpQ15, maxAmpQ15, 15u);

    /* ud^2 = ud*ud */
    Ifx_Math_Fract16 realVoltSqr = Ifx_Math_MulShR_F16(self->voltageDQ.real, self->voltageDQ.real, 15u);

    /* uq^2 = ulim^2 -ud^2 */
    Ifx_Math_Fract16 imagVoltSqr = Ifx_Math_Sub_F16(maxAmpSqrQ15, realVoltSqr);

    /* uq = sqrt(uq^2) */
    Ifx_Math_Fract16 imagVoltAbs = Ifx_Math_Interp1DLut_F16(imagVoltSqr, focControllerInterp1DLut,
        IFX_MDA_FOCCONTROLLERF16_SHIFT);

    /* Limit Q voltage to sqrt(Umax^2-ud^2) */
    if (Ifx_Math_Abs_F16(self->voltageDQ.imag) >= imagVoltAbs)
    {
        /* Check Q voltage sign */
        if (self->voltageDQ.imag >= 0)
        {
            /* Set Q antiwindup flag to positive saturation */
            self->p_qAntiwindupCtrl = IFX_MDA_FOCCONTROLLER_FLAG_AW_POS;

            /* Limit positive Q */
            self->voltageDQ.imag = imagVoltAbs;
        }
        else
        {
            /* Set Q antiwindup flag to negative saturation */
            self->p_qAntiwindupCtrl = IFX_MDA_FOCCONTROLLER_FLAG_AW_NEG;

            /* Limit negative Q */
            self->voltageDQ.imag = Ifx_Math_Neg_F16(imagVoltAbs);
        }
    }
    else
    {
        /* Set Q antiwindup flag to no saturation */
        self->p_qAntiwindupCtrl = IFX_MDA_FOCCONTROLLER_FLAG_AW_NO;
    }
}


#endif /* IFX_MDA_FOCCONTROLLERF16_CFG_LIMIT_VOLT_VECTOR_D_PRIO */

/* polyspace-end MISRA2012:D4.14 [Justified:Low] "The caller function has to guarantee that NULL is not passed as
 * argument." */
