 /******************************************************************************
 *
 * Module: Standard Types
 *
 * File Name: std_types.h
 *
 * Description: Standard Types
 *
 * Author: Team 17
 *
 *******************************************************************************/


#ifndef STD_TYPES_H
#define STD_TYPES_H

#define NULL                    (void*)(0)

typedef unsigned char           uint8;
typedef unsigned char*          uint8_ptr;

typedef signed char             int8;
typedef signed char*            int8_ptr;

typedef unsigned short          uint16;
typedef unsigned short*         uint16_ptr;

typedef signed short            int16;    
typedef signed short*           int16_ptr;    

typedef unsigned long           uint32;
typedef unsigned long*          uint32_ptr;

typedef signed long             int32;  
typedef signed long*            int32_ptr;  

typedef unsigned long long      uint64;
typedef unsigned long long*     uint64_ptr;

typedef signed long long        int64;  
typedef signed long long*       int64_ptr;  

typedef float                   float32; 
typedef float*                  float32_ptr; 

typedef double                  float64;
typedef double*                 float64_ptr;


#define FUNC_INPUT_ERROR        0xFF
#define GPIO_INPUT_ERROR        0xFE        

#define SUCCESS                 0x00

#define TRUE                    1
#define FLASE                   0

#define LOGIC_HIGH        	    (1u)
#define LOGIC_LOW      		    (0u)

typedef union{
  uint8 allBits;
  struct{
    uint8 bit0: 1;
    uint8 bit1: 1;
    uint8 bit2: 1;
    uint8 bit3: 1;
    uint8 bit4: 1;
    uint8 bit5: 1;
    uint8 bit6: 1;
    uint8 bit7: 1;
  }Bits;
}Byte;

#endif
