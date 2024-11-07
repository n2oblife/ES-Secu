#ifdef INCLUDE_PATH_AVAILABLE
    #include "PartA.h"
#else
    #include <xmc_common.h>

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

    //*****************************************************************************
    // Implementation of utils functions
    //*****************************************************************************

    void initCCU4() {
        // Release the reset of the CCU40
        SCU_RESET->PRCLR0 = SCU_RESET_PRCLR0_CCU40RS_Msk;
        // Enable the clock for the CCU40 (step 3)
        SCU_CLK->CLKSET = SCU_CLK_CLKSET_CCUCEN_Msk;
        // Disable the CCU40 slice protection
        CCU40->GIDLC = CCU4_GIDLC_SPRB_Msk;
        // Set the period and prescaler for the timer slice
        CCU40_CC42->PRS = 0xffff;
        CCU40_CC42->PSC = 0b1010;
        CCU40_CC42->CRS = 0xffff * (1 - 0.5); // Set duty cycle for dimming (50%)
        // Enable CCU40 slice
        CCU40->GCSS = CCU4_GCSS_S2SE_Msk;
        // Clear slice input selection
        CCU40->GIDLC = CCU4_GIDLC_CS2I_Msk;
        // Set the run bit to start the timer
        CCU40_CC42->TCSET = CCU4_CC4_TCSET_TRBS_Msk;
    }

    void connectLED() {
        const static uint8_t ALT3 = 0b10011;
        PORT1->IOCR0 = (PORT1->IOCR0 & ~PORT0_IOCR0_PC1_Msk) | (ALT3 << PORT1_IOCR0_PC1_Pos);
    }

    void delay_ms(uint32_t ms) {
        for (volatile uint32_t i = 0; i < ms * 1000; i++)
            __NOP();  // TODO No Operation; Replace with timer-based delay if available
    }

    uint32_t get_time_ms() {
        // TODO
        // Placeholder: Implement timer-based time retrieval
    }

    void button1_wrapper(void (*funct)(void)) {
        if (is_transmitting) return;  // Ignore button presses during transmission
        button1_is_pressed = 1;

        (*funct)();  // Call the function to be wrapped
        
        button1_is_pressed = 0;
    }

    void button2_wrapper(void (*funct)(void)) {
        if (is_transmitting) return;  // Ignore button presses during transmission
        button2_is_pressed = 1;
        
        (*funct)();  // Call the function to be wrapped
        
        button2_is_pressed = 0;
    }

    //*****************************************************************************
    // Implementation of Morse functions
    //*****************************************************************************

    void send_morse_code(const char *message) {
        if (is_transmitting) return;  // Ignore button presses during transmission
        is_transmitting = 1;

        for (const char *p = message; *p != '\0'; p++) {
            if (*p == ' ') {
                delay_ms(WORD_SPACE);  // Pause between words
            } else {
                send_character(*p);
                delay_ms(LETTER_SPACE);  // Pause between letters
            }
        }

        is_transmitting = 0;
    }

    void send_character(char c) {
        if (c >= 'A' && c <= 'Z') {
            const char *morse = morse_map[c - 'A'];
            while (*morse) {
                if (*morse == '.') {
                    send_dot();
                } else if (*morse == '-') {
                    send_dash();
                }
                morse++;
                delay_ms(DOT_DURATION);  // Inter-symbol space within a character
            }
        }
    }

    void send_dot() {
        XMC_GPIO_SetOutputHigh(LED1);  // Turn on LED1 for dot duration
        delay_ms(DOT_DURATION);
        XMC_GPIO_SetOutputLow(LED1);   // Turn off LED1
    }

    void send_dash() {
        XMC_GPIO_SetOutputHigh(LED1);  // Turn on LED1 for dash duration
        delay_ms(DASH_DURATION);
        XMC_GPIO_SetOutputLow(LED1);   // Turn off LED1
    }

    void send_number_as_morse(uint32_t number) {
        char buffer[10];
        snprintf(buffer, sizeof(buffer), "%u", number);  // Convert number to string
        send_morse_code(buffer);  // Send each digit as Morse code
    }

#endif


int main(void) {
    initCCU4();    // Initialize timer and LED
    connectLED();  // Connect and set up LED1

    while (1) {
        send_morse_code("I CAN MORSE"); // Continuously send message in Morse code
        delay_ms(END_OF_MESSAGE);       // 5-second delay between transmissions
    }

    return 0;
}

