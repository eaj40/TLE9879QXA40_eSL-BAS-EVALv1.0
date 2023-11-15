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
 * \file Ifx_ComponentID.h
 * \brief Defines the component ID structure.
 */

#ifndef IFX_COMPONENTID_H
#define IFX_COMPONENTID_H
#include "types.h"

/**
 * \brief Data structure that stores all data of module instance.
 *
 */
typedef struct Ifx_ComponentID
{
    /**
     * Source ID, used to identify the producer
     */
    uint32 sourceID : 3;

    /**
     * Library ID
     */
    uint32 libraryID : 9;

    /**
     * Module ID
     */
    uint32 moduleID : 8;

    /**
     * Component ID I, used to identify hardware or implementation specific implementation
     */
    uint32 componentID1 : 8;

    /**
     * Component ID II, used to hold the marketing option
     */
    uint32 componentID2 : 4;
} Ifx_ComponentID;

/**
 * Available options for Component ID II
 */
typedef enum Ifx_ComponentID_ComponentID2
{
    Ifx_ComponentID_ComponentID2_reserved = 0, /**<Reserved*/
    Ifx_ComponentID_ComponentID2_eco      = 1, /**<Eco*/
    Ifx_ComponentID_ComponentID2_basic    = 2, /**<Basic*/
    Ifx_ComponentID_ComponentID2_advanced = 3  /**<Advanced*/
} Ifx_ComponentID_ComponentID2;

/**
 * Available options for library ID
 */
typedef enum Ifx_ComponentID_LibraryID
{
    Ifx_ComponentID_LibraryID_lld                      = 0, /**<Low level driver*/
    Ifx_ComponentID_LibraryID_mctrlHardwareAbstraction = 1, /**<MCTRL Hardware Abstraction*/
    Ifx_ComponentID_LibraryID_math                     = 2, /**<Math*/
    Ifx_ComponentID_LibraryID_mctrlDriveAlgorithm      = 3, /**<MCTRL Drive Algorithm*/
    Ifx_ComponentID_LibraryID_mctrlActuatorSensor      = 4, /**<MCTRL Actuator Sensor*/
    Ifx_ComponentID_LibraryID_mctrlSolution            = 5  /**<MCTRL Solution*/
} Ifx_ComponentID_LibraryID;

/**
 * Available options for source ID
 */
typedef enum Ifx_ComponentID_SourceID
{
    Ifx_ComponentID_SourceID_infineonTechnologiesAG = 0, /**<Infineon Technologies AG*/
    Ifx_ComponentID_SourceID_reservedForInfineon1   = 1, /**<Reserved (for Infineon)*/
    Ifx_ComponentID_SourceID_reservedForInfineon2   = 2, /**<Reserved (for Infineon)*/
    Ifx_ComponentID_SourceID_partlyReuseOf0         = 3, /**<Partly Reuse of 0*/
    Ifx_ComponentID_SourceID_customizedDriver1      = 4, /**<Customized Driver 1*/
    Ifx_ComponentID_SourceID_reservedForCustomer1   = 5, /**<Reserved (for customer)*/
    Ifx_ComponentID_SourceID_reservedForCustomer2   = 6, /**<Reserved (for customer)*/
    Ifx_ComponentID_SourceID_reservedForCustomer3   = 7  /**<Reserved (for customer)*/
} Ifx_ComponentID_SourceID;

#endif /*IFX_COMPONENTID_H*/
