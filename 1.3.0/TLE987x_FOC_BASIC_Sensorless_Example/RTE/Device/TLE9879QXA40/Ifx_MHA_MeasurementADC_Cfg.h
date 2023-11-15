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
#ifndef IFX_MHA_MEASUREMENTADC_CFG_H
#define IFX_MHA_MEASUREMENTADC_CFG_H

/* XML Version 1.0.4 */
#define IFX_MHA_MEASUREMENTADC_CFG_XML_VERSION (10004)

#define IFX_MHA_MEASUREMENTADC_CFG_BASE_CURRENT_A (5.00000)

#define IFX_MHA_MEASUREMENTADC_CFG_BASE_VOLTAGE_V (24.00000)

#define IFX_MHA_MEASUREMENTADC_CFG_CALIBRATION_CYCLES (0x80) /*decimal 128*/

#define IFX_MHA_MEASUREMENTADC_CFG_CONVERT_VDC_TO_Q15 (0x5000) /*decimal 20480*/

#define IFX_MHA_MEASUREMENTADC_CFG_CSA_GAIN (0x3) /*decimal 3*/

#define IFX_MHA_MEASUREMENTADC_CFG_CURRENT_GAIN_Q_10 (0x5000) /*decimal 20480*/

#define IFX_MHA_MEASUREMENTADC_CFG_CURRENT_GAIN_Q_20 (0x5000) /*decimal 20480*/

#define IFX_MHA_MEASUREMENTADC_CFG_CURRENT_GAIN_Q_40 (0x5000) /*decimal 20480*/

#define IFX_MHA_MEASUREMENTADC_CFG_CURRENT_GAIN_Q_60 (0x6AAA) /*decimal 27306*/

#define IFX_MHA_MEASUREMENTADC_CFG_CURRENT_GAIN_SHIFT_10 (0x7) /*decimal 7*/

#define IFX_MHA_MEASUREMENTADC_CFG_CURRENT_GAIN_SHIFT_20 (0x8) /*decimal 8*/

#define IFX_MHA_MEASUREMENTADC_CFG_CURRENT_GAIN_SHIFT_40 (0x9) /*decimal 9*/

#define IFX_MHA_MEASUREMENTADC_CFG_CURRENT_GAIN_SHIFT_60 (0xA) /*decimal 10*/

#define IFX_MHA_MEASUREMENTADC_CFG_SHUNT_RES (0.00500)

#define IFX_MHA_MEASUREMENTADC_CFG_VDC_BITS (0xB) /*decimal 11*/

#endif /* IFX_MHA_MEASUREMENTADC_CFG_H */
