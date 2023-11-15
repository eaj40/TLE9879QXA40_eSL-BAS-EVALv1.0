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
 * \file Ifx_Math_Arithmetic.h
 * Module containing all the arithmetic functions
 */

#ifndef IFX_MATH_ARITHMETIC_H
#define IFX_MATH_ARITHMETIC_H
#include "Ifx_ComponentID.h"
#include "Ifx_ComponentVersion.h"
#include "Ifx_Math_Abs.h"
#include "Ifx_Math_AbsSat.h"
#include "Ifx_Math_Add.h"
#include "Ifx_Math_AddSat.h"
#include "Ifx_Math_ConvSat.h"
#include "Ifx_Math_Div.h"
#include "Ifx_Math_DivSat.h"
#include "Ifx_Math_DivShL.h"
#include "Ifx_Math_DivShLR.h"
#include "Ifx_Math_DivShLRSat.h"
#include "Ifx_Math_DivShLSat.h"
#include "Ifx_Math_DivShLSatNZ.h"
#include "Ifx_Math_Mul.h"
#include "Ifx_Math_MulSat.h"
#include "Ifx_Math_MulShL.h"
#include "Ifx_Math_MulShLR.h"
#include "Ifx_Math_MulShLRAdd.h"
#include "Ifx_Math_MulShLRAddSat.h"
#include "Ifx_Math_MulShLRSat.h"
#include "Ifx_Math_MulShLRSub.h"
#include "Ifx_Math_MulShLRSubSat.h"
#include "Ifx_Math_MulShR.h"
#include "Ifx_Math_MulShRAdd.h"
#include "Ifx_Math_MulShRAddSat.h"
#include "Ifx_Math_MulShRSat.h"
#include "Ifx_Math_MulShRSub.h"
#include "Ifx_Math_MulShRSubSat.h"
#include "Ifx_Math_Neg.h"
#include "Ifx_Math_NegSat.h"
#include "Ifx_Math_Sat.h"
#include "Ifx_Math_ShL.h"
#include "Ifx_Math_ShLR.h"
#include "Ifx_Math_ShLRSat.h"
#include "Ifx_Math_ShLSat.h"
#include "Ifx_Math_ShR.h"
#include "Ifx_Math_ShRRnd.h"
#include "Ifx_Math_Sub.h"
#include "Ifx_Math_SubSat.h"

/**
 *  \brief Returns the component ID
 *
 *
 *  \param [out] *componentID Variable to store the address of the component ID
 *
 */
void Ifx_Math_Arithmetic_getID(const Ifx_ComponentID** componentID);

/**
 *  \brief Returns the component version
 *
 *
 *  \param [out] *componentVersion Variable to store the address of the component version
 *
 */
void Ifx_Math_Arithmetic_getVersion(const Ifx_ComponentVersion** componentVersion);

#endif /*IFX_MATH_ARITHMETIC_H*/
