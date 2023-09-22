 /******************************************************************************
 *
 * Module: SysTick
 *
 * File Name: systick.c
 *
 * Description: Source file for The System Tick Time
 *
 * Author: Team 17
 *
 *******************************************************************************/

#include "systic.h"
#include "tivac_regs.h"

void SYSTIC_delay_ms(uint16 delayTime)
{
  uint8 reqNumOfIter, curIter;
  uint16 reloadVal;
  
  reloadVal = delayTime % SYSTIC_MAX_MS_TIME;
  
  if(reloadVal == 0)
  {
    reloadVal = SYSTIC_MAX_MS_TIME;
  }
  
  reqNumOfIter = delayTime / reloadVal;
  
  NVIC_ST_CTRL_R = SYSTIC_ENABLE;
  NVIC_ST_RELOAD_R = reloadVal * SYSTIC_TIME_MS_SCALE_FACTOR;
  
  for(curIter = 0; curIter < reqNumOfIter; curIter++)
  {
    while((NVIC_ST_CTRL_R & 0x10000) == 0);
  }
  
  NVIC_ST_CTRL_R = 0;
  NVIC_ST_RELOAD_R = 0;
  NVIC_ST_CURRENT_R = 0;
}

void SYSTIC_delay_us(uint32 delayTime)
{
  uint8 reqNumOfIter, curIter;
  uint32 reloadVal;
  
  reloadVal = delayTime % SYSTIC_MAX_US_TIME;
  
  if(reloadVal == 0)
  {
    reloadVal = SYSTIC_MAX_US_TIME;
  }
  
  reqNumOfIter = delayTime / reloadVal;
  
  NVIC_ST_CTRL_R = SYSTIC_ENABLE;
  NVIC_ST_RELOAD_R = reloadVal * SYSTIC_TIME_US_SCALE_FACTOR;
  
  for(curIter = 0; curIter < reqNumOfIter; curIter++)
  {
    while((NVIC_ST_CTRL_R & 0x10000) == 0);
  }
  
  NVIC_ST_CTRL_R = 0;
  NVIC_ST_RELOAD_R = 0;
  NVIC_ST_CURRENT_R = 0;  
}
