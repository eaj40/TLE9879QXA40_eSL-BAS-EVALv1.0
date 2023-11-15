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
 * \file Ifx_Math_Clarke.h
 * \brief Provides the Clarke transformation function.
 * The Clarke transformation function is also known as the (u, v, w) to \f${\alpha}, {\beta}\f$ projection, which
 * converts the variables from the three phase reference frame (u, v, w) into the two-axis orthogonal reference frame
 * \f${\alpha}, {\beta}\f$.
 */

#ifndef IFX_MATH_CLARKE_H
#define IFX_MATH_CLARKE_H
#include "Ifx_Math.h"

/**
 *  \brief Calculates the 16-bit Clarke transformation.
 *
 *  The Clarke transformation function is calculating the quantities of the two-axis orthogonal system \f$I_{\alpha}\f$,
 * and \f$I_{\beta}\f$ from the quantities of the three-phase reference frame \f$I_u\f$, \f$I_v\f$, and \f$I_w\f$, as
 * expressed by the following equations:
 *
 *  \f[\begin{equation} I_{\alpha} = I_u \end{equation}\f]
 *
 *  \f[\begin{equation} I_{\beta} = \frac{1}{\sqrt{3}}(I_v - I_w) \end{equation}\f]
 *
 *  where:
 *  \f$I_u, I_v, I_w,\f$ are the three-phase reference frame quantities;
 *  \f$I_{\alpha}, I_{\beta},\f$ are the two-axis orthogonal reference frame quantities;
 *  \f$I_u + I_v + I_w = 0\f$
 *  The result \f$I_{\beta}\f$ is limited between IFX_MATH_FRACT16_MIN and IFX_MATH_FRACT16_MAX
 *
 *  These equations are only applicable for the case of a balanced system where the sum of the three-phase reference
 * frame quantities is 0, which is the case of a three phase electrical machine.
 *
 *  \param [in] phase Data Container of the three phases {U, V, W}
 *
 *  \return Data Container of the 2-phase stationary frame quantities: \f$I_{\alpha}\f$ is Real, \f$I_{\beta}\f$ is Imag
 */
Ifx_Math_CmpFract16 Ifx_Math_Clarke_F16(Ifx_Math_3PhaseFract16 phase);

#endif /*IFX_MATH_CLARKE_H*/
