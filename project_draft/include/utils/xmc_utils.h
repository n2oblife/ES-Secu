#pragma once 

#include <xmc_common.h>

//*****************************************************************************
// Init and basic functions
//*****************************************************************************

// clk handling
#define TIMER_RESOLUTION 100 // 100 ms resolution (10 Hz)
volatile size_t ticks = 0;

const float pwm_rate = 0; // duty cycle -> 0% when not dimming 

/*
    Initialize the CCU4 of XMC4500 uC based on Ref manual.
 */
void initCCU4(void);

/*
    Connect the LED1 to the CCU4 slice output.
 */
void connectLED(void);

/*
    SysTick interrupt handler.
 */
void SysTick_Handler(void);

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

