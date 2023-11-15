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
 * \file Ifx_MHA_MeasurementADC.h
 * \brief A generalized measurement ADC module with attributes and operations common and available for all the
 * measurement ADC modules.
 */

#ifndef IFX_MHA_MEASUREMENTADC_H
#define IFX_MHA_MEASUREMENTADC_H
#include "Ifx_Math.h"

/**
 * \brief Data structure that stores all data of module instance.
 *
 */
typedef struct Ifx_MHA_MeasurementADC
{
    /**
     * State machine enable setting
     */
    bool p_enable;
} Ifx_MHA_MeasurementADC;

/**
 *  \brief This is a virtual function.
 *
 *  This function is implemented in the child module.
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *
 */
void Ifx_MHA_MeasurementADC_init(Ifx_MHA_MeasurementADC* self);

/**
 *  \brief This is a virtual function.
 *
 *  This function is implemented in the child module.
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *
 */
void Ifx_MHA_MeasurementADC_execute(Ifx_MHA_MeasurementADC* self);

/**
 *  \brief This API enables or disables the module based on the input parameter enable.
 * If the input parameter is TRUE and no fault is detected, then the module will be enabled in the next call to
 * execute().
 * This API also set the HW-related members of the internal "self" data structure to the corresponding HW registers
 * according to the user manual.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *  \param [in] enable Parameter of boolean type to enable or disable the module
 *
 */
static inline void Ifx_MHA_MeasurementADC_enable(Ifx_MHA_MeasurementADC* self, bool enable)
{
    self->p_enable = enable;
}


#endif /*IFX_MHA_MEASUREMENTADC_H*/
