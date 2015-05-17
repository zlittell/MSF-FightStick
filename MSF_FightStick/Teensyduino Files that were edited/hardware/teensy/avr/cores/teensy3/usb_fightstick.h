#ifndef USBFightStick_h_
#define USBFightStick_h_

#include "usb_desc.h"

#if defined(FIGHTSTICK_INTERFACE)

#include <inttypes.h>

// C language implementation
#ifdef __cplusplus
extern "C" {
#endif
int usb_fightstick_recv(void *buffer, uint32_t timeout);
int usb_fightstick_available(void);
int usb_fightstick_send(const void *buffer, uint32_t timeout);
#ifdef __cplusplus
}
#endif


// C++ interface
#ifdef __cplusplus
class usb_fightstick_class
{
public:
	int available(void) {return usb_fightstick_available(); }
	int recv(void *buffer, uint16_t timeout) { return usb_fightstick_recv(buffer, timeout); }
	int send(const void *buffer, uint16_t timeout) { return usb_fightstick_send(buffer, timeout); }
};

extern usb_fightstick_class FightStick;

#endif // __cplusplus

#endif // FIGHTSTICK_INTERFACE

#endif // USBFightStick_h_
