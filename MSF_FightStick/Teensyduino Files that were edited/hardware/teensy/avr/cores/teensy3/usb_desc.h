#ifndef _usb_desc_h_
#define _usb_desc_h_

#include <stdint.h>
#include <stddef.h>

#define ENDPOINT_UNUSED			0x00
#define ENDPOINT_TRANSIMIT_ONLY		0x15
#define ENDPOINT_RECEIVE_ONLY		0x19
#define ENDPOINT_TRANSMIT_AND_RECEIVE	0x1D
 
#if defined(USB_FIGHTSTICK)
  #define DEVICE_CLASS	0xFF
  #define DEVICE_SUBCLASS	0xFF
  #define DEVICE_PROTOCOL	0xFF
  #define DEVICE_VERSION 0x0114
  #define DEVICE_ATTRIBUTES 0xA0
  #define DEVICE_POWER	0xFA
  #define VENDOR_ID		0x045e
  #define PRODUCT_ID		0x028e
  #define MANUFACTURER_NAME	{'©','M','i','c','r','o','s','o','f','t'}
  #define MANUFACTURER_NAME_LEN	10
  #define PRODUCT_NAME		{'C','o','n','t','r','o','l','l','e','r'}
  #define PRODUCT_NAME_LEN	10
  #define EP0_SIZE	8
  #define NUM_ENDPOINTS	6
  #define NUM_USB_BUFFERS	24
  #define NUM_INTERFACE	4
  #define FIGHTSTICK_INTERFACE	0
  #define FIGHTSTICK_RX_ENDPOINT	2
  #define FIGHTSTICK_RX_SIZE 3
  #define FIGHTSTICK_TX_ENDPOINT	1
  #define FIGHTSTICK_TX_SIZE 20
  #define CONFIG_DESC_SIZE 153
  #define ENDPOINT1_CONFIG ENDPOINT_TRANSIMIT_ONLY
  #define ENDPOINT2_CONFIG ENDPOINT_RECEIVE_ONLY
  #define ENDPOINT3_CONFIG ENDPOINT_TRANSIMIT_ONLY
  #define ENDPOINT4_CONFIG ENDPOINT_RECEIVE_ONLY
  #define ENDPOINT5_CONFIG ENDPOINT_TRANSMIT_AND_RECEIVE
  #define ENDPOINT6_CONFIG ENDPOINT_TRANSIMIT_ONLY
#endif

#ifdef USB_DESC_LIST_DEFINE
#if defined(NUM_ENDPOINTS) && NUM_ENDPOINTS > 0
// NUM_ENDPOINTS = number of non-zero endpoints (0 to 15)
extern const uint8_t usb_endpoint_config_table[NUM_ENDPOINTS];

typedef struct {
	uint16_t	wValue;
	uint16_t	wIndex;
	const uint8_t	*addr;
	uint16_t	length;
} usb_descriptor_list_t;

extern const usb_descriptor_list_t usb_descriptor_list[];
#endif // NUM_ENDPOINTS
#endif // USB_DESC_LIST_DEFINE

#endif
