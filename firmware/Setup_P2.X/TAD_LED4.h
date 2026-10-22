#ifndef TAD_LED4_H
#define TAD_LED4_H

#define LED_ON 1
#define LED_OFF 0

void initLED4(void);
//Post: initializes the LED for
//operation

void setIntensity4(unsigned char value);
//Pre: a user has entered/left or the inensity
//has been modified using the keypad
//Post: sets the intensity varaible

void LED4_Motor (void);
//Post: uses the intensity variable to
//creat a 50hz PWM signal controlling the 
//intensity of the LED

#endif