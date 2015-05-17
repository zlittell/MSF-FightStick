/* Teensyduino Core Library
 * http://www.pjrc.com/teensy/
 * Copyright (c) 2013 PJRC.COM, LLC.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * 1. The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * 2. If the Software is incorporated into a build system that allows
 * selection among a list of target devices, then similar target
 * devices manufactured by PJRC.COM must be included in the list of
 * target devices and selectable in the same manner.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#if F_CPU >= 20000000

#define USB_DESC_LIST_DEFINE
#include "usb_desc.h"
#ifdef NUM_ENDPOINTS
#include "usb_names.h"
#include "kinetis.h"
#include "avr_functions.h"

// USB Descriptors are binary data which the USB host reads to
// automatically detect a USB device's capabilities.  The format
// and meaning of every field is documented in numerous USB
// standards.  When working with USB descriptors, despite the
// complexity of the standards and poor writing quality in many
// of those documents, remember descriptors are nothing more
// than constant binary data that tells the USB host what the
// device can do.  Computers will load drivers based on this data.
// Those drivers then communicate on the endpoints specified by
// the descriptors.

// To configure a new combination of interfaces or make minor
// changes to existing configuration (eg, change the name or ID
// numbers), usually you would edit "usb_desc.h".  This file
// is meant to be configured by the header, so generally it is
// only edited to add completely new USB interfaces or features.



// **************************************************************
//   USB Device
// **************************************************************

#define LSB(n) ((n) & 255)
#define MSB(n) (((n) >> 8) & 255)

// USB Device Descriptor.  The USB host reads this first, to learn
// what type of device is connected.
static uint8_t device_descriptor[] = {
        18,                                     // bLength
        1,                                      // bDescriptorType
        0x00, 0x02,                             // bcdUSB
#ifdef DEVICE_CLASS
        DEVICE_CLASS,                           // bDeviceClass
#else
	0,
#endif
#ifdef DEVICE_SUBCLASS
        DEVICE_SUBCLASS,                        // bDeviceSubClass
#else
	0,
#endif
#ifdef DEVICE_PROTOCOL
        DEVICE_PROTOCOL,                        // bDeviceProtocol
#else
	0,
#endif
        EP0_SIZE,                               // bMaxPacketSize0
        LSB(VENDOR_ID), MSB(VENDOR_ID),         // idVendor
        LSB(PRODUCT_ID), MSB(PRODUCT_ID),       // idProduct
		  #define DEVICE_VERSION 0x0114
#ifdef DEVICE_VERSION							// bcdDevice
		LSB(DEVICE_VERSION), MSB(DEVICE_VERSION),
#else
        0x00, 0x01,                             
#endif
        1,                                      // iManufacturer
        2,                                      // iProduct
        3,                                      // iSerialNumber
        1                                       // bNumConfigurations
};

// **************************************************************
//   USB Configuration
// **************************************************************

// USB Configuration Descriptor.  This huge descriptor tells all
// of the devices capbilities.
static uint8_t config_descriptor[CONFIG_DESC_SIZE] = {
        // configuration descriptor, USB spec 9.6.3, page 264-266, Table 9-10
        9,                                      // bLength;
        2,                                      // bDescriptorType;
        LSB(CONFIG_DESC_SIZE),                 // wTotalLength
        MSB(CONFIG_DESC_SIZE),
        NUM_INTERFACE,                          // bNumInterfaces
        1,                                      // bConfigurationValue
        0,                                      // iConfiguration
#ifdef DEVICE_ATTRIBUTES
		DEVICE_ATTRIBUTES,						// bmAttributes
#else
        0xC0,                                   
#endif
#ifdef DEVICE_POWER
		DEVICE_POWER,							// bMaxPower
#else
        50,                                     
#endif

#ifdef FIGHTSTICK_INTERFACE
//Interface 0
	9,				//bLength (length of interface descriptor 9 bytes)
	4,				//bDescriptorType (4 is interface)
	0,				//bInterfaceNumber (This is interface 0)
	0,				//bAlternateSetting (used to select alternate setting.  notused)
	2,				//bNumEndpoints (this interface has 2 endpoints)
	0xFF,			//bInterfaceClass (Vendor Defined is 255)
	0x5D,			//bInterfaceSubClass
	0x01,			//bInterfaceProtocol
	0,				//iInterface (Index of string descriptor for describing this notused)
	//Some sort of common descriptor? I pulled this from Message Analyzer dumps of an actual controller
	17,33,0,1,1,37,129,20,0,0,0,0,19,2,8,0,0,
	//Endpoint 1 IN
	7,				//bLength (length of ep1in in descriptor 7 bytes)
	5,				//bDescriptorType (5 is endpoint)
	0x81,			//bEndpointAddress (0x81 is IN1)
	0x03,			//bmAttributes (0x03 is interrupt no synch, usage type data)
	0x20, 0x00,		//wMaxPacketSize (0x0020 is 1x32 bytes)
	4,				//bInterval (polling interval in frames 4 frames)
	//Endpoint 2 OUT
	7,				//bLength (length of ep2out in descriptor 7 bytes)
	5,				//bDescriptorType (5 is endpoint)
	0x02,			//bEndpointAddress (0x02 is OUT2)
	0x03,			//bmAttributes (0x03 is interrupt no synch, usage type data)
	0x20, 0x00,		//wMaxPacketSize (0x0020 is 1x32 bytes)
	8,				//bInterval (polling interval in frames 8 frames)
//Interface 1
	9,				//bLength (length of interface descriptor 9 bytes)
	4,				//bDescriptorType (4 is interface)
	1,				//bInterfaceNumber (This is interface 1)
	0,				//bAlternateSetting (used to select alternate setting.  notused)
	4,				//bNumEndpoints (this interface has 4 endpoints)
	0xFF,			//bInterfaceClass (Vendor Defined is 255)
	0x5D,			//bInterfaceSubClass (93)
	0x03,			//bInterfaceProtocol (3)
	0,				//iInterface (Index of string descriptor for describing this notused)
	//A different common descriptor? I pulled this from Message Analyzer dumps of an actual controller
	27,33,0,1,1,1,131,64,1,4,32,22,133,0,0,0,0,0,0,22,5,0,0,0,0,0,0,
	//Endpoint 3 IN
	7,				//bLength (length of ep3in descriptor 7 bytes)
	5,				//bDescriptorType (5 is endpoint)
	0x83,			//bEndpointAddress (0x83 is IN3)
	0x03,			//bmAttributes (0x03 is interrupt no synch, usage type data)
	0x20, 0x00,		//wMaxPacketSize (0x0020 is 1x32 bytes)
	2,				//bInterval (polling interval in frames 2 frames)
	//Endpoint 4 OUT
	7,				//bLength (length of ep4out descriptor 7 bytes)
	5,				//bDescriptorType (5 is endpoint)
	0x04,			//bEndpointAddress (0x04 is OUT4)
	0x03,			//bmAttributes (0x03 is interrupt no synch, usage type data)
	0x20, 0x00,		//wMaxPacketSize (0x0020 is 1x32 bytes)
	4,				//bInterval (polling interval in frames 4 frames)
	//Endpoint 5 IN
	7,				//bLength (length of ep5in descriptor 7 bytes)
	5,				//bDescriptorType (5 is endpoint)
	0x85,			//bEndpointAddress (0x85 is IN5)
	0x03,			//bmAttributes (0x03 is interrupt no synch, usage type data)
	0x20, 0x00,		//wMaxPacketSize (0x0020 is 1x32 bytes)
	64,				//bInterval (polling interval in frames 64 frames)
	//Endpoint 5 OUT (shares endpoint number with previous)
	7,				//bLength (length of ep5out descriptor 7 bytes)
	5,				//bDescriptorType (5 is endpoint)
	0x05,			//bEndpointAddress (0x05 is OUT5)
	0x03,			//bmAttributes (0x03 is interrupt no synch, usage type data)
	0x20, 0x00,		//wMaxPacketSize (0x0020 is 1x32 bytes)
	16,				//bInterval (polling interval in frames 16 frames)
//Interface 2
	9,				//bLength (length of interface descriptor 9 bytes)
	4,				//bDescriptorType (4 is interface)
	2,				//bInterfaceNumber (This is interface 2)
	0,				//bAlternateSetting (used to select alternate setting.  notused)
	1,				//bNumEndpoints (this interface has 4 endpoints)
	0xFF,			//bInterfaceClass (Vendor Defined is 255)
	0x5D,			//bInterfaceSubClass (93)
	0x02,			//bInterfaceProtocol (3)
	0,				//iInterface (Index of string descriptor for describing this notused)
	//Common Descriptor.  Seems that these come after every interface description?
	9,33,0,1,1,34,134,7,0,
	//Endpoint 6 IN
	7,				//bLength (length of ep6in descriptor 7 bytes)
	5,				//bDescriptorType (5 is endpoint)
	0x86,			//bEndpointAddress (0x86 is IN6)
	0x03,			//bmAttributes (0x03 is interrupt no synch, usage type data)
	0x20, 0x00,		//wMaxPacketSize (0x0020 is 1x32 bytes)
	16,				//bInterval (polling interval in frames 64 frames)+
//Interface 3
//This is the interface on which all the security handshaking takes place
//We don't use this but it could be used for man-in-the-middle stuff
	9,				//bLength (length of interface descriptor 9 bytes)
	4,				//bDescriptorType (4 is interface)
	3,				//bInterfaceNumber (This is interface 3)
	0,				//bAlternateSetting (used to select alternate setting.  notused)
	0,				//bNumEndpoints (this interface has 0 endpoints ???)
	0xFF,			//bInterfaceClass (Vendor Defined is 255)
	0xFD,			//bInterfaceSubClass (253)
	0x13,			//bInterfaceProtocol (19)
	4,				//iInterface (Computer never asks for this, but an x360 would. so include one day?)
	//Another interface another Common Descriptor
	6,65,0,1,1,3
#endif // FIGHTSTICK_INTERFACE
};


// **************************************************************
//   String Descriptors
// **************************************************************

// The descriptors above can provide human readable strings,
// referenced by index numbers.  These descriptors are the
// actual string data

/* defined in usb_names.h
struct usb_string_descriptor_struct {
        uint8_t bLength;
        uint8_t bDescriptorType;
        uint16_t wString[];
};
*/

extern struct usb_string_descriptor_struct usb_string_manufacturer_name
        __attribute__ ((weak, alias("usb_string_manufacturer_name_default")));
extern struct usb_string_descriptor_struct usb_string_product_name
        __attribute__ ((weak, alias("usb_string_product_name_default")));
extern struct usb_string_descriptor_struct usb_string_serial_number
        __attribute__ ((weak, alias("usb_string_serial_number_default")));

struct usb_string_descriptor_struct string0 = {
        4,
        3,
        {0x0409}
};

struct usb_string_descriptor_struct usb_string_manufacturer_name_default = {
        2 + MANUFACTURER_NAME_LEN * 2,
        3,
        MANUFACTURER_NAME
};
struct usb_string_descriptor_struct usb_string_product_name_default = {
	2 + PRODUCT_NAME_LEN * 2,
        3,
        PRODUCT_NAME
};
struct usb_string_descriptor_struct usb_string_serial_number_default = {
        12,
        3,
        {0,0,0,0,0,0,0,0,0,0}
};

void usb_init_serialnumber(void)
{
	char buf[11];
	uint32_t i, num;

	__disable_irq();
	FTFL_FSTAT = FTFL_FSTAT_RDCOLERR | FTFL_FSTAT_ACCERR | FTFL_FSTAT_FPVIOL;
	FTFL_FCCOB0 = 0x41;
	FTFL_FCCOB1 = 15;
	FTFL_FSTAT = FTFL_FSTAT_CCIF;
	while (!(FTFL_FSTAT & FTFL_FSTAT_CCIF)) ; // wait
	num = *(uint32_t *)&FTFL_FCCOB7;
	__enable_irq();
	// add extra zero to work around OS-X CDC-ACM driver bug
	if (num < 10000000) num = num * 10;
	ultoa(num, buf, 10);
	for (i=0; i<10; i++) {
		char c = buf[i];
		if (!c) break;
		usb_string_serial_number_default.wString[i] = c;
	}
	usb_string_serial_number_default.bLength = i * 2 + 2;
}


// **************************************************************
//   Descriptors List
// **************************************************************

// This table provides access to all the descriptor data above.

const usb_descriptor_list_t usb_descriptor_list[] = {
	//wValue, wIndex, address,          length
	{0x0100, 0x0000, device_descriptor, sizeof(device_descriptor)},
	{0x0200, 0x0000, config_descriptor, sizeof(config_descriptor)},
    {0x0300, 0x0000, (const uint8_t *)&string0, 0},
    {0x0301, 0x0409, (const uint8_t *)&usb_string_manufacturer_name, 0},
    {0x0302, 0x0409, (const uint8_t *)&usb_string_product_name, 0},
    {0x0303, 0x0409, (const uint8_t *)&usb_string_serial_number, 0},
	{0, 0, NULL, 0}
};


// **************************************************************
//   Endpoint Configuration
// **************************************************************

#if 0
// 0x00 = not used
// 0x19 = Recieve only
// 0x15 = Transmit only
// 0x1D = Transmit & Recieve
//
const uint8_t usb_endpoint_config_table[NUM_ENDPOINTS] =
{
	0x00, 0x15, 0x19, 0x15, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
#endif


const uint8_t usb_endpoint_config_table[NUM_ENDPOINTS] =
{
#if (defined(ENDPOINT1_CONFIG) && NUM_ENDPOINTS >= 1)
	ENDPOINT1_CONFIG,
#elif (NUM_ENDPOINTS >= 1)
	ENDPOINT_UNUSED,
#endif
#if (defined(ENDPOINT2_CONFIG) && NUM_ENDPOINTS >= 2)
	ENDPOINT2_CONFIG,
#elif (NUM_ENDPOINTS >= 2)
	ENDPOINT_UNUSED,
#endif
#if (defined(ENDPOINT3_CONFIG) && NUM_ENDPOINTS >= 3)
	ENDPOINT3_CONFIG,
#elif (NUM_ENDPOINTS >= 3)
	ENDPOINT_UNUSED,
#endif
#if (defined(ENDPOINT4_CONFIG) && NUM_ENDPOINTS >= 4)
	ENDPOINT4_CONFIG,
#elif (NUM_ENDPOINTS >= 4)
	ENDPOINT_UNUSED,
#endif
#if (defined(ENDPOINT5_CONFIG) && NUM_ENDPOINTS >= 5)
	ENDPOINT5_CONFIG,
#elif (NUM_ENDPOINTS >= 5)
	ENDPOINT_UNUSED,
#endif
#if (defined(ENDPOINT6_CONFIG) && NUM_ENDPOINTS >= 6)
	ENDPOINT6_CONFIG,
#elif (NUM_ENDPOINTS >= 6)
	ENDPOINT_UNUSED,
#endif
#if (defined(ENDPOINT7_CONFIG) && NUM_ENDPOINTS >= 7)
	ENDPOINT7_CONFIG,
#elif (NUM_ENDPOINTS >= 7)
	ENDPOINT_UNUSED,
#endif
#if (defined(ENDPOINT8_CONFIG) && NUM_ENDPOINTS >= 8)
	ENDPOINT8_CONFIG,
#elif (NUM_ENDPOINTS >= 8)
	ENDPOINT_UNUSED,
#endif
#if (defined(ENDPOINT9_CONFIG) && NUM_ENDPOINTS >= 9)
	ENDPOINT9_CONFIG,
#elif (NUM_ENDPOINTS >= 9)
	ENDPOINT_UNUSED,
#endif
#if (defined(ENDPOINT10_CONFIG) && NUM_ENDPOINTS >= 10)
	ENDPOINT10_CONFIG,
#elif (NUM_ENDPOINTS >= 10)
	ENDPOINT_UNUSED,
#endif
#if (defined(ENDPOINT11_CONFIG) && NUM_ENDPOINTS >= 11)
	ENDPOINT11_CONFIG,
#elif (NUM_ENDPOINTS >= 11)
	ENDPOINT_UNUSED,
#endif
#if (defined(ENDPOINT12_CONFIG) && NUM_ENDPOINTS >= 12)
	ENDPOINT12_CONFIG,
#elif (NUM_ENDPOINTS >= 12)
	ENDPOINT_UNUSED,
#endif
#if (defined(ENDPOINT13_CONFIG) && NUM_ENDPOINTS >= 13)
	ENDPOINT13_CONFIG,
#elif (NUM_ENDPOINTS >= 13)
	ENDPOINT_UNUSED,
#endif
#if (defined(ENDPOINT14_CONFIG) && NUM_ENDPOINTS >= 14)
	ENDPOINT14_CONFIG,
#elif (NUM_ENDPOINTS >= 14)
	ENDPOINT_UNUSED,
#endif
#if (defined(ENDPOINT15_CONFIG) && NUM_ENDPOINTS >= 15)
	ENDPOINT15_CONFIG,
#elif (NUM_ENDPOINTS >= 15)
	ENDPOINT_UNUSED,
#endif
};


#endif // NUM_ENDPOINTS
#endif // F_CPU >= 20 MHz
