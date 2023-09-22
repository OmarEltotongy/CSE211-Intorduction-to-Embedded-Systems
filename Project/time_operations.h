#ifndef TIME_OPERATIONS_H
#define TIME_OPERATIONS_H

#include "std_types.h"

// This function receives time format "MMSS" and return its corresponding seconds 
uint16 fromTimeToSec(char* str);
// This function receives seconds and returns time format "MM:SS" in str
uint8 fromSecToTime(uint16 seconds, char str[]);

#endif