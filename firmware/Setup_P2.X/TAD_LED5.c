/*
 * File:   TAD_LED1.c
 * Author: teoA1
 *
 * Created on February 25, 2025, 5:13 PM
 */


#include <xc.h>
#include "TAD_TIMER.h"
#include "TAD_LED5.h"

static unsigned char state5 = 0;
static unsigned char intensity5 = 0;
static unsigned char Timer5;

void initLED5() {
    TI_NewTimer(&Timer5);
}

void setIntensity5(unsigned char value) {
    if (value - '0' == 17) {
        intensity5 = 10;
    }
    else {
        intensity5 = value - '0';
    } 
}

void LED5_Motor () {
   
    switch (state5) {
        case 0:
            if (TI_GetTics(Timer5) >= intensity5) {
                if (intensity5 != 10) {
                    LATAbits.LATA4 = 0;
                    state5 = 1;
                }
            }
            break;
        case 1:
            if (TI_GetTics(Timer5) >= 10) {
                    if (intensity5 != 0) {
                    TI_ResetTics(Timer5);
                    LATAbits.LATA4 = 1;
                    state5 = 0;
                }
            }
            break;
    }
    
}
