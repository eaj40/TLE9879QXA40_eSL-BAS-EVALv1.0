/*
 * Copyright (c) 2022 Infineon Technologies AG. All Rights Reserved.
 *
 * Use of this file is subject to the terms of use agreed between (i) you or the company in which ordinary course of
 * business you are acting and (ii) Infineon Technologies AG, its affiliates or its licensees. If and as long as no
 * such terms of use are agreed, use of this file is subject to the Evaluation Software License Agreement distributed
 * along with this file within the software delivery package.
 *
 */

#include "Ifx_Math_PLLF16.h"

/* Arithmetic functions */
#include "Ifx_Math_Add.h"
#include "Ifx_Math_ShR.h"
#include "Ifx_Math_ShL.h"
#include "Ifx_Math_Sub.h"
#include "Ifx_Math_MulShR.h"

/* Shift factor for the average rate of change of the input angle */
#if IFX_MATH_CFG_PLL_DELAY_LENGTH == IFX_MATH_USROPT_PLL_DELAY_32
#define IFX_MATH_PLLF16_SHIFT_FACTOR (5)
#elif IFX_MATH_CFG_PLL_DELAY_LENGTH == IFX_MATH_USROPT_PLL_DELAY_16
#define IFX_MATH_PLLF16_SHIFT_FACTOR (4)
#elif IFX_MATH_CFG_PLL_DELAY_LENGTH == IFX_MATH_USROPT_PLL_DELAY_8
#define IFX_MATH_PLLF16_SHIFT_FACTOR (3)
#elif IFX_MATH_CFG_PLL_DELAY_LENGTH == IFX_MATH_USROPT_PLL_DELAY_4
#define IFX_MATH_PLLF16_SHIFT_FACTOR (2)
#elif IFX_MATH_CFG_PLL_DELAY_LENGTH == IFX_MATH_USROPT_PLL_DELAY_2
#define IFX_MATH_PLLF16_SHIFT_FACTOR (1)
#endif

/* Macro to saturate the value with wrapping */
#define IFX_MATH_PLLF16_2MAX         (-2 * IFX_MATH_FRACT16_MIN)

/**
 *  \brief Calculate the average rate of change of the input angle
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *  \param [in] inputMinusPiToPi Input angle between -pi and pi
 *
 *  \return Average rate of change in Ifx_Math_Fract16 format
 */
static inline Ifx_Math_Fract16 Ifx_Math_PLLF16_p_calcAvgRate(Ifx_Math_PLLF16* self, uint32 input);

/**
 *  \brief Change the scaling of the input average rate of change of the angle, add to the angle error multiplied by the
 * proportional gain and integrate the result
 *
 *
 *  \param [inout] self Reference to structure that contains instance data members
 *  \param [in] inputKp Angle error between the input angle and previous angle multiplied by the proportional gain
 *  \param [in] speed Average rate of change of the input angle to the PLL
 *
 */
static inline void Ifx_Math_PLLF16_p_speedCalc(Ifx_Math_PLLF16* self, Ifx_Math_Fract32 inputKp, Ifx_Math_Fract16
                                               speed);

/**
 *  \brief Convert input angle from range 0 to 2*pi [0...2^32] to -pi to pi [-2^15...2^15)
 *
 *
 *  \param [in] input Input angle in uint32 format
 *
 *  \return Converted angle in Ifx_Math_Fract16 format
 */
static inline Ifx_Math_Fract16 Ifx_Math_PLLF16_p_convAngle(uint32 input);

/* polyspace-begin MISRA2012:D4.14 [Justified:Low] "The caller function has to guarantee that NULL is not passed as
 * argument." */
void Ifx_Math_PLLF16_init(Ifx_Math_PLLF16* self)
{
    /* Initialize filter parameters */
    self->p_propGain.value       = 0;
    self->p_propGain.qFormat     = Ifx_Math_FractQFormat_q0;
    self->p_samplingTime_us      = 0;
    self->p_propGainSamplingTime = 0;
    self->p_previousValue        = 0;

    /* Initialize buffer contents */
    self->p_bufferIndex = 0;

    for (int i = 0; i < IFX_MATH_CFG_PLL_DELAY_LENGTH; i++)
    {
        self->p_buffer[i] = 0;
    }
}


void Ifx_Math_PLLF16_resetBuffer(Ifx_Math_PLLF16* self)
{
    /* Initialize buffer contents */
    self->p_bufferIndex = 0;

    for (int i = 0; i < IFX_MATH_CFG_PLL_DELAY_LENGTH; i++)
    {
        self->p_buffer[i] = 0;
    }
}


Ifx_Math_PLLF16_Type Ifx_Math_PLLF16_execute(Ifx_Math_PLLF16* self, uint32 input)
{
    /* Stores the output values */
    Ifx_Math_PLLF16_Type output;

    /* Get the wrapped the value from the previous execution */
    uint32               previousValue = self->p_previousValue;

    /* Subtract and convert to Q15 */
    uint32               deltaPhi    = input - previousValue;
    Ifx_Math_Fract16     deltaPhiQ15 = Ifx_Math_PLLF16_p_convAngle(deltaPhi);

    /* input * Kp * Ts, scaled to Q24 */
    Ifx_Math_Fract32     inputKpQ24 = Ifx_Math_MulShR_F32_F16F16(deltaPhiQ15, self->p_propGainSamplingTime, 6u);

    /* Calculate the average rate of change of the input angle */
    Ifx_Math_Fract16     speedQ15 = Ifx_Math_PLLF16_p_calcAvgRate(self, input);

    /* Calculate the speed from average rate and integrate */
    Ifx_Math_PLLF16_p_speedCalc(self, inputKpQ24, speedQ15);

    /* Return the output angle variation, in 16-bit fractional format, and the angle in 32-bit */
    output.deltaAngle = speedQ15;
    output.angle      = previousValue;

    return output;
}


void Ifx_Math_PLLF16_setPropGain(Ifx_Math_PLLF16* self, Ifx_Math_Fract16Q propGain)
{
    /* Represents the sampling time in Q30 format */
    Ifx_Math_Fract32 samplingTimeQ30;

    /* Convert sampling time from microseconds to seconds, in Ifx_Math_FractQFormat_q30 */

    /* polyspace +3 MISRA2012:10.1 [Justified:Low] "Bitwise operators on signed values are required by fixed point
     * arithmetic." */
    samplingTimeQ30 = (Ifx_Math_Fract32)((((sint64)self->p_samplingTime_us) << (sint8)Ifx_Math_FractQFormat_q30) /
                                         IFX_MATH_MICROSECONDS_TO_SECONDS);

    /* Calculate the proportional gain * sampling time, keeping the proportional gain Q format */
    self->p_propGainSamplingTime = (Ifx_Math_Fract16)Ifx_Math_MulShR_F32(propGain.value, samplingTimeQ30,
        Ifx_Math_MulShR_ShiftMul(propGain.qFormat, Ifx_Math_FractQFormat_q30, Ifx_Math_FractQFormat_q15));
    self->p_propGain             = propGain;
}


void Ifx_Math_PLLF16_setSamplingTime_us(Ifx_Math_PLLF16* self, uint32 samplingTime_us)
{
    /* Represents the sampling time in Q30 format */
    Ifx_Math_Fract32 samplingTimeQ30;

    /* Convert sampling time from microseconds to seconds, in Ifx_Math_FractQFormat_q30 */

    /* polyspace +3 MISRA2012:10.1 [Justified:Low] "Bitwise operators on signed values are required by fixed point
     * arithmetic." */
    samplingTimeQ30 = (Ifx_Math_Fract32)((((sint64)samplingTime_us) << (sint8)Ifx_Math_FractQFormat_q30) /
                                         IFX_MATH_MICROSECONDS_TO_SECONDS);

    /* Calculate the proportional gain * sampling time, keeping the integral proportional Q format */
    self->p_propGainSamplingTime = (Ifx_Math_Fract16)Ifx_Math_MulShR_F32(self->p_propGain.value, samplingTimeQ30,
        Ifx_Math_MulShR_ShiftMul(self->p_propGain.qFormat, Ifx_Math_FractQFormat_q30, Ifx_Math_FractQFormat_q15));
    self->p_samplingTime_us      = samplingTime_us;
}


/* Calculate the average rate of change of the input angle */
static inline Ifx_Math_Fract16 Ifx_Math_PLLF16_p_calcAvgRate(Ifx_Math_PLLF16* self, uint32 input)
{
    uint32 previousStep = self->p_buffer[self->p_bufferIndex];
    self->p_buffer[self->p_bufferIndex] = input;

    /* Increment buffer pointer */
    if (self->p_bufferIndex == (IFX_MATH_CFG_PLL_DELAY_LENGTH - 1))
    {
        self->p_bufferIndex = 0;
    }
    else
    {
        self->p_bufferIndex++;
    }

    /* Subtract, use natural overflow and convert to Q15 */
    uint32           rawAngle     = input - previousStep;
    Ifx_Math_Fract16 rawAngleWrap = Ifx_Math_PLLF16_p_convAngle(rawAngle);

    /* Get the speed from the average rate of change of the input angle */
    Ifx_Math_Fract16 speed = Ifx_Math_ShR_F16(rawAngleWrap, IFX_MATH_PLLF16_SHIFT_FACTOR);

    return speed;
}


/* Convert angle to 16-bit, -pi to pi */
static inline Ifx_Math_Fract16 Ifx_Math_PLLF16_p_convAngle(uint32 input)
{
/* Convert input angle to 16-bit representation */
    uint16           input16Bit = (uint16)(input >> 16);
    Ifx_Math_Fract16 inputMinusPiToPi;

    if (input < IFX_MATH_PI_INDEX)
    {
        inputMinusPiToPi = (Ifx_Math_Fract16)input16Bit;
    }
    else
    {
        inputMinusPiToPi = (Ifx_Math_Fract16)(Ifx_Math_Sub_F32((Ifx_Math_Fract32)input16Bit, IFX_MATH_PLLF16_2MAX));
    }

    return inputMinusPiToPi;
}


/* Calculate the speed from average rate and integrate */
static inline void Ifx_Math_PLLF16_p_speedCalc(Ifx_Math_PLLF16* self, Ifx_Math_Fract32 inputKp, Ifx_Math_Fract16
                                               speed)
{
    /* Scale speed to Q24, from Q15 */
    Ifx_Math_Fract32 speedQ24 = Ifx_Math_ShL_F32((Ifx_Math_Fract32)speed, 9);

    /* Calculate speed variation */
    Ifx_Math_Fract32 deltaSpeedQ24 = Ifx_Math_Add_F32(inputKp, speedQ24);
    Ifx_Math_Fract32 deltaSpeed    = Ifx_Math_ShL_F32(deltaSpeedQ24, 7U);

    /* Speed integration (Forward Euler) */
    uint32           anglePLL = (self->p_previousValue + (uint32)deltaSpeed);

    /* Store output for the next cycle */
    self->p_previousValue = anglePLL;
}


/* polyspace-end MISRA2012:D4.14 [Justified:Low] "The caller function has to guarantee that NULL is not passed as
 * argument." */
