#ifndef TAD_TIME_H
#define TAD_TIME_H

void initTime(void);
//Post: initializes the clock on the LCD screen

void resetHrs(void);
//Pre: user chooses option to change system time
//Post: sets the variable for the hour to 0

void resetMins(void);
//Pre: user chooses option to change system time
//Post: sets the variable for the mins to 0

void setHrs(unsigned char val);
//Pre: user chooses option to change system time
//Post: sets the hour of the clock

void setMins(unsigned char val);
//Pre: user chooses option to change system time
//Post: sets the mins of the clock

void Time_Motor(void);
//Post: Manages the system time that is
//printed on the LCD

#endif