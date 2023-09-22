#include "lcd.h"
#include "keypad.h"
#include "gpio.h"
#include "Calculator.h"
#include "time_operations.h"
#include "gptm.h"

#define NUM_OF_MODES            3

#define CALCULATOR_MODE         0
#define STOPWATCH_MODE          1
#define TIMER_MODE              2 

char keypadBuff[16];    
int8 keypadBuffCntr = 0;

uint8 curMode = 0;      // curMode corresponds to current mode displayed on LCD

volatile uint16 timerModeSeconds = 0;    // current number of seconds of the timer mode
volatile uint8 timerModeActive = 0;      // 0 if timer mode is not active, 1 if timer mode active
volatile uint8 timerModeTimeChanged = 1; // when the timerModeSeconds changes, this flag is raised to one to update current time on LCD

volatile uint16 stopwatchModeSeconds = 0;       // current number of seconds of the stopwatch mode
volatile uint16 stopwatchModeActive = 0;        // 0 if stopwatch mode is not active, 1 if stopwatch mode is active
volatile uint8 stopwatchModeTimeChanged = 1;    // when the stopwatchModeSecond changes, this flag is raised to one to update current time on LCD

void displayCurrentStatus(void);


// ISR for timer0 which controls timer mode
void timer0ISR(void)
{
  if(timerModeSeconds == 0)
  {
    GPIO_write(PORTF_ID, PIN1_ID, GPIO_LOGIC_HIGH);     // output high on pin when the timer mode is timout
  }
  else
  {
    timerModeSeconds--;         // if timer didn't timeout, continue decrementing current seconds
    timerModeTimeChanged = 1;   // flag is raised to update current time on LCD
  }
}


// ISR for timer1 which controls stopwatch mode
void timer1ISR(void)
{
  stopwatchModeSeconds++;           // As long as timer1 (which controls the stopwatch) is working, keep incrementing current seconds
  stopwatchModeTimeChanged = 1;     // flag is raised to update current time on LCD    
}


// ISR for PORTD to enable timer1 which controls the stopwatch
void portdISR(void)
{
  if(!stopwatchModeActive)
  {
    Timer_Conf timer1Conf = {TIMER_DOUBLE_MODE, TIMER_PERIODIC_UP, TIMER_A, TIMER1_ID, 1, TA_HALT};     // Timer Configuration struct
    GPTM_Init(&timer1Conf);                     // Init Timer
    stopwatchModeActive = 1;                         
    stopwatchModeTimeChanged = 1;
    displayCurrentStatus();                     // Update "SN" part in LCD
  }
  GPIO_interruptClear(PORTD_ID, PIN7_ID);       // Clear interrupt flag
}


// ISR for PORTA to disable timer1 which controls the stopwatch causing the stopwatch to pause
void portaISR(void)
{
  if(stopwatchModeActive)
  {
    GPTM_DeInit(TIMER1_ID);             // DeInit Timer
    stopwatchModeActive = 0;
    displayCurrentStatus();             // Update "SW" part in LCD
    GPIO_write(PORTF_ID, PIN2_ID, GPIO_LOGIC_LOW);
  }
  GPIO_interruptClear(PORTA_ID, PIN2_ID);       // Clear interrupt flag
}


// ISR for PORTE to reset stopwatch
void porteISR(void)
{
  stopwatchModeSeconds = 0;             // reset seconds to 0
  stopwatchModeTimeChanged = 1;
  GPIO_write(PORTF_ID, PIN2_ID, GPIO_LOGIC_LOW);
  GPIO_interruptClear(PORTE_ID, PIN4_ID); // Clear interrupt flag
}


// This function clears screen, display input string, and move cursor to the second row
void diplayCurrentMode(const char* mode)
{
  LCD_clearScreen();
  LCD_displayString(mode);
  LCD_moveCursor(1, 0); 
}


// This function updates "SN" part and "SW" part in LCD
void displayCurrentStatus(void)
{
  LCD_moveCursor(0, 11);
  LCD_displayCharacter('S');    // 'S' Represents stopwatch mode
  if(stopwatchModeActive)
  {
    LCD_displayCharacter('W');  // 'W' Represents working
  }
  else
  {
    LCD_displayCharacter('N');  // 'N' Represents not working
  }
  
  LCD_displayString(" T");      // 'T' Represents timer mode
  if(timerModeActive)
  {
    LCD_displayCharacter('W');  // 'W' Represents working
  }
  else
  {
    LCD_displayCharacter('N');  // 'N' Represents not working
  }
  
  LCD_moveCursor(1, 0);
}


void calculatorMode(void)
{
  while(1)
  {
    keypadBuffCntr = 0;
    diplayCurrentMode("CALCULATOR");    // display "CALCULATOR" on screen
    displayCurrentStatus();             // display "SN" (or "SW") and "TN" (or "TW") part in LCD         
    uint8 keypadChar;
    uint8 operationChar;
    while(1)    // keep receiving from keypad                    
    {
      keypadChar = KEYPAD_read();       // Wait for keypad input
      if(keypadChar == '#')             // '#' key returns to main menu
      {
        keypadBuffCntr = 0;
        LCD_clearScreen();
        return;
      }
      if(keypadChar == 'D')             // 'D' does nothing in that case
      {
        continue;
      }
      if((operationChar = CALCULATOR_recOperation(keypadChar)))         // if the user enetered any operation key 'A', 'B', 'C', '*', stop receiving from keypad, store the equivalent opreation in operationChar variable
      {
        break;
      }
      LCD_displayCharacter(keypadChar); 
      keypadBuff[keypadBuffCntr++] = keypadChar;        // store first number in buffer
    }
    keypadBuff[keypadBuffCntr] = '\0';
    keypadBuffCntr = 0;
    
    CALCULATOR_recFirstNum(keypadBuff);   
    LCD_displayCharacter(operationChar);  // display '+', '-', '/', or '*'      
    
    while(1)    // keep receiving from keypad
    {
      keypadChar = KEYPAD_read();       // Wait for another keypad input
      if(keypadChar == '#')     // '#' key returns to main menu
      {
        keypadBuffCntr = 0;
        LCD_clearScreen();
        return;
      }
      if('A' == keypadChar || 'B' == keypadChar || 'C' == keypadChar || '*' == keypadChar)      // 'A', 'B', 'C', 'D' do nothing in that case
      {
        continue;
      }
      if('D' == keypadChar)     // 'if the user enetered 'D' which corresponds to '=', stop receving from keypad
      {
        break;
      }
      LCD_displayCharacter(keypadChar);
      keypadBuff[keypadBuffCntr++] = keypadChar;    // store second number in buffer  
    }
    keypadBuff[keypadBuffCntr] = '\0';
    keypadBuffCntr = 0;
    
    CALCULATOR_recSecondNum(keypadBuff);
    LCD_displayCharacter('=');
    
    LCD_intgerToString(CALCULATOR_getResult());     // calculate the result and display it to the LCD

    do
    {
      keypadChar = KEYPAD_read();
    }while('#' != keypadChar && 'D' != keypadChar);     // any key pressed on keypad does nothing as long as this key is not '#' and 'D'.. 'D' clears screens and allows for another operation
    
    if('#' == keypadChar)       // '#' key returns to main menu
    {
      keypadBuffCntr = 0;       
      return;
    }
  }
}


void stopwatchMode(void)
{
  uint8 ovfIndicator;
  while(1)
  {
    stopwatchModeTimeChanged = 0;
    char keypadStr[16];
    LCD_clearScreen();
    ovfIndicator = fromSecToTime(stopwatchModeSeconds, keypadStr);     // convert current stopwatch time in seconds, to time format "MM:SS" which is stored in keypadStr
    if(ovfIndicator)
    {
      GPIO_write(PORTF_ID, PIN2_ID, GPIO_LOGIC_HIGH);
    }
    LCD_displayString(keypadStr);       // keypadStr which is in the format "MM:SS" is displayed on the LCD
    displayCurrentStatus();             // display "SN" (or "SW") and "TN" (or "TW") part in LCD       
    
    LCD_moveCursor(1, 0);
    LCD_displayString("SW1:menu");   

    while(!stopwatchModeTimeChanged)
    {
      if(!GPIO_read(PORTF_ID, PIN4_ID)) // if the user pressed SW1, the user is returned to the main menu without affecting the stopwatch time
      {
        while(!GPIO_read(PORTF_ID, PIN4_ID));
        return;
      }        
    }
  }
}

void timerMode(void)
{
  uint8 keypadChar; 
  keypadBuffCntr = 0;
  diplayCurrentMode("TIMER"); // display "TIMER" on screen
  displayCurrentStatus();     // display "SN" (or "SW") and "TN" (or "TW") part in LCD 
  // There is a menu if the timer mode is working, and another menu if the timer mode is not working
  if(!timerModeActive)  // if timer mode is not working, display its corresponding menu              
  {
    LCD_displayString("--:--");

    for(int8 input = 0; input < 4; input++)    // receive for characters from LCD
    {
      keypadChar = KEYPAD_read();               // wait input from keypad
      if(keypadChar == 'D')                     // 'D' key starts timer operation
      {
        for(uint8 remaining = input; remaining < 4; remaining++)        // this loop puts the remaining characters to '0'. For example, if the user entered only the first two characters, the remaning two characters are set to '0'
        {
          keypadBuff[keypadBuffCntr++] = '0';
        }
        break;  // stop receiving from keypad
      }
      if(keypadChar == 'A' || keypadChar == 'B' || keypadChar == 'C' || keypadChar == '*')      // 'A', 'B', 'C', 'D' do nothing in that case
      {
        input--;
        continue;
      }
      if(keypadChar == '#')     // '#' returns to main menu
      {
        keypadBuffCntr = 0;
        LCD_clearScreen();
        return;
      }
  
      keypadBuff[keypadBuffCntr++] = keypadChar;        // store current character to buffer
      // The first two characters are placed before ':' in LCD, and the last two characters are placed after the ':'
      if(input < 2)
      {
        LCD_moveCursor(1, input);
      }
      else
      {
        LCD_moveCursor(1, input + 1);
      }
      LCD_displayCharacter(keypadChar);
    }
  
    keypadBuff[keypadBuffCntr] = '\0';
    while(keypadChar != 'D')    // wait for 'D' to be pressed
    {
      keypadChar = KEYPAD_read();
      if(keypadChar == '#')     // '#' returns to main menu
      {
        keypadBuffCntr = 0;
        LCD_clearScreen();
        return;
      }
    }
    timerModeSeconds = fromTimeToSec(keypadBuff);       // convert from time format "MMSS" which is stored in buffer to number of seconds required for the timer
    keypadBuffCntr = 0;
    if(timerModeSeconds == 0xFFFF)      // if the user eneted wrong time, return to main menu
    {
      timerModeSeconds = 0;
      return;
    }
    Timer_Conf timer0Conf = {TIMER_DOUBLE_MODE, TIMER_PERIODIC_UP, TIMER_A, TIMER0_ID, 1, TA_HALT};     // Timer Configuration struct
    GPTM_Init(&timer0Conf);     // Init timer controlling timer mode
    timerModeActive = 1;
  }
  // Switch to timer mode working menu
  while(1)
  {
    timerModeTimeChanged = 0;
    char keypadStr[16];
    LCD_clearScreen();
    fromSecToTime(timerModeSeconds, keypadStr);   // convert current timer mode time in seconds, to time format "MM:SS" which is stored in keypadStr      
    LCD_displayString(keypadStr);       // display time format "MM:SS"
    displayCurrentStatus();     // display "SN" (or "SW") and "TN" (or "TW") part in LCD  
    
    LCD_moveCursor(1, 0);
    LCD_displayString("SW1:menu SW2:rst");

    while(!timerModeTimeChanged)
    {
      if(!GPIO_read(PORTF_ID, PIN0_ID))         // If SW2 is pressed 
      {
        while(!GPIO_read(PORTF_ID, PIN0_ID));   
        GPTM_DeInit(TIMER0_ID);                // DeInit timer mode
        LCD_clearScreen();
        timerModeSeconds = 0;
        timerModeActive = 0;
        GPIO_write(PORTF_ID, PIN1_ID, GPIO_LOGIC_LOW);  // output low on pin
        return;
      }
      if(!GPIO_read(PORTF_ID, PIN4_ID)) // If SW1 is pressed, return to main menu without affecting timer 
      {
        while(!GPIO_read(PORTF_ID, PIN4_ID));
        return;
      }        
    }
  }
  
}

// This function corresponds to main menu
void modeChooser()
{
  uint8 modeChosen = 0;
  uint8 getNext = 1;    // only display on LCD if there are any updates
  do{
    if(getNext)
    {
      getNext = 0;
      if(CALCULATOR_MODE == curMode)    // main menu is currently asking user to enter calculator mode
      {
        diplayCurrentMode("Calculator");
        displayCurrentStatus(); // display "SN" (or "SW") and "TN" (or "TW") part in LCD 
        LCD_displayString("SW1 ok   SW2 nxt"); 
      }
      else if(STOPWATCH_MODE == curMode)
      {
        diplayCurrentMode("Stopwatch");
        displayCurrentStatus(); // display "SN" (or "SW") and "TN" (or "TW") part in LCD 
        LCD_displayString("SW1 ok   SW2 nxt");
      }
      else
      {
        diplayCurrentMode("Timer");
        displayCurrentStatus(); // display "SN" (or "SW") and "TN" (or "TW") part in LCD 
        LCD_displayString("SW1 ok   SW2 nxt");      
      }
    }
    if(!GPIO_read(PORTF_ID, PIN0_ID))   // if SW2 is pressed 
    {
      while(!GPIO_read(PORTF_ID, PIN0_ID));
      curMode++;        // get next mode
      if(NUM_OF_MODES <= curMode) 
      {
        curMode = 0;
      }
      getNext = 1;      // only display on LCD if there are any updates
    }
    if(!GPIO_read(PORTF_ID, PIN4_ID))   // if SW1 is pressed, enter mode
    {
      while(!GPIO_read(PORTF_ID, PIN4_ID));
      modeChosen = 1;
    }
  }while(!modeChosen);
}


void main()
{
  LCD_init();           
  KEYPAD_init();        
  GPIO_init(PORTF_ID, FIRST_THREE_PINS | PIN4_ID);        // init PIN0 (for SW2), PIN1 (for Buzzer), PIN2 (for stopwatch ovf), PIN4 (for SW1) in PORTF
  GPIO_init(PORTD_ID, PIN7_ID);                         // init PIN7 in PORTD (for stopwatch mode to start counting) 
  GPIO_init(PORTA_ID, PIN2_ID);                         // init PIN2 in PORTA (for stopwatch mode to pause) 
  GPIO_init(PORTE_ID, PIN4_ID);                         // init PIN4 in PORTE (for stopwatch mode to reset)
  GPIO_setDirection(PORTF_ID, PIN1_ID | PIN2_ID, GPIO_OUTPUT);    // PIN1 (for Buzzer) is output, PIN2 (for stopwatch ovf) is output
  GPIO_setDirection(PORTF_ID, PIN0_ID | PIN4_ID, GPIO_INPUT_PULLUP); // PIN0 and PIN4 are input working pullup
  GPIO_setDirection(PORTD_ID, PIN7_ID, GPIO_INPUT_PULLUP);           // PIN7 is input working pullup
  GPIO_setDirection(PORTA_ID, PIN2_ID, GPIO_INPUT_PULLUP);           // PIN2 is input working pullup   
  GPIO_setDirection(PORTE_ID, PIN4_ID, GPIO_INPUT_PULLUP);           // PIN4 is input working pullup   
  
  GPIO_interruptEnable(PORTD_ID, PIN7_ID, GPIO_FALLING_EDGE);   // init PIN7 interrupt in PORTD (for stopwatch mode to start counting) 
  GPIO_interruptEnable(PORTA_ID, PIN2_ID, GPIO_FALLING_EDGE);   // init PIN2 interrupt in PORTA (for stopwatch mode to pause) 
  GPIO_interruptEnable(PORTE_ID, PIN4_ID, GPIO_FALLING_EDGE);   // init PIN4 interrupt in PORTE (for stopwatch mode to reset)        
  GPIO_setCallBack(PORTD_ID, portdISR);
  GPIO_setCallBack(PORTA_ID, portaISR);
  GPIO_setCallBack(PORTE_ID, porteISR);
  
  GPTM0_setCallBackA(timer0ISR);
  GPTM1_setCallBackA(timer1ISR);
  
  displayCurrentStatus();       // display "SN" (or "SW") and "TN" (or "TW") part in LCD 
  
  while(1)
  {
    modeChooser();      // dislay main menu, processor keeps in that function as long as the user did not choose any mode
    if(CALCULATOR_MODE == curMode)      // if user chosed calculator mode
    {
      calculatorMode();         // enter calculator mode, processor keeps in that function as long as the user did not choose to exit calculator mode
    }
    else if(STOPWATCH_MODE == curMode)  // if user chosed stopwatch mode
    {
      stopwatchMode();          // enter stopwatch mode, processor keeps in that function as long as the user did not choose to exit stopwatch mode
    }
    else if(TIMER_MODE == curMode)      // if user chosed timer mode
    {
      timerMode();              // enter timer mode, processor keeps in that function as long as the user did not choose to exit timer mode
    }
  }
}
