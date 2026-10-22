#ifndef TAD_KEYPAD_H
#define TAD_KEYPAD_H

void initKeypad(void);
//Post: intilizes the Keypad ports and the 
//necessary timers

unsigned char decodeKey(unsigned char row, unsigned char col);
//Pre: a Key has been pressed
//Post: returns the value of the key pressed by
//deconding it using the row and column active

void driveRow(unsigned char r);
//Post: changes the row active

unsigned char whichColPressed(void);
//Pre: A key has been pressed
//Post: returns which column has been pressed

unsigned char anyKeyPressed(void);
//Post: returns true if a key has been pressed

void keypad_Motor(void);
//Post: manages the keypad to control
//the configurations of the LEDs and other
//functionalities

void enableSystem(unsigned char en);
//Post: enables/disables the operation of 
//the keypad

#endif
