 /******************************************************************************
 *
 * Module: KEYPAD
 *
 * File Name: keypad.h
 *
 * Description: Keypad driver layer
 *
 * Author: Mokhtar Khaled
 *
 *******************************************************************************/

#ifndef KEYPAD_H_
#define KEYPAD_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define NUM_OF_ROWS			4
#define NUM_OF_COLS			4

#define KEYPAD_ROW_PORT			PORTE_ID
#define KEYPAD_START_ROW_PIN	        0

#define KEYPAD_COL_PORT			PORTC_ID
#define KEYPAD_START_COL_PIN	        4

#define KEYPAD_CHECK_VALUE		LOGIC_LOW
#define BUTTON_IS_PRESSED		LOGIC_LOW

/*******************************************************************************
 *                            Function Prototypes                              *
 *******************************************************************************/

void KEYPAD_init(void);
uint8 KEYPAD_read(void);

#endif /* KEYPAD_H_ */
