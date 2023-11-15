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

/*******************************************************************************
**                                  Includes                                  **
*******************************************************************************/
#include "timer3.h"
#include "scu.h"
#include "cmsis_misra.h"
#include "timer3_defines.h"
#include "sfr_access.h"
#include "RTE_Components.h"

/*******************************************************************************
**                         Global Function Definitions                        **
*******************************************************************************/

void TIMER3_Init(void)
{
#if ((TIMER3_CTRL & 1u) != 0u)
  /* loading of Timer3 registers from configuration */
  TIMER3->CTRL.reg = (uint32) TIMER3_CTRL;
  TIMER3->MODE_CONF.reg = (uint32) TIMER3_MODE_CONF;
#if (CONFIGWIZARD == 1)
  TIMER3->T3_TRIGG_CTRL.reg = (uint32) TIMER3_TRIGG_CTRL;
#else /* (CONFIGWIZARD == 2) */
  TIMER3->T3_TRIGG_CTRL.reg = (uint32) TIMER3_T3_TRIGG_CTRL;
#endif
  TIMER3->CMP.reg = (uint32) TIMER3_CMP;
  TIMER3->CNT.reg = (uint32) TIMER3_T3;
  TIMER3->CTRL.reg = (uint32) TIMER3_CTRL;
  TIMER3->ISRCLR.reg = (uint32) 0xA0;
#endif
}


void TIMER3_Start(void)
{
#if ((TIMER3_MODE_CONF & 3u) < TIMER3_MODE3)
  TIMER3_T3L_Start();
#else
  TIMER3_T3HL_Start();    /* set TR3L and TR3H */
#endif
}


void TIMER3_Stop(void)
{
#if ((TIMER3_MODE_CONF & 3u) < TIMER3_MODE3)
  TIMER3_T3L_Stop();
#else
  TIMER3_T3HL_Stop();   /* clr TR3L and TR3H */
#endif
}


void TIMER3_Set_Cnt_Value(uint16 Cnt_Value)
{
  Field_Wrt32(&TIMER3->CNT.reg, TIMER3_CNT_LO_Pos, (TIMER3_CNT_LO_Msk | TIMER3_CNT_HI_Msk), (uint32) Cnt_Value);
}


void TIMER3_Set_Cmp_Value(uint16 Cmp_Value)
{
  Field_Wrt32(&TIMER3->CMP.reg, TIMER3_CMP_LO_Pos, (TIMER3_CMP_LO_Msk | TIMER3_CMP_HI_Msk), (uint32) Cmp_Value);
}


uint16 TIMER3_Get_Cnt_Value(void)
{
  uint16 val;
  /* enable read request */
  TIMER3_ReadRequest_En();
  /* read out the timer value */
  val = u16_Field_Rd32(&TIMER3->CNT.reg, TIMER3_CNT_LO_Pos, (TIMER3_CNT_LO_Msk | TIMER3_CNT_HI_Msk));
  /* disable read request */
  TIMER3_ReadRequest_Dis();
  return (val);
}


uint16 TIMER3_Value_Get(void)
{
  uint16 val;
  /* enable read request */
  TIMER3_ReadRequest_En();
  /* read out the timer value */
  val = u16_Field_Rd32(&TIMER3->CNT.reg, TIMER3_CNT_LO_Pos, (TIMER3_CNT_LO_Msk | TIMER3_CNT_HI_Msk));
  /* disable read request */
  TIMER3_ReadRequest_Dis();
  return (val);
}


void TIMER3_Clk_Source_Sel(TTIMER3_Clk_Source t3c)
{
  sint32 int_was_mask;
  /* disable all interrupts                */
  int_was_mask = CMSIS_Irq_Dis();
  /* open access to protected register     */
  SCU_OpenPASSWD();
  /* write timer3 clock select register    */
  Field_Mod8(&SCU->APCLK_CTRL1.reg, (uint8)SCU_APCLK_CTRL1_T3CLK_SEL_Pos, (uint8)SCU_APCLK_CTRL1_T3CLK_SEL_Msk, (uint8)t3c);
  /* close access to protected register    */
  SCU_ClosePASSWD();

  /* enable all interrupts                 */
  if (int_was_mask == 0)
  {
    CMSIS_Irq_En();
  }
}


void TIMER3_Clk_Prescaler_Sel(TTIMER3_Clk_Prescaler t3div)
{
  sint32 int_was_mask;
  /* disable all interrupts                */
  int_was_mask = CMSIS_Irq_Dis();
  /* open access to protected register     */
  SCU_OpenPASSWD();
  /* write timer3 clock prescaler          */
  Field_Mod8(&SCU->APCLK_CTRL2.reg, (uint8)SCU_APCLK_CTRL2_T3CLK_DIV_Pos, (uint8)SCU_APCLK_CTRL2_T3CLK_DIV_Msk, (uint8)t3div);
  /* close access to protected register    */
  SCU_ClosePASSWD();

  /* enable all interrupts                 */
  if (int_was_mask == 0)
  {
    CMSIS_Irq_En();
  }

  /* trigger register set                  */
  SCU->APCLK_CTRL1.bit.APCLK_SET = 1u;
}
