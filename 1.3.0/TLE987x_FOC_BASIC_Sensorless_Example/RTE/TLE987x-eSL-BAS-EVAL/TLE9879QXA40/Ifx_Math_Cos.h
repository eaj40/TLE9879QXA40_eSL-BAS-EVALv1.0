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
 * \file Ifx_Math_Cos.h
 * \brief Provides a function to calculate the cosinus value of an angle.
 */

#ifndef IFX_MATH_COS_H
#define IFX_MATH_COS_H
#include "Ifx_Math.h"
#include "Ifx_Math_Sin.h"

/**
 *  \brief Calculate the cosinus value of the 32-bit input angle.
 *
 *  This function calculates the cosinus value of the 32-bit input angle, in the range \f$[0, 2*\pi]\f$, normalized by
 * \f$2*\pi\f$. The output resolution is configurable by controlling the sine lookup table size. The size of the lookup
 * table is given by IFX_MATH_CFG_SIN_LUT_SIZE. The valid configuration set is: IFX_MATH_CFG_SIN_LUT_SIZE =
 * {IFX_MATH_USROPT_SIN_LUT_SIZE_8; IFX_MATH_USROPT_SIN_LUT_SIZE_10; IFX_MATH_USROPT_SIN_LUT_SIZE_12}.
 *
 *  \param [in] x 32-bit angle value, in the range \f$[0, 2*\pi]\f$, normalized by\f$2*\pi\f$
 *
 *  \return 16-bit cosinus value in Q15 format
 */
static inline Ifx_Math_Fract16 Ifx_Math_Cos_F16(uint32 x)
{
    /* Stores the cosine output */
    Ifx_Math_Fract16 cosOutput;

    /* Stores the sine input angle */
    uint32           sinInputAngle;

    /* Cosine(theta) = sin(pi/2 - theta) */
    if (x <= IFX_MATH_PI_INDEX / 2)
    {
        sinInputAngle = IFX_MATH_PI_INDEX / 2 - x;
    }

    /* Convert to supported range of sine by adding 2*pi */
    else
    {
        sinInputAngle = (2 * IFX_MATH_PI_INDEX - 1 - x) + IFX_MATH_PI_INDEX / 2;
    }

    /* Call the existing sine function to calculate the cosine output */
    cosOutput = Ifx_Math_Sin_F16(sinInputAngle);

    return cosOutput;
}


#endif /*IFX_MATH_COS_H*/
