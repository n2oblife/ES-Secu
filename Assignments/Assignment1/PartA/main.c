#include <xmc_gpio.h>

// ********************
// XMC configuration for LED
const XMC_GPIO_CONFIG_t LED_config = \
        {.mode=XMC_GPIO_MODE_OUTPUT_PUSH_PULL,\
        .output_level=XMC_GPIO_OUTPUT_LEVEL_LOW,\
        .output_strength=XMC_GPIO_OUTPUT_STRENGTH_STRONG_SHARP_EDGE};


// ********************
// Time Constants
#define TICKS_FREQUENCY 1000            // Tick frequency in Hz, 1 ms resolution
static volatile uint32_t ticks = 0;     // keep track of time

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
// Functions Definition
void initCCU4(void);
void connectLED(void);

void SysTick_Handler(void);
static inline uint32_t current_time(void);
void delay_ms(uint32_t ms);

void send_morse_code(const char *message);
void send_character(char c);
void send_dot(void);
void send_dash(void);


// ********************
int main(void) {
    // initCCU4();       // Initialize CCU4 for timing
    // connectLED();     // Set up LED1

        /*2nd parameter = 1 refers to LED 1*/
    XMC_GPIO_Init(XMC_GPIO_PORT1, 1, &LED_config);

        /* System timer configuration */
    SysTick_Config(SystemCoreClock / TICKS_FREQUENCY);

    while (1) {
        send_morse_code("I CAN MORSE");  // Send the string in Morse code
        delay_ms(5000);                 // 5-second pause between repetitions
    }

    return 0;
}
// ********************


// ********************
// Functions Implmeentation
void initCCU4() {
    // Release the reset of the CCU40
    SCU_RESET->PRCLR0 = SCU_RESET_PRCLR0_CCU40RS_Msk;
    // Enable the clock for the CCU40 (step 3)
    SCU_CLK->CLKSET = SCU_CLK_CLKSET_CCUCEN_Msk;
    // Disable the CCU40 slice protection
    CCU40->GIDLC = CCU4_GIDLC_SPRB_Msk;
    // Set the period and prescaler for the timer slice
    CCU40_CC42->PRS = 0xffff;
    CCU40_CC42->PSC = 0b1010;           // Prescaler to get 1ms resolution
    // Enable CCU40 slice
    CCU40->GCSS = CCU4_GCSS_S2SE_Msk;
    // Set the run bit to start the timer
    CCU40_CC42->TCSET = CCU4_CC4_TCSET_TRBS_Msk;
}

void connectLED() {
    const static uint8_t ALT3 = 0b10011;
    PORT1->IOCR0 = (PORT1->IOCR0 & ~PORT0_IOCR0_PC1_Msk) | (ALT3 << PORT1_IOCR0_PC1_Pos);
}

void SysTick_Handler(){
  ticks++;
}

static inline uint32_t current_time(){
  return ticks;
}

void delay_ms(uint32_t ms) {
//   uint32_t start_pt = current_time();
  uint32_t start_pt = ticks;
//   while ((current_time() - start_pt) < ms);     
  while ((ticks - start_pt) < ms);     
}


void send_morse_code(const char *message) {
    for (const char *p = message; *p != '\0'; p++) {
        if (*p == ' ') {
            delay_ms(WORD_SPACE);  // Pause between words
        } else {
            send_character(*p);    // Send each character in Morse code
            if (*(p + 1) && *(p + 1) != ' ') // Add LETTER_SPACE if not end of message/word
                delay_ms(LETTER_SPACE);
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

void send_dot() {
    // PORT1->OMR = (uint32_t)0x1U << 1;  // Set P1.1 (Turn on LED)
    XMC_GPIO_SetOutputHigh(XMC_GPIO_PORT1, 1);
    delay_ms(DOT_DURATION);
    XMC_GPIO_SetOutputLow(XMC_GPIO_PORT1, 1);
    // PORT1->OMR = 0x10000U << 1;  // Clear P1.1 (Turn off LED)
}

void send_dash() {
    // PORT1->OMR = (uint32_t)0x1U << 1;  // Set P1.1 (Turn on LED)
    XMC_GPIO_SetOutputHigh(XMC_GPIO_PORT1, 1);
    delay_ms(DASH_DURATION);
    XMC_GPIO_SetOutputLow(XMC_GPIO_PORT1, 1);
    // PORT1->OMR = 0x10000U << 1;  // Clear P1.1 (Turn off LED)
}

