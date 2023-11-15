/*
 * Copyright (c) 2022 Infineon Technologies AG. All Rights Reserved.
 *
 * Use of this file is subject to the terms of use agreed between (i) you or the company in which ordinary course of
 * business you are acting and (ii) Infineon Technologies AG, its affiliates or its licensees. If and as long as no
 * such terms of use are agreed, use of this file is subject to the Evaluation Software License Agreement distributed
 * along with this file within the software delivery package.
 *
 */

/**
 * \file Ifx_Math_DivShL.h
 * \brief Provides functions to calculate the shifted fractional quotient of two fractional inputs.
 * The functions in this module calculate the fractional quotient of the two input parameters, x and y, where x is the
 * numerator and y the denominator, left shifted by n without saturation and overflow detection.
 * The division is protected from division by zero by returning the maximum of the return type of the function if the
 * numerator is greater or equal to zero and the minimum if the numerator is less than zero.
 */

#ifndef IFX_MATH_DIVSHL_H
#define IFX_MATH_DIVSHL_H
#include "Ifx_Math.h"
#include "Ifx_Math_ShL.h"

/**
 * Lookup table to convert the left shift bits to a mask
 */
extern const uint32 Ifx_Math_Lut_DivShL_F32_table[];

/**
 *  \brief Calculate the bits need to be shifted when dividing two fractional types
 *
 *
 *  \param [in] typeX The format of the first operand of division
 *  \param [in] typeY The format of the second operand of division
 *  \param [in] typeReturn The format of the quotient
 *
 *  \return Shift value
 */
static inline sint8 Ifx_Math_DivShL_ShiftDiv(Ifx_Math_FractQFormat typeX, Ifx_Math_FractQFormat typeY,
                                             Ifx_Math_FractQFormat typeReturn)
{
    return -(sint8)typeX + (sint8)typeY + (sint8)typeReturn;
}


/**
 *  \brief Calculates the 32-bit fractional quotient of two 32-bit fractional inputs.
 *
 *  This functions calculates the 32-bit fractional quotient of the two 32-bit fractional inputs, x and y, where x is
 * the numerator and y the denominator, shifted by n without saturation and overflow detection. The division is
 * protected from division by zero by returning IFX_MATH_FRACT32_MAX if x &gt;= 0 and IFX_MATH_FRACT32_MIN if x &lt; 0.
 *
 *  \param [in] x 32-bit fractional number with the format Qa
 *  \param [in] y 32-bit fractional number with the format Qb
 *  \param [in] n Number of bits to shift the division result, between 0 and 16. Use a constant for flash optimization.
 *
 *  \return 32-bit fractional number with the format Q(a-b+n)
 */

/* polyspace-begin CODE-METRIC:VOCF [Justified:Low] "For readability and not to add private function in the header file,
 * this is justified." */
static inline Ifx_Math_Fract32 Ifx_Math_DivShL_p_F32_F32F32(Ifx_Math_Fract32 x, Ifx_Math_Fract32 y, uint8 n)
{
    /* Stores the return value */
    Ifx_Math_Fract32 divReturn;

    /* Variable to store the temporary value */
    uint32           reference;

    /* Protect against division by 0 */
    if (y == 0)
    {
        if (x >= 0)
        {
            divReturn = ((Ifx_Math_Fract32)IFX_MATH_FRACT32_MAX);
        }
        else
        {
            divReturn = (Ifx_Math_Fract32)IFX_MATH_FRACT32_MIN;
        }
    }
    else
    {
        if (x >= 0)
        {
            reference = 0;
        }
        else
        {
            reference = Ifx_Math_Lut_DivShL_F32_table[n];
        }

        /* Check if shift can be done before division */
        if (((uint32)x & Ifx_Math_Lut_DivShL_F32_table[n]) == reference)
        {
            /* polyspace +2 MISRA2012:10.1 [Justified:Low] "Bitwise operators on signed values are required by fixed
             * point arithmetic." */
            divReturn = (x << n) / y;
        }
        else
        {
            /* polyspace +2 MISRA2012:10.1 [Justified:Low] "Bitwise operators on signed values are required by fixed
             * point arithmetic." */
            divReturn = (x / y) << n;
        }
    }

    return divReturn;
}


/* polyspace-end CODE-METRIC:VOCF [Justified:Low] "For readability and not to add private function in the header file,
 * this is justified." */

/**
 *  \brief Calculates the 32-bit shifted fractional quotient of two 32-bit fractional inputs.
 *
 *  This functions calculates the 32-bit fractional quotient of the two 32-bit fractional inputs x and y, where x is the
 * numerator and y the denominator, shifted by n without saturation and overflow detection. The division is protected
 * from division by zero by returning IFX_MATH_FRACT32_MAX if x &gt;= 0 and IFX_MATH_FRACT32_MIN if x &lt; 0.
 *
 *  \param [in] x 32-bit fractional number with the format Qa
 *  \param [in] y 32-bit fractional number with the format Qb
 *  \param [in] n Number of bits to left shift the division result, between 0 and 16
 *
 *  \return 32-bit fractional number with the format Q(a-b+n)
 */
static inline Ifx_Math_Fract32 Ifx_Math_DivShL_F32(Ifx_Math_Fract32 x, Ifx_Math_Fract32 y, uint8 n)
{
    return Ifx_Math_DivShL_p_F32_F32F32(x, y, n);
}


/**
 *  \brief Calculates the 16-bit shifted fractional quotient of two 16-bit fractional inputs.
 *
 *  This function calculates the fractional quotient between x and y, where x is the numerator and y the denominator,
 * shifted by n without saturation and overflow detection.  The division is protected from division by zero by returning
 * IFX_MATH_FRACT16_MAX if x &gt;= 0 and IFX_MATH_FRACT16_MIN if x &lt; 0.
 *
 *  \param [in] x 16-bit fractional number with the format Qa
 *  \param [in] y 16-bit fractional number with the format Qb
 *  \param [in] n Number of bits to left shift the division result, between 0 and 16
 *
 *  \return 16-bit fractional number with the format Q(a-b+n)
 */
static inline Ifx_Math_Fract16 Ifx_Math_DivShL_F16(Ifx_Math_Fract16 x, Ifx_Math_Fract16 y, uint8 n)
{
    /* Stores the return value */
    Ifx_Math_Fract16 divReturn;

    /* Protect against division by 0 */
    if (y == 0)
    {
        if (x >= 0)
        {
            divReturn = IFX_MATH_FRACT16_MAX;
        }
        else
        {
            divReturn = IFX_MATH_FRACT16_MIN;
        }
    }
    else
    {
        divReturn = (Ifx_Math_Fract16)(Ifx_Math_ShL_F32((Ifx_Math_Fract32)x, n) / (Ifx_Math_Fract32)y);
    }

    return divReturn;
}


/**
 *  \brief Calculates the 64-bit fractional quotient of two 32-bit fractional inputs.
 *
 *  This functions calculates the 64-bit fractional quotient of the two 32-bit fractional inputs, x and y, where x is
 * the numerator and y the denominator, shifted by n without saturation and overflow detection. The division is
 * protected from division by zero by returning IFX_MATH_FRACT32_MAX if x &gt;= 0 and IFX_MATH_FRACT32_MIN if x &lt; 0.
 *
 *  \param [in] x 32-bit fractional number with the format Qa
 *  \param [in] y 32-bit fractional number with the format Qb
 *  \param [in] n Number of bits to left shift the division result, between 0 and 16
 *
 *  \return 64-bit fractional number with the format Q(a-b+n)
 */
static inline Ifx_Math_Fract64 Ifx_Math_DivShL_p_F64_F32F32(Ifx_Math_Fract32 x, Ifx_Math_Fract32 y, uint8 n)
{
    /* Stores the return value */
    Ifx_Math_Fract64 divReturn;

    /* Protect against division by 0 */
    if (y == 0)
    {
        if (x >= 0)
        {
            divReturn = (Ifx_Math_Fract64)IFX_MATH_FRACT32_MAX;
        }
        else
        {
            divReturn = (Ifx_Math_Fract64)IFX_MATH_FRACT32_MIN;
        }
    }
    else
    {
        /* polyspace +2 MISRA2012:10.1 [Justified:Low] "Bitwise operators on signed values are required by fixed point
         * arithmetic." */
        divReturn = ((Ifx_Math_Fract64)x << n) / (Ifx_Math_Fract64)y;
    }

    return divReturn;
}


#endif /*IFX_MATH_DIVSHL_H*/
