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
 * \file Ifx_ComponentVersion.h
 * \brief Defines the component version structure.
 */

#ifndef IFX_COMPONENTVERSION_H
#define IFX_COMPONENTVERSION_H
#include "types.h"

/**
 * \brief Data structure that stores all data of module instance.
 *
 */
typedef struct Ifx_ComponentVersion
{
    /**
     * Major version
     */
    uint32 major : 16;

    /**
     * Minor version
     */
    uint32 minor : 16;

    /**
     * Patch number
     */
    uint32 patch : 16;

    /**
     * Development stage
     */
    uint32 t : 5;

    /**
     * Revision number
     */
    uint32 rev : 11;
} Ifx_ComponentVersion;

#endif /*IFX_COMPONENTVERSION_H*/
