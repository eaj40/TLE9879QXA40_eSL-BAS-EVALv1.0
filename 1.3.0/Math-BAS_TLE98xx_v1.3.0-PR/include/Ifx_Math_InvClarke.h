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
 * \file Ifx_Math_InvClarke.h
 * \brief Provides a function to perform the inverse Clarke transformation.
 * This module contains the Inverse Clarke transformation function, also known as the \f$(\alpha, \beta)\f$ to (u, v, w)
 * projection, which transforms the two-axis orthogonal reference frame \f$\alpha, \beta\f$ into the three phase
 * reference frame (u, v, w).
 */

#ifndef IFX_MATH_INVCLARKE_H
#define IFX_MATH_INVCLARKE_H
#include "Ifx_Math.h"

/**
 *  \brief Performs the inverse Clarke transformation.
 *
 *  The Inverse Clarke transformation function is calculating the quantities of the three-phase reference frame
 * (\f$\begin{equation} I_u, I_v, I_w \end{equation}\f$) from the quantities of the two-axis orthogonal system
 * (\f$\begin{equation} I_{\alpha}, I_{\beta} \end{equation}\f$), as expressed by the following equations:
 *
 *  \f[\begin{equation} I_u = I_{\alpha} \end{equation}\f]
 *
 *  \f[\begin{equation} I_v = -\frac{1}{2}I_{\alpha} + \frac{\sqrt{3}}{2}I_{\beta} \end{equation}\f]
 *
 *  \f[\begin{equation} I_w = -\frac{1}{2}I_{\alpha} - \frac{\sqrt{3}}{2}I_{\beta} \end{equation}\f]
 *
 *  where:
 *  \f$I_{\alpha}, I_{\beta},\f$ are the two-axis orthogonal reference frame quantities;
 *  \f$I_u, I_v, I_w,\f$ are the three-phase reference frame quantities;
 *  The results \f$I_v\f$ and \f$I_w\f$ are limited between FRACT16_MIN and FRACT16_MAX
 *
 *  \param [in] stationary Data Container of the 2-phase stationary frame quantities {\f$I_{\alpha}\f$ Real,
 * \f$I_{\beta}\f$ Imag}.
 *
 *  \return Data Container of the three phases {U, V, W}
 */
Ifx_Math_3PhaseFract16 Ifx_Math_InvClarke_F16(Ifx_Math_CmpFract16 stationary);

#endif /*IFX_MATH_INVCLARKE_H*/
