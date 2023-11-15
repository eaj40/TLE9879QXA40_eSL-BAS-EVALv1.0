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
 * \file Ifx_Math_CurrentReconstruction.h
 * \brief Provides a function to reconstruct and return the three phase fractional current from the measured currents
 * values.
 */

#ifndef IFX_MATH_CURRENTRECONSTRUCTION_H
#define IFX_MATH_CURRENTRECONSTRUCTION_H
#include "Ifx_Math.h"

/**
 * Structure which contains all the different input variables needed for current reconstruction
 */
typedef struct Ifx_Math_CurrentReconstruction_info
{
    /**
     * Sector number for current reconstruction
     */
    uint8 sector;

    /**
     * Whether the second current trigger is measuring the sum of two phase currents or only one phase current.
     */
    bool secondTriggerIsSum;
} Ifx_Math_CurrentReconstruction_info;

/**
 *  \brief Returns the 3-phase 16-bit current values reconstructed from the 16-bit measured current values.
 *
 *
 *  \param [in] info Information for current reconstruction
 *  \param [inout] currentMeasurements Array containing 2 current measurement values
 *
 *  \return 3-phase currents
 */
Ifx_Math_3PhaseFract16 Ifx_Math_CurrentReconstruction_F16(Ifx_Math_CurrentReconstruction_info info,
                                                          Ifx_Math_Fract16                  * currentMeasurements);

#endif /*IFX_MATH_CURRENTRECONSTRUCTION_H*/
