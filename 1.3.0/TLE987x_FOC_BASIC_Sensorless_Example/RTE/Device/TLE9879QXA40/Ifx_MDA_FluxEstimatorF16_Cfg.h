/**
 * @cond
 ***********************************************************************************************************************
 *
 * Copyright (c) 2018, Infineon Technologies AG
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
#ifndef IFX_MDA_FLUXESTIMATORF16_CFG_H
#define IFX_MDA_FLUXESTIMATORF16_CFG_H

/* XML Version 1.0.5 */
#define IFX_MDA_FLUXESTIMATORF16_CFG_XML_VERSION (10005)

#define IFX_MDA_FLUXESTIMATORF16_CFG_ADJUSTMENT_FACTOR (0.06002)

#define IFX_MDA_FLUXESTIMATORF16_CFG_ALPHA_GAIN_Q14 (0x42A51) /*decimal 272977*/

#define IFX_MDA_FLUXESTIMATORF16_CFG_ALPHA_TC_US (0x26DB) /*decimal 9947*/

#define IFX_MDA_FLUXESTIMATORF16_CFG_BASE_CURRENT_A (5.00000)

#define IFX_MDA_FLUXESTIMATORF16_CFG_BASE_ELEC_SPEED_RADPS (1675.00000)

#define IFX_MDA_FLUXESTIMATORF16_CFG_BASE_FLUX_WB (0.01)

#define IFX_MDA_FLUXESTIMATORF16_CFG_BASE_INDUCTANCE_MH (2.87)

#define IFX_MDA_FLUXESTIMATORF16_CFG_BASE_POWER_W (0x78) /*decimal 120*/

#define IFX_MDA_FLUXESTIMATORF16_CFG_BASE_RESISTANCE_OHM (4.8)

#define IFX_MDA_FLUXESTIMATORF16_CFG_BASE_TIME_MS (0.597015)

#define IFX_MDA_FLUXESTIMATORF16_CFG_BASE_TORQUE_NM (0.07)

#define IFX_MDA_FLUXESTIMATORF16_CFG_BASE_VOLTAGE_V (24.00000)

#define IFX_MDA_FLUXESTIMATORF16_CFG_BETA_GAIN_Q14 (0x42A51) /*decimal 272977*/

#define IFX_MDA_FLUXESTIMATORF16_CFG_BETA_TC_US (0x26DB) /*decimal 9947*/

#define IFX_MDA_FLUXESTIMATORF16_CFG_PHASE_IND_Q15 (0x2EE6) /*decimal 12006*/

#define IFX_MDA_FLUXESTIMATORF16_CFG_PHASE_RES_Q15 (0x1400) /*decimal 5120*/

#define IFX_MDA_FLUXESTIMATORF16_CFG_PLL_GAIN_Q (0x6400) /*decimal 25600*/

#define IFX_MDA_FLUXESTIMATORF16_CFG_PLL_GAIN_Q_FORMAT (0x7) /*decimal 7*/

#define IFX_MDA_FLUXESTIMATORF16_CFG_SAMPLING_TIME_US (0x96) /*decimal 150*/

#define IFX_MDA_FLUXESTIMATORF16_CFG_SPEED_TC_US (0x26DB) /*decimal 9947*/

#define IFX_MDA_FLUXESTIMATORF16_CFG_SYSTEM_BASE_TIME_Q30 (0x3D757C) /*decimal 4027772*/

#endif /* IFX_MDA_FLUXESTIMATORF16_CFG_H */
