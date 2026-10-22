/*
 * File:   TAD_LED1.c
 * Author: teoA1
 *
 * Created on February 25, 2025, 5:13 PM
 */


#include <xc.h>
#include "TAD_TIMER.h"
#include "TAD_LED4.h"

static unsigned char state4 = 0;
static unsigned char intensity4 = 0;
static unsigned char Timer4;

void initLED4() {
    TI_NewTimer(&Timer4);
}

void setIntensity4(unsigned char value) {
    if (value - '0' == 17) {
        intensity4 = 10;
    }
    else {
        intensity4 = value - '0';
    } 
}

void LED4_Motor () {
   
    switch (state4) {
        case 0:
            if (TI_GetTics(Timer4) >= intensity4) {
                if (intensity4 != 10) {
                    LATAbits.LATA3 = 0;
                    state4 = 1;
                }
            }
            break;
        case 1:
            if (TI_GetTics(Timer4) >= 10) {
                    if (intensity4 != 0) {
                    TI_ResetTics(Timer4);
                    LATAbits.LATA3 = 1;
                    state4 = 0;
                }
            }
            break;
    }
    
}
