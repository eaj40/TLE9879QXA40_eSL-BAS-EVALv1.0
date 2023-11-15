/*
 * Copyright (c) 2022 Infineon Technologies AG. All Rights Reserved.
 *
 * Use of this file is subject to the terms of use agreed between (i) you or the company in which ordinary course of
 * business you are acting and (ii) Infineon Technologies AG, its affiliates or its licensees. If and as long as no
 * such terms of use are agreed, use of this file is subject to the Evaluation Software License Agreement distributed
 * along with this file within the software delivery package.
 *
 */

#include "Ifx_Math_Interp1DLut.h"
#include "Ifx_Math_Add.h"
#include "Ifx_Math_MulShR.h"
#include "Ifx_Math_ShL.h"
#include "Ifx_Math_ShR.h"
#include "Ifx_Math_Sub.h"

Ifx_Math_Fract16 Ifx_Math_Interp1DLut_F16(Ifx_Math_Fract16 x, Ifx_Math_Interp1DLut_Type table, uint8 shift)
{
    /* Declared local variables and interpolated output variable */
    Ifx_Math_Fract16 lutOutput;
    Ifx_Math_Fract16 deltaX;
    Ifx_Math_Fract16 interpolationFactor;

    /* Shift of input according to given shift parameter */
    Ifx_Math_Fract16 shiftedInput = Ifx_Math_ShR_F16(x, shift);

    /* Table index */
    uint16           tableIndex = (uint16)shiftedInput;

    /* Difference of y2 and y1 points on table */
    Ifx_Math_Fract16 deltaOut;

    /* Check if the shifted input value is bigger than table size */
    if (tableIndex >= (table.size - 1))
    {
        /* polyspace +1 MISRA2012:D4.14 [Justified:Low] "The lookup table has to be defined by the user externally." */
        lutOutput = table.data[table.size - 1];
    }
    else
    {
        /* polyspace +1 MISRA2012:D4.14 [Justified:Low] "The lookup table has to be defined by the user externally." */
        deltaOut = Ifx_Math_Sub_F16(table.data[tableIndex + 1], table.data[tableIndex]);

        /* deltaX stores x-x1 value */
        deltaX = Ifx_Math_Sub_F16(x, Ifx_Math_ShL_F16(shiftedInput, shift));

        /* interpolationFactor stores (y2-y1/x2-x1)*(x-x1) value */
        interpolationFactor = Ifx_Math_MulShR_F16(deltaOut, deltaX, shift);

        /* Calculation of linear interpolation */
        lutOutput = Ifx_Math_Add_F16(table.data[tableIndex], interpolationFactor);
    }

    return lutOutput;
}
