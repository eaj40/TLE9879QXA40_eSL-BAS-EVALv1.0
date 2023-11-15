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
 * \file Ifx_Math_SpeedPreControlF16.h
 * \brief This module calculates the reference q-axis current based on the reference speed and motor parameters.
 */

#ifndef IFX_MATH_SPEEDPRECONTROLF16_H
#define IFX_MATH_SPEEDPRECONTROLF16_H

#include "Ifx_Math.h"

/**
 * Contains the static configuration parameters
 */
typedef struct Ifx_Math_SpeedPreControlF16_StaticConfig
{
    /**
     * Friction constant of the motor
     */
    Ifx_Math_Fract16 fricitionConstant;

    /**
     * Normalized rotor inertia divide by normalized sampling time of the motor
     */
    Ifx_Math_Fract16Q rotorInertiaOverSamplingTime;
} Ifx_Math_SpeedPreControlF16_StaticConfig;

/**
 * \brief Data structure that stores all data of module instance.
 *
 */
typedef struct Ifx_Math_SpeedPreControlF16
{
    /**
     * Static configuration of the speed pre control
     */
    Ifx_Math_SpeedPreControlF16_StaticConfig p_staticConfiguration;

    /**
     * Q format for the frictional torque
     */
    Ifx_Math_FractQFormat p_frictionalTorqueQformat;

    /**
     * Previous speed of the motor
     */
    Ifx_Math_Fract16 p_previousSpeed;

    /**
     * One over torque constant of the motor
     */
    Ifx_Math_Fract16 p_inverseTorqueConstant;
} Ifx_Math_SpeedPreControlF16;

/**
 *  \brief Initializes all speed pre control parameters
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *  \param [in] staticConfiguration Static parameters friction contsant and rotor inertia divide by sampling time
 *
 */
void Ifx_Math_SpeedPreControlF16_init(Ifx_Math_SpeedPreControlF16* self, Ifx_Math_SpeedPreControlF16_StaticConfig
                                      staticConfiguration);

/**
 *  \brief Set one over torque constant.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *  \param [in] inverseTorqueConstant One over torque constant
 *
 */
static inline void Ifx_Math_SpeedPreControlF16_setInverseTorqueConstant(Ifx_Math_SpeedPreControlF16* self,
                                                                        Ifx_Math_Fract16
                                                                        inverseTorqueConstant)
{
    self->p_inverseTorqueConstant = inverseTorqueConstant;
}


/**
 *  \brief Get the inverse torque constant.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *
 *  \return inverse  torque constant
 */
static inline Ifx_Math_Fract16 Ifx_Math_SpeedPreControlF16_getInverseTorqueConstant(Ifx_Math_SpeedPreControlF16* self)
{
    return self->p_inverseTorqueConstant;
}


/**
 *  \brief Calculate the quadrature current.
 * This function calculates quadrature current. The calculated current can be used as a feed forward in a field oriented
 * control scheme to improve the dynamics of the speed control.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *  \param [in] speed Electrical speed
 *
 *  \return 16-bit quadrature current
 */
Ifx_Math_Fract16 Ifx_Math_SpeedPreControlF16_execute(Ifx_Math_SpeedPreControlF16* self, Ifx_Math_Fract16 speed);

/**
 *  \brief Set the previous speed.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *  \param [in] previousSpeed Previous speed
 *
 */
static inline void Ifx_Math_SpeedPreControlF16_setPreviousSpeed(Ifx_Math_SpeedPreControlF16* self, Ifx_Math_Fract16
                                                                previousSpeed)
{
    self->p_previousSpeed = previousSpeed;
}


/**
 *  \brief Get the previous speed.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *
 *  \return previous speed
 */
static inline Ifx_Math_Fract16 Ifx_Math_SpeedPreControlF16_getPreviousSpeed(Ifx_Math_SpeedPreControlF16* self)
{
    return self->p_previousSpeed;
}


#endif /*IFX_MATH_SPEEDPRECONTROLF16_H*/
