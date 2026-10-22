#ifndef TAD_MENU_H
#define TAD_MENU_H

void printMenuHash(void);
//Pre: the hashtag on the keypad has been
//pressed for less than 3 sec
//Post: sets the state of the motor to print
//the menu

void Menu_Motor(void);
//Post: manages the interaction by the user
//through the PC keyboard to print the neccesary
//information in the putty terminal

#endif