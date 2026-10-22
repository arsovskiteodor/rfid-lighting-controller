/*
 * File:   TAD_LED1.c
 * Author: teoA1
 *
 * Created on February 25, 2025, 5:13 PM
 */


#include <xc.h>
#include "TAD_TIMER.h"
#include "TAD_LED6.h"

static unsigned char state6 = 0;
static unsigned char intensity6 = 0;
static unsigned char Timer6;

void initLED6() {
    TI_NewTimer(&Timer6);
}

void setIntensity6(unsigned char value) {
    if (value - '0' == 17) {
        intensity6 = 10;
    }
    else {
        intensity6 = value - '0';
    } 
}

void LED6_Motor () {
   
    switch (state6) {
        case 0:
            if (TI_GetTics(Timer6) >= intensity6) {
                if (intensity6 != 10) {
                    LATAbits.LATA5 = 0;
                    state6 = 1;
                }
            }
            break;
        case 1:
            if (TI_GetTics(Timer6) >= 10) {
                    if (intensity6 != 0) {
                    TI_ResetTics(Timer6);
                    LATAbits.LATA5 = 1;
                    state6 = 0;
                }
            }
            break;
    }
    
}
