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
 * \file Ifx_Math_LimitF16.h
 * \brief Provides functions to limit the input between two given values.
 */

#ifndef IFX_MATH_LIMITF16_H
#define IFX_MATH_LIMITF16_H
#include "Ifx_Math.h"

/**
 * \brief Data structure that stores all data of module instance.
 *
 */
typedef struct Ifx_Math_LimitF16
{
    /**
     * Upper limit of the function
     */
    Ifx_Math_Fract16 p_upperLimit;

    /**
     * Lower limit of the function
     */
    Ifx_Math_Fract16 p_lowerLimit;
} Ifx_Math_LimitF16;

/**
 *  \brief 16-bit version of the limit function.
 *
 *  This function returns the input limited between the two set limits.
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *  \param [in] input Input value to limit
 *
 *  \return 16-bit limited value of the input
 */
static inline Ifx_Math_Fract16 Ifx_Math_LimitF16_execute(Ifx_Math_LimitF16* self, Ifx_Math_Fract16 input)
{
    /* Stores the output */
    Ifx_Math_Fract16 output;

    /* Set input to upper limit*/
    if (input >= self->p_upperLimit)
    {
        output = self->p_upperLimit;
    }

    /* Set input to lower limit*/
    else if (input <= self->p_lowerLimit)
    {
        output = self->p_lowerLimit;
    }
    else
    {
        output = input;
    }

    return output;
}


/**
 *  \brief Set the lower limit value.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *  \param [in] lowerLimit The value of the lower limit to set
 *
 */
static inline void Ifx_Math_LimitF16_setLowerLimit(Ifx_Math_LimitF16* self, Ifx_Math_Fract16 lowerLimit)
{
    self->p_lowerLimit = lowerLimit;
}


/**
 *  \brief Set the upper limit value.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *  \param [in] upperLimit The value of the upper limit to set
 *
 */
static inline void Ifx_Math_LimitF16_setUpperLimit(Ifx_Math_LimitF16* self, Ifx_Math_Fract16 upperLimit)
{
    self->p_upperLimit = upperLimit;
}


/**
 *  \brief Get the lower limit value.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *
 *  \return Lower limit
 */
static inline Ifx_Math_Fract16 Ifx_Math_LimitF16_getLowerLimit(Ifx_Math_LimitF16* self)
{
    return self->p_lowerLimit;
}


/**
 *  \brief Get the upper limit value.
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *
 *  \return Upper limit
 */
static inline Ifx_Math_Fract16 Ifx_Math_LimitF16_getUpperLimit(Ifx_Math_LimitF16* self)
{
    return self->p_upperLimit;
}


#endif /*IFX_MATH_LIMITF16_H*/
