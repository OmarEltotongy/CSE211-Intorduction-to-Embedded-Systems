#include "time_operations.h"
#include <stdlib.h>
#include "basic_operations.h"


// This function receives time format "MMSS" and return its corresponding seconds 
uint16 fromTimeToSec(char* str)
{
  char minStr[3] = {str[0], str[1], '\0'};      // create minute string
  char secStr[3] = {str[2], str[3], '\0'};      // create second string
  
  uint8 min = atoi(minStr);     // convert minute string to minute integer
  uint8 sec = atoi(secStr);     // convert second string to second integer
  
  if(min > 99)  // return error code if the minute integer is greater than 59
  {
    return 0xFFFF;
  }
  
  if(sec > 59)  // return error code if the second integer is greater than 59
  {
    return 0xFFFF;
  }
    
  
  return sec + (min * 60);
}

// This function receives seconds and returns time format "MM:SS" in str
uint8 fromSecToTime(uint16 seconds, char str[])
{
  uint8 strCntr = 0;
  uint8 minPart = seconds / 60;         // create minute integer from total seconds
  uint8 secPart = seconds % 60;         // create second integer from total seconds
 
  if(minPart > 99)
  {
    return 0xFF;
  }
  
  char minStr[3];
  char secStr[3];
  
  itoa(minPart, minStr, 10);            // convert minute integer to minute string
  itoa(secPart, secStr, 10);            // convert second integer to second string
  
  // Adjust format to be "MM:SS"
  if(minPart < 10)
  {
    str[strCntr++] = '0';
    str[strCntr++] = minStr[0];
  }
  else
  {
    str[strCntr++] = minStr[0];
    str[strCntr++] = minStr[1];
  }
  
  str[strCntr++] = ':';
  
  if(secPart < 10)
  {
    str[strCntr++] = '0';
    str[strCntr++] = secStr[0];
  }
  else
  {
    str[strCntr++] = secStr[0];
    str[strCntr++] = secStr[1];
  }
  
  str[strCntr] = '\0';
  return SUCCESS;
}