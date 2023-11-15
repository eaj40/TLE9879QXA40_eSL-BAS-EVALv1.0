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
 * \file Ifx_Math_SinCos.h
 * \brief Provides a function to calculate the sinus and cosinus value of an angle.
 */

#ifndef IFX_MATH_SINCOS_H
#define IFX_MATH_SINCOS_H
#include "Ifx_Math.h"
#include "Ifx_Math_Cos.h"

/**
 * Structure for returning the sine and cosine values
 */
typedef struct Ifx_Math_SinCos_Type
{
    /**
     * Sine value
     */
    Ifx_Math_Fract16 sin;

    /**
     * Cosine value
     */
    Ifx_Math_Fract16 cos;
} Ifx_Math_SinCos_Type;

/**
 *  \brief Calculates the sinus and cosinus value of a 32-bit angle value.
 *
 *  This function calculates the sinus and cosinus value of a 32-bit angle value, in the range \f$[0, 2*\pi]\f$,
 * normalized by\f$2*\pi\f$. The output resolution is configurable by controlling the sine lookup table size. The size
 * of the lookup table is given by IFX_MATH_CFG_SIN_LUT_SIZE.
 *  The valid configuration set is: IFX_MATH_CFG_SIN_LUT_SIZE = {IFX_MATH_USROPT_SIN_LUT_SIZE_8;
 * IFX_MATH_USROPT_SIN_LUT_SIZE_10; IFX_MATH_USROPT_SIN_LUT_SIZE_12}.
 *
 *  \param [in] x 32-bit angle value, in the range \f$[0, 2*\pi]\f$, normalized by\f$2*\pi\f$
 *
 *  \return Data container containing the sinus and cosinus if the input in Q15 format
 */
static inline Ifx_Math_SinCos_Type Ifx_Math_SinCos_F16(uint32 x)
{
    Ifx_Math_SinCos_Type sinCosReturn;
    sinCosReturn.sin = Ifx_Math_Sin_F16(x);
    sinCosReturn.cos = Ifx_Math_Cos_F16(x);

    return sinCosReturn;
}


#endif /*IFX_MATH_SINCOS_H*/
