 /******************************************************************************
 *
 * Module: timer
 *
 * File Name: timer.c
 *
 * Description: Timers driver for TM4C123GH6PM
 *
 * Author: OMAR TAREK
 *
 *******************************************************************************/
#include "gptm.h"
#include "tivac_regs.h"
#include "common_macros.h"

//**********************************************Vars*******************************************************
uint32 ticks = 0;
static void (*localPtrToFunc0A)(void) = NULL;
static void (*localPtrToFunc0B)(void) = NULL;
static void (*localPtrToFunc1A)(void) = NULL;
static void (*localPtrToFunc1B)(void) = NULL;
static void (*localPtrToFunc2A)(void) = NULL;
static void (*localPtrToFunc2B)(void) = NULL;
static void (*localPtrToFunc3A)(void) = NULL;
static void (*localPtrToFunc3B)(void) = NULL;
static void (*localPtrToFunc4A)(void) = NULL;
static void (*localPtrToFunc4B)(void) = NULL;
static void (*localPtrToFunc5A)(void) = NULL;
static void (*localPtrToFunc5B)(void) = NULL;


//*********************************************ISR*********************************************************
void GPTM_Timer0A_ISR (void){                                                   //timer0 A ISR

  if(NULL != localPtrToFunc0A){
  
    localPtrToFunc0A();
    
  }
    TIMER0_ICR_R  = TAIEN;                                                      //clear interrupt flag 
}

void GPTM_Timer0B_ISR (void){                                                   //timer0 B ISR

  if(NULL != localPtrToFunc0B){
  
    localPtrToFunc0B();
  
  }
    TIMER0_ICR_R  = TBIEN;                                                      //clear interrupt flag   
}

void GPTM_Timer1A_ISR (void){                                                   //timer1 A ISR

  if(NULL != localPtrToFunc1A){
  
    localPtrToFunc1A();
  
  }
    TIMER1_ICR_R  = TAIEN;                                                      //clear interrupt flag 
}

void GPTM_Timer1B_ISR (void){                                                   //timer1 B ISR

  if(NULL != localPtrToFunc1B){
  
    localPtrToFunc1B();
  
  }
    TIMER1_ICR_R  = TBIEN;                                                      //clear interrupt flag 
}

void GPTM_Timer2A_ISR (void){                                                   //timer2 A ISR

  if(NULL != localPtrToFunc2A){
  
    localPtrToFunc2A();
  
  }
    TIMER2_ICR_R  = TAIEN;                                                      //clear interrupt flag 
}

void GPTM_Timer2B_ISR (void){                                                   //timer2 B ISR

  if(NULL != localPtrToFunc2B){
  
    localPtrToFunc2B();
  
  }
    TIMER2_ICR_R  = TBIEN;                                                      //clear interrupt flag 
}

void GPTM_Timer3A_ISR (void){                                                   //timer3 A ISR

  if(NULL != localPtrToFunc3A){
  
    localPtrToFunc3A();
  
  }
    TIMER3_ICR_R  = TAIEN;                                                      //clear interrupt flag 
}

void GPTM_Timer3B_ISR (void){                                                   //timer3 B ISR

  if(NULL != localPtrToFunc3B){
  
    localPtrToFunc3B();
  
  }
    TIMER3_ICR_R  = TBIEN;                                                      //clear interrupt flag 
}

void GPTM_Timer4A_ISR (void){                                                   //timer4 A ISR

  if(NULL != localPtrToFunc4A){
  
    localPtrToFunc4A();
  
  }
    TIMER4_ICR_R  = TAIEN;                                                      //clear interrupt flag 
}

void GPTM_Timer4B_ISR (void){                                                   //timer4 B ISR

  if(NULL != localPtrToFunc4B){
  
    localPtrToFunc4B();
  
  }
    TIMER4_ICR_R  = TBIEN;                                                      //clear interrupt flag 
}

void GPTM_Timer5A_ISR (void){                                                   //timer5 A ISR

  if(NULL != localPtrToFunc5A){
  
    localPtrToFunc5A();
  
  }
    TIMER5_ICR_R  = TAIEN;                                                      //clear interrupt flag 
}

void GPTM_Timer5B_ISR (void){                                                   //timer5 B ISR

  if(NULL != localPtrToFunc5B){
  
    localPtrToFunc5B();
  
  }
    TIMER5_ICR_R  = TBIEN;                                                      //clear interrupt flag 
}

//********************************************FUNCTIONS****************************************************

//For initializing timer.
void GPTM_Init(Timer_Conf *configuration){
  if((configuration->TIMER_WAIT) > 1.2){
    ticks = 16000000;                                                           //make it 1 Second by default if > 1.2secs
  }
  else{
    ticks = (16000000*(configuration->TIMER_WAIT));
  }
  switch (configuration->TIMER_NO){
    
  case TIMER0_ID:
    SYSCTL_RCGCTIMER_R |= TIMER0_ID;                                            //Enable clock for timer0
    TIMER0_CFG_R = configuration->WIDTH;                                        //make it single or double
    if ((configuration->WIDTH)==(0x04) && ((configuration->TYPE)==TIMER_B)){    //condition for either single or double

    TIMER0_TBMR_R = (configuration->MODE);                                        //set oneshot or periodic and up or down

#ifdef TIMER0_INT                                                                  
    TIMER0_ICR_R  = TBIEN;                                                      //clear flag just in case
    NVIC_EN0_R   |= TIMER0_B_NVIC_INT_NUM;                                      //Enable interrupt in NVIC for timer 0    
    TIMER0_IMR_R  = TBIEN;                                                      //done in case interrupt is to be used which uncovers interrupt mask
#endif
//at user input is lower than or equal to 1 seconds (to be modified later)
    TIMER0_TBILR_R = ticks;                                                     //count user_seconds for default
    
    
    TIMER0_CTL_R = (configuration->DEBUG_MODE);                                                        //enable timer and start operating
    TIMER0_CTL_R |= TBEN;
    }
    
    else
    {
    
    TIMER0_TAMR_R = (configuration->MODE);                                        //set oneshot or periodic and up or down

#ifdef TIMER0_INT 
    TIMER0_ICR_R  = TAIEN;                                                      //clear flag of interrupt just in case
    SET_BIT(NVIC_EN0_R, TIMER0_A_NVIC_INT_NUM);
    TIMER0_IMR_R  = TAIEN;                                                      //done in case interrupt is to be used which uncovers interrupt mask
#endif
//at user input is lower than or equal to 1 seconds (to be modified later)
    TIMER0_TAILR_R = ticks; 
    
    TIMER0_CTL_R = (configuration->DEBUG_MODE);                                                        //enable timer and start operating
    TIMER0_CTL_R |= TAEN;
    }
    break;
//**************************************************************************END OF TIMER 0*********************************************
    
    
  case TIMER1_ID:
    SYSCTL_RCGCTIMER_R |= TIMER1_ID;
    TIMER1_CFG_R = configuration->WIDTH;
    if ((configuration->WIDTH)==(0x04) && ((configuration->TYPE)==TIMER_B)){    //condition for either single or double

    TIMER1_TBMR_R = (configuration->MODE);                                        //set oneshot or periodic and up or down

#ifdef TIMER1_INT                                                                  
    TIMER1_ICR_R  = TBIEN;                                                      //clear flag just in case 
    NVIC_EN0_R   |= TIMER1_B_NVIC_INT_NUM;                                      //Enable interrupt in NVIC for timer 1    
    TIMER1_IMR_R  = TBIEN;                                                      //done in case interrupt is to be used which uncovers interrupt mask
#endif
//at user input is lower than or equal to 1 seconds (to be modified later)
    TIMER1_TBILR_R = 16000000;                                                  //count user_seconds for default
    
    
    TIMER1_CTL_R = (configuration->DEBUG_MODE);                                                        //enable timer and start operating
    TIMER1_CTL_R |= TBEN;
    }
    
    else
    {
    
    TIMER1_TAMR_R = (configuration->MODE);                                        //set oneshot or periodic and up or down

#ifdef TIMER1_INT 
    TIMER1_ICR_R  = TAIEN;                                                      //clear flag of interrupt just in case
    SET_BIT(NVIC_EN0_R, TIMER1_A_NVIC_INT_NUM);
    TIMER1_IMR_R  = TAIEN;                                                      //done in case interrupt is to be used which uncovers interrupt mask
#endif
//at user input is lower than or equal to 1 seconds (to be modified later)
    TIMER1_TAILR_R = ticks;                                                     //count 1 sec for default
    
    
    TIMER1_CTL_R = (configuration->DEBUG_MODE);                                                        //enable timer and start operating
    TIMER1_CTL_R |= TAEN;
    }
    break;
//**************************************************************************END OF TIMER 1*********************************************
    
 
  case TIMER2_ID:
    SYSCTL_RCGCTIMER_R |= TIMER2_ID;
    TIMER2_CFG_R = configuration->WIDTH;
    if ((configuration->WIDTH)==(0x04) && ((configuration->TYPE)==TIMER_B)){    //condition for either single or double

    TIMER2_TBMR_R = (configuration->MODE);                                        //set oneshot or periodic and up or down

#ifdef TIMER2_INT                                                                  
    TIMER2_ICR_R  = TBIEN;                                                      //clear flag just in case
    NVIC_EN0_R   |= TIMER2_B_NVIC_INT_NUM;                                      //Enable interrupt in NVIC for timer 1    
    TIMER2_IMR_R  = TBIEN;                                                      //done in case interrupt is to be used which uncovers interrupt mask
#endif
//at user input is lower than or equal to 1 seconds (to be modified later)
    TIMER2_TBILR_R = ticks;                                                     //count user_seconds for default
    
    
    TIMER2_CTL_R = (configuration->DEBUG_MODE);                                                        //enable timer and start operating
    TIMER2_CTL_R |= TBEN;
    }
    
    else
    {
    
    TIMER2_TAMR_R = (configuration->MODE);                                        //set oneshot or periodic and up or down

#ifdef TIMER2_INT 
    TIMER2_ICR_R  = TAIEN;                                                      //clear flag of interrupt just in case
    NVIC_EN0_R |= TIMER2_A_NVIC_INT_NUM;
    TIMER2_IMR_R  = TAIEN;                                                      //done in case interrupt is to be used which uncovers interrupt mask
#endif
//at user input is lower than or equal to 1 seconds (to be modified later)
    TIMER2_TAILR_R = ticks;                                                     //count 1 sec for default
    
    
    TIMER2_CTL_R = (configuration->DEBUG_MODE);                                                        //enable timer and start operating
    TIMER2_CTL_R |= TAEN;                                                    //enable timer and start operating
    }
    break;
//**************************************************************************END OF TIMER 2*********************************************
    
 
  case TIMER3_ID:
    SYSCTL_RCGCTIMER_R |= TIMER3_ID;
    TIMER3_CFG_R = configuration->WIDTH;
    if ((configuration->WIDTH)==(0x04) && ((configuration->TYPE)==TIMER_B)){    //condition for either single or double

    TIMER3_TBMR_R = (configuration->MODE);                                        //set oneshot or periodic and up or down

#ifdef TIMER3_INT                                                                  
    TIMER3_ICR_R  = TBIEN;                                                      //clear flag just in case
    NVIC_EN1_R   |= TIMER3_B_NVIC_INT_NUM;                                       //Enable interrupt in NVIC for timer 1    
    TIMER3_IMR_R  = TBIEN;                                                      //done in case interrupt is to be used which uncovers interrupt mask
#endif
//at user input is lower than or equal to 1 seconds (to be modified later)    
    TIMER3_TBILR_R = ticks;                                                  //count user_seconds for default
    
    
    TIMER3_CTL_R = (configuration->DEBUG_MODE);                                                        //enable timer and start operating
    TIMER3_CTL_R |= TBEN;
    }
    
    else
    {
    
    TIMER3_TAMR_R = (configuration->MODE);                                        //set oneshot or periodic and up or down

#ifdef TIMER3_INT 
    TIMER3_ICR_R  = TAIEN;                                                      //clear flag of interrupt just in case
    NVIC_EN1_R |= TIMER3_A_NVIC_INT_NUM;
    TIMER3_IMR_R  = TAIEN;                                                      //done in case interrupt is to be used which uncovers interrupt mask
#endif
//at user input is lower than or equal to 1 seconds (to be modified later)
    TIMER3_TAILR_R = ticks;                                                  //count 1 sec for default
    
    
    TIMER3_CTL_R = (configuration->DEBUG_MODE);                                                        //enable timer and start operating
    TIMER3_CTL_R |= TAEN;
    }
    break;
//**************************************************************************END OF TIMER 3*********************************************
    
    
  case TIMER4_ID:
    SYSCTL_RCGCTIMER_R |= TIMER4_ID;
    TIMER4_CFG_R = configuration->WIDTH;
    if ((configuration->WIDTH)==(0x04) && ((configuration->TYPE)==TIMER_B)){    //condition for either single or double

    TIMER4_TBMR_R = (configuration->MODE);                                        //set oneshot or periodic and up or down

#ifdef TIMER4_INT                                                                  
    TIMER4_ICR_R  = TBIEN;                                                      //clear flag just in case
    NVIC_EN2_R   |= TIMER4_B_NVIC_INT_NUM;                                       //Enable interrupt in NVIC for timer 1    
    TIMER4_IMR_R  = TBIEN;                                                      //done in case interrupt is to be used which uncovers interrupt mask
#endif
//at user input is lower than or equal to 1 seconds (to be modified later)
    TIMER4_TBILR_R = ticks;                                                  //count user_seconds for default
    
    
    TIMER4_CTL_R = (configuration->DEBUG_MODE);                                                        //enable timer and start operating
    TIMER4_CTL_R |= TBEN;
    }
    
    else
    {
    
    TIMER4_TAMR_R = (configuration->MODE);                                        //set oneshot or periodic and up or down

#ifdef TIMER4_INT 
    TIMER4_ICR_R  = TAIEN;                                                      //clear flag of interrupt just in case
    NVIC_EN2_R |= TIMER4_A_NVIC_INT_NUM;
    TIMER4_IMR_R  = TAIEN;                                                      //done in case interrupt is to be used which uncovers interrupt mask
#endif
//at user input is lower than or equal to 1 seconds (to be modified later)
    TIMER4_TAILR_R = ticks;                                                  //count 1 sec for default
    
    
    TIMER4_CTL_R = (configuration->DEBUG_MODE);                                                        //enable timer and start operating
    TIMER4_CTL_R |= TAEN;
    }
    break;
//**************************************************************************END OF TIMER 4*********************************************
    
    
  case TIMER5_ID:
    SYSCTL_RCGCTIMER_R |= TIMER5_ID;
    TIMER5_CFG_R = configuration->WIDTH;
        if ((configuration->WIDTH)==(0x04) && ((configuration->TYPE)==TIMER_B)){    //condition for either single or double

    TIMER4_TBMR_R = (configuration->MODE);                                        //set oneshot or periodic and up or down

#ifdef TIMER5_INT                                                                  
    TIMER5_ICR_R  = TBIEN;                                                      //clear flag just in case
    NVIC_EN2_R   |= TIMER5_B_NVIC_INT_NUM;                                       //Enable interrupt in NVIC for timer 1    
    TIMER5_IMR_R  = TBIEN;                                                      //done in case interrupt is to be used which uncovers interrupt mask
#endif
//at user input is lower than or equal to 1 seconds (to be modified later)    
    TIMER5_TBILR_R = ticks;                                                  //count user_seconds for default
    
    
    TIMER5_CTL_R = (configuration->DEBUG_MODE);                                                        //enable timer and start operating
    TIMER5_CTL_R |= TBEN;
    }
    
    else
    {
    
    TIMER5_TAMR_R = (configuration->MODE);                                        //set oneshot or periodic and up or down

#ifdef TIMER5_INT 
    TIMER5_ICR_R  = TAIEN;                                                      //clear flag of interrupt just in case
    NVIC_EN2_R |= TIMER5_A_NVIC_INT_NUM;
    TIMER5_IMR_R  = TAIEN;                                                      //done in case interrupt is to be used which uncovers interrupt mask
#endif
//at user input is lower than or equal to 1 seconds (to be modified later)
    TIMER5_TAILR_R = ticks;                                                  //count 1 sec for default
    
    
    TIMER5_CTL_R = (configuration->DEBUG_MODE);                                                        //enable timer and start operating
    TIMER5_CTL_R |= TAEN;
    }
    break;
//**************************************************************************END OF TIMER 5*********************************************
    
   
  }
}

//For Deinitializing timer.
void GPTM_DeInit(uint8 timer_no){
  SYSCTL_RCGCTIMER_R &= ~timer_no;
}

/*CALLBACK FUNCTION IN ORDER TO EXECUTE ISR*/
#ifdef TIMER0_INT
void GPTM0_setCallBackA(void (*a_ptr)(void)){

  localPtrToFunc0A = a_ptr;
  
}
void GPTM0_setCallBackB(void (*a_ptr)(void)){

  localPtrToFunc0B = a_ptr;

}
#endif

#ifdef TIMER1_INT
void GPTM1_setCallBackA(void (*a_ptr)(void)){

  localPtrToFunc1A = a_ptr;

}
void GPTM1_setCallBackB(void (*a_ptr)(void)){

  localPtrToFunc1B = a_ptr;

}
#endif

#ifdef TIMER2_INT
void GPTM2_setCallBackA(void (*a_ptr)(void)){

  localPtrToFunc2A = a_ptr;

}
void GPTM2_setCallBackB(void (*a_ptr)(void)){

  localPtrToFunc2B = a_ptr;

}
#endif

#ifdef TIMER3_INT
void GPTM3_setCallBackA(void (*a_ptr)(void)){
  
  localPtrToFunc3A = a_ptr;
  
}
void GPTM3_setCallBackB(void (*a_ptr)(void)){

  localPtrToFunc3B = a_ptr;

}
#endif

#ifdef TIMER4_INT
void GPTM4_setCallBackA(void (*a_ptr)(void)){

  localPtrToFunc4A = a_ptr;

}
void GPTM4_setCallBackB(void (*a_ptr)(void)){

  localPtrToFunc4B = a_ptr;

}
#endif

#ifdef TIMER5_INT
void GPTM5_setCallBackA(void (*a_ptr)(void)){

  localPtrToFunc5A = a_ptr;

}
void GPTM5_setCallBackB(void (*a_ptr)(void)){

  localPtrToFunc5B = a_ptr;

}
#endif