/*
 * g6core.h
 *
 *  Created on: Mar 24, 2018
 *      Author: tom
 */

#ifndef G6CORE_H_
#define G6CORE_H_

#define false 0
#define true 1


// Axis array index values. Must start with 0 and be continuous.
#ifdef STM32

#include "float.h"

#define SOME_LARGE_VALUE FLT_MAX

//-- number of axes
#if (defined (STM32F1_3) || defined(STM32F4_3))
#define N_AXIS 3
#endif
#if (defined (STM32F1_4) || defined(STM32F4_4))
#define N_AXIS 4  // Number of axes
#endif
#if (defined (STM32F1_5) || defined(STM32F4_5))
#define N_AXIS 5  // Number of axes
#endif
#if (defined (STM32F1_6) || defined(STM32F4_6))
#define N_AXIS 6  // Number of axes
#endif


#ifdef STM32F4
#define N_OUTPUTS_DIG 	8 // Number of digital outputs
#define N_INPUTS_DIG 		8 // Number of digital inputs
#define N_OUTPUTS_ANA 	8 // Number of analog outputs (PWM)
#endif

#ifdef STM32F1
#ifdef STM32F13
#define N_OUTPUTS_DIG   4 // Number of digital outputs
#define N_OUTPUTS_ANA   0 // Number of analog outputs (PWM)
#endif
#ifdef STM32F16
#define N_OUTPUTS_DIG 	1 // Number of digital outputs
#define N_OUTPUTS_ANA 	0 // Number of analog outputs (PWM)
#endif
#endif
#else //-- not STM32
#define N_AXIS 3 // Number of axes
#define SOME_LARGE_VALUE 1.0E+38
#endif

#define X_AXIS 0 // Axis indexing value.
#define Y_AXIS 1
#define Z_AXIS 2
#ifdef STM32
#if (defined(STM32F1_4) || defined(STM32F4_4))
#define A_AXIS 3
#endif
#if (defined(STM32F1_5) || defined(STM32F4_5))
#define A_AXIS 3
#define B_AXIS 4
#endif
#if (defined(STM32F1_6) || defined(STM32F4_6))
#define A_AXIS 3
#define B_AXIS 4
#define C_AXIS 5
#endif
#endif

#define PIN_MASK uint16_t


#endif /* G6CORE_H_ */
