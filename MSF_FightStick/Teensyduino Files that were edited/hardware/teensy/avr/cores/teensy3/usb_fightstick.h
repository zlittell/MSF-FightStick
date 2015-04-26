#ifndef _USB_FIGHTSTICK_H_
#define _USB_FIGHTSTICK_H_

#if defined(USB_FIGHTSTICK)

#include <inttypes.h>

// C language implementation
#ifdef __cplusplus
extern "C" {
#endif
int usb_fightstick_send(void);
// Need 7 bytes to cover all data needed for the fight stick
// First byte is for the hat 4 padding bits and then 4 hat bits xxxxHHHH
// Second byte is buttons 1-8
// Third byte is buttons 9-13 and then 3 padding bits BBBBBxxx
// Rest were added for X, Y, Z, Rx, Ry
// This was done because Mortal Kombat X sucks
extern uint8_t usb_fightstick_data[8];
#ifdef __cplusplus
}
#endif

// C++ interface
#ifdef __cplusplus
class usb_fightstick_class
{
public:
    void send(uint8_t usbData[8]) {
		for (int i=0; i<8; i++)
		{
			usb_fightstick_data[i] = usbData[i];
		}
        usb_fightstick_send();
    }
};
extern usb_fightstick_class FightStick;

#endif
#endif
#endif