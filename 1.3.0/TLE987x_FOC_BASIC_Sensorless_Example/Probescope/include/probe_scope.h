/*
 * Copyright (c) 2022 Infineon Technologies AG. All Rights Reserved.
 *
 * Use of this file is subject to the terms of use agreed between (i) you or the company in which ordinary course of
 * business you are acting and (ii) Infineon Technologies AG, its affiliates or its licensees. If and as long as no
 * such terms of use are agreed, use of this file is subject to the Evaluation Software License Agreement distributed
 * along with this file within the software delivery package.
 *
 */

/*
 ************************************************************************************************************************
 *                                                     Oscilloscope
 *
 *                                    (c) Copyright 2016; Micrium, Inc.; Weston, FL
 *                           All rights reserved.  Protected by international copyright laws.
 *
 *
 * File    : PROBE_SCOPE.H
 * By      : JJL
 * Version : V1.00.00
 ************************************************************************************************************************
 */

/*
 *********************************************************************************************************
 *                                             INCLUDE FILES
 *********************************************************************************************************
 */

#include  <stdint.h>

/*
 *********************************************************************************************************
 *                                           FUNCTION PROTOTYPES
 *********************************************************************************************************
 */
void ProbeScope_Init(uint32_t sampling_clk_hz);
void ProbeScope_Sampling(void);
void ProbeScope_SamplingTmrInitHz(uint32_t sampling_clk_hz);
