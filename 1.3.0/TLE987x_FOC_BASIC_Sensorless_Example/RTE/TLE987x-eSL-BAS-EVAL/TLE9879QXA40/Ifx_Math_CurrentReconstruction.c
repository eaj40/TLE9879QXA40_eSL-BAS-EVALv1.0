/*
 * Copyright (c) 2022 Infineon Technologies AG. All Rights Reserved.
 *
 * Use of this file is subject to the terms of use agreed between (i) you or the company in which ordinary course of
 * business you are acting and (ii) Infineon Technologies AG, its affiliates or its licensees. If and as long as no
 * such terms of use are agreed, use of this file is subject to the Evaluation Software License Agreement distributed
 * along with this file within the software delivery package.
 *
 */

#include "Ifx_Math_CurrentReconstruction.h"
#include "Ifx_Math_SubSat.h"
#include "Ifx_Math_NegSat.h"

/* polyspace-begin CODE-METRIC:VOCF [Justified:Low] "For readability and performance, switch case implementation is
 * justified"
 * */
Ifx_Math_3PhaseFract16 Ifx_Math_CurrentReconstruction_F16(Ifx_Math_CurrentReconstruction_info info,
                                                          Ifx_Math_Fract16                  * currentMeasurements)
{
    /* Local variable for storing the return value */
    Ifx_Math_3PhaseFract16 threePhaseCurr;

    /* Local variables for storing the phase current values */
    Ifx_Math_Fract16       firCurr;
    Ifx_Math_Fract16       secCurr;
    Ifx_Math_Fract16       thrCurr;

    /* polyspace +2 MISRA2012:D4.14 [Justified:Low] "The caller function has to guarantee that NULL is not passed as
     * argument." */
    firCurr = currentMeasurements[0];

    if (info.secondTriggerIsSum == true)
    {
        /* Negation of the second measured current */
        secCurr = Ifx_Math_NegSat_F16(currentMeasurements[1]);

        /* Subtraction of the measured current to calculate third phase current */
        thrCurr = Ifx_Math_SubSat_F16(currentMeasurements[1], currentMeasurements[0]);
    }
    else
    {
        /* Second measured current */
        secCurr = currentMeasurements[1];

        /* Subtraction of the measured current to calculate third phase current */
        thrCurr = Ifx_Math_SubSat_F16(Ifx_Math_NegSat_F16(currentMeasurements[0]), currentMeasurements[1]);
    }

    /* Calculation of phase current based on the sector number*/
    switch (info.sector)
    {
        /* Sector 0 */
        case 0:

            /* first phase current */
            threePhaseCurr.u = firCurr;

            /* second phase current */
            threePhaseCurr.v = thrCurr;

            /* third phase current */
            threePhaseCurr.w = secCurr;
            break;

        /* Sector 1 */
        case 1:
            threePhaseCurr.u = thrCurr;
            threePhaseCurr.v = firCurr;
            threePhaseCurr.w = secCurr;
            break;

        /* Sector 2 */
        case 2:
            threePhaseCurr.u = secCurr;
            threePhaseCurr.v = firCurr;
            threePhaseCurr.w = thrCurr;
            break;

        /* Sector 3 */
        case 3:
            threePhaseCurr.u = secCurr;
            threePhaseCurr.v = thrCurr;
            threePhaseCurr.w = firCurr;
            break;

        /* Sector 4 */
        case 4:
            threePhaseCurr.u = thrCurr;
            threePhaseCurr.v = secCurr;
            threePhaseCurr.w = firCurr;
            break;

        /* Sector 5 */
        default:
            threePhaseCurr.u = firCurr;
            threePhaseCurr.v = secCurr;
            threePhaseCurr.w = thrCurr;
            break;
    }

    return threePhaseCurr;
}


/* polyspace-end CODE-METRIC:VOCF [Justified:Low] "For readability and performance, switch case implementation is
 * justified"
 * */
