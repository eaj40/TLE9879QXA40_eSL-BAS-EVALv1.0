/*
 * Copyright (c) 2022 Infineon Technologies AG. All Rights Reserved.
 *
 * Use of this file is subject to the terms of use agreed between (i) you or the company in which ordinary course of
 * business you are acting and (ii) Infineon Technologies AG, its affiliates or its licensees. If and as long as no
 * such terms of use are agreed, use of this file is subject to the Evaluation Software License Agreement distributed
 * along with this file within the software delivery package.
 *
 */

#include "Ifx_Math_Atan2.h"
#include "Ifx_Math_Atan.h"
#include "Ifx_Math_ShL.h"

/* polyspace-begin CODE-METRIC:VOCF [Justified:Low] "Not possible to achieve the threshold limit for the code metrics
 * without violating MISRA rules or other code metrics." */
uint32 Ifx_Math_Atan2_F16(Ifx_Math_Fract16 y, Ifx_Math_Fract16 x)
{
    /* Local variables to store the tangent and angle */
    Ifx_Math_Fract32 tangent;
    uint32           angle;

    /* Divide unit circle in four partitions */
    if (y < -x)
    {
        if (x > y)                                               /* Partition 4: 5*Pi/4..7*Pi/4 */
        {
            angle   = IFX_MATH_PI_INDEX + IFX_MATH_PI_INDEX / 2; /* Set pre-angle of 3/2*Pi */
            tangent = -(Ifx_Math_ShL_F32(x, 15)) / y;            /* Calculate tangent of the angle, stretched by y and
                                                                  * y+x (x and y are interpreted as vectors) */
        }
        else                                                     /* Partition 3: 3*Pi/4..5*Pi/4 */
        {
            angle   = IFX_MATH_PI_INDEX;                         /* Set pre-angle of Pi */
            tangent = (Ifx_Math_ShL_F32(y, 15)) / x;             /* Calculate tangent of the angle, stretched by x and
                                                                  * y+x (x and y are interpreted as vectors) */
        }
    }
    else
    {
        if (x < y)                                               /* Partition 2: 1*Pi/4..3*Pi/4 */
        {
            angle   = IFX_MATH_PI_INDEX / 2;                     /* Set pre-angle of 1/2*Pi */
            tangent = -(Ifx_Math_ShL_F32(x, 15)) / y;            /* Calculate tangent of the angle, stretched by y and
                                                                  * y+x (x and y are interpreted as vectors) */
        }
        else                                                     /* Partition 1: -1*Pi/4..1*Pi/4 */
        {
            angle = 0;                                           /* Set no pre-angle */

            if (x == 0)                                          /* Prevent division by zero */
            {
                tangent = 0;
            }
            else
            {
                tangent = (Ifx_Math_ShL_F32(y, 15)) / x;         /* Calculate tangent of the angle, stretched by x and
                                                                  * y+x (x and y are interpreted as vectors) */
            }
        }
    }

    /* Saturate tangent */
    if (tangent == (IFX_MATH_FRACT16_MAX + 1))
    {
        tangent = IFX_MATH_FRACT16_MAX;
    }

    /* Read partial angle from LUT, add up total angle and return */
    return angle + Ifx_Math_Atan_F16((Ifx_Math_Fract16)tangent);
}


/* polyspace-end CODE-METRIC:VOCF [Justified:Low] "Not possible to achieve the threshold limit for the code metrics
 * without violating MISRA rules or other code metrics." */
