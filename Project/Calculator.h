#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "std_types.h"

// This function receives string and convert it into integer and stores the integer in firstNum
void CALCULATOR_recFirstNum(char* str);
// This function receives string from keypad input, each symbol corresponds to certain operation, any other character is entered in "keypadSymbol" other than 'A', 'B', 'C', '*', the function returns 0
uint8 CALCULATOR_recOperation(const char keypadSymbol);
// This function receives string and convert it into integer and stores the integer in secondNum
void CALCULATOR_recSecondNum(char* str);
// Based on operation, result is returned by making the required operation between the two numbers
int CALCULATOR_getResult(void);

#endif
