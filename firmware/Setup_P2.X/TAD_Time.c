/*
 * File:   TAD_Time.c
 * Author: teoA1
 *
 * Created on February 28, 2025, 3:03 PM
 */


#include <xc.h>
#include "TAD_Time.h"
#include "TAD_TIMER.h"
#include "LCTLCD.h"
#include "TAD_SIO.h"

static unsigned char sec = 0;
static unsigned char mins = 0;
static unsigned char hrs = 0;
static unsigned char state = 0;
static unsigned char timer;
static unsigned char i = 0;

void initTime() {
    TI_NewTimer(&timer);
}

void resetHrs() {
    hrs = 0;
}

void resetMins() {
    mins = 0;
}

void setHrs(unsigned char val) {
    hrs = hrs + val;
}

void setMins(unsigned char val) {
    mins = mins + val;
    sec = 0;
}

void Time_Motor() {
    
    switch (state) {
        case 0:
            TI_ResetTics(timer);
            state = 1;
            break;
        case 1:
            
            if (TI_GetTics(timer) >= 500) {
                sec++;
                state = 2;
                
            }
            break;
        case 2:
            
            if (sec < 60) {
                state = 0;
            }
            else {
                
                sec = 0;
                mins++;
                state = 3;
            }
            break;
        case 3:
            if (mins < 60) {
                state = 7;
            }
            else {
                mins = 0;
                hrs++;
                state = 4;
            }
            break;
        case 4:
            if (hrs >= 24) {
               
                hrs = 0;
            }
            state = 5;
            break;
     
        case 5: 
            LcGotoXY(2, 0);
            LcPutChar(hrs / 10 + '0');
            state = 6;
            break;
        case 6:
            LcGotoXY(3, 0);
            LcPutChar(hrs % 10 + '0');
            state = 7;
            break;
        case 7:
            LcGotoXY(5, 0);
            LcPutChar(mins / 10 + '0');
            state = 8;
            break;
        case 8:
            LcGotoXY(6, 0);
            LcPutChar(mins % 10 + '0');
            state = 0;
            break;
    }
    
}
