/*
 * Copyright (c) 2022 Infineon Technologies AG. All Rights Reserved.
 *
 * Use of this file is subject to the terms of use agreed between (i) you or the company in which ordinary course of
 * business you are acting and (ii) Infineon Technologies AG, its affiliates or its licensees. If and as long as no
 * such terms of use are agreed, use of this file is subject to the Evaluation Software License Agreement distributed
 * along with this file within the software delivery package.
 *
 */

#include "Ifx_MDA_IToFControllerF16.h"
#include "Ifx_Math_Arithmetic.h"
#include "Ifx_MDA_IToFControllerF16_Cfg.h"

/* Macros to define the component ID */
#define IFX_MDA_ITOFCONTROLLER_COMPONENTID_SOURCEID     ((uint8)Ifx_ComponentID_SourceID_infineonTechnologiesAG)
#define IFX_MDA_ITOFCONTROLLER_COMPONENTID_LIBRARYID    ((uint16)Ifx_ComponentID_LibraryID_mctrlDriveAlgorithm)
#define IFX_MDA_ITOFCONTROLLER_COMPONENTID_MODULEID     (2U)
#define IFX_MDA_ITOFCONTROLLER_COMPONENTID_COMPONENTID1 (1U)

#define IFX_MDA_ITOFCONTROLLER_COMPONENTID_COMPONENTID2 ((uint8)Ifx_ComponentID_ComponentID2_basic)

/* Macros to define the component version */
#define IFX_MDA_ITOFCONTROLLER_COMPONENTVERSION_MAJOR   (1U)
#define IFX_MDA_ITOFCONTROLLER_COMPONENTVERSION_MINOR   (3U)
#define IFX_MDA_ITOFCONTROLLER_COMPONENTVERSION_PATCH   (0U)
#define IFX_MDA_ITOFCONTROLLER_COMPONENTVERSION_T       (0U)
#define IFX_MDA_ITOFCONTROLLER_COMPONENTVERSION_REV     (0U)

/* *INDENT-OFF* */
/* Component ID */
static const Ifx_ComponentID      Ifx_MDA_IToFController_componentID = {
    .sourceID     = IFX_MDA_ITOFCONTROLLER_COMPONENTID_SOURCEID,
    .libraryID    = IFX_MDA_ITOFCONTROLLER_COMPONENTID_LIBRARYID,
    .moduleID     = IFX_MDA_ITOFCONTROLLER_COMPONENTID_MODULEID,
    .componentID1 = IFX_MDA_ITOFCONTROLLER_COMPONENTID_COMPONENTID1,
    .componentID2 = IFX_MDA_ITOFCONTROLLER_COMPONENTID_COMPONENTID2,
};

/* Component Version */
static const Ifx_ComponentVersion Ifx_MDA_IToFController_componentVersion = {
    .major = IFX_MDA_ITOFCONTROLLER_COMPONENTVERSION_MAJOR,
    .minor = IFX_MDA_ITOFCONTROLLER_COMPONENTVERSION_MINOR,
    .patch = IFX_MDA_ITOFCONTROLLER_COMPONENTVERSION_PATCH,
    .t     = IFX_MDA_ITOFCONTROLLER_COMPONENTVERSION_T,
    .rev   = IFX_MDA_ITOFCONTROLLER_COMPONENTVERSION_REV
};
/* *INDENT-ON* */

/* polyspace-begin MISRA2012:D4.14 [Justified:Low] "The caller function has to guarantee that NULL is not passed as
 * argument." */

/* API to generate the electrical angle output */
static inline uint32 Ifx_MDA_IToFControllerF16_angleCalc(Ifx_MDA_IToFControllerF16* self, Ifx_Math_Fract16 speedQ15);

/* Function to get the component ID */
void Ifx_MDA_IToFControllerF16_getID(const Ifx_ComponentID** componentID)
{
    *componentID = &Ifx_MDA_IToFController_componentID;
}


/* Function to get the component version */
void Ifx_MDA_IToFControllerF16_getVersion(const Ifx_ComponentVersion** componentVersion)
{
    *componentVersion = &Ifx_MDA_IToFController_componentVersion;
}


/* Initialize parameters of the module */
void Ifx_MDA_IToFControllerF16_init(Ifx_MDA_IToFControllerF16* self)
{
    /* Set the angle multiplication factor
     * Calculate as floor(sampling_time*BaseSpeed/(2*PI)*2^14)
     * */
    self->p_angleIncrementQ14 = IFX_MDA_ITOFCONTROLLERF16_CFG_ANGLE_INC_Q14;

    /* Initialize the angle state */
    self->p_anglePreviousValue = 0;

    /* Initialize reference currents */
    self->p_output.refCurrent.real = IFX_MDA_ITOFCONTROLLERF16_CFG_REF_CURRENT_REAL_N;
    Ifx_MDA_IToFControllerF16_setReferenceCurrentImag(self, IFX_MDA_ITOFCONTROLLERF16_CFG_REF_CURRENT_IMAG_N);
}


/* Adds 180 degree in the current value of angle*/
void Ifx_MDA_IToFControllerF16_addOneEightyDegreeInAngle(Ifx_MDA_IToFControllerF16* self)
{
    Ifx_MDA_IToFControllerF16_setAnglePreviousValue(self, self->p_output.currentVecAngle_rad + IFX_MATH_PI_INDEX);
}


/* Calculate the angle of the current vector */
void Ifx_MDA_IToFControllerF16_execute(Ifx_MDA_IToFControllerF16* self, Ifx_Math_Fract16 speedQ15)
{
    /* Operations: */
    /* Integrate the rate limited input speed */
    self->p_output.currentVecAngle_rad = Ifx_MDA_IToFControllerF16_angleCalc(self, speedQ15);
}


static inline uint32 Ifx_MDA_IToFControllerF16_angleCalc(Ifx_MDA_IToFControllerF16* self, Ifx_Math_Fract16 speedQ15)
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
