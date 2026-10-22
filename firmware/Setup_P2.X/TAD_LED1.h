#ifndef TAD_LED1_H
#define TAD_LED1_H

#define LED_ON 1
#define LED_OFF 0

void initLED1(void);
//Post: initializes the LEDs for
//operation, and sets PORTA to digital

void setIntensity1(unsigned char value);
//Pre: a user has entered/left or the inensity
//has been modified using the keypad
//Post: sets the intensity varaible

void LED1_Motor (void);
//Post: uses the intensity variable to
//creat a 50hz PWM signal controlling the 
//intensity of the LED

#endif