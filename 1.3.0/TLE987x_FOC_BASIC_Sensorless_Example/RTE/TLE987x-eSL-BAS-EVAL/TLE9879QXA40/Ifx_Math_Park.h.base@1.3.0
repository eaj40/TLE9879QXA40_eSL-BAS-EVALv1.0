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
 * \file Ifx_Math_Park.h
 * \brief Provides a function to perform the Park transformation.
 * This module contains the Park transformation function, also known as the (\f$\alpha\f$, \f$\beta\f$) to (d, q)
 * projection which transforms the two-phase orthogonal system (\f$\alpha\f$, \f$\beta\f$) fixed to the stator of an
 * electrical machine into the (d, q) rotating reference system fixed to the rotor.
 */

#ifndef IFX_MATH_PARK_H
#define IFX_MATH_PARK_H
#include "Ifx_Math.h"

/**
 *  \brief Performs the inverse Park transformation.
 *
 *  The Park transformation function is calculating the quantities of the rotating reference system (Id and Iq) from the
 * quantities of the two-axis orthogonal system (\f$I_{\alpha}\f$ and \f$I_{\beta}\f$) based on the angle \f$\theta\f$,
 * as expressed by the following equations:
 *
 *  \f[\begin{equation} I_{d} = I_{\alpha} \cdot \cos(\theta) + I_{\beta} \cdot \sin(\theta) \end{equation}\f]
 *
 *  \f[\begin{equation} I_{q} = I_{\beta} \cdot \cos(\theta) - I_{\alpha} \cdot \sin(\theta) \end{equation}\f]
 *
 *  where:
 *  \f$I_{d}\f$, \f$I_{q}\f$ are the rotating reference frame quantities: current, voltage, flux, etc.;
 *  \f$I_{\alpha}\f$, \f$I_{\beta}\f$ are the two-axis orthogonal stator reference frame quantities;
 *  \f$\theta\f$ is the rotating angle.
 *
 *  \param [in] stationary Data container of the stationary frame quantities {\f$I_{\alpha}\f$ Real, \f$I_{\beta}\f$
 * Imag}
 *  \param [in] angle The rotating Angle \f$\theta\f$. 32-bit angle value, in the range \f$[0, 2*\pi]\f$, normalized
 * by\f$2*\pi\f$
 *
 *  \return Data container of the rotating reference frame quantities {Id real, Iq imag}
 */
Ifx_Math_CmpFract16 Ifx_Math_Park_F16(Ifx_Math_CmpFract16 stationary, uint32 angle);

#endif /*IFX_MATH_PARK_H*/
