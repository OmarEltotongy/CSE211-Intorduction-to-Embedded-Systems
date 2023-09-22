 /******************************************************************************
 *
 * Module: GPIO
 *
 * File Name: gpio.c
 *
 * Description: General Purpose input/output
 *
 * Author: Team 17
 *
 *******************************************************************************/

#include "gpio.h"
#include "tivac_regs.h"
#include "common_macros.h"

/* 
  Registers pointers for GPIO_init function
*/
const static GPIO_InitRegs initRegs[NUM_OF_PORTS] = {
  {&GPIO_PORTA_LOCK_R, &GPIO_PORTA_CR_R, &GPIO_PORTA_DEN_R}, 
  {&GPIO_PORTB_LOCK_R, &GPIO_PORTB_CR_R, &GPIO_PORTB_DEN_R}, 
  {&GPIO_PORTC_LOCK_R, &GPIO_PORTC_CR_R, &GPIO_PORTC_DEN_R}, 
  {&GPIO_PORTD_LOCK_R, &GPIO_PORTD_CR_R, &GPIO_PORTD_DEN_R}, 
  {&GPIO_PORTE_LOCK_R, &GPIO_PORTE_CR_R, &GPIO_PORTE_DEN_R}, 
  {&GPIO_PORTF_LOCK_R, &GPIO_PORTF_CR_R, &GPIO_PORTF_DEN_R}
};

/*
  Registers pointers for GPIO_setDirection function
*/
const static GPIO_DirectionRegs directionRegs[NUM_OF_PORTS] = {
  {&GPIO_PORTA_DIR_R, &GPIO_PORTA_PUR_R, &GPIO_PORTA_PDR_R},
  {&GPIO_PORTB_DIR_R, &GPIO_PORTB_PUR_R, &GPIO_PORTB_PDR_R},
  {&GPIO_PORTC_DIR_R, &GPIO_PORTC_PUR_R, &GPIO_PORTC_PDR_R},
  {&GPIO_PORTD_DIR_R, &GPIO_PORTD_PUR_R, &GPIO_PORTD_PDR_R},
  {&GPIO_PORTE_DIR_R, &GPIO_PORTE_PUR_R, &GPIO_PORTE_PDR_R},
  {&GPIO_PORTF_DIR_R, &GPIO_PORTF_PUR_R, &GPIO_PORTF_PDR_R}
};

const static GPIO_InterruptRegs interruptRegs[NUM_OF_PORTS] = {
  {&GPIO_PORTA_IEV_R, &GPIO_PORTA_IM_R, &GPIO_PORTA_ICR_R, &GPIO_PORTA_RIS_R},
  {&GPIO_PORTB_IEV_R, &GPIO_PORTB_IM_R, &GPIO_PORTB_ICR_R, &GPIO_PORTB_RIS_R},
  {&GPIO_PORTC_IEV_R, &GPIO_PORTC_IM_R, &GPIO_PORTC_ICR_R, &GPIO_PORTC_RIS_R},
  {&GPIO_PORTD_IEV_R, &GPIO_PORTD_IM_R, &GPIO_PORTD_ICR_R, &GPIO_PORTD_RIS_R},
  {&GPIO_PORTE_IEV_R, &GPIO_PORTE_IM_R, &GPIO_PORTE_ICR_R, &GPIO_PORTE_RIS_R},
  {&GPIO_PORTF_IEV_R, &GPIO_PORTF_IM_R, &GPIO_PORTF_ICR_R, &GPIO_PORTF_RIS_R}
};

/*
  Registers pointers for GPIO_write and GPIO_read functions
*/
static volatile uint32* const dataRegs[NUM_OF_PORTS] = {
  GPIO_PORTA_DATA_BITS_R,
  GPIO_PORTB_DATA_BITS_R,
  GPIO_PORTC_DATA_BITS_R,
  GPIO_PORTD_DATA_BITS_R,
  GPIO_PORTE_DATA_BITS_R,
  GPIO_PORTF_DATA_BITS_R
};

/*
  Each element in array corresponds to port handler (index 0 corresponds to PORTA, index 1 corresponds to PORTB, etc.)
*/
static void (*ptrToFuncs[NUM_OF_PORTS])(void) = {
  NULL, NULL, NULL, NULL, NULL, NULL
};

/*
  PORTA ISR
*/
void GPIO_portAHandler(void)
{
  if(NULL != ptrToFuncs[PORTA_ID])
  {
    ptrToFuncs[PORTA_ID]();
  }
}

/*
  PORTB ISR
*/
void GPIO_portBHandler(void)
{
  if(NULL != ptrToFuncs[PORTB_ID])
  {
    ptrToFuncs[PORTB_ID]();
  }
}

/*
  PORTC ISR
*/
void GPIO_portCHandler(void)
{
  if(NULL != ptrToFuncs[PORTC_ID])
  {
    ptrToFuncs[PORTC_ID]();
  }
}

/*
  PORTD ISR
*/
void GPIO_portDHandler(void)
{
  if(NULL != ptrToFuncs[PORTD_ID])
  {
    ptrToFuncs[PORTD_ID]();
  }
}

/*
  PORTE ISR
*/
void GPIO_portEHandler(void)
{
  if(NULL != ptrToFuncs[PORTE_ID])
  {
    ptrToFuncs[PORTE_ID]();
  }
}

/*
  PORTF ISR
*/
void GPIO_portFHandler(void)
{
  if(NULL != ptrToFuncs[PORTF_ID])
  {
    ptrToFuncs[PORTF_ID]();
  }
}

/*
  Receives Port number in GPIO_port
  (PORTA_ID, PORTB_ID, PORTC_ID, PORTD_ID, PORTE_ID, PORTF_ID)


  and Pin number in GPIO_pin
  (PIN0_ID, PIN1_ID, PIN2_ID, PIN3_ID, PIN4_ID, PIN5_ID, PIN6_ID, PIN7_ID)
  It can receive hexadecimal number in GPIO_pin
  Ex:
  (0x11, init pin 0 and pin 4)
  (0x03, init first 2 pins)
*/
uint8 GPIO_init(uint8 GPIO_port, uint8 GPIO_pin)
{
  if(NUM_OF_PORTS <= GPIO_port)
  {
    return FUNC_INPUT_ERROR;
  }
  
  SET_BIT(SYSCTL_RCGCGPIO_R, GPIO_port);
  while(GET_BIT(SYSCTL_PRGPIO_R, GPIO_port) == 0);
  
  *(initRegs[GPIO_port].lockReg) = LOCK_PASS;
  *(initRegs[GPIO_port].commitReg) |= GPIO_pin;
  *(initRegs[GPIO_port].digitalEnableReg) |= GPIO_pin;
  
  return SUCCESS;
}


/*
  Receives Port number in GPIO_port
  (PORTA_ID, PORTB_ID, PORTC_ID, PORTD_ID, PORTE_ID, PORTF_ID)


  and Pin number in GPIO_pin
  (PIN0_ID, PIN1_ID, PIN2_ID, PIN3_ID, PIN4_ID, PIN5_ID, PIN6_ID, PIN7_ID)
  It can receive hexadecimal number in GPIO_pin

  and GPIO_direction
  (GPIO_OUTPUT, GPIO_INPUT, GPIO_INPUT_PULLUP, GPIO_INPUT_PULLDOWN)
  Ex:
  (0x11, setDirection of pin 0 and pin 4)
  (0x03, setDirection first 2 pins)
*/
uint8 GPIO_setDirection(uint8 GPIO_port, uint8 GPIO_pin, uint8 GPIO_direction)
{
  if(NUM_OF_PORTS <= GPIO_port)
  {
    return FUNC_INPUT_ERROR;
  }
  
  *(directionRegs[GPIO_port].directionReg) &= ~GPIO_pin;
  *(directionRegs[GPIO_port].directionReg) |= (GPIO_pin & (GET_BIT(GPIO_direction, 1) * 0xFF));
    
  if(GET_BIT(GPIO_direction, 1) == 0)
  {
    *(directionRegs[GPIO_port].pullUpReg) &= ~GPIO_pin;
    *(directionRegs[GPIO_port].pullUpReg) |= (GPIO_pin & (GET_BIT(GPIO_direction, 0) * 0xFF));
    
    *(directionRegs[GPIO_port].pullDownReg) &= ~GPIO_pin;
    *(directionRegs[GPIO_port].pullDownReg) |= (GPIO_pin & (GET_BIT(GPIO_direction, 2) * 0xFF));
  }
  else
  {
      
  }
  
  return SUCCESS;
}


/*
  Receives Port number in GPIO_port
  (PORTA_ID, PORTB_ID, PORTC_ID, PORTD_ID, PORTE_ID, PORTF_ID)


  and Pin number in GPIO_pin
  (PIN0_ID, PIN1_ID, PIN2_ID, PIN3_ID, PIN4_ID, PIN5_ID, PIN6_ID, PIN7_ID)
  It can receive hexadecimal number in GPIO_pin

  and GPIO_intEvent
  (GPIO_RISING_EDGE, GPIO_FALLING_EDGE)
  Ex:
  (0x11, enable interrupt for pin 0 and pin 4)
  (0x03, enable interrupt for first 2 pins)
*/
uint8 GPIO_interruptEnable(uint8 GPIO_port, uint8 GPIO_pin, uint8 GPIO_intEvent)
{
  if(NUM_OF_PORTS <= GPIO_port)
  {
    return FUNC_INPUT_ERROR;
  }
  
  *(interruptRegs[GPIO_port].intEventReg) &= ~GPIO_pin;
  *(interruptRegs[GPIO_port].intEventReg) |= (GPIO_pin & GPIO_intEvent);
  
  *(interruptRegs[GPIO_port].intMaskReg) |= GPIO_pin;
  
  *(interruptRegs[GPIO_port].intClearReg) |= GPIO_pin;
  
  if(PORTF_ID != GPIO_port)
  {
    SET_BIT(NVIC_EN0_R, (GPIO_port + 16 - INT_GPIOA));
  }
  else
  {
    SET_BIT(NVIC_EN0_R, (INT_GPIOF - INT_GPIOA));
  }
  
  return SUCCESS;
}

/*
  Receives Port number in GPIO_port
  (PORTA_ID, PORTB_ID, PORTC_ID, PORTD_ID, PORTE_ID, PORTF_ID)


  If interrupt occured, the function returns the corresponding pin in the given port
*/
uint8 GPIO_getIntPin(uint8 GPIO_port)
{
  if(NUM_OF_PORTS <= GPIO_port)
  {
    return FUNC_INPUT_ERROR;
  }
  
  return *(interruptRegs[GPIO_port].rawIntStatusReg);
  
}

/*
  Receives Port number in GPIO_port
  (PORTA_ID, PORTB_ID, PORTC_ID, PORTD_ID, PORTE_ID, PORTF_ID)


  and Pin number in GPIO_pin
  (PIN0_ID, PIN1_ID, PIN2_ID, PIN3_ID, PIN4_ID, PIN5_ID, PIN6_ID, PIN7_ID)
  It can receive hexadecimal number in GPIO_pin

  Ex:
  (0x11, clear interrupt for pin 0 and pin 4)
  (0x03, clear interrupt for first 2 pins)
*/
uint8 GPIO_interruptClear(uint8 GPIO_port, uint8 GPIO_pin)
{
  if(NUM_OF_PORTS <= GPIO_port)
  {
    return FUNC_INPUT_ERROR;
  }
  
  *(interruptRegs[GPIO_port].intClearReg) |= GPIO_pin;
  
  return SUCCESS;
}

/*
  Receives Port number in GPIO_port
  (PORTA_ID, PORTB_ID, PORTC_ID, PORTD_ID, PORTE_ID, PORTF_ID)


  and ptrToFunc, which corresponds to the required handler wanted for the given port
*/
uint8 GPIO_setCallBack(uint8 GPIO_port, void(*ptrToFunc)(void))
{
  if(NUM_OF_PORTS <= GPIO_port)
  {
    return FUNC_INPUT_ERROR;
  }
  
  ptrToFuncs[GPIO_port] = ptrToFunc;
  
  return SUCCESS;
}

/*
  Receives Port number in GPIO_port
  (PORTA_ID, PORTB_ID, PORTC_ID, PORTD_ID, PORTE_ID, PORTF_ID)


  and Pin number in GPIO_pin
  (PIN0_ID, PIN1_ID, PIN2_ID, PIN3_ID, PIN4_ID, PIN5_ID, PIN6_ID, PIN7_ID)
  It can receive hexadecimal number in GPIO_pin

  and GPIO_logicVal
  (GPIO_LOGIC_HIGH, GPIO_LOGIC_LOW)
  Ex:
  (0x11, write for pin 0 and pin 4)
  (0x03, write for first 2 pins)
*/
uint8 GPIO_write(uint8 GPIO_port, uint8 GPIO_pin, uint8 GPIO_logicVal)
{
  if(NUM_OF_PORTS <= GPIO_port)
  {
    return FUNC_INPUT_ERROR;
  }

#if 0
  *(dataRegs[GPIO_port]) = GPIO_PORTF_DATA_R & ~GPIO_pin;
  *(dataRegs[GPIO_port]) |= (GPIO_pin & GPIO_logicVal);
#endif

#if 0
  GPIO_PORTF_DATA_BITS_R[GPIO_pin] = GPIO_logicVal;
#endif
  
  dataRegs[GPIO_port][GPIO_pin] = GPIO_logicVal;
  
  return SUCCESS;
}

/*
  Receives Port number in GPIO_port
  (PORTA_ID, PORTB_ID, PORTC_ID, PORTD_ID, PORTE_ID, PORTF_ID)


  and Pin number in GPIO_pin
  (PIN0_ID, PIN1_ID, PIN2_ID, PIN3_ID, PIN4_ID, PIN5_ID, PIN6_ID, PIN7_ID)
  It can receive hexadecimal number in GPIO_pin

  Ex:
  (0x11, read pin 0 and pin 4)
  (0x03, read first 2 pins)
*/
uint32 GPIO_read(uint8 GPIO_port, uint8 GPIO_pin)
{
  if(NUM_OF_PORTS <= GPIO_port)
  {
    return FUNC_INPUT_ERROR;
  }
#if 0
  return (*(dataRegs[GPIO_port]) & GPIO_pin);
#endif

#if 0
  return GPIO_PORTF_DATA_BITS_R[GPIO_pin];
#endif
  
  return dataRegs[GPIO_port][GPIO_pin];
}
