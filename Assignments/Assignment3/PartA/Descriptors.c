/*
   LUFA Library
   Copyright (C) Dean Camera, 2014.

   dean [at] fourwalledcubicle [dot] com
   www.lufa-lib.org
*/

/*
  Copyright 2014  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Permission to use, copy, modify, distribute, and sell this
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaims all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

/** \file
  * This file implements USB Device Descriptors for LUFA library. Descriptors are 
  * essential for USB communication, as they define the device's capabilities 
  * and are parsed by the host during enumeration.
  */
#include "Descriptors.h"

/** HID class report descriptor. 
  * Describes the HID device capabilities and report format to the host. 
  * This descriptor is parsed by the host to understand the format and 
  * length of the data the HID device sends/receives. 
  *
  * HID_DESCRIPTOR_KEYBOARD(6) specifies a keyboard with support for up to 6 
  * simultaneous keypresses.
  */
const USB_Descriptor_HIDReport_Datatype_t KeyboardReport[] = {
	HID_DESCRIPTOR_KEYBOARD(6)  // Standard descriptor for a 6-key rollover keyboard.
};

/** Device descriptor structure.
  * Provides basic information about the USB device, such as USB version, vendor ID, 
  * product ID, and other general device characteristics. This is the first descriptor 
  * requested by the host during enumeration.
  */
const USB_Descriptor_Device_t DeviceDescriptor = {
	.Header                 = { .Size = sizeof(USB_Descriptor_Device_t),
								.Type = DTYPE_Device }, // Descriptor type: Device.

	.USBSpecification       = VERSION_BCD(1,1,0), // USB version: 1.1.
	.Class                  = USB_CSCP_NoDeviceClass, // No specific device class.
	.SubClass               = USB_CSCP_NoDeviceSubclass,
	.Protocol               = USB_CSCP_NoDeviceProtocol,

	.Endpoint0Size          = KEYBOARD_EPSIZE, // Maximum packet size for control endpoint (Endpoint 0).

	.VendorID               = 0x03EB, // Example vendor ID (Atmel/Microchip).
	.ProductID              = 0x2042, // Example product ID.
	.ReleaseNumber          = VERSION_BCD(0,0,1), // Device release number.

	.ManufacturerStrIndex   = STRING_ID_Manufacturer, // Index to manufacturer string.
	.ProductStrIndex        = STRING_ID_Product,      // Index to product string.
	.SerialNumStrIndex      = NO_DESCRIPTOR,         // No serial number string.

	.NumberOfConfigurations = 1 // Only one configuration supported.
};

/** Configuration descriptor structure.
  * Describes the overall configuration of the device, including interfaces, endpoints, 
  * and HID-specific information. This is the primary descriptor for host-to-device 
  * communication setup.
  */
const USB_Descriptor_Configuration_t ConfigurationDescriptor = {
	.Config = {
		.Header = {
			.Size = sizeof( USB_Descriptor_Configuration_Header_t ),
			.Type = DTYPE_Configuration // Descriptor type: Configuration.
		},

		.TotalConfigurationSize = sizeof(USB_Descriptor_Configuration_t), // Total size of this configuration descriptor.
		.TotalInterfaces        = 1, // Number of interfaces in this configuration.

		.ConfigurationNumber    = 1, // ID of this configuration.
		.ConfigurationStrIndex  = NO_DESCRIPTOR, // No string descriptor for this configuration.

		.ConfigAttributes       = ( USB_CONFIG_ATTR_RESERVED | // Reserved bit (must be set).
									USB_CONFIG_ATTR_SELFPOWERED ), // Device is self-powered.

		.MaxPowerConsumption    = USB_CONFIG_POWER_MA(100) // Maximum power consumption: 100mA.
	},

	.HID_Interface = {
		.Header                 = { .Size = sizeof(USB_Descriptor_Interface_t),
									.Type = DTYPE_Interface }, // Descriptor type: Interface.

		.InterfaceNumber        = INTERFACE_ID_KeyboardHID, // Unique ID for the HID interface.
		.AlternateSetting       = 0x00, // No alternate settings.

		.TotalEndpoints         = 1, // One endpoint for this interface.

		.Class                  = HID_CSCP_HIDClass, // HID class.
		.SubClass               = 1, // Boot interface subclass.
		.Protocol               = HID_CSCP_KeyboardBootProtocol, // Protocol for boot keyboards.

		.InterfaceStrIndex      = NO_DESCRIPTOR // No string descriptor for this interface.
	},

	.HID_KeyboardHID = {
		.Header                 = { .Size = sizeof(USB_HID_Descriptor_HID_t),
									.Type = HID_DTYPE_HID }, // Descriptor type: HID.

		.HIDSpec                = VERSION_BCD(1,1,1), // HID specification version.
		.CountryCode            = 0x00, // Not localized.
		.TotalReportDescriptors = 1, // One report descriptor.
		.HIDReportType          = HID_DTYPE_Report, // Report descriptor type.
		.HIDReportLength        = sizeof(KeyboardReport) // Size of the report descriptor.
	},

	.HID_ReportINEndpoint = {
		.Header                 = { .Size = sizeof(USB_Descriptor_Endpoint_t),
									.Type = DTYPE_Endpoint }, // Descriptor type: Endpoint.

		.EndpointAddress        = KEYBOARD_IN_EPADDR, // IN endpoint address.
		.Attributes             = ( EP_TYPE_INTERRUPT | // Interrupt endpoint type.
									ENDPOINT_ATTR_NO_SYNC |
									ENDPOINT_USAGE_DATA ),
		.EndpointSize           = KEYBOARD_REPORT_SIZE, // Maximum packet size.
		.PollingIntervalMS      = 0x01 // Polling interval: 1ms.
	},
};

/** Language descriptor structure.
  * Describes the supported languages for string descriptors. The first descriptor the 
  * host requests to identify language support.
  */
const USB_Descriptor_String_t LanguageString =
			USB_STRING_DESCRIPTOR_ARRAY(LANGUAGE_ID_ENG);

/** Manufacturer string descriptor.
  * A human-readable Unicode string identifying the manufacturer.
  */
const USB_Descriptor_String_t ManufacturerString =
			USB_STRING_DESCRIPTOR(L"Manufacturer String");

/** Product string descriptor.
  * A human-readable Unicode string identifying the product.
  */
const USB_Descriptor_String_t ProductString =
			USB_STRING_DESCRIPTOR(L"Descriptor String");

/** This function handles USB descriptor requests from the host.
  * Called when the host requests a specific descriptor during enumeration.
  *
  * Parameters:
  * - `wValue`: Combines the descriptor type (high byte) and descriptor index (low byte).
  * - `wIndex`: Additional information such as language ID or interface index.
  * - `DescriptorAddress`: Pointer to the address of the requested descriptor.
  *
  * Returns the size of the requested descriptor.
  */
uint16_t CALLBACK_USB_GetDescriptor( const uint16_t wValue,
									 const uint8_t wIndex,
									 const void** const DescriptorAddress ) {
	// Extract descriptor type and index from wValue.
	const uint8_t  DescriptorType   = (wValue >> 8);
	const uint8_t  DescriptorNumber = (wValue & 0xFF);

	const void* Address = NULL;
	uint16_t    Size    = NO_DESCRIPTOR;

	// Determine which descriptor was requested.
	switch (DescriptorType) {
		case DTYPE_Device: // Device descriptor requested.
			Address = &DeviceDescriptor;
			Size    = sizeof(USB_Descriptor_Device_t);
			break;
		case DTYPE_Configuration: // Configuration descriptor requested.
			Address = &ConfigurationDescriptor;
			Size    = sizeof(USB_Descriptor_Configuration_t);
			break;
		case DTYPE_String: // String descriptor requested.
			switch (DescriptorNumber) {
				case STRING_ID_Language: // Language descriptor.
					Address = &LanguageString;
					Size    = LanguageString.Header.Size;
					break;
				case STRING_ID_Manufacturer: // Manufacturer string descriptor.
					Address = &ManufacturerString;
					Size    = ManufacturerString.Header.Size;
					break;
				case STRING_ID_Product: // Product string descriptor.
					Address = &ProductString;
					Size    = ProductString.Header.Size;
					break;
			}
			break;
		case HID_DTYPE_HID: // HID descriptor requested.
			Address = &ConfigurationDescriptor.HID_KeyboardHID;
			Size    = sizeof(USB_HID_Descriptor_HID_t);
			break;
		case HID_DTYPE_Report: // HID report descriptor requested.
			Address = &KeyboardReport;
			Size    = sizeof(KeyboardReport);
			break;
	}

	*DescriptorAddress = Address; // Provide the descriptor address to the USB library.
	return Size; // Return the descriptor size.
}
