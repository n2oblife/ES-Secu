#pragma once

//*****************************************************************************
// Morse code constants
//*****************************************************************************

#define DOT_DURATION 100                // 100 ms for dot (should change for debug)
#define END_OF_MESSAGE (5000)           // 5 seconds for end of message between transmissions
#define DASH_DURATION (DOT_DURATION * 3)
#define LETTER_SPACE (DOT_DURATION * 3)
#define WORD_SPACE (DOT_DURATION * 7)

// Morse code map for the required characters
const char *morse_map[26] = {
    ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---",
    "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-",  
    "..-", "...-", ".--", "-..-", "-.--", "--.."                          
};

//*****************************************************************************
// Function prototypes to send Morse code
//*****************************************************************************

// State variable for transmission handling
volatile uint8_t is_transmitting = 0;

/*
    Send a message in Morse code by parsing it 
    and sending each character.
 */
void send_morse_code(const char *message);

/*
    Send a single character in Morse code.
 */
void send_character(char c);

/*
    Send a dot in Morse code by making the LED1 blink.
    Need of intializing the LED1 before calling this function.
 */
void send_dot(void);

/*
    Send a dash in Morse code by making the LED1 blink.
    Need of intializing the LED1 before calling this function.
 */
void send_dash(void);

/*
    Send a number in Morse code by converting it to a string
    and sending each digit.
 */
void send_number_as_morse(uint32_t number);