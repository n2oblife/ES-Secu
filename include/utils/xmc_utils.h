#pragma once 

//*****************************************************************************
// Init and basic functions
//*****************************************************************************

/*
    Initialize the CCU4 of XMC4500 uC based on Ref manual.
 */
void initCCU4(void);

/*
    Connect the LED1 to the CCU4 slice output.
 */
void connectLED(void);

/*
    Delay for a given number of milliseconds.
    (Using NOP for now; replace with timer-based delay if available)
 */
void delay_ms(uint32_t ms);


//*****************************************************************************
// Boutons handling
//*****************************************************************************

// State variables for boutons handling
volatile uint8_t button1_is_pressed = 0;
volatile uint8_t button2_is_pressed = 0;

/*
    Handle button1 press event.
 */
uint32_t get_time_ms(void);

/*
    Wrapper for handling button1 press event.
 */
void button1_wrapper(void (*funct)(void));

/*
    Wrapper for handling button2 press event.
 */
void button2_wrapper(void (*funct)(void));

