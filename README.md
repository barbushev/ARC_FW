# ARC_FW

Arduino Relay Control

Controls pin states based on commands received via the USB serial interface.
Serial operates at 9600 baud 8N1.

Command syntax:
S#/n, where (S)tate is either (H)igh or (L)ow, and # is pin #, /n represents a new line and is used for end of transmission.

Valid pin numbers are 2 to 19. Pins 0 and 1 are reserved for USB serial communication.
If the command is accepted and executed, it will be echoed back.
In case of an invalid command, the device will return "FLT".

Example "L2/n" will set pin 2 Low, and "H2/n" will set pin 2 high.

Note: 
	The command buffer is 4 characters long. Commands longer than 4 characters and not containing a new line will be processed 4 characters by
	overwriting the 4th character as a new line.