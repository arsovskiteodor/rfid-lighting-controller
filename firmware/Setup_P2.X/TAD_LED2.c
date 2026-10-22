/*
 * File:   TAD_LED1.c
 * Author: teoA1
 *
 * Created on February 25, 2025, 5:13 PM
 */


#include <xc.h>
#include "TAD_TIMER.h"
#include "TAD_LED2.h"
#include "TAD_SIO.h"

static unsigned char state2 = 0;
static unsigned char intensity2 = 0;
static unsigned char Timer2;

void initLED2() {
    TI_NewTimer(&Timer2);
}

void setIntensity2(unsigned char value) {
    if (value - '0' == 17) {
        intensity2 = 10;
    }
    else {
        intensity2 = value - '0';
    } 
    
}

void LED2_Motor () {
   
    switch (state2) {
        case 0:
            if (TI_GetTics(Timer2) >= intensity2) {
                if (intensity2 != 10) {
                    LATAbits.LATA1 = 0;
                    state2 = 1;
                }
            }
            break;
        case 1:
            if (TI_GetTics(Timer2) >= 10) {
                    if (intensity2 != 0) {
                    TI_ResetTics(Timer2);
                    LATAbits.LATA1 = 1;
                    state2 = 0;
                }
            }
            break;
    }
    
}
