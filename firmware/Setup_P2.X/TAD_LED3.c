/*
 * File:   TAD_LED1.c
 * Author: teoA1
 *
 * Created on February 25, 2025, 5:13 PM
 */


#include <xc.h>
#include "TAD_TIMER.h"
#include "TAD_LED3.h"

static unsigned char state3 = 0;
static unsigned char intensity3 = 0;
static unsigned char Timer3;

void initLED3() {
    TI_NewTimer(&Timer3);
}

void setIntensity3(unsigned char value) {
    if (value - '0' == 17) {
        intensity3 = 10;
    }
    else {
        intensity3 = value - '0';
    } 
}

void LED3_Motor () {
   
    switch (state3) {
        case 0:
            if (TI_GetTics(Timer3) >= intensity3) {
                if (intensity3 != 10) {
                    LATAbits.LATA2 = 0;
                    state3 = 1;
                }
            }
            break;
        case 1:
            if (TI_GetTics(Timer3) >= 10) {
                    if (intensity3 != 0) {
                    TI_ResetTics(Timer3);
                    LATAbits.LATA2 = 1;
                    state3 = 0;
                }
            }
            break;
    }
    
}
