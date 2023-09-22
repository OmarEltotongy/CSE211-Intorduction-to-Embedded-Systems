 /******************************************************************************
 *
 * Module: KEYPAD
 *
 * File Name: keypad.c
 *
 * Description: KEYPAD driver
 *
 * Author: Mokhtar Khaled
 *
 *******************************************************************************/


#include "keypad.h"
#include "gpio.h"
#include "systic.h"

static uint8 KEYPAD_UX[4][4] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

void KEYPAD_init(void)
{
  GPIO_init(KEYPAD_ROW_PORT, FIRST_FOUR_PINS << KEYPAD_START_ROW_PIN);
  GPIO_setDirection(KEYPAD_ROW_PORT, FIRST_FOUR_PINS << KEYPAD_START_ROW_PIN, GPIO_OUTPUT);
  
  GPIO_init(KEYPAD_COL_PORT, FIRST_FOUR_PINS << KEYPAD_START_COL_PIN);
  GPIO_setDirection(KEYPAD_COL_PORT, FIRST_FOUR_PINS << KEYPAD_START_COL_PIN, GPIO_INPUT_PULLUP); 
}

uint8 KEYPAD_read(void)
{
  uint8 startVal;
  while(1)
  {
    uint8 rowNum, colNum;
    startVal = 1 << KEYPAD_START_ROW_PIN;
    for(rowNum = 0; rowNum < 4; rowNum++)
    {
      GPIO_write(KEYPAD_ROW_PORT, FIRST_FOUR_PINS << KEYPAD_START_ROW_PIN, ~startVal);
      for(colNum = 0; colNum < 4; colNum++)
      {
        if(!GPIO_read(KEYPAD_COL_PORT, (0x10 << colNum)))
        {
          while(!GPIO_read(KEYPAD_COL_PORT, (0x10 << colNum)));
          SYSTIC_delay_ms(100);
          return KEYPAD_UX[rowNum][colNum];
        }
      }
      startVal <<= 1;
    }
  }
}

