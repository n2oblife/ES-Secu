#include <morse.h>

void LED1_On() {
    PORT1->IOCR0 = (PORT1->IOCR0 & ~PORT0_IOCR0_PC1_Msk)| (ALT3 << PORT1_IOCR0_PC1_Pos);
}

void LED1_Off() {
    PORT1->IOCR0 = (PORT1->IOCR0 & ~PORT0_IOCR0_PC1_Msk);
}

void send_morse_code(const char *message) {
    if (is_transmitting) return;  // Ignore button presses during transmission
    is_transmitting = 1;

    for (const char *p = message; *p != '\0'; p++) {
        if (*p == ' ') {
            for (int i = 0; i < WORD_SPACE* 1000; i++) {}
            // delay_ms(WORD_SPACE);  // Pause between words
        } else {
            send_character(*p);
            for (int i = 0; i < LETTER_SPACE* 1000; i++) {}
            // delay_ms(LETTER_SPACE);  // Pause between letters
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
            for (int i = 0; i < DOT_DURATION* 1000; i++) {}
            // delay_ms(DOT_DURATION);  // Inter-symbol space within a character
        }
    }
}

void send_dot() {
    LED1_On();
    for (int i = 0; i < DOT_DURATION* 1000; i++) {}
    // delay_ms(DOT_DURATION);
    LED1_Off();  
}

void send_dash() {
    LED1_On();
    for (int i = 0; i < DASH_DURATION* 1000; i++) {}
    // delay_ms(DASH_DURATION);
    LED1_Off();  
}

void send_number_as_morse(uint32_t number) {
    char buffer[10];
    snprintf(buffer, sizeof(buffer), "%u", number);  // Convert number to string
    send_morse_code(buffer);  // Send each digit as Morse code
}