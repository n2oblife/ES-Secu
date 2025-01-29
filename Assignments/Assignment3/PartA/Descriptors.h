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
 *
 *  Header file for Descriptors.c.
 */

#ifndef _DESCRIPTORS_H_
#define _DESCRIPTORS_H_

/* Includes: */
#include <USB.h>  // Includes LUFA's USB-related definitions and utilities.

/* Type Defines: */
/** Type define for the device configuration descriptor structure. This must be
  *  defined in the application code, as the configuration descriptor contains
  *  several sub-descriptors which vary between devices, and which describe the
  *  device's usage to the host.
  *
  *  The structure aggregates:
  *  1. A general USB configuration header.
  *  2. Details about the HID interface.
  *  3. A descriptor for the HID device itself.
  *  4. An endpoint for sending HID reports (e.g., keypress data).
  **/
typedef struct {
	USB_Descriptor_Configuration_Header_t Config;  // Basic USB configuration descriptor.

	// Keyboard HID Interface
	USB_Descriptor_Interface_t HID_Interface;      // Descriptor for the HID interface.
	USB_HID_Descriptor_HID_t   HID_KeyboardHID;    // Descriptor for HID-specific data.
	USB_Descriptor_Endpoint_t  HID_ReportINEndpoint; // Descriptor for the HID IN endpoint, used to send data to the host.
} USB_Descriptor_Configuration_t;

/** Enum for the device interface descriptor IDs within the device. Each
  *  interface descriptor should have a unique ID index associated with it,
  *  which can be used to refer to the interface from other descriptors.
  **/
enum InterfaceDescriptors_t {
	INTERFACE_ID_KeyboardHID = 0x00,  // Assigns a unique ID to the HID interface.
};

/** Enum for the device string descriptor IDs within the device.
  *  Each string descriptor should have a unique ID index associated with it,
  *  which can be used to refer to the string from other descriptors.
  *
  *  These descriptors are requested by the host to display device information
  *  such as manufacturer or product name.
  **/
enum StringDescriptors_t {
	STRING_ID_Language     = 0, /**< Supported Languages string descriptor ID (must be zero). */
	STRING_ID_Manufacturer = 1, /**< Manufacturer string ID. */
	STRING_ID_Product      = 2, /**< Product string ID. */
};

/* Macros: */
// keyboard sends 8-byte reports [Modifier][Reserved][KeyCode*6]
// Example: Modifier keys (Ctrl, Alt) are encoded in the first byte, while the next
// six bytes contain up to six key codes for the keys currently pressed.
#define KEYBOARD_REPORT_SIZE 8

/** Endpoint address of the Keyboard HID reporting IN endpoint. 
  *  - `ENDPOINT_DIR_IN` indicates data flows from the device to the host.
  *  - `1` is the endpoint number (Endpoint 1).
  **/
#define KEYBOARD_IN_EPADDR   (ENDPOINT_DIR_IN | 1)

/** Size in bytes of the Keyboard HID reporting endpoint.
  *  - Specifies the maximum packet size for the endpoint.
  *  - This must match the host's expectations as defined in the descriptor.
  **/
#define KEYBOARD_EPSIZE      64

/* Function Prototypes: */
/** Callback function for handling USB descriptor requests from the host.
  *
  *  This function is invoked whenever the host requests a descriptor (e.g., Device, 
  *  Configuration, or HID Report descriptors).
  *
  *  - `wValue`: Encodes the type and index of the requested descriptor.
  *  - `wIndex`: Additional information (e.g., language ID or interface index).
  *  - `DescriptorAddress`: Pointer to the requested descriptor's address.
  *
  *  The function returns the size of the requested descriptor.
  *
  *  Attributes:
  *  - `ATTR_WARN_UNUSED_RESULT`: Ensures the return value is checked.
  *  - `ATTR_NON_NULL_PTR_ARG(3)`: Ensures `DescriptorAddress` is not NULL.
  **/
uint16_t CALLBACK_USB_GetDescriptor( const uint16_t wValue,
									 const uint8_t wIndex,
									 const void** const DescriptorAddress )
	ATTR_WARN_UNUSED_RESULT ATTR_NON_NULL_PTR_ARG(3);

#endif
