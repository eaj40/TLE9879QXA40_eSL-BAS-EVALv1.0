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
 * \file Ifx_Math_InvPark.h
 * \brief Provides a function to perform the inverse Park transformation.
 * This module contains the Inverse Park transformation function, also known as the (d, q) to (\f${\alpha}, {\beta}\f$)
 * projection, which is calculating the stationary two-axis orthogonal reference frame quantities \f$I_{\alpha}\f$ and
 * \f$I_{\beta}\f$, from the rotating reference frame quantities Id and Iq, based on the angle \f$\theta\f$.
 */

#ifndef IFX_MATH_INVPARK_H
#define IFX_MATH_INVPARK_H
#include "Ifx_Math.h"

/**
 *  \brief Performs the inverse Park transformation.
 *
 *  The Inverse Park transformation function, also known as the (d, q) to (\f$\alpha, \beta \f$) projection is
 * calculating the stationary two-axis orthogonal reference frame quantities \f$I_{\alpha}\f$ and  \f$I_{\beta}\f$ ,
 * from the rotating reference frame quantities Id and Iq, based on the angle \f$\theta\f$, as expressed by the
 * following equations:
 *
 *  \f[\begin{equation} I_{\alpha} = I_{d} \cdot \cos(\theta) - I_{q} \cdot \sin(\theta) \end{equation}\f]
 *
 *  \f[\begin{equation} I_{\beta} = I_{q} \cdot \cos(\theta) + I_{d} \cdot \sin(\theta) \end{equation}\f]
 *
 *  where:
 *  Id, Iq are the rotating reference frame quantities: current, voltage, flux, etc.;
 *  \f$I_{\alpha}\f$, \f$I_{\beta}\f$ are the two-axis orthogonal stator reference frame quantities;
 *  \f$\theta\f$ is the rotating angle.
 *
 *  \param [in] rotating Data container of the rotating reference frame quantities {Id real, Iq imag}
 *  \param [in] angle The rotating angle \f$\theta\f$ with the corresponding properties specified in the signal
 * database. 32-bit angle value, in the range \f$[0, 2*\pi]\f$, normalized by\f$2*\pi\f$.
 *
 *  \return Data container of the stationary frame quantities {\f$I_{\alpha}\f$ Real, \f$I_{\beta}\f$ Imag} with the
 * corresponding properties specified in the signal database
 */
Ifx_Math_CmpFract16 Ifx_Math_InvPark_F16(Ifx_Math_CmpFract16 rotating, uint32 angle);

#endif /*IFX_MATH_INVPARK_H*/
