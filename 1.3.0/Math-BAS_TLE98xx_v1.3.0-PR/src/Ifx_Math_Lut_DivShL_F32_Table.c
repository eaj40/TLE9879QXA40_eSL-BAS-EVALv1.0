/*
 * Copyright (c) 2022 Infineon Technologies AG. All Rights Reserved.
 *
 * Use of this file is subject to the terms of use agreed between (i) you or the company in which ordinary course of
 * business you are acting and (ii) Infineon Technologies AG, its affiliates or its licensees. If and as long as no
 * such terms of use are agreed, use of this file is subject to the Evaluation Software License Agreement distributed
 * along with this file within the software delivery package.
 *
 */

#include "Ifx_Math.h"
#include "Ifx_Math_DivShL.h"

/* This table is used to avoid a overflow while left shift a mask is required to check the unused bits.
 * This table converts the left shift bits to a mask*/
const uint32 Ifx_Math_Lut_DivShL_F32_table[17] = {
    0x80000000U, 0xC0000000U, 0xE0000000U, 0xF0000000U, 0xF8000000U, 0xFC000000U, 0xFE000000U, 0xFF000000U,
    0xFF800000U, 0xFFC00000U, 0xFFE00000U, 0xFFF00000U, 0xFFF80000U, 0xFFFC0000U, 0xFFFE0000U, 0xFFFF0000U,
    0xFFFF8000U,
};
