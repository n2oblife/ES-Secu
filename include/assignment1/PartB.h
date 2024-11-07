#pragma once

#include <xmc_common.h>
#include <xmc_utils.h>
#include <xmc_morse.h>


// State variables for buttons handling in assignment1 partB
volatile uint32_t button1_press_time = 0;
volatile uint32_t button1_last_time = 0;
volatile uint8_t button1_press_count = 0;

/*
    Handle button1 press event inside the bouton1_wrapper.
 */
void bouton1_handler(void);

/*
    Handle button2 press event inside the bouton2_wrapper.
 */
void bouton2_handler(void);