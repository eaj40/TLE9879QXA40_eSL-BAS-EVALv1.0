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
 * \file Ifx_Math_AdvancedMath.h
 * Contains the advanced math functions:
 *  - Acceleration limiter
 *  - 1D Interpolation
 *  - Limit
 *  - First order low pass filter
 *  - Phase locked loop
 *  - PI controller
 *  - Rate Limit
 */

#ifndef IFX_MATH_ADVANCEDMATH_H
#define IFX_MATH_ADVANCEDMATH_H
#include "Ifx_ComponentID.h"
#include "Ifx_ComponentVersion.h"
#include "Ifx_Math_AccelLimitF16.h"
#include "Ifx_Math_Interp1DLut.h"
#include "Ifx_Math_LimitF16.h"
#include "Ifx_Math_LowPass1stF16.h"
#include "Ifx_Math_PLLF16.h"
#include "Ifx_Math_PiF16.h"
#include "Ifx_Math_RateLimitF16.h"

/**
 *  \brief Returns the component ID
 *
 *
 *  \param [out] *componentID Variable to store the address of the component ID
 *
 */
void Ifx_Math_AdvancedMath_getID(const Ifx_ComponentID** componentID);

/**
 *  \brief Returns the component version
 *
 *
 *  \param [out] *componentVersion Variable to store the address of the component version
 *
 */
void Ifx_Math_AdvancedMath_getVersion(const Ifx_ComponentVersion** componentVersion);

#endif /*IFX_MATH_ADVANCEDMATH_H*/
