#include <morse.h>

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