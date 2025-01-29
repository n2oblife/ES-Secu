#include <string.h>  // Standard library for memory operations (e.g., memset, memcpy).

#include "KeyboardHID.h" // Header file defining the HID interface and USB initialization functions.
#include "system_XMC4500.h" // XMC4500 system-level configurations for USB and peripherals.

// Buffer to hold the previous HID report, used for comparison to avoid sending duplicate reports.
static uint8_t PrevHIDReportBuffer[KEYBOARD_REPORT_SIZE];

// Global configuration structure for the HID device. This contains runtime data and configuration details.
USB_ClassInfo_HID_Device_t Keyboard_HID_Interface = {
	.Config = {
		// Interface number for the HID device.
		.InterfaceNumber              = INTERFACE_ID_KeyboardHID,
		
		// IN endpoint configuration for sending HID reports.
		.ReportINEndpoint             = {
			.Address              = KEYBOARD_IN_EPADDR,  // Endpoint address (defined in Descriptors.h).
			.Size                 = KEYBOARD_REPORT_SIZE, // Size of HID reports.
			.Banks                = 1, // Number of banks used for endpoint buffering (1 for simplicity).
		},

		// Buffer for storing the last sent report to minimize redundant transmissions.
		.PrevReportINBuffer           = PrevHIDReportBuffer,
		.PrevReportINBufferSize       = sizeof(PrevHIDReportBuffer),
	},
};

// Buffer used as a working space for USB endpoint operations.
uint8_t buffer[KEYBOARD_REPORT_SIZE];

/* USB runtime structure */
XMC_USBD_t USB_runtime = {
	.usbd = USB0, // USB hardware module to be used (USB0 for XMC4500).
	.usbd_max_num_eps = XMC_USBD_MAX_NUM_EPS_6, // Maximum number of endpoints supported by the device.
	.usbd_transfer_mode = XMC_USBD_USE_FIFO, // Transfer mode (FIFO for USB data handling).
	
	// Callback functions for USB device and endpoint events.
	.cb_xmc_device_event = USBD_SignalDeviceEventHandler,
	.cb_endpoint_event = USBD_SignalEndpointEvent_Handler
};

/**
 * @brief USB interrupt handler for the XMC4500.
 * This function is called when the USB module generates an interrupt. It delegates
 * the handling of the interrupt to the XMC USB driver.
 */
void USB0_0_IRQHandler(void) {
	XMC_USBD_IRQHandler(&USB_runtime); // Call the XMC USB driver to handle the interrupt.
}

/**
 * @brief Initializes the USB core layer and registers callback functions.
 * This function sets up the USB runtime structure and configures the USB interrupts
 * and endpoint buffers. It also attaches the device to the USB bus.
 */
void USB_Init(void) {
	// Initialize the USB driver with the runtime configuration.
	USBD_Initialize(&USB_runtime);

	/* Configure USB interrupts */
	NVIC_SetPriority( USB0_0_IRQn, // USB interrupt number.
					  NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 63, 0) ); // Set low priority.
	NVIC_ClearPendingIRQ(USB0_0_IRQn); // Clear any pending interrupts.
	NVIC_EnableIRQ(USB0_0_IRQn); // Enable USB interrupts.

	/* Attach the USB device to the host */
	USB_Attach();
}

/**
 * Placeholder for the USB connection event.
 * Currently, this function does nothing but can be customized to handle
 * device connection events.
 */
void EVENT_USB_Device_(void) {
}

/**
 * @brief Event handler for USB connection events.
 * Triggered when the USB device is connected to a host.
 * Indicates the device is ready.
 */
void EVENT_USB_Device_Connect(void) {
	// Device is now ready to communicate with the host.
}

/**
 * @brief Event handler for USB reset events.
 * Triggered when the USB bus is reset by the host.
 */
void EVENT_USB_Device_Reset(void) {
	if(device.IsConfigured) {
		USB_Init(); // Reinitialize the USB device.
		device.IsConfigured = 0; // Mark the device as not configured.
	}
}

/**
 * @brief Event handler for USB configuration change events.
 * This function sets up the endpoints and configures the device for communication.
 */
void EVENT_USB_Device_ConfigurationChanged(void) {
	bool ConfigSuccess = true; // Track configuration success.

	// Set the buffer for the HID IN endpoint (used to send data to the host).
	USBD_SetEndpointBuffer(KEYBOARD_IN_EPADDR, buffer, KEYBOARD_REPORT_SIZE);

	// Configure the HID-specific endpoints.
	ConfigSuccess &= HID_Device_ConfigureEndpoints(&Keyboard_HID_Interface);

	// Update the device's configuration status.
	device.IsConfigured = ConfigSuccess;
}

/**
 * @brief Event handler for USB control requests.
 * Processes control requests sent by the host to the device.
 * This includes standard requests like GET_DESCRIPTOR or HID-specific requests.
 */
void EVENT_USB_Device_ControlRequest(void) {
	// Forward control requests to the HID driver.
	HID_Device_ProcessControlRequest(&Keyboard_HID_Interface);
}

/**
 * @brief Callback function to filter HID report items.
 * This function is called by the HID parser to determine whether a specific HID
 * report item should be processed or ignored.
 * @param CurrentItem Pointer to the current HID report item being parsed.
 * @return true Always processes all items.
 */
bool CALLBACK_HIDParser_FilterHIDReportItem(
									HID_ReportItem_t* const CurrentItem ) {
	return true; // Allow all items to be processed.
}
