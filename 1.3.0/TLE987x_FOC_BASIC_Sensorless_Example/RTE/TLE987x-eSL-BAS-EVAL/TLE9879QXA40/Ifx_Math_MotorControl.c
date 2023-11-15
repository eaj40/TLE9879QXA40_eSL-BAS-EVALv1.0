/*
 * Copyright (c) 2022 Infineon Technologies AG. All Rights Reserved.
 *
 * Use of this file is subject to the terms of use agreed between (i) you or the company in which ordinary course of
 * business you are acting and (ii) Infineon Technologies AG, its affiliates or its licensees. If and as long as no
 * such terms of use are agreed, use of this file is subject to the Evaluation Software License Agreement distributed
 * along with this file within the software delivery package.
 *
 */

#include "Ifx_Math_MotorControl.h"

/* Macros to define the component ID */
#define IFX_MATH_MOTORCONTROL_COMPONENTID_SOURCEID     ((uint8)Ifx_ComponentID_SourceID_infineonTechnologiesAG)
#define IFX_MATH_MOTORCONTROL_COMPONENTID_LIBRARYID    ((uint16)Ifx_ComponentID_LibraryID_math)
#define IFX_MATH_MOTORCONTROL_COMPONENTID_MODULEID     (3U)
#define IFX_MATH_MOTORCONTROL_COMPONENTID_COMPONENTID1 (1U)

#define IFX_MATH_MOTORCONTROL_COMPONENTID_COMPONENTID2 ((uint8)Ifx_ComponentID_ComponentID2_basic)

/* Macros to define the component version */
#define IFX_MATH_MOTORCONTROL_COMPONENTVERSION_MAJOR   (1U)
#define IFX_MATH_MOTORCONTROL_COMPONENTVERSION_MINOR   (3U)
#define IFX_MATH_MOTORCONTROL_COMPONENTVERSION_PATCH   (0U)
#define IFX_MATH_MOTORCONTROL_COMPONENTVERSION_T       (0U)
#define IFX_MATH_MOTORCONTROL_COMPONENTVERSION_REV     (0U)

/* Component ID */
static const Ifx_ComponentID      Ifx_Math_MotorControl_componentID = {
    .sourceID                                                         = IFX_MATH_MOTORCONTROL_COMPONENTID_SOURCEID,
    .libraryID                                                        = IFX_MATH_MOTORCONTROL_COMPONENTID_LIBRARYID,
    .moduleID                                                         = IFX_MATH_MOTORCONTROL_COMPONENTID_MODULEID,
    .componentID1                                                     =
        IFX_MATH_MOTORCONTROL_COMPONENTID_COMPONENTID1, .componentID2 =
        IFX_MATH_MOTORCONTROL_COMPONENTID_COMPONENTID2,
};

/* Component Version */
static const Ifx_ComponentVersion Ifx_Math_MotorControl_componentVersion = {
    .major = IFX_MATH_MOTORCONTROL_COMPONENTVERSION_MAJOR, .minor = IFX_MATH_MOTORCONTROL_COMPONENTVERSION_MINOR,
    .patch = IFX_MATH_MOTORCONTROL_COMPONENTVERSION_PATCH, .t = IFX_MATH_MOTORCONTROL_COMPONENTVERSION_T, .rev =
        IFX_MATH_MOTORCONTROL_COMPONENTVERSION_REV
};

/* polyspace-begin MISRA-C3:D4.14 [Justified:Low] "The caller function has to guarantee that NULL is not passed as
 * argument." */

/* Function to get the component ID */
void Ifx_Math_MotorControl_getID(const Ifx_ComponentID** componentID)
{
    *componentID = &Ifx_Math_MotorControl_componentID;
}


/* Function to get the component version */
void Ifx_Math_MotorControl_getVersion(const Ifx_ComponentVersion** componentVersion)
{
    *componentVersion = &Ifx_Math_MotorControl_componentVersion;
}


/* polyspace-end MISRA-C3:D4.14 [Justified:Low] "The caller function has to guarantee that NULL is not passed as
 * argument." */
