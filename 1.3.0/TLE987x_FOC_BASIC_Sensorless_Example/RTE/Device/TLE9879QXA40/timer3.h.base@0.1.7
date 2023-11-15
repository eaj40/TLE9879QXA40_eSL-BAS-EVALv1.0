/*
 ***********************************************************************************************************************
 *
 * Copyright (c) 2015, Infineon Technologies AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,are permitted provided that the
 * following conditions are met:
 *
 *   Redistributions of source code must retain the above copyright notice, this list of conditions and the  following
 *   disclaimer.
 *
 *   Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *   following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *   Neither the name of the copyright holders nor the names of its contributors may be used to endorse or promote
 *   products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT  OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 **********************************************************************************************************************/
/**
 * \file     timer3.h
 *
 * \brief    Timer3 low level access library
 *
 * \version  V0.1.7
 * \date     21. Jul 2020
 */

/*******************************************************************************
**                             Author(s) Identity                             **
********************************************************************************
** Initials     Name                                                          **
** ---------------------------------------------------------------------------**
** DM           Daniel Mysliwitz                                              **
** DCM          Dragos C. Molocea                                             **
** BG           Blandine Guillot                                              **
** JO           Julia Ott                                                     **
*******************************************************************************/

/*******************************************************************************
**                          Revision Control History                          **
********************************************************************************
** V0.1.0: 2014-05-12, DM:   Initial version                                  **
** V0.1.1: 2015-02-10, DM:   Individual header file added                     **
** V0.1.2: 2017-02-22, DM:   Macros/functions for bit access added            **
** V0.1.3: 2017-10-20, DM:   MISRA 2012 compliance, the following PC-Lint     **
**                           rules are globally deactivated:                  **
**                           - Info 793: ANSI/ISO limit of 6 'significant     **
**                             characters in an external identifier           **
**                           - Info 835: A zero has been given as right       **
**                             argument to operator                           **
**                           - Info 845: The left argument to operator '&'    **
**                             is certain to be 0                             **
** V0.1.4: 2017-07-09, BG:   TIMER3_ReadRequest_En, TIMER3_ReadRequest_Dis,   **
**                           TIMER3_CountReady_Sts, TIMER3_HighByte_Ovf_Sts,  **
**                           TIMER3_LowByte_Ovf_Sts, TIMER3_T3L_Start,        **
**                           TIMER3_T3L_Stop, TIMER3_T3HL_Start,              **
**                           TIMER3_T3HL_Stop and                             **
**                           TIMER3_Mode3b_Trigger_Select(TIMER3_TRIGG_IN_SEL)**
**                           rewritten for unit tests                         **
** V0.1.5: 2018-11-27, JO:   Doxygen update                                   **
**                           Moved revision history from timer3.c to timer3.h **
** V0.1.6: 2020-04-15, BG:   Updated revision history format                  **
** V0.1.7: 2020-07-21, BG:   EP-439: Formatted .h/.c files                    **
*******************************************************************************/

#ifndef TIMER3_H
#define TIMER3_H

/*******************************************************************************
**                                  Includes                                  **
*******************************************************************************/
#include "tle987x.h"
#include "types.h"
#include "sfr_access.h"

/*******************************************************************************
**                           Global Type Definitions                          **
*******************************************************************************/
/**\brief Timer3 Mode Flag macro, MODE0 */
#define TIMER3_MODE0 (0u)
/**\brief Timer3 Mode Flag macro, MODE1 */
#define TIMER3_MODE1 (1u)
/**\brief Timer3 Mode Flag macro, MODE2 */
#define TIMER3_MODE2 (2u)
/**\brief Timer3 Mode Flag macro, MODE3 */
#define TIMER3_MODE3 (3u)

/** \enum TTIMER3_TRIGG_IN_SEL
 *  \brief This enum lists TIMER3 Trigger Source Configuration.
 */
typedef enum
{
  TIMER3_CCU6_CC60   = 0u, /**< Timer3 Trigger Input Event Select: CCU6-CC60 */
  TIMER3_CCU6_CC61   = 1u, /**< Timer3 Trigger Input Event Select: CCU6-CC61 */
  TIMER3_CCU6_CC62   = 2u, /**< Timer3 Trigger Input Event Select: CCU6-CC62 */
  TIMER3_CCU6_ZM     = 3u, /**< Timer3 Trigger Input Event Select: CCU6-T12 ZM */
  TIMER3_CCU6_PM     = 4u, /**< Timer3 Trigger Input Event Select: CCU6-T12 ZM */
  TIMER3_CCU6_COUT60 = 5u, /**< Timer3 Trigger Input Event Select: CCU6-COUT60 */
  TIMER3_CCU6_COUT61 = 6u, /**< Timer3 Trigger Input Event Select: CCU6-COUT61 */
  TIMER3_CCU6_COUT62 = 7u  /**< Timer3 Trigger Input Event Select: CCU6-COUT62 */
} TTIMER3_TRIGG_IN_SEL;

/** \enum TTIMER3_Clk_Source
 *  \brief This enum lists TIMER3 Clock Source Configuration.
 */
typedef enum
{
  TIMER3_LP_CLK = 0u, /**< Timer3 Clock Source Select: LP_CLK */
  TIMER3_MI_CLK = 1u  /**< Timer3 Clock Source Select: MI_CLK */
} TTIMER3_Clk_Source;

/** \enum TTIMER3_Clk_Prescaler
 *  \brief This enum lists TIMER3 Clock Prescaler Configuration.
 */
typedef enum
{
  TIMER3_Div_1 = 0u, /**< Timer3 Input Clock Select: fsys */
  TIMER3_Div_2 = 1u, /**< Timer3 Input Clock Select: fsys/2 */
  TIMER3_Div_4 = 2u, /**< Timer3 Input Clock Select: fsys/4 */
  TIMER3_Div_8 = 3u  /**< Timer3 Input Clock Select: fsys/8 */
} TTIMER3_Clk_Prescaler;

/******************************************************************************
**                        Global Function Definitions                        **
*******************************************************************************/
/** \brief Enables the Timer3 module.
 *
 * \brief <b>Example</b><br>
 * \brief This example enables the Timer3 module.
 * ~~~~~~~~~~~~~~~{.c}
 * void Example_Function(void)
 * {
 *     TIMER3_Power_On();
 * }
 * ~~~~~~~~~~~~~~~
 * \ingroup timer3_api
 */
INLINE void TIMER3_Power_On(void)
{
  Field_Mod32(&TIMER3->CTRL.reg, (uint32)TIMER3_CTRL_T3_PD_N_Pos, (uint32)TIMER3_CTRL_T3_PD_N_Msk, 1u);
}

/** \brief Disables the Timer3 module.
 *
 * \brief <b>Example</b><br>
 * \brief This example disables the Timer3 module.
 * ~~~~~~~~~~~~~~~{.c}
 * void Example_Function(void)
 * {
 *     TIMER3_Power_Off();
 * }
 * ~~~~~~~~~~~~~~~
 * \ingroup timer3_api
 */
INLINE void TIMER3_Power_Off(void)
{
  Field_Mod32(&TIMER3->CTRL.reg, (uint32)TIMER3_CTRL_T3_PD_N_Pos, (uint32)TIMER3_CTRL_T3_PD_N_Msk, 0u);
}

/** \brief enables reading from Timer3.
 *
 * \brief <b>Example</b><br>
 * \brief This example enables reading from Timer3.
 * ~~~~~~~~~~~~~~~{.c}
 * void Example_Function(void)
 * {
 *     TIMER3_Power_On();
 *     TIMER3_ReadRequest_Sw_Set();
 *     TIMER3_ReadRequest_En();
 * }
 * ~~~~~~~~~~~~~~~
 * \ingroup timer3_api
 */
INLINE void TIMER3_ReadRequest_En(void)
{
  Field_Mod32(&TIMER3->CTRL.reg, (uint32)TIMER3_CTRL_T3_RD_REQ_Pos, (uint32)TIMER3_CTRL_T3_RD_REQ_Msk, 1u);
}

/** \brief disables reading from Timer3.
 *
 * \brief <b>Example</b><br>
 * \brief This example disables reading from Timer3.
 * ~~~~~~~~~~~~~~~{.c}
 * void Example_Function(void)
 * {
 *     TIMER3_Power_On();
 *     TIMER3_ReadRequest_Dis();
 * }
 * ~~~~~~~~~~~~~~~
 * \ingroup timer3_api
 */
INLINE void TIMER3_ReadRequest_Dis(void)
{
  Field_Mod32(&TIMER3->CTRL.reg, (uint32)TIMER3_CTRL_T3_RD_REQ_Pos, (uint32)TIMER3_CTRL_T3_RD_REQ_Msk, 0u);
}

/** \brief enables triggering Timer3 Read Request by Hardware.
 *
 * \brief <b>Example</b><br>
 * \brief This example enables triggering TIMER3 Read Request by Hardware.
 * ~~~~~~~~~~~~~~~{.c}
 * void Example_Function(void)
 * {
 *     TIMER3_Power_On();
 *     TIMER3_ReadRequest_Hw_Set();
 * }
 * ~~~~~~~~~~~~~~~
 * \ingroup timer3_api
 */
INLINE void TIMER3_ReadRequest_Hw_Set(void)
{
  Field_Mod32(&TIMER3->CTRL.reg, (uint32)TIMER3_CTRL_T3_RD_REQ_CONF_Pos, (uint32)TIMER3_CTRL_T3_RD_REQ_CONF_Msk, 1u);
}

/** \brief enables triggering Timer3 Read Request by Software.
 *
 * \brief <b>Example</b><br>
 * \brief This example enables reading from TIMER3.
 * ~~~~~~~~~~~~~~~{.c}
 * void Example_Function(void)
 * {
 *     TIMER3_Power_On();
 *     TIMER3_ReadRequest_Sw_Set();
 *     TIMER3_ReadRequest_En();
 * }
 * ~~~~~~~~~~~~~~~
 * \ingroup timer3_api
 */
INLINE void TIMER3_ReadRequest_Sw_Set(void)
{
  Field_Mod32(&TIMER3->CTRL.reg, (uint32)TIMER3_CTRL_T3_RD_REQ_CONF_Pos, (uint32)TIMER3_CTRL_T3_RD_REQ_CONF_Msk, 0u);
}

/** \brief Reads the Timer3 Count Ready Status.
 *
 * \retval 1 Timer has finished counting
 * \retval 0 Timer hasn’t finished counting
 *
 * \brief <b>Example</b><br>
 * \brief This example reads the Timer3 Count Ready Status.
 * ~~~~~~~~~~~~~~~{.c}
 * void Example_Function(void)
 * {
 *     uint8 sts;
 *
 *     TIMER3_Power_On();
 *     sts = TIMER3_CountReady_Sts();
 * }
 * ~~~~~~~~~~~~~~~
 * \ingroup timer3_api
 */
INLINE uint8 TIMER3_CountReady_Sts(void)
{
  return ( u1_Field_Rd32(&TIMER3->CTRL.reg, (uint32)TIMER3_CTRL_CNT_RDY_Pos, (uint32)TIMER3_CTRL_CNT_RDY_Msk) );
}

/** \brief Reads the Timer3 Overflow Flag (High Byte Timer).
 *
 * \retval 1 No Overflow is occurred.
 * \retval 0 Overflow is occurred.
 *
 * \brief <b>Example</b><br>
 * \brief This example treats the Timer 3 Overflow Interrupt (High Byte Timer).
 * ~~~~~~~~~~~~~~~{.c}
 * void Example_Function(void)
 * {
 *   TIMER3_Power_On();
 *   TIMER3_T3H_Start();
 *   TIMER3_HB_OF_Int_En();
 *   if (TIMER3_HighByte_Ovf_Sts() == (uint8)1)
 *   {
 *     TIMER3_HBOF_CALLBACK();
 *     TIMER3_HB_OF_Int_Clr();
 *   }
 * }
 * ~~~~~~~~~~~~~~~
 * \ingroup timer3_api
 */
INLINE uint8 TIMER3_HighByte_Ovf_Sts(void)
{
  return ( u1_Field_Rd32(&TIMER3->CTRL.reg, (uint32)TIMER3_CTRL_T3H_OVF_STS_Pos, (uint32)TIMER3_CTRL_T3H_OVF_STS_Msk) );
}

/** \brief Reads the Timer3 Overflow Flag (Low Byte Timer).
 *
 * \retval 1 No Overflow is occurred.
 * \retval 0 Overflow is occurred.
 *
 * \brief <b>Example</b><br>
 * \brief This example treats the Timer 3 Overflow Interrupt (Low Byte Timer).
 * ~~~~~~~~~~~~~~~{.c}
 * void Example_Function(void)
 * {
 *   TIMER3_Power_On();
 *   TIMER3_T3L_Start();
 *   TIMER3_LB_OF_Int_En();
 *   if (TIMER3_LowByte_Ovf_Sts() == (uint8)1)
 *   {
 *     TIMER3_LBOF_CALLBACK();
 *     TIMER3_LB_OF_Int_Clr();
 *   }
 * }
 * ~~~~~~~~~~~~~~~
 * \ingroup timer3_api
 */
INLINE uint8 TIMER3_LowByte_Ovf_Sts(void)
{
  return ( u1_Field_Rd32(&TIMER3->CTRL.reg, (uint32)TIMER3_CTRL_T3L_OVF_STS_Pos, (uint32)TIMER3_CTRL_T3L_OVF_STS_Msk) );
}

/** \brief starts running Timer3 (High Byte Timer)
 *
 * \brief <b>Example</b><br>
 * \brief This example treats the Timer 3 Overflow Interrupt (High Byte Timer).
 * ~~~~~~~~~~~~~~~{.c}
 * void Example_Function(void)
 * {
 *   TIMER3_Power_On();
 *   TIMER3_T3H_Start();
 *   TIMER3_HB_OF_Int_En();
 *   if (TIMER3_HighByte_Ovf_Sts() == (uint8)1)
 *   {
 *     TIMER3_HBOF_CALLBACK();
 *     TIMER3_HB_OF_Int_Clr();
 *   }
 * }
 * ~~~~~~~~~~~~~~~
 * \ingroup timer3_api
 */
INLINE void TIMER3_T3H_Start(void)
{
  Field_Mod32(&TIMER3->CTRL.reg, (uint32)TIMER3_CTRL_TR3H_Pos, (uint32)TIMER3_CTRL_TR3H_Msk, 1u);
}

/** \brief stops running Timer3 (High Byte Timer)
 *
 * \brief <b>Example</b><br>
 * \brief This example stops running Timer3 (High Byte Timer).
 * ~~~~~~~~~~~~~~~{.c}
 * void Example_Function(void)
 * {
 *     TIMER3_Power_On();
 *     TIMER3_T3H_Stop();
 * }
 * ~~~~~~~~~~~~~~~
 * \ingroup timer3_api
 */
INLINE void TIMER3_T3H_Stop(void)
{
  Field_Mod32(&TIMER3->CTRL.reg, (uint32)TIMER3_CTRL_TR3H_Pos, (uint32)TIMER3_CTRL_TR3H_Msk, 0u);
}

/** \brief starts running Timer3 (Low Byte Timer)
 *
 * \retval 1 if No Overflow is occurred.
 * \retval 0 if Overflow is occurred.
 *
 * \brief <b>Example</b><br>
 * \brief This example treats the Timer 3 Overflow Interrupt (Low Byte Timer).
 * ~~~~~~~~~~~~~~~{.c}
 * void Example_Function(void)
 * {
 *   TIMER3_Power_On();
 *   TIMER3_T3L_Start();
 *   TIMER3_LB_OF_Int_En();
 *   if (TIMER3_LowByte_Ovf_Sts() == (uint8)1)
 *   {
 *     TIMER3_LBOF_CALLBACK();
 *     TIMER3_LB_OF_Int_Clr();
 *   }
 * }
 * ~~~~~~~~~~~~~~~
 * \ingroup timer3_api
 */
INLINE void TIMER3_T3L_Start(void)
{
  Field_Mod32(&TIMER3->CTRL.reg, (uint32)TIMER3_CTRL_TR3L_Pos, (uint32)TIMER3_CTRL_TR3L_Msk, 1u);
}

/** \brief stops running Timer3 (Low Byte Timer)
 *
 * \brief <b>Example</b><br>
 * \brief This example stops running Timer3 (Low Byte Timer).
 * ~~~~~~~~~~~~~~~{.c}
 * void Example_Function(void)
 * {
 *     TIMER3_Power_On();
 *     TIMER3_T3L_Stop();
 * }
 * ~~~~~~~~~~~~~~~
 * \ingroup timer3_api
 */
INLINE void TIMER3_T3L_Stop(void)
{
  Field_Mod32(&TIMER3->CTRL.reg, (uint32)TIMER3_CTRL_TR3L_Pos, (uint32)TIMER3_CTRL_TR3L_Msk, 0u);
}

/** \brief starts running Timer3 (High and Low Byte Timer)
 *
 * \brief <b>Example</b><br>
 * \brief This example starts running Timer3 (High and Low Byte Timer).
 * ~~~~~~~~~~~~~~~{.c}
 * void Example_Function(void)
 * {
 *     TIMER3_Power_On();
 *     TIMER3_T3HL_Start();
 * }
 * ~~~~~~~~~~~~~~~
 * \ingroup timer3_api
 */
INLINE void TIMER3_T3HL_Start(void)
{
  Field_Mod32(&TIMER3->CTRL.reg, (uint32)TIMER3_CTRL_TR3H_Pos, (TIMER3_CTRL_TR3H_Msk | TIMER3_CTRL_TR3L_Msk),  5u);
}

/** \brief stops running Timer3 (High and Low Byte Timer)
 *
 * \brief <b>Example</b><br>
 * \brief This example stops running Timer3 (High and Low Byte Timer).
 * ~~~~~~~~~~~~~~~{.c}
 * void Example_Function(void)
 * {
 *     TIMER3_Power_On();
 *     TIMER3_T3HL_Stop();
 * }
 * ~~~~~~~~~~~~~~~
 * \ingroup timer3_api
 */
INLINE void TIMER3_T3HL_Stop(void)
{
  Field_Mod32(&TIMER3->CTRL.reg, (uint32)TIMER3_CTRL_TR3H_Pos, (TIMER3_CTRL_TR3H_Msk | TIMER3_CTRL_TR3L_Msk),  0u);
}

/** \brief enables re-trigging for CCU6-T12 ZM and CCU6-T12 PM.
 *
 * \brief <b>Example</b><br>
 * \brief This example enables re-trigging for CCU6-T12 ZM and CCU6-T12 PM.
 * ~~~~~~~~~~~~~~~{.c}
 * void Example_Function(void)
 * {
 *     TIMER3_Power_On();
 *     TIMER3_Retrigger_En();
 * }
 * ~~~~~~~~~~~~~~~
 * \ingroup timer3_api
 */
INLINE void TIMER3_Retrigger_En(void)
{
  Field_Mod32(&TIMER3->T3_TRIGG_CTRL.reg, (uint32)TIMER3_T3_TRIGG_CTRL_RETRIG_Pos, (uint32)TIMER3_T3_TRIGG_CTRL_RETRIG_Msk, 1u);
}

/** \brief disables re-trigging for CCU6-T12 ZM and CCU6-T12 PM.
 *
 * \brief <b>Example</b><br>
 * \brief This example disables re-trigging for CCU6-T12 ZM and CCU6-T12 PM.
 * ~~~~~~~~~~~~~~~{.c}
 * void Example_Function(void)
 * {
 *     TIMER3_Power_On();
 *     TIMER3_Retrigger_Dis();
 * }
 * ~~~~~~~~~~~~~~~
 * \ingroup timer3_api
 */
INLINE void TIMER3_Retrigger_Dis(void)
{
  Field_Mod32(&TIMER3->T3_TRIGG_CTRL.reg, (uint32)TIMER3_T3_TRIGG_CTRL_RETRIG_Pos, (uint32)TIMER3_T3_TRIGG_CTRL_RETRIG_Msk, 0u);
}

/** \brief enables Timer 3 Trigger Reset On Rising Edge for Mode 1b.
 *
 * \brief <b>Example</b><br>
 * \brief This example enables Timer 3 Trigger Reset On Rising Edge for Mode 1b.
 * ~~~~~~~~~~~~~~~{.c}
 * void Example_Function(void)
 * {
 *     TIMER3_Power_On();
 *     TIMER3_Mode1b_Reset_On_Rising_Edge_En();
 * }
 * ~~~~~~~~~~~~~~~
 * \ingroup timer3_api
 */
INLINE void TIMER3_Mode1b_Reset_On_Rising_Edge_En(void)
{
  Field_Mod32(&TIMER3->T3_TRIGG_CTRL.reg, 4u, 0x10u, 1u);
}

/** \brief disables Timer 3 Trigger Reset On Rising Edge for Mode 1b.
 *
 * \brief <b>Example</b><br>
 * \brief This example disables Timer 3 Trigger Reset On Rising Edge for Mode 1b.
 * ~~~~~~~~~~~~~~~{.c}
 * void Example_Function(void)
 * {
 *     TIMER3_Power_On();
 *     TIMER3_Mode1b_Reset_On_Rising_Edge_Dis();
 * }
 * ~~~~~~~~~~~~~~~
 * \ingroup timer3_api
 */
INLINE void TIMER3_Mode1b_Reset_On_Rising_Edge_Dis(void)
{
  Field_Mod32(&TIMER3->T3_TRIGG_CTRL.reg, 4u, 0x10u, 0u);
}

/** \brief enables Timer 3 Trigger Reset On Falling Edge for Mode 1b.
 *
 * \brief <b>Example</b><br>
 * \brief This example enables Timer 3 Trigger Reset On Falling Edge for Mode 1b.
 * ~~~~~~~~~~~~~~~{.c}
 * void Example_Function(void)
 * {
 *     TIMER3_Power_On();
 *     TIMER3_Mode1b_Reset_On_Falling_Edge_En();
 * }
 * ~~~~~~~~~~~~~~~
 * \ingroup timer3_api
 */
INLINE void TIMER3_Mode1b_Reset_On_Falling_Edge_En(void)
{
  Field_Mod32(&TIMER3->T3_TRIGG_CTRL.reg, 5u, 0x20u, 1u);
}

/** \brief disables Timer 3 Trigger Reset On Falling Edge for Mode 1b.
 *
 * \brief <b>Example</b><br>
 * \brief This example disables Timer 3 Trigger Reset On Falling Edge for Mode 1b.
 * ~~~~~~~~~~~~~~~{.c}
 * void Example_Function(void)
 * {
 *     TIMER3_Power_On();
 *     TIMER3_Mode1b_Reset_On_Falling_Edge_Dis();
 * }
 * ~~~~~~~~~~~~~~~
 * \ingroup timer3_api
 */
INLINE void TIMER3_Mode1b_Reset_On_Falling_Edge_Dis(void)
{
  Field_Mod32(&TIMER3->T3_TRIGG_CTRL.reg, 5u, 0x20u, 0u);
}

/** \brief Sets TIMER3 Value.
 *
 * \param t3 TIMER3 Value
 *
 * \brief <b>Example</b><br>
 * \brief This example sets TIMER3 Value to 5000.
 * ~~~~~~~~~~~~~~~{.c}
 * void Example_Function(void)
 * {
 *     TIMER3_Value_Set(0x1388);
 * }
 * ~~~~~~~~~~~~~~~
 * \ingroup timer3_api
 */
INLINE void TIMER3_Value_Set(uint32 t3)
{
  Field_Wrt32(&TIMER3->CNT.reg, 0u, 0xFFFFu, t3);
}

/** \brief enables Timer 3 Overflow Interrupt (High Byte Timer).
 *
 * \brief <b>Example</b><br>
 * \brief This example treats the Timer 3 Overflow Interrupt (High Byte Timer).
 * ~~~~~~~~~~~~~~~{.c}
 * void Example_Function(void)
 * {
 *   TIMER3_Power_On();
 *   TIMER3_T3H_Start();
 *   TIMER3_HB_OF_Int_En();
 *   if (TIMER3_HighByte_Ovf_Sts() == (uint8)1)
 *   {
 *     TIMER3_HBOF_CALLBACK();
 *     TIMER3_HB_OF_Int_Clr();
 *   }
 * }
 * ~~~~~~~~~~~~~~~
 * \ingroup timer3_api
 */
INLINE void TIMER3_HB_OF_Int_En(void)
{
  Field_Mod32(&TIMER3->CTRL.reg, (uint32)TIMER3_CTRL_T3H_OVF_IE_Pos, (uint32)TIMER3_CTRL_T3H_OVF_IE_Msk, 1u);
}

/** \brief disables Timer 3 Overflow Interrupt (High Byte Timer).
 *
 * \brief <b>Example</b><br>
 * \brief This example treats the Timer 3 Overflow Interrupt (High Byte Timer).
 * ~~~~~~~~~~~~~~~{.c}
 * void Example_Function(void)
 * {
 *   TIMER3_Power_On();
 *   TIMER3_T3H_Start();
 *   TIMER3_HB_OF_Int_En();
 *   if (TIMER3_HighByte_Ovf_Sts() == (uint8)1)
 *   {
 *     TIMER3_HBOF_CALLBACK();
 *     TIMER3_HB_OF_Int_Clr();
 *   }
 *   TIMER3_HB_OF_Int_Dis();
 * }
 * ~~~~~~~~~~~~~~~
 * \ingroup timer3_api
 */
INLINE void TIMER3_HB_OF_Int_Dis(void)
{
  Field_Mod32(&TIMER3->CTRL.reg, (uint32)TIMER3_CTRL_T3H_OVF_IE_Pos, (uint32)TIMER3_CTRL_T3H_OVF_IE_Msk, 0u);
}

/** \brief enables Timer 3 Overflow Interrupt (Low Byte Timer).
 *
 * \brief <b>Example</b><br>
 * \brief This example treats the Timer 3 Overflow Interrupt (Low Byte Timer).
 * ~~~~~~~~~~~~~~~{.c}
 * void Example_Function(void)
 * {
 *   TIMER3_Power_On();
 *   TIMER3_T3L_Start();
 *   TIMER3_LB_OF_Int_En();
 *   if (TIMER3_LowByte_Ovf_Sts() == (uint8)1)
 *   {
 *     TIMER3_LBOF_CALLBACK();
 *     TIMER3_LB_OF_Int_Clr();
 *   }
 * }
 * ~~~~~~~~~~~~~~~
 * \ingroup timer3_api
 */
INLINE void TIMER3_LB_OF_Int_En(void)
{
  Field_Mod32(&TIMER3->CTRL.reg, (uint32)TIMER3_CTRL_T3L_OVF_IE_Pos, (uint32)TIMER3_CTRL_T3L_OVF_IE_Msk, 1u);
}

/** \brief disables Timer 3 Overflow Interrupt (Low Byte Timer).
 *
 * \brief <b>Example</b><br>
 * \brief This example treats the Timer 3 Overflow Interrupt (Low Byte Timer).
 * ~~~~~~~~~~~~~~~{.c}
 * void Example_Function(void)
 * {
 *   TIMER3_Power_On();
 *   TIMER3_T3L_Start();
 *   TIMER3_LB_OF_Int_En();
 *   if (TIMER3_LowByte_Ovf_Sts() == (uint8)1)
 *   {
 *     TIMER3_LBOF_CALLBACK();
 *     TIMER3_LB_OF_Int_Clr();
 *   }
 *   TIMER3_LB_OF_Int_Dis();
 * }
 * ~~~~~~~~~~~~~~~
 * \ingroup timer3_api
 */
INLINE void TIMER3_LB_OF_Int_Dis(void)
{
  Field_Mod32(&TIMER3->CTRL.reg, (uint32)TIMER3_CTRL_T3L_OVF_IE_Pos, (uint32)TIMER3_CTRL_T3L_OVF_IE_Msk, 0u);
}

/** \brief clears Timer 3 Overflow Interrupt flag (High Byte Timer).
 *
 * \brief <b>Example</b><br>
 * \brief This example treats the Timer 3 Overflow Interrupt (High Byte Timer).
 * ~~~~~~~~~~~~~~~{.c}
 * void Example_Function(void)
 * {
 *   TIMER3_Power_On();
 *   TIMER3_T3H_Start();
 *   TIMER3_HB_OF_Int_En();
 *   if (TIMER3_HighByte_Ovf_Sts() == (uint8)1)
 *   {
 *     TIMER3_HBOF_CALLBACK();
 *     TIMER3_HB_OF_Int_Clr();
 *   }
 * }
 * ~~~~~~~~~~~~~~~
 * \ingroup timer3_api
 */
INLINE void TIMER3_HB_OF_Int_Clr(void)
{
  Field_Wrt32(&TIMER3->ISRCLR.reg, (uint32)TIMER3_ISRCLR_T3H_OVF_ICLR_Pos, (uint32)TIMER3_ISRCLR_T3H_OVF_ICLR_Msk, 1u);
}

/** \brief clears Timer 3 Overflow Interrupt flag (Low Byte Timer).
 *
 * \brief <b>Example</b><br>
 * \brief This example treats the Timer 3 Overflow Interrupt (Low Byte Timer).
 * ~~~~~~~~~~~~~~~{.c}
 * void Example_Function(void)
 * {
 *   TIMER3_Power_On();
 *   TIMER3_T3L_Start();
 *   TIMER3_LB_OF_Int_En();
 *   if (TIMER3_LowByte_Ovf_Sts() == (uint8)1)
 *   {
 *     TIMER3_LBOF_CALLBACK();
 *     TIMER3_LB_OF_Int_Clr();
 *   }
 * }
 * ~~~~~~~~~~~~~~~
 * \ingroup timer3_api
 */
INLINE void TIMER3_LB_OF_Int_Clr(void)
{
  Field_Wrt32(&TIMER3->ISRCLR.reg, (uint32)TIMER3_ISRCLR_T3L_OVF_ICLR_Pos, (uint32)TIMER3_ISRCLR_T3L_OVF_ICLR_Msk, 1u);
}

/*******************************************************************************
**                        Global Function Declarations                        **
*******************************************************************************/
/** \brief Initializes the Timer3 module.
 *
 * \ingroup timer3_api
 */
void TIMER3_Init(void);

/** \brief Starts the Timer3 module.
 *
 * \brief <b>Example</b><br>
 * \brief This example starts Timer 3.
 * ~~~~~~~~~~~~~~~{.c}
 * void Example_Function(void)
 * {
 *   TIMER3_Start();
 * }
 * ~~~~~~~~~~~~~~~
 * \ingroup timer3_api
 */
void TIMER3_Start(void);

/** \brief Stops the Timer3 module.
 *
 * \brief <b>Example</b><br>
 * \brief This example stops Timer 3.
 * ~~~~~~~~~~~~~~~{.c}
 * void Example_Function(void)
 * {
 *   TIMER3_Stop();
 * }
 * ~~~~~~~~~~~~~~~
 * \ingroup timer3_api
 */
void TIMER3_Stop(void);

/** \brief Set the Timer3 counter value.
 *
 * \param Cnt_Value 16bit count value
 *
 * \brief <b>Example</b><br>
 * \brief This example sets TIMER3 Counter Value to 5000.
 * ~~~~~~~~~~~~~~~{.c}
 * void Example_Function(void)
 * {
 *     TIMER3_Set_Cnt_Value(0x1388);
 * }
 * ~~~~~~~~~~~~~~~
 * \ingroup timer3_api
 */
void TIMER3_Set_Cnt_Value(uint16 Cnt_Value);

/** \brief Set the Timer3 compare value.
 *
 * \param Cmp_Value 16bit compare value
 *
 * \brief <b>Example</b><br>
 * \brief This example sets TIMER3 Compare Value to 5000.
 * ~~~~~~~~~~~~~~~{.c}
 * void Example_Function(void)
 * {
 *     TIMER3_Set_Cmp_Value(0x1388);
 * }
 * ~~~~~~~~~~~~~~~
 * \ingroup timer3_api
 */
void TIMER3_Set_Cmp_Value(uint16 Cmp_Value);

/** \brief Get the Timer3 counter value.
 *
 * \return 16bit count value
 *
 * \brief <b>Example</b><br>
 * \brief This example reads the TIMER3 Counter Value.
 * ~~~~~~~~~~~~~~~{.c}
 * void Example_Function(void)
 * {
 *     uint16 val;
 *
 *     val = TIMER3_Get_Cnt_Value();
 * }
 * ~~~~~~~~~~~~~~~
 * \ingroup timer3_api
 */
uint16 TIMER3_Get_Cnt_Value(void);

/** \brief Get the Timer3 counter value.
 *
 * \return 16bit count value
 *
 * \brief <b>Example</b><br>
 * \brief This example reads the TIMER3 Counter Value.
 * ~~~~~~~~~~~~~~~{.c}
 * void Example_Function(void)
 * {
 *     uint16 val;
 *
 *     val = TIMER3_Value_Get();
 * }
 * ~~~~~~~~~~~~~~~
 * \ingroup timer3_api
 */
uint16 TIMER3_Value_Get(void);

/** \brief Set the Timer3 clock source.
 *
 * \param t3c Clock source
 *
 * \brief <b>Example</b><br>
 * \brief This example sets TIMER3 Clock Source to MI_CLK.
 * ~~~~~~~~~~~~~~~{.c}
 * void Example_Function(void)
 * {
 *     TIMER3_Set_Cmp_Value(TIMER3_MI_CLK);
 * }
 * ~~~~~~~~~~~~~~~
 * \ingroup timer3_api
 */
void TIMER3_Clk_Source_Sel(TTIMER3_Clk_Source t3c);

/** \brief Set the Timer3 clock prescaler.
 *
 * \param t3div Clock prescaler
 *
 * \brief <b>Example</b><br>
 * \brief This example sets TIMER3 Clock Prescaler to Div_8.
 * ~~~~~~~~~~~~~~~{.c}
 * void Example_Function(void)
 * {
 *     TIMER3_Clk_Prescaler_Sel(TIMER3_Div_8);
 * }
 * ~~~~~~~~~~~~~~~
 * \ingroup timer3_api
 */
void TIMER3_Clk_Prescaler_Sel(TTIMER3_Clk_Prescaler t3div);


/*******************************************************************************
**                     Global Inline Function Declarations                    **
*******************************************************************************/

INLINE void TIMER3_Mode3b_Trigger_Select(TTIMER3_TRIGG_IN_SEL insel);
INLINE bool TIMER3_isCountReady(void);
INLINE bool TIMER3_isHighByteOverflow(void);
INLINE bool TIMER3_isLowByteOverflow(void);

/*******************************************************************************
**                     Global Inline Function Definitions                     **
*******************************************************************************/

/** \brief sets TIMER3 Trigger Input Selection
 *
 * \param insel Trigger select, TTIMER3_TRIGG_IN_SEL
 *
 * \brief <b>Example</b><br>
 * \brief This example sets Capture Compare Unit Channel 0 (CC60) as TIMER3 Trigger Input Selection.
 * ~~~~~~~~~~~~~~~{.c}
 * void Example_Function(void)
 * {
 *   TIMER3_Mode3b_Trigger_Select(TIMER3_CCU6_CC60);
 * }
 * ~~~~~~~~~~~~~~~
 * \ingroup TIMER3_api
 */
INLINE void TIMER3_Mode3b_Trigger_Select(TTIMER3_TRIGG_IN_SEL insel)
{
  Field_Mod32(&TIMER3->T3_TRIGG_CTRL.reg, TIMER3_T3_TRIGG_CTRL_T3_TRIGG_INP_SEL_Pos, TIMER3_T3_TRIGG_CTRL_T3_TRIGG_INP_SEL_Msk, (uint8)insel);
}

/** \brief checks Timer3 count ready (Mode 1b, 3b)
 *
 * \retval TRUE Timer has finished counting
 * \retval FALSE Timer hasn’t finished counting
 *
 * \brief <b>Example</b><br>
 * \brief This example reads the Timer3 Count Ready Status.
 * ~~~~~~~~~~~~~~~{.c}
 * void Example_Function(void)
 * {
 *     TIMER3_Power_On();
 *     while (!TIMER3_isCountReady())
 *     {
 *     }
 * }
 * ~~~~~~~~~~~~~~~
 * \ingroup TIMER3_api
 */
INLINE bool TIMER3_isCountReady(void)
{
  bool res = false;

  if (TIMER3_CountReady_Sts() == (uint8)1)
  {
    res = true;
  }

  return (res);
}

/** \brief checks Timer3 High Byte overflow status
 *
 * \retval TRUE overflow status set
 * \retval FALSE overflow status not set
 *
 * \brief <b>Example</b><br>
 * \brief This example treats the Timer 3 Overflow Interrupt (High Byte Timer).
 * ~~~~~~~~~~~~~~~{.c}
 * void Example_Function(void)
 * {
 *   TIMER3_Power_On();
 *   TIMER3_T3H_Start();
 *   TIMER3_HB_OF_Int_En();
 *   if (TIMER3_isHighByteOverflow() == TRUE)
 *   {
 *     TIMER3_HBOF_CALLBACK();
 *     TIMER3_HB_OF_Int_Clr();
 *   }
 * }
 * ~~~~~~~~~~~~~~~
 * \ingroup TIMER3_api
 */
INLINE bool TIMER3_isHighByteOverflow(void)
{
  bool res = false;

  if (TIMER3_HighByte_Ovf_Sts() == (uint8)1)
  {
    res = true;
  }

  return (res);
}

/** \brief checks Timer3 Low Byte overflow status
 *
 * \retval TRUE overflow status set
 * \retval FALSE overflow status not set
 *
 * \brief <b>Example</b><br>
 * \brief This example treats the Timer 3 Overflow Interrupt (Low Byte Timer).
 * ~~~~~~~~~~~~~~~{.c}
 * void Example_Function(void)
 * {
 *   TIMER3_Power_On();
 *   TIMER3_T3L_Start();
 *   TIMER3_LB_OF_Int_En();
 *   if (TIMER3_isLowByteOverflow() == TRUE)
 *   {
 *     TIMER3_LBOF_CALLBACK();
 *     TIMER3_LB_OF_Int_Clr();
 *   }
 * }
 * ~~~~~~~~~~~~~~~
 * \ingroup TIMER3_api
 */
INLINE bool TIMER3_isLowByteOverflow(void)
{
  bool res = false;

  if (TIMER3_LowByte_Ovf_Sts() == (uint8)1)
  {
    res = true;
  }

  return (res);
}

#endif
