 /******************************************************************************
 *
 * Module: timer
 *
 * File Name: timer.h
 *
 * Description: Timers driver for TM4C123GH6PM
 *
 * Author: OMAR TAREK
 *
 *******************************************************************************/
#ifndef TIMER_H_
#define TIMER_H_
#include "std_types.h"

#define TIMER0_ID  1
#define TIMER1_ID  (TIMER0_ID<<1)
#define TIMER2_ID  (TIMER0_ID<<2)
#define TIMER3_ID  (TIMER0_ID<<3)
#define TIMER4_ID  (TIMER0_ID<<4)
#define TIMER5_ID  (TIMER0_ID<<5)
   
#define TIMER_A 0       //just a name for timer A to easily set
#define TIMER_B 1       //just a name for timer B to easily set
        
//Enable A at GPTMCTL 0th bit
//Enable B at GPTMCTL 8th bit   
#define TAEN 1 
#define TBEN (1<<8)   

#define TAIEN 1         //timer a interrupt enable.
#define TBIEN (1<<8)    //timer b interrupt enable.
 
//timers number in nvic in order to activate interrupt
#define TIMER0_A_NVIC_INT_NUM 19
#define TIMER0_B_NVIC_INT_NUM 20
#define TIMER1_A_NVIC_INT_NUM 21
#define TIMER1_B_NVIC_INT_NUM 22
#define TIMER2_A_NVIC_INT_NUM 23
#define TIMER2_B_NVIC_INT_NUM 24
#define TIMER3_A_NVIC_INT_NUM 35
#define TIMER3_B_NVIC_INT_NUM 36
#define TIMER4_A_NVIC_INT_NUM 70
#define TIMER4_B_NVIC_INT_NUM 71
#define TIMER5_A_NVIC_INT_NUM 92
#define TIMER5_B_NVIC_INT_NUM 93

   
#if 0    
#define DOUBLE_TIMER    //for future developments in case of setting 2 timers in same timer 
#endif 
//******************************Interrupt***********************//
//Bits are in reg GPTMIMR A at bit 0, B at bit 8

#define TIMER0_INT 
#define TIMER1_INT 
#define TIMER2_INT
#define TIMER3_INT
#define TIMER4_INT
#define TIMER5_INT


//******************************Interrupt***********************//
//Bits are in reg GPTMTAPR first 8 bits are for single, second 8 for double
/*
#define TIMER0_PRE 
#define TIMER1_PRE 
#define TIMER2_PRE
#define TIMER3_PRE
#define TIMER4_PRE
#define TIMER5_PRE
*/

typedef enum {
  TIMER_SINGLE_MODE = 0x04, //1000
  TIMER_DOUBLE_MODE = 0x00 //0000
}TIMER_MODE_WIDTH;

typedef enum {
  TIMER_ONESHOT_UP = 0x11,TIMER_ONESHOT_DOWN = 0x01, 
  TIMER_PERIODIC_UP = 0x12,TIMER_PERIODIC_DOWN = 0x02, 
  TIMER_CAPTURE_UP = 0x13,TIMER_CAPTURE_DOWN = 0x03
}TIMER_MODE;
  
typedef enum { 
  TA_HALT = 2, TB_HALT = (2<<8) ,NORMAL = 0
}TIMER_HALT;

typedef struct{
  TIMER_MODE_WIDTH WIDTH; //For Single 16/32 of double 32/64
  TIMER_MODE MODE;        //For oneshot/periodic , up/down
  uint8 TYPE;        //For which timer desired
  uint8 TIMER_NO;
  double TIMER_WAIT;
  TIMER_HALT DEBUG_MODE;
}Timer_Conf;



//********************************************FUNCTIONS****************************************************

//For initializing timer.
void GPTM_Init(Timer_Conf *configuration);

//For Deinitializing timer.
void GPTM_DeInit(uint8 timer_no);


/*CALLBACK FUNCTION IN ORDER TO EXECUTE ISR*/
#ifdef TIMER0_INT
void GPTM0_setCallBackA(void (*a_ptr)(void));
void GPTM0_setCallBackB(void (*a_ptr)(void));
#endif

#ifdef TIMER1_INT
void GPTM1_setCallBackA(void (*a_ptr)(void));
void GPTM1_setCallBackB(void (*a_ptr)(void));
#endif

#ifdef TIMER2_INT
void GPTM2_setCallBackA(void (*a_ptr)(void));
void GPTM2_setCallBackA(void (*a_ptr)(void));
#endif

#ifdef TIMER3_INT
void GPTM3_setCallBackB(void (*a_ptr)(void));
void GPTM3_setCallBackA(void (*a_ptr)(void));
#endif

#ifdef TIMER4_INT
void GPTM4_setCallBackA(void (*a_ptr)(void));
void GPTM4_setCallBackB(void (*a_ptr)(void));
#endif

#ifdef TIMER5_INT
void GPTM5_setCallBackA(void (*a_ptr)(void));
void GPTM5_setCallBackB(void (*a_ptr)(void));
#endif



#endif /*gptm.h*/