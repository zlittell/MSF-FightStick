#include "usb_dev.h"
#include "usb_fightstick.h"
#include "core_pins.h" // for yield()
#include "HardwareSerial.h"
#include <string.h> // for memcpy()

#ifdef USB_FIGHTSTICK // defined by usb_dev.h -> usb_desc.h

// Need 7 bytes to cover all data needed for the fight stick
// First byte is for the hat 4 padding bits and then 4 hat bits xxxxHHHH
// Second byte is buttons 1-8
// Third byte is buttons 9-13 and then 3 padding bits BBBBBxxx
// Rest were added for X, Y, Z, Rx, Ry
// This was done because Mortal Kombat X sucks
uint8_t usb_fightstick_data[8];

// Maximum number of transmit packets to queue so we don't starve other endpoints for memory
#define TX_PACKET_LIMIT 3

static uint8_t transmit_previous_timeout=0;

// When the PC isn't listening, how long do we wait before discarding data?
#define TX_TIMEOUT_MSEC 30

#if F_CPU == 48000000
  #define TX_TIMEOUT (TX_TIMEOUT_MSEC * 428)
#endif

int usb_fightstick_send(void)
{
    uint32_t wait_count=0;
    usb_packet_t *tx_packet;

    while (1) {
        if (!usb_configuration) {
            return -1;
        }
        if (usb_tx_packet_count(FIGHTSTICK_ENDPOINT) < TX_PACKET_LIMIT) {           tx_packet = usb_malloc();           if (tx_packet) break;       }       if (++wait_count > TX_TIMEOUT || transmit_previous_timeout) {
                                                                        transmit_previous_timeout = 1;
                                                                        return -1;
        }
        yield();
    }

    transmit_previous_timeout = 0;
    memcpy(tx_packet->buf, usb_fightstick_data, 8);
    tx_packet->len = 8;
    usb_tx(FIGHTSTICK_ENDPOINT, tx_packet);

    return 0;
}

#endif