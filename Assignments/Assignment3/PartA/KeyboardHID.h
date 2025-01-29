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
 *  Header file for GenericHID.c.
 *  This file defines constants, function prototypes, and global variables used for the 
 *  implementation of a HID (Human Interface Device) keyboard. It serves as the interface 
 *  between the main code and the USB HID functions.
 */

#ifndef _KEYBOARDHID_H_ // Include guard to prevent multiple inclusions of this header file.
#define _KEYBOARDHID_H_

/* Includes: */
#include <string.h> // Standard library for string manipulation functions.
#include <xmc_gpio.h> // XMC GPIO library for GPIO configuration and operations.
#include <USB.h> // LUFA USB library for USB device functionality.
#include "Descriptors.h" // Header file containing USB descriptor definitions for this device.

/* Global variables: */
/**
 * Global instance of the HID device configuration and runtime information.
 * This structure contains information about the HID interface, such as endpoint
 * configurations and buffer pointers, and is used by the LUFA library to manage USB HID communication.
 */
extern USB_ClassInfo_HID_Device_t Keyboard_HID_Interface;

/* Function Prototypes: */
/**
 * @brief Initializes the USB stack and prepares the device for enumeration by the host.
 * This function is typically called during system initialization.
 */
void USB_Init(void);

/**
 * @brief Event handler for the USB device connection event.
 * This function is triggered when the USB device is connected to a host.
 */
void EVENT_USB_Device_Connect(void);

/**
 * @brief Event handler for the USB device disconnection event.
 * This function is triggered when the USB device is disconnected from the host.
 */
void EVENT_USB_Device_Disconnect(void);

/**
 * @brief Event handler for the USB device configuration change event.
 * This function is triggered when the host sets the device configuration. It is
 * used to configure endpoints and initialize the device for communication.
 */
void EVENT_USB_Device_ConfigurationChanged(void);

/**
 * @brief Event handler for USB control requests.
 * This function processes control requests sent by the host to the device. Control
 * requests can include standard USB requests (e.g., GET_DESCRIPTOR) or class-specific 
 * requests for the HID interface.
 */
void EVENT_USB_Device_ControlRequest(void);

#endif // End of include guard: _KEYBOARDHID_H_
