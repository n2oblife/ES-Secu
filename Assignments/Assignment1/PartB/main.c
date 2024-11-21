#include <xmc_gpio.h>
#include <stdio.h>
#include <stdbool.h>

// ********************
// GPIO Pins
#define LED_PIN 1
#define BUTTON1_PIN 14
#define BUTTON2_PIN 15

// GPIO Configuration for LED and Buttons
const XMC_GPIO_CONFIG_t LED_config = {
    .mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL,
    .output_level = XMC_GPIO_OUTPUT_LEVEL_LOW,
    .output_strength = XMC_GPIO_OUTPUT_STRENGTH_STRONG_SHARP_EDGE
};

const XMC_GPIO_CONFIG_t BUTTON_config ={
    .mode=XMC_GPIO_MODE_INPUT_TRISTATE,
    .output_level=XMC_GPIO_OUTPUT_LEVEL_LOW,
    .output_strength=XMC_GPIO_OUTPUT_STRENGTH_STRONG_SHARP_EDGE
};


// ********************
// Time Constants
#define TICKS_FREQUENCY 1000       // 1 ms resolution
#define DEBOUNCE_DELAY 50          // 50 ms debounce delay
static volatile uint32_t ticks = 0;// keep track of time

// ********************
// Morse Code Timing Constants
#define DOT_DURATION 100              // 100 ms for a dot
#define DASH_DURATION 3*DOT_DURATION  // 300 ms for a dash (3x dot)
#define SYMBOL_SPACE DOT_DURATION     // Space between symbols in a letter
#define LETTER_SPACE 3*DOT_DURATION   // Space between letters (3x dot)
#define WORD_SPACE 7*DOT_DURATION     // Space between words (7x dot)

// Morse Code Map for Characters 'A'-'Z' and Digits '0'-'9'
const char *morse_map[36] = {
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---",                    // A-J
  "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-",                      // K-T
  "..-", "...-", ".--", "-..-", "-.--", "--..",                                             // U-Z
  "-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----."  // 0-9                                                     // 8-9
};
// ********************
// Global State Variables
bool is_transmitting = false;
uint32_t last_button1_press = 0;
uint32_t second_last_button1_press = 0;

// ********************
// Function Prototypes
void SysTick_Handler(void);
void delay_ms(uint32_t ms);

void send_morse_code(const char *message);
void send_character(char c);
void send_number(uint32_t number);
void send_dot(void);
void send_dash(void);

bool is_button_pressed(XMC_GPIO_PORT_t *port, uint8_t pin);

int main(void) {
    // Initialize LED and Buttons
    XMC_GPIO_Init(XMC_GPIO_PORT1, LED_PIN, &LED_config);
    XMC_GPIO_Init(XMC_GPIO_PORT1, BUTTON1_PIN, &BUTTON_config);
    XMC_GPIO_Init(XMC_GPIO_PORT1, BUTTON2_PIN, &BUTTON_config);

    // Configure SysTick for 1 ms ticks
    SysTick_Config(SystemCoreClock / TICKS_FREQUENCY);

    while (1) {
        // Button1 pressed: Send "I CAN MORSE"
        if (is_button_pressed(XMC_GPIO_PORT1, BUTTON1_PIN) && !is_transmitting) {
            delay_ms(DEBOUNCE_DELAY);
            if (is_button_pressed(XMC_GPIO_PORT1, BUTTON1_PIN)) {
                is_transmitting = true;

                // Update press timestamps
                second_last_button1_press = last_button1_press;
                last_button1_press = ticks;

                // Send Morse code
                send_morse_code("I CAN MORSE");
                is_transmitting = false;
            }
        }

        // Button2 pressed: Send time difference
        if (is_button_pressed(XMC_GPIO_PORT1, BUTTON2_PIN) && !is_transmitting) {
            delay_ms(DEBOUNCE_DELAY);
            if (is_button_pressed(XMC_GPIO_PORT1, BUTTON2_PIN)) {
                is_transmitting = true;

                uint32_t time_diff;
                if (last_button1_press == 0) {
                    // No Button1 press yet
                    time_diff = 0;
                } else if (second_last_button1_press == 0) {
                    // Only one Button1 press
                    time_diff = last_button1_press;
                } else {
                    // Time between the last two Button1 presses
                    time_diff = last_button1_press - second_last_button1_press;
                }

                send_number(time_diff);
                is_transmitting = false;
            }
        }
    }

    return 0;
}

// SysTick Handler for 1 ms ticks
void SysTick_Handler(void) {
    ticks++;
}

// Delay with millisecond precision
void delay_ms(uint32_t ms) {
    uint32_t start = ticks;
    while ((ticks - start) < ms);
}

// Check if a button is pressed (active low)
bool is_button_pressed(XMC_GPIO_PORT_t *port, uint8_t pin) {
    return (XMC_GPIO_GetInput(port, pin) == 0);
}

// Send a string in Morse code
void send_morse_code(const char *message) {
    for (const char *p = message; *p != '\0'; p++) {
        if (*p == ' ') {
            delay_ms(WORD_SPACE);  // Pause between words
        } else {
            send_character(*p);    // Send each character in Morse code
            if (*(p + 1) && *(p + 1) != ' ') // Add LETTER_SPACE if not end of message/word
                delay_ms(LETTER_SPACE); // TODO if pb remove this condition
        }
    }
}

// Send a character in Morse code
void send_character(char c) {
    const char *morse = NULL;
    if (c >= 'A' && c <= 'Z') {
        morse = morse_map[c - 'A'];
    } else if (c >= '0' && c <= '9') {
        morse = morse_map[c - '0' + 26];
    }

    if (morse) {
        while (*morse) {
            if (*morse == '.') {
                send_dot();
            } else if (*morse == '-') {
                send_dash();
            }
            morse++;
            // Add SYMBOL_SPACE only if there are more symbols in the character
            if (*morse) delay_ms(SYMBOL_SPACE);
        }
    }
}

// Send a number in Morse code
void send_number(uint32_t number) {
    char buffer[11]; // Max 10 digits + null terminator
    snprintf(buffer, sizeof(buffer), "%lu", (unsigned long)number); // Convert number to string
    send_morse_code(buffer);
}

// Send a dot (.)
void send_dot(void) {
    // PORT1->OMR = (uint32_t)0x1U << 1;  // Set P1.1 (Turn on LED)
    XMC_GPIO_SetOutputHigh(XMC_GPIO_PORT1, LED_PIN);
    delay_ms(DOT_DURATION);
    XMC_GPIO_SetOutputLow(XMC_GPIO_PORT1, LED_PIN);
    // PORT1->OMR = 0x10000U << 1;  // Clear P1.1 (Turn off LED)
}

// Send a dash (-)
void send_dash(void) {
    // PORT1->OMR = (uint32_t)0x1U << 1;  // Set P1.1 (Turn on LED)
    XMC_GPIO_SetOutputHigh(XMC_GPIO_PORT1, LED_PIN);
    delay_ms(DASH_DURATION);
    XMC_GPIO_SetOutputLow(XMC_GPIO_PORT1, LED_PIN);
    // PORT1->OMR = 0x10000U << 1;  // Clear P1.1 (Turn off LED)

}
