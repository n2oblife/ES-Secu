#ifndef GERMAN_KEYBOARDCODES_H
#define GERMAN_KEYBOARDCODES_H

// Number of entries in the keymap
#define GERMAN_KEYMAP_SIZE (sizeof(germanKeymap) / sizeof(HIDMapping))

// Character set to test
static const char passwordCharset[] = 
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!()-_=~;:,.<>[]{}/?&$";

static const uint8_t charsetSize = sizeof(passwordCharset) - 1;  // Exclude null terminator


#define GERMAN_KEYBOARD_SC_CAPS_LOCK                                        0x01
#define GERMAN_KEYBOARD_SC_LEFT_SHIFT                                       0x02
#define GERMAN_KEYBOARD_SC_A                                                0x04
#define GERMAN_KEYBOARD_SC_B                                                0x05
#define GERMAN_KEYBOARD_SC_C                                                0x06
#define GERMAN_KEYBOARD_SC_D                                                0x07
#define GERMAN_KEYBOARD_SC_E                                                0x08
#define GERMAN_KEYBOARD_SC_F                                                0x09
#define GERMAN_KEYBOARD_SC_G                                                0x0A
#define GERMAN_KEYBOARD_SC_H                                                0x0B
#define GERMAN_KEYBOARD_SC_I                                                0x0C
#define GERMAN_KEYBOARD_SC_J                                                0x0D
#define GERMAN_KEYBOARD_SC_K                                                0x0E
#define GERMAN_KEYBOARD_SC_L                                                0x0F
#define GERMAN_KEYBOARD_SC_M                                                0x10
#define GERMAN_KEYBOARD_SC_N                                                0x11
#define GERMAN_KEYBOARD_SC_O                                                0x12
#define GERMAN_KEYBOARD_SC_P                                                0x13
#define GERMAN_KEYBOARD_SC_Q                                                0x14
#define GERMAN_KEYBOARD_SC_R                                                0x15
#define GERMAN_KEYBOARD_SC_S                                                0x16
#define GERMAN_KEYBOARD_SC_T                                                0x17
#define GERMAN_KEYBOARD_SC_U                                                0x18
#define GERMAN_KEYBOARD_SC_V                                                0x19
#define GERMAN_KEYBOARD_SC_W                                                0x1A
#define GERMAN_KEYBOARD_SC_X                                                0x1B
#define GERMAN_KEYBOARD_SC_Y                                                0x1D
#define GERMAN_KEYBOARD_SC_Z                                                0x1C
#define GERMAN_KEYBOARD_SC_1_AND_EXCLAMATION                                0x1E
#define GERMAN_KEYBOARD_SC_2_AND_QUOTES                                     0x1F
#define GERMAN_KEYBOARD_SC_3_AND_PARAGRAPH                                  0x20
#define GERMAN_KEYBOARD_SC_4_AND_DOLLAR                                     0x21
#define GERMAN_KEYBOARD_SC_5_AND_PERCENTAGE                                 0x22
#define GERMAN_KEYBOARD_SC_6_AND_AMPERSAND                                  0x23
#define GERMAN_KEYBOARD_SC_7_AND_SLASH_AND_OPENING_BRACE                    0x24
#define GERMAN_KEYBOARD_SC_8_AND_OPENING_PARENTHESIS_AND_OPENING_BRACKET    0x25
#define GERMAN_KEYBOARD_SC_9_AND_CLOSING_PARENTHESIS_AND_CLOSING_BRACKET    0x26
#define GERMAN_KEYBOARD_SC_0_AND_EQUAL_AND_CLOSING_BRACE                    0x27
#define GERMAN_KEYBOARD_SC_SHARP_S_AND_QUESTION_AND_BACKSLASH               0x2D
#define GERMAN_KEYBOARD_SC_CARET_AND_DEGREE                                 0x35
#define GERMAN_KEYBOARD_SC_PLUS_AND_ASTERISK_AND_TILDE                      0x30
#define GERMAN_KEYBOARD_SC_MINUS_AND_UNDERSCORE                             0x38
#define GERMAN_KEYBOARD_SC_COMMA_AND_SEMICOLON                              0x36
#define GERMAN_KEYBOARD_SC_DOT_AND_COLON                                    0x37
#define GERMAN_KEYBOARD_SC_ENTER                                            0x28
#define GERMAN_KEYBOARD_SC_ESCAPE                                           0x29
#define GERMAN_KEYBOARD_SC_BACKSPACE                                        0x2A
#define GERMAN_KEYBOARD_SC_TAB                                              0x2B
#define GERMAN_KEYBOARD_SC_SPACE                                            0x2C
#define GERMAN_KEYBOARD_SC_HASHMARK_AND_APOSTROPHE                          0x32
#define GERMAN_KEYBOARD_SC_LESS_THAN_AND_GREATER_THAN_AND_PIPE              0x64



typedef struct {
    const char* character; // Human-readable string representation of the key
    uint8_t hidValue;      // Corresponding HID value
} HIDMapping;

// Dictionary-like structure to map lowercase strings to HID values
HIDMapping germanKeymap[] = {
    {"a", GERMAN_KEYBOARD_SC_A},
    {"b", GERMAN_KEYBOARD_SC_B},
    {"c", GERMAN_KEYBOARD_SC_C},
    {"d", GERMAN_KEYBOARD_SC_D},
    {"e", GERMAN_KEYBOARD_SC_E},
    {"f", GERMAN_KEYBOARD_SC_F},
    {"g", GERMAN_KEYBOARD_SC_G},
    {"h", GERMAN_KEYBOARD_SC_H},
    {"i", GERMAN_KEYBOARD_SC_I},
    {"j", GERMAN_KEYBOARD_SC_J},
    {"k", GERMAN_KEYBOARD_SC_K},
    {"l", GERMAN_KEYBOARD_SC_L},
    {"m", GERMAN_KEYBOARD_SC_M},
    {"n", GERMAN_KEYBOARD_SC_N},
    {"o", GERMAN_KEYBOARD_SC_O},
    {"p", GERMAN_KEYBOARD_SC_P},
    {"q", GERMAN_KEYBOARD_SC_Q},
    {"r", GERMAN_KEYBOARD_SC_R},
    {"s", GERMAN_KEYBOARD_SC_S},
    {"t", GERMAN_KEYBOARD_SC_T},
    {"u", GERMAN_KEYBOARD_SC_U},
    {"v", GERMAN_KEYBOARD_SC_V},
    {"w", GERMAN_KEYBOARD_SC_W},
    {"x", GERMAN_KEYBOARD_SC_X},
    {"y", GERMAN_KEYBOARD_SC_Y},
    {"z", GERMAN_KEYBOARD_SC_Z}, 
    {"A", GERMAN_KEYBOARD_SC_A},
    {"B", GERMAN_KEYBOARD_SC_B},
    {"C", GERMAN_KEYBOARD_SC_C},
    {"D", GERMAN_KEYBOARD_SC_D},
    {"E", GERMAN_KEYBOARD_SC_E},
    {"F", GERMAN_KEYBOARD_SC_F},
    {"G", GERMAN_KEYBOARD_SC_G},
    {"H", GERMAN_KEYBOARD_SC_H},
    {"I", GERMAN_KEYBOARD_SC_I},
    {"J", GERMAN_KEYBOARD_SC_J},
    {"K", GERMAN_KEYBOARD_SC_K},
    {"L", GERMAN_KEYBOARD_SC_L},
    {"M", GERMAN_KEYBOARD_SC_M},
    {"N", GERMAN_KEYBOARD_SC_N},
    {"O", GERMAN_KEYBOARD_SC_O},
    {"P", GERMAN_KEYBOARD_SC_P},
    {"Q", GERMAN_KEYBOARD_SC_Q},
    {"R", GERMAN_KEYBOARD_SC_R},
    {"S", GERMAN_KEYBOARD_SC_S},
    {"T", GERMAN_KEYBOARD_SC_T},
    {"U", GERMAN_KEYBOARD_SC_U},
    {"V", GERMAN_KEYBOARD_SC_V},
    {"W", GERMAN_KEYBOARD_SC_W},
    {"X", GERMAN_KEYBOARD_SC_X},
    {"Y", GERMAN_KEYBOARD_SC_Y},
    {"Z", GERMAN_KEYBOARD_SC_Z},
    {"1", GERMAN_KEYBOARD_SC_1_AND_EXCLAMATION},
    {"2", GERMAN_KEYBOARD_SC_2_AND_QUOTES},
    {"3", GERMAN_KEYBOARD_SC_3_AND_PARAGRAPH},
    {"4", GERMAN_KEYBOARD_SC_4_AND_DOLLAR},
    {"5", GERMAN_KEYBOARD_SC_5_AND_PERCENTAGE},
    {"6", GERMAN_KEYBOARD_SC_6_AND_AMPERSAND},
    {"7", GERMAN_KEYBOARD_SC_7_AND_SLASH_AND_OPENING_BRACE},
    {"8", GERMAN_KEYBOARD_SC_8_AND_OPENING_PARENTHESIS_AND_OPENING_BRACKET},
    {"9", GERMAN_KEYBOARD_SC_9_AND_CLOSING_PARENTHESIS_AND_CLOSING_BRACKET},
    {"0", GERMAN_KEYBOARD_SC_0_AND_EQUAL_AND_CLOSING_BRACE},
    {"ß", GERMAN_KEYBOARD_SC_SHARP_S_AND_QUESTION_AND_BACKSLASH},
    {"^", GERMAN_KEYBOARD_SC_CARET_AND_DEGREE},
    {"+", GERMAN_KEYBOARD_SC_PLUS_AND_ASTERISK_AND_TILDE},
    {"-", GERMAN_KEYBOARD_SC_MINUS_AND_UNDERSCORE},
    {",", GERMAN_KEYBOARD_SC_COMMA_AND_SEMICOLON},
    {".", GERMAN_KEYBOARD_SC_DOT_AND_COLON},
    {"(", GERMAN_KEYBOARD_SC_8_AND_OPENING_PARENTHESIS_AND_OPENING_BRACKET}, // '(' on German layout
    {")", GERMAN_KEYBOARD_SC_9_AND_CLOSING_PARENTHESIS_AND_CLOSING_BRACKET}, // ')' on German layout
    {"!", GERMAN_KEYBOARD_SC_1_AND_EXCLAMATION},
    {"=", GERMAN_KEYBOARD_SC_0_AND_EQUAL_AND_CLOSING_BRACE}, // '=' on German layout
    {"~", GERMAN_KEYBOARD_SC_PLUS_AND_ASTERISK_AND_TILDE},
    {";", GERMAN_KEYBOARD_SC_COMMA_AND_SEMICOLON}, // ';' on German layout
    {":", GERMAN_KEYBOARD_SC_DOT_AND_COLON},       // ':' on German layout
    {"_", GERMAN_KEYBOARD_SC_MINUS_AND_UNDERSCORE},
    {"<", GERMAN_KEYBOARD_SC_LESS_THAN_AND_GREATER_THAN_AND_PIPE},
    {">", GERMAN_KEYBOARD_SC_LESS_THAN_AND_GREATER_THAN_AND_PIPE}, // Requires Shift
    {"[", GERMAN_KEYBOARD_SC_8_AND_OPENING_PARENTHESIS_AND_OPENING_BRACKET}, // '[' on German layout
    {"]", GERMAN_KEYBOARD_SC_9_AND_CLOSING_PARENTHESIS_AND_CLOSING_BRACKET}, // ']' on German layout
    {"/", GERMAN_KEYBOARD_SC_7_AND_SLASH_AND_OPENING_BRACE}, // '/' on German layout
    {"?", GERMAN_KEYBOARD_SC_SHARP_S_AND_QUESTION_AND_BACKSLASH}, // '?' on German layout
    {"&", GERMAN_KEYBOARD_SC_6_AND_AMPERSAND}, // '&' on German layout
    {"$", GERMAN_KEYBOARD_SC_4_AND_DOLLAR}, // '$' on German layout
    {"enter", GERMAN_KEYBOARD_SC_ENTER},
    {"esc", GERMAN_KEYBOARD_SC_ESCAPE},
    {"backspace", GERMAN_KEYBOARD_SC_BACKSPACE},
    {"tab", GERMAN_KEYBOARD_SC_TAB},
    {"caps", GERMAN_KEYBOARD_SC_CAPS_LOCK},
    {" ", GERMAN_KEYBOARD_SC_SPACE}, // Space key
    {"#", GERMAN_KEYBOARD_SC_HASHMARK_AND_APOSTROPHE},
};



#endif //GERMAN_KEYBOARDCODES_H
