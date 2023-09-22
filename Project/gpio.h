 /******************************************************************************
 *
 * Module: GPIO
 *
 * File Name: gpio.h
 *
 * Description: General Purpose input/output
 *
 * Author: Team 17
 *
 *******************************************************************************/


#ifndef GPIO_H
#define GPIO_H

#include "std_types.h"

#define PORTA_ID                0
#define PORTB_ID                1
#define PORTC_ID                2
#define PORTD_ID                3
#define PORTE_ID                4
#define PORTF_ID                5

#define NUM_OF_PORTS            6

#define PIN0_ID                 1
#define PIN1_ID                 (PIN0_ID << 1)
#define PIN2_ID                 (PIN0_ID << 2)
#define PIN3_ID                 (PIN0_ID << 3)
#define PIN4_ID                 (PIN0_ID << 4)
#define PIN5_ID                 (PIN0_ID << 5)
#define PIN6_ID                 (PIN0_ID << 6)
#define PIN7_ID                 (PIN0_ID << 7)

#define FIRST_TWO_PINS          0x03
#define FIRST_THREE_PINS        0x07
#define FIRST_FOUR_PINS         0x0F
#define FIRST_FIVE_PINS         0x1F
#define FIRST_SIX_PINS          0x3F
#define FIRST_SEVEN_PINS        0x7F
#define ALL_PINS                0xFF

#define GPIO_INPUT              0x00
#define GPIO_INPUT_PULLUP       0x01
#define GPIO_INPUT_PULLDOWN     0x04
#define GPIO_OUTPUT             0x02

#define GPIO_RISING_EDGE        0xFF
#define GPIO_FALLING_EDGE       0x00  

#define GPIO_LOGIC_LOW          0x00
#define GPIO_LOGIC_HIGH         0xFF

typedef struct{
  volatile uint32* lockReg;
  volatile uint32* commitReg;
  volatile uint32* digitalEnableReg;
}GPIO_InitRegs;

typedef struct{
  volatile uint32* directionReg;
  volatile uint32* pullUpReg;
  volatile uint32* pullDownReg;
}GPIO_DirectionRegs;

typedef struct{
  volatile uint32* intEventReg;
  volatile uint32* intMaskReg;
  volatile uint32* intClearReg;
  volatile uint32* rawIntStatusReg;
}GPIO_InterruptRegs;      

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
uint8 GPIO_init(uint8 GPIO_port, uint8 GPIO_pin);
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
uint8 GPIO_setDirection(uint8 GPIO_port, uint8 GPIO_pin, uint8 GPIO_direction);
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
uint8 GPIO_interruptEnable(uint8 GPIO_port, uint8 GPIO_pin, uint8 GPIO_intEvent);
/*
  Receives Port number in GPIO_port
  (PORTA_ID, PORTB_ID, PORTC_ID, PORTD_ID, PORTE_ID, PORTF_ID)


  If interrupt occured, the function returns the corresponding pin in the given port
*/
uint8 GPIO_getIntPin(uint8 GPIO_port);
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
uint8 GPIO_interruptClear(uint8 GPIO_port, uint8 GPIO_pin);
/*
  Receives Port number in GPIO_port
  (PORTA_ID, PORTB_ID, PORTC_ID, PORTD_ID, PORTE_ID, PORTF_ID)


  and ptrToFunc, which corresponds to the required handler wanted for the given port
*/
uint8 GPIO_setCallBack(uint8 GPIO_port, void(*ptrToFunc)(void));
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
uint8 GPIO_write(uint8 GPIO_port, uint8 GPIO_pin, uint8 GPIO_logicVal);
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
uint32 GPIO_read(uint8 GPIO_port, uint8 GPIO_pin);

#endif
