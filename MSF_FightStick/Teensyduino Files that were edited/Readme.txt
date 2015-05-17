These are files that were edited in the teensyduino installation:
arduino install\hardware\teensy\avr\boards.txt
arduino install\hardware\teensy\avr\cores\teensy3\usb_desc.c
arduino install\hardware\teensy\avr\cores\teensy3\usb_desc.h
arduino install\hardware\teensy\avr\cores\teensy3\usb_xinput.c
arduino install\hardware\teensy\avr\cores\teensy3\usb_xinput.h
arduino install\hardware\teensy\avr\cores\teensy3\usb_inst.cpp
arduino install\hardware\teensy\avr\cores\teensy3\usb_serial.h
arduino install\hardware\teensy\avr\cores\teensy3\WProgram.h

They mainly add support for the custom usb type FightStick

This creates the 13 button + 1 Dhat USB Joystick that we need

They also add FightStick as an option in the USB menu for the TeensyLC
Make sure to select this

you can create a backup of your files
then you should be able to copy the hardware folder in paste it into the root of your arduino installation.

just reinstall teensyduino to get the original files back if you did not create a backup.




*
Also used hotfix from xxxajk for issues with USB serial yield when not using a serial interface on the usb
https://github.com/xxxajk/cores/commit/f8938ec150118ef0aac2d0712f35f176006345bb
this may be integrated into usb_serial.h at some point
*