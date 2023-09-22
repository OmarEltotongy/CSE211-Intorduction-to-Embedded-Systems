 /******************************************************************************
 *
 * Module: SysTick
 *
 * File Name: systick.h
 *
 * Description: Header file for The System Tick Time
 *
 * Author: Team 17
 *
 *******************************************************************************/
#ifndef SYSTIC_H
#define SYSTIC_H

#include "std_types.h"

#define CPU_FREQ                        16000000

#define SYSTIC_TIME_MS_SCALE_FACTOR     (CPU_FREQ/1000)
#define SYSTIC_TIME_US_SCALE_FACTOR     (CPU_FREQ/1000000)
#define SYSTIC_ENABLE_WITH_INTERRUPT    7
#define SYSTIC_ENABLE                   5
#define SYSTIC_MAX_MS_TIME              1000
#define SYSTIC_MAX_US_TIME              1000000

void SYSTIC_delay_ms(uint16 delayTime);
void SYSTIC_delay_us(uint32 delayTime);

#endif
