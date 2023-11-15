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
 * \file Ifx_Math_Trigonometric.h
 * Includes the trigonometric functions available in this library:
 *  - Arctangent
 *  - Arctangent 2 (4 quadrant)
 *  - Cosine
 *  - Sine
 *  - Sine and cosine (result of both returned in a structure)
 */

#ifndef IFX_MATH_TRIGONOMETRIC_H
#define IFX_MATH_TRIGONOMETRIC_H
#include "Ifx_ComponentID.h"
#include "Ifx_ComponentVersion.h"
#include "Ifx_Math_Atan.h"
#include "Ifx_Math_Atan2.h"
#include "Ifx_Math_Cos.h"
#include "Ifx_Math_Sin.h"
#include "Ifx_Math_SinCos.h"

/**
 *  \brief Returns the component ID
 *
 *
 *  \param [out] *componentID Variable to store the address of the component ID
 *
 */
void Ifx_Math_Trigonometric_getID(const Ifx_ComponentID** componentID);

/**
 *  \brief Returns the component version
 *
 *
 *  \param [out] *componentVersion Variable to store the address of the component version
 *
 */
void Ifx_Math_Trigonometric_getVersion(const Ifx_ComponentVersion** componentVersion);

#endif /*IFX_MATH_TRIGONOMETRIC_H*/
