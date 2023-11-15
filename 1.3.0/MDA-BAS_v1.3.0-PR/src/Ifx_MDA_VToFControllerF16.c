/*
 * Copyright (c) 2022 Infineon Technologies AG. All Rights Reserved.
 *
 * Use of this file is subject to the terms of use agreed between (i) you or the company in which ordinary course of
 * business you are acting and (ii) Infineon Technologies AG, its affiliates or its licensees. If and as long as no
 * such terms of use are agreed, use of this file is subject to the Evaluation Software License Agreement distributed
 * along with this file within the software delivery package.
 *
 */

#include "Ifx_MDA_VToFControllerF16.h"
#include "Ifx_Math_Arithmetic.h"
#include "Ifx_MDA_VToFControllerF16_Cfg.h"

/* Macros to define the component ID */
#define IFX_MDA_VTOFCONTROLLER_COMPONENTID_SOURCEID     ((uint8)Ifx_ComponentID_SourceID_infineonTechnologiesAG)
#define IFX_MDA_VTOFCONTROLLER_COMPONENTID_LIBRARYID    ((uint16)Ifx_ComponentID_LibraryID_mctrlDriveAlgorithm)
#define IFX_MDA_VTOFCONTROLLER_COMPONENTID_MODULEID     (4U)
#define IFX_MDA_VTOFCONTROLLER_COMPONENTID_COMPONENTID1 (1U)

#define IFX_MDA_VTOFCONTROLLER_COMPONENTID_COMPONENTID2 ((uint8)Ifx_ComponentID_ComponentID2_basic)

/* Macros to define the component version */
#define IFX_MDA_VTOFCONTROLLER_COMPONENTVERSION_MAJOR   (1U)
#define IFX_MDA_VTOFCONTROLLER_COMPONENTVERSION_MINOR   (3U)
#define IFX_MDA_VTOFCONTROLLER_COMPONENTVERSION_PATCH   (0U)
#define IFX_MDA_VTOFCONTROLLER_COMPONENTVERSION_T       (0U)
#define IFX_MDA_VTOFCONTROLLER_COMPONENTVERSION_REV     (0U)

/* *INDENT-OFF* */
/* Component ID */
static const Ifx_ComponentID      Ifx_MDA_VToFController_componentID = {
    .sourceID     = IFX_MDA_VTOFCONTROLLER_COMPONENTID_SOURCEID,
    .libraryID    = IFX_MDA_VTOFCONTROLLER_COMPONENTID_LIBRARYID,
    .moduleID     = IFX_MDA_VTOFCONTROLLER_COMPONENTID_MODULEID,
    .componentID1 = IFX_MDA_VTOFCONTROLLER_COMPONENTID_COMPONENTID1,
    .componentID2 = IFX_MDA_VTOFCONTROLLER_COMPONENTID_COMPONENTID2,
};

/* Component Version */
static const Ifx_ComponentVersion Ifx_MDA_VToFController_componentVersion = {
    .major = IFX_MDA_VTOFCONTROLLER_COMPONENTVERSION_MAJOR,
    .minor = IFX_MDA_VTOFCONTROLLER_COMPONENTVERSION_MINOR,
    .patch = IFX_MDA_VTOFCONTROLLER_COMPONENTVERSION_PATCH,
    .t     = IFX_MDA_VTOFCONTROLLER_COMPONENTVERSION_T,
    .rev   = IFX_MDA_VTOFCONTROLLER_COMPONENTVERSION_REV
};
/* *INDENT-ON* */

/* polyspace-begin MISRA2012:D4.14 [Justified:Low] "The caller function has to guarantee that NULL is not passed as
 * argument." */

/* API to convert speed to voltage amplitude */
static inline Ifx_Math_Fract16 Ifx_MDA_VToFControllerF16_LUT(Ifx_MDA_VToFControllerF16_VToFLutNorm* vTofLutNorm,
                                                             Ifx_Math_Fract16                       speedQ15);

/* API to interpolate between two points in a LUT */
static inline Ifx_Math_Fract16 Ifx_MDA_VToFControllerF16_interp(Ifx_Math_Fract16 absc1, Ifx_Math_Fract16 absc2,
                                                                Ifx_Math_Fract16 ord1, Ifx_Math_Fract16 ord2,
                                                                Ifx_Math_Fract16 abscIn);

/* API to generate the electrical angle output */
static inline uint32 Ifx_MDA_VToFControllerF16_angleCalc(Ifx_MDA_VToFControllerF16* self, Ifx_Math_Fract16 speedQ15);

/* Function to get the component ID */
void Ifx_MDA_VToFControllerF16_getID(const Ifx_ComponentID** componentID)
{
    *componentID = &Ifx_MDA_VToFController_componentID;
}


/* Function to get the component version */
void Ifx_MDA_VToFControllerF16_getVersion(const Ifx_ComponentVersion** componentVersion)
{
    *componentVersion = &Ifx_MDA_VToFController_componentVersion;
}


/* Initialize parameters of the module */
void Ifx_MDA_VToFControllerF16_init(Ifx_MDA_VToFControllerF16* self)
{
    /* Set the parameters in the normalized table */
    /* Rated Speed */
    self->p_VToFLUTnorm.p_ratedSpeedQ15 = IFX_MDA_VTOFCONTROLLERF16_CFG_RATED_SPEED;

    /* Corner speed */
    self->p_VToFLUTnorm.p_cornerSpeedQ15 = IFX_MDA_VTOFCONTROLLERF16_CFG_CORNER_SPEED;

    /* Voltage at rated speed */
    self->p_VToFLUTnorm.p_ratedVoltQ15 = IFX_MDA_VTOFCONTROLLERF16_CFG_V2RATED_SPEED;

    /* Voltage at corner speed */
    self->p_VToFLUTnorm.p_cornerVoltQ15 = IFX_MDA_VTOFCONTROLLERF16_CFG_V2CORNER_SPEED;

    /* Voltage at min. speed */
    self->p_VToFLUTnorm.p_minVoltQ15 = IFX_MDA_VTOFCONTROLLERF16_CFG_V2MIN_SPEED;

    /* Set the angle increment factor */
    self->p_angleIncrementQ14 = (Ifx_Math_Fract16)IFX_MDA_VTOFCONTROLLERF16_CFG_ANGLE_INC_Q14;

    /* Initialize the angle state */
    self->p_anglePreviousValue = 0;

    /* Initialize amplitude of the output voltage vector */
    self->p_output.voltageVector.amplitude = 0;

    /* Initialize angle of the output voltage vector */
    self->p_output.voltageVector.angle = 0;
}


/* Calculate the amplitude and angle of the voltage vector */
void Ifx_MDA_VToFControllerF16_execute(Ifx_MDA_VToFControllerF16* self, Ifx_Math_Fract16 speedQ15)
{
    /* Operations: */
    /* Interpolate to get the voltage amplitude */
    self->p_output.voltageVector.amplitude = Ifx_MDA_VToFControllerF16_LUT(&self->p_VToFLUTnorm, Ifx_Math_Abs_F16(
        speedQ15));

    /* Integrate the rate limited input speed */
    self->p_output.voltageVector.angle = Ifx_MDA_VToFControllerF16_angleCalc(self, speedQ15);
}


static inline Ifx_Math_Fract16 Ifx_MDA_VToFControllerF16_LUT(Ifx_MDA_VToFControllerF16_VToFLutNorm* vTofLutNorm,
                                                             Ifx_Math_Fract16                       speedQ15)
{
    /* Local variable to store temporary result */
    Ifx_Math_Fract16 voltVectAmpN;

    /* Check in which range is the input speed */
    if (speedQ15 > vTofLutNorm->p_cornerSpeedQ15)
    {
        /* Interpolate in the second range */
        voltVectAmpN = Ifx_MDA_VToFControllerF16_interp(vTofLutNorm->p_cornerSpeedQ15, vTofLutNorm->p_ratedSpeedQ15,
            vTofLutNorm->p_cornerVoltQ15, vTofLutNorm->p_ratedVoltQ15, speedQ15);
    }
    else
    {
        /* Interpolate in the first range */
        voltVectAmpN = Ifx_MDA_VToFControllerF16_interp(0, vTofLutNorm->p_cornerSpeedQ15, vTofLutNorm->p_minVoltQ15,
            vTofLutNorm->p_cornerVoltQ15, speedQ15);
    }

    return voltVectAmpN;
}


static inline Ifx_Math_Fract16 Ifx_MDA_VToFControllerF16_interp(Ifx_Math_Fract16 absc1, Ifx_Math_Fract16 absc2,
                                                                Ifx_Math_Fract16 ord1, Ifx_Math_Fract16 ord2,
                                                                Ifx_Math_Fract16 abscIn)
{
    /* Local variable to store delta x */
    Ifx_Math_Fract16 deltaX;

    /* Local variable to store delta y */
    Ifx_Math_Fract16 deltaY;

    /* Local variable to store x - x1 */
    Ifx_Math_Fract16 deltaIn;

    /* Local variable to store the result */
    Ifx_Math_Fract16 result;

    /* Local variable to store the result of the division */
    Ifx_Math_Fract16 deltaYX;
    deltaX  = Ifx_Math_Sub_F16(absc2, absc1);
    deltaY  = Ifx_Math_Sub_F16(ord2, ord1);
    deltaIn = Ifx_Math_Sub_F16(abscIn, absc1);

    /* del_in/del_X, Q15, using DivSat limits the output to rated voltage */
    deltaYX = Ifx_Math_DivSat_F16(deltaIn, deltaX);

    /* Interpolated voltage, Q15 */
    result = Ifx_Math_Add_F16(Ifx_Math_Mul_F16(deltaYX, deltaY), ord1);

    return result;
}


static inline uint32 Ifx_MDA_VToFControllerF16_angleCalc(Ifx_MDA_VToFControllerF16* self, Ifx_Math_Fract16 speedQ15)
{
    /* Local variables for electrical angle change, electrical frequency and temporary Angle */
    Ifx_Math_Fract32 electricalAngleDelta;

    /* Calculate the delTheta = normalized speed x normalized factor */
    electricalAngleDelta = Ifx_Math_MulShL_F32_F16F16(speedQ15, self->p_angleIncrementQ14, 3u);

    /* Total electrical angle after change */
    if (electricalAngleDelta >= 0)
    {
        self->p_anglePreviousValue = self->p_anglePreviousValue + (uint32)electricalAngleDelta;
    }
    else
    {
        self->p_anglePreviousValue = self->p_anglePreviousValue - (uint32)(Ifx_Math_Neg_F32(electricalAngleDelta));
    }

    /* Calculate the output */
    return self->p_anglePreviousValue;
}


/* polyspace-end MISRA2012:D4.14 [Justified:Low] "The caller function has to guarantee that NULL is not passed as
 * argument." */
