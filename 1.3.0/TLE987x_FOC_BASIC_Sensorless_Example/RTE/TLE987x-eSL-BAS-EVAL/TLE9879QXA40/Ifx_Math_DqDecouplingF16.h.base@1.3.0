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
 * \file Ifx_Math_DqDecouplingF16.h
 * \brief This module calculates the stator voltage in rotor reference frame which compensates for the cross-coupling
 * effect.
 */

#ifndef IFX_MATH_DQDECOUPLINGF16_H
#define IFX_MATH_DQDECOUPLINGF16_H
#include "Ifx_Math.h"
#include "Ifx_Math_Mul.h"
#include "Ifx_Math_MulSat.h"

/**
 * \brief Data structure that stores all data of module instance.
 *
 */
typedef struct Ifx_Math_DqDecouplingF16
{
    /**
     * Complex variable to store normalized direct and quadrature inductances
     */
    Ifx_Math_CmpFract16 p_inductanceDQ;
} Ifx_Math_DqDecouplingF16;

/**
 *  \brief Initializes the inductance values to zero.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *
 */
static inline void Ifx_Math_DqDecouplingF16_init(Ifx_Math_DqDecouplingF16* self)
{
    self->p_inductanceDQ.real = 0;
    self->p_inductanceDQ.imag = 0;
}


/**
 *  \brief Set the quadrature inductance.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *  \param [in] inductanceQ Quadrature inductance
 *
 */
static inline void Ifx_Math_DqDecouplingF16_setInductanceQ(Ifx_Math_DqDecouplingF16* self, Ifx_Math_Fract16
                                                           inductanceQ)
{
    self->p_inductanceDQ.imag = inductanceQ;
}


/**
 *  \brief Get the quadrature inductance.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *
 *  \return Quadrature inductance
 */
static inline Ifx_Math_Fract16 Ifx_Math_DqDecouplingF16_getInductanceQ(Ifx_Math_DqDecouplingF16* self)
{
    return self->p_inductanceDQ.imag;
}


/**
 *  \brief Set the direct inductance.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *  \param [in] inductanceD Direct inductance
 *
 */
static inline void Ifx_Math_DqDecouplingF16_setInductanceD(Ifx_Math_DqDecouplingF16* self, Ifx_Math_Fract16
                                                           inductanceD)
{
    self->p_inductanceDQ.real = inductanceD;
}


/**
 *  \brief Get the direct inductance.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *
 *  \return Direct inductance
 */
static inline Ifx_Math_Fract16 Ifx_Math_DqDecouplingF16_getInductanceD(Ifx_Math_DqDecouplingF16* self)
{
    return self->p_inductanceDQ.real;
}


/**
 *  \brief Calculate the compensation voltage.
 *
 *  This function calculates compensation voltage \f$U_{dq,comp}\f$ which compensates the cross-coupling effect between
 * the d and q axis of a permanent magnet synchronous motor. The calculated voltages can be used as a feed forward in a
 * field oriented control scheme to improve the dynamics of the current control.
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *  \param [in] currentsDQ 16-bit signed fractional number complex currents in rotor reference frame
 *  \param [in] speed Electrical speed
 *
 *  \return 16-bit complex voltages \f$U_{dq,comp}\f$
 */
static inline Ifx_Math_CmpFract16 Ifx_Math_DqDecouplingF16_execute(Ifx_Math_DqDecouplingF16* self, Ifx_Math_CmpFract16
                                                                   currentsDQ, Ifx_Math_Fract16 speed)
{
    /* Local variable to store the compensation voltages*/
    Ifx_Math_CmpFract16 compVoltages;

    /* Real part of the compensation voltage*/
    compVoltages.real = Ifx_Math_Mul_F16(Ifx_Math_MulSat_F16(currentsDQ.imag, speed), self->p_inductanceDQ.imag);

    /* Imaginary part of the compensation voltage*/
    compVoltages.imag = Ifx_Math_Mul_F16(Ifx_Math_MulSat_F16(currentsDQ.real, speed), self->p_inductanceDQ.real);

    return compVoltages;
}


#endif /*IFX_MATH_DQDECOUPLINGF16_H*/
