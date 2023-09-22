#include "Calculator.h"
#include <stdlib.h>

static uint16 firstNum = 0;     // stores first number in operation
static char symbol = 0;         // stores symbol
static uint16 secondNum = 0;    // store second number in operation

// This function receives string and convert it into integer and stores the integer in firstNum
void CALCULATOR_recFirstNum(char* str)
{
  firstNum = atoi(str);         
}

// This function receives string from keypad input, each symbol corresponds to certain operation, any other character is entered in "keypadSymbol" other than 'A', 'B', 'C', '*', the function returns 0
uint8 CALCULATOR_recOperation(const char keypadSymbol)
{
  if('A' == keypadSymbol)
  {
    symbol = '+';
  }
  else if('B' == keypadSymbol)
  {
    symbol = '-';
  }
  else if('C' == keypadSymbol)
  {
    symbol = '/';
  }
  else if('*' == keypadSymbol)
  {
    symbol = '*';
  }
  else 
  {
    return 0;
  }
  return symbol;
}

// This function receives string and convert it into integer and stores the integer in secondNum
void CALCULATOR_recSecondNum(char* str)
{
  secondNum = atoi(str);
}

// Based on operation, result is returned by making the required operation between the two numbers
int CALCULATOR_getResult(void)
{
  if('+' == symbol)
  {
    return firstNum + secondNum;
  }
  else if('-' == symbol)
  {
    return firstNum - secondNum;
  }
  else if('*' == symbol)
  {
    return firstNum * secondNum;
  }
  else
  {
    return firstNum / secondNum;
  }
}