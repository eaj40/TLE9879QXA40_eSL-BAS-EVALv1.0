/*
 * Copyright (c) 2022 Infineon Technologies AG. All Rights Reserved.
 *
 * Use of this file is subject to the terms of use agreed between (i) you or the company in which ordinary course of
 * business you are acting and (ii) Infineon Technologies AG, its affiliates or its licensees. If and as long as no
 * such terms of use are agreed, use of this file is subject to the Evaluation Software License Agreement distributed
 * along with this file within the software delivery package.
 *
 */

/* Includes from math library*/
#include "Ifx_Math_Add.h"
#include "Ifx_Math_MulShR.h"
#include "Ifx_Math_MulShRSat.h"
#include "Ifx_Math_SubSat.h"
#include "Ifx_Math_AddSat.h"

#include "Ifx_Math_SpeedPreControlF16.h"

/* polyspace-begin MISRA2012:D4.14 [Justified:Low] "The caller function has to guarantee that NULL is not passed as
 * argument." */
void Ifx_Math_SpeedPreControlF16_init(Ifx_Math_SpeedPreControlF16* self, Ifx_Math_SpeedPreControlF16_StaticConfig
                                      staticConfiguration)
{
    self->p_staticConfiguration     = staticConfiguration;
    self->p_inverseTorqueConstant   = 0;
    self->p_previousSpeed           = 0;
    self->p_frictionalTorqueQformat = (Ifx_Math_FractQFormat)Ifx_Math_MulShR_ShiftMul(Ifx_Math_FractQFormat_q15,
        Ifx_Math_FractQFormat_q15, self->p_staticConfiguration.rotorInertiaOverSamplingTime.qFormat);
}


Ifx_Math_Fract16 Ifx_Math_SpeedPreControlF16_execute(Ifx_Math_SpeedPreControlF16* self, Ifx_Math_Fract16 speed)
{
    /* Variables  to store q current, various torques, sampling time and some intermediate variables */
    Ifx_Math_Fract16 qCurrent;
    Ifx_Math_Fract16 frictionalTorque;
    Ifx_Math_Fract16 jDomegaDt;
    Ifx_Math_Fract16 inducedTorque;
    Ifx_Math_Fract16 deltaOmega;
    deltaOmega = Ifx_Math_SubSat_F16(speed, self->p_previousSpeed);

    /* J *dw/dt and shift factor is (Q15+Qx-Qx) */
    jDomegaDt = Ifx_Math_MulShRSat_F16(deltaOmega, self->p_staticConfiguration.rotorInertiaOverSamplingTime.value,
        (uint8)Ifx_Math_FractQFormat_q15);

    /* Frictional torque */
    frictionalTorque = Ifx_Math_MulShRSat_F16(self->p_staticConfiguration.fricitionConstant, speed,
        (uint8)self->p_frictionalTorqueQformat);

    /* Induced torque */
    inducedTorque = Ifx_Math_AddSat_F16(frictionalTorque, jDomegaDt);

    /* shift factor is (Qx+Q15-Q15) */
    qCurrent              = Ifx_Math_MulShRSat_F16(inducedTorque, self->p_inverseTorqueConstant,
        (uint8)self->p_staticConfiguration.rotorInertiaOverSamplingTime.qFormat);
    self->p_previousSpeed = speed;

    return qCurrent;
}


/* polyspace-end MISRA2012:D4.14 [Justified:Low] "The caller function has to guarantee that NULL is not passed as
 * argument." */
