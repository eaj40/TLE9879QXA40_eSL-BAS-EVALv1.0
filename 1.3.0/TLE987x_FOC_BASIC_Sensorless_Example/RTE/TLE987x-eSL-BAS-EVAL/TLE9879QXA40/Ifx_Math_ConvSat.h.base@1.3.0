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
 * \file Ifx_Math_ConvSat.h
 * \brief Provides functions to convert the input from one data type representation to another.
 * The functions in this module convert the input from one data type representation to another. For conversions from
 * floating point to fixed point the result is limited between the minimum and maximum of the corresponding return type
 * of the function.
 */

#ifndef IFX_MATH_CONVSAT_H
#define IFX_MATH_CONVSAT_H
#include "Ifx_Math.h"
#include "Ifx_Math_Sat.h"
#include "Ifx_Math_ShLR.h"

/**
 *  \brief Convert 16-bit signed fractional type x with the format qFormat to single precision floating point type.
 *
 *
 *  \param [in] x 16-bit signed fractional number
 *  \param [in] qForm The format of the source fractional type
 *
 *  \return Float value of the input parameter
 */
static inline Ifx_Math_Float32 Ifx_Math_ConvSat_F16ToFlt32(Ifx_Math_Fract16 x, Ifx_Math_FractQFormat qForm)
{
    /* polyspace +1 MISRA2012:D1.1 [Justified:Low] "Behavior verified by unit test." */
    return (Ifx_Math_Float32)x / (Ifx_Math_Float32)Ifx_Math_ShL_F32((Ifx_Math_Fract32)1U, (uint8)qForm);
}


/**
 *  \brief Converts single precision floating point type to 16-bit fractional type with saturation.
 *
 *  This function Converts single precision floating point type to 16-bit fractional type. The result is saturated
 * between IFX_MATH_FRACT16_MIN and IFX_MATH_FRACT16_MAX.
 *
 *  \param [in] x Single precision floating point number
 *  \param [in] qForm The format of the target fractional type
 *
 *  \return 16-bit signed fractional type with type specified by format qFormat
 */
static inline Ifx_Math_Fract16 Ifx_Math_ConvSat_Flt32ToF16(Ifx_Math_Float32 x, Ifx_Math_FractQFormat qForm)
{
    /* polyspace +1 MISRA2012:D1.1 [Justified:Low] "Behavior verified by unit test." */
    Ifx_Math_Float32 temp = (x * (Ifx_Math_Float32)Ifx_Math_ShL_F32((Ifx_Math_Fract32)1U, (uint8)qForm));

    return Ifx_Math_Sat_F16_F32((Ifx_Math_Fract32)temp);
}


/**
 *  \brief Convert 32-bit signed fractional type x with the format qFormat to single precision floating point type.
 *
 *
 *  \param [in] x 32-bit signed fractional number
 *  \param [in] qForm The format of the source fractional type
 *
 *  \return Float value of the input parameter
 */
static inline Ifx_Math_Float32 Ifx_Math_ConvSat_F32ToFlt32(Ifx_Math_Fract32 x, Ifx_Math_FractQFormat qForm)
{
    /* polyspace +1 MISRA2012:D1.1 [Justified:Low] "Behavior verified by unit test." */
    return (Ifx_Math_Float32)x / (Ifx_Math_Float32)(uint32)Ifx_Math_ShL_F32((Ifx_Math_Fract32)1U, (uint8)qForm);
}


/**
 *  \brief Converts single precision floating point type to fractional type, with saturation.
 *
 *  This function converts single precision floating point type to fractional type. The result is saturated between
 * IFX_MATH_FRACT32_MIN and IFX_MATH_FRACT32_MAX.
 *
 *  \param [in] x Single precision floating point number
 *  \param [in] qForm The format of the target fractional type
 *
 *  \return 32-bit signed fractional type with type specified by format qFormat
 */
static inline Ifx_Math_Fract32 Ifx_Math_ConvSat_Flt32ToF32(Ifx_Math_Float32 x, Ifx_Math_FractQFormat qForm)
{
    /* polyspace +2 MISRA2012:D1.1 [Justified:Low] "Behavior verified by unit test." */
    uint64           powerOfTwo = 1ULL << (uint8)qForm;
    Ifx_Math_Float32 temp       = x * (Ifx_Math_Float32)powerOfTwo;

    return Ifx_Math_Sat_F32_F64((Ifx_Math_Fract64)temp);
}


#endif /*IFX_MATH_CONVSAT_H*/
